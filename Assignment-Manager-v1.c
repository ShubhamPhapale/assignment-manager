#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<limits.h>


#define MAX_NAME_LENGTH 50
#define MAX_ASSIGNMENT_LENGTH 50
#define MAX_GROUP_SIZE 3
#define MAX_SIZE 15

typedef enum { not_submitted1, submitted1, evaluated1 } StudentStatus;
typedef enum { declared2, not_submitted2, submitted2, evaluated2 } AssignmentStatus;
typedef enum {declared3, due_date_overdue3, evaluated3} AssignmentGroupStatus;

typedef struct Student {
    int student_id;
    char name[MAX_NAME_LENGTH];
    struct Student* group_partner;
    int group_id;
    char assignment_name[MAX_ASSIGNMENT_LENGTH];
    //time_t deadline;
    int deadline;
    StudentStatus stustatus;
    int evaluation_marks;
    int viva_marks;
    struct Student* left;
    struct Student* right;
} Student;

typedef struct AssignmentGroup {
    int groupId;
    int studentId1;
    int studentId2;
    AssignmentGroupStatus asgpstatus;
    int marks;
} AssignmentGroup;

typedef struct Assignment {
    int id;
    //time_t deadline;
    int deadline;
    char topic[MAX_ASSIGNMENT_LENGTH];
    AssignmentStatus status;
    AssignmentGroup* groups;
    struct Assignment* left;
    struct Assignment* right;
} Assignment;


typedef struct AssignmentGroup_BTreeNode {
    AssignmentGroup *keys[MAX_GROUP_SIZE - 1];
    struct AssignmentGroup_BTreeNode *children[MAX_GROUP_SIZE];
    int numKeys;
    int isLeaf;
} AssignmentGroup_BTreeNode;

AssignmentGroup_BTreeNode* AssignmentGroup_createNode();
//void insert(AssignmentGroup_BTreeNode *root, AssignmentGroup *key);
//void traverse(AssignmentGroup_BTreeNode *root);
void AssignmentGroup_insert_in_node(AssignmentGroup_BTreeNode* node, AssignmentGroup* key, int index);
void AssignmentGroup_split_child(AssignmentGroup_BTreeNode* parent, int index);
AssignmentGroup_BTreeNode* AssignmentGroup_insert_into_node(AssignmentGroup_BTreeNode* node, AssignmentGroup* key);
AssignmentGroup_BTreeNode* AssignmentGroup_insert(AssignmentGroup_BTreeNode* root, AssignmentGroup* key);
AssignmentGroup* AssignmentGroup_search(AssignmentGroup_BTreeNode* root, int aid);
void AssignmentGroup_inorder_traversal(AssignmentGroup_BTreeNode* root);
void printAssignmentGroupDetails(AssignmentGroup* group);





AssignmentGroup_BTreeNode* AssignmentGroup_createNode() {
    AssignmentGroup_BTreeNode *newNode = (AssignmentGroup_BTreeNode*)malloc(sizeof(AssignmentGroup_BTreeNode));
    newNode->numKeys = 0;
    newNode->isLeaf = 1;
    for(int i = 0; i < MAX_GROUP_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

/*void insert(AssignmentGroup_BTreeNode *root, AssignmentGroup *key) {
    if(root->numKeys == 2*MAX_GROUP_SIZE - 1) {
        AssignmentGroup_BTreeNode *newNode = AssignmentGroup_createNode();
        newNode->isLeaf = 0;
        newNode->children[0] = root;
        root = newNode;
    }
    if(root->isLeaf == 1) {
        int i = root->numKeys - 1;
        while(i >= 0 && root->keys[i]->groupId > key->groupId) {
            root->keys[i+1] = root->keys[i];
            i--;
        }
        root->keys[i+1] = key;
        root->numKeys++;
    }
    else {
        int i = root->numKeys - 1;
        while(i >= 0 && root->keys[i]->groupId > key->groupId) {
            i--;
        }
        i++;
        if(root->children[i]->numKeys == 2*MAX_GROUP_SIZE - 1) {
            insert(root->children[i], key);
        }
        else {
            int j = root->children[i]->numKeys - 1;
            while(j >= 0 && root->children[i]->keys[j]->groupId > key->groupId) {
                root->children[i]->keys[j+1] = root->children[i]->keys[j];
                j--;
            }
            root->children[i]->keys[j+1] = key;
            root->children[i]->numKeys++;
        }
    }
}

void traverse(AssignmentGroup_BTreeNode *root) {
    if(root != NULL) {
        for(int i = 0; i < root->numKeys; i++) {
            printf("%d ", root->keys[i]->groupId);
        }
        printf("\n");
        if(root->isLeaf == 0) {
            for(int i = 0; i < root->numKeys+1; i++) {
                traverse(root->children[i]);
            }
        }
    }
}*/


void AssignmentGroup_insert_in_node(AssignmentGroup_BTreeNode* node, AssignmentGroup* key, int index) 
{
    for (int i = node->numKeys; i > index; i--) 
    {
        node->keys[i] = node->keys[i-1];
    }
    node->keys[index] = key;
    node->numKeys++;
}

void AssignmentGroup_split_child(AssignmentGroup_BTreeNode* parent, int index) {
    AssignmentGroup_BTreeNode* new_node = AssignmentGroup_createNode();
    AssignmentGroup_BTreeNode* old_node = parent->children[index];
    new_node->isLeaf = old_node->isLeaf;
    new_node->numKeys = MAX_GROUP_SIZE / 2;
    for (int i = 0; i < MAX_GROUP_SIZE / 2; i++) {
        new_node->keys[i] = old_node->keys[i + MAX_GROUP_SIZE / 2];
    }
    if (!old_node->isLeaf) 
    {
        for (int i = 0; i < MAX_GROUP_SIZE / 2 + 1; i++) {
            new_node->children[i] = old_node->children[i + MAX_GROUP_SIZE / 2];
        }
    }
    old_node->numKeys = MAX_GROUP_SIZE / 2;
    for (int i = parent->numKeys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = new_node;
    AssignmentGroup_insert_in_node(parent, old_node->keys[MAX_GROUP_SIZE / 2], index);
}

AssignmentGroup_BTreeNode* AssignmentGroup_insert_into_node(AssignmentGroup_BTreeNode* node, AssignmentGroup* key) 
{
    int index = node->numKeys - 1;
    if (node->isLeaf) 
    {
        while (index >= 0 && node->keys[index]->groupId > key->groupId) 
        {
            node->keys[index+1] = node->keys[index];
            index--;
        }
        node->keys[index+1] = key;
        node->numKeys++;
    }
    else 
    {
        while (index >= 0 && node->keys[index]->groupId > key->groupId) 
        {
            index--;
        }
        index++;
        if (node->children[index]->numKeys == MAX_GROUP_SIZE) 
        {
            AssignmentGroup_split_child(node, index);
            if (node->keys[index]->groupId < key->groupId) 
            {
                index++;
            }
        }
        AssignmentGroup_insert_into_node(node->children[index], key);
    }
    return node;
}

AssignmentGroup_BTreeNode* AssignmentGroup_insert(AssignmentGroup_BTreeNode* root, AssignmentGroup* key) 
{
    if (root == NULL) 
    {
        AssignmentGroup_BTreeNode* new_node = AssignmentGroup_createNode();
        new_node->keys[0] = key;
        new_node->numKeys = 1;
        return new_node;
    }
    else 
    {
        if (root->numKeys == MAX_GROUP_SIZE) 
        {
            AssignmentGroup_BTreeNode* new_node = AssignmentGroup_createNode();
            new_node->isLeaf = 0;
            new_node->children[0] = root;
            AssignmentGroup_split_child(new_node, 0);
            int i = 0;
            if (new_node->keys[0]->groupId < key->groupId) 
            {
                i++;
            }
            AssignmentGroup_insert_into_node(new_node->children[i], key);
            return new_node;
        }
        else   
        {
            AssignmentGroup_insert_into_node(root, key);
            return root;
        }
    }
}

AssignmentGroup* AssignmentGroup_search(AssignmentGroup_BTreeNode* root, int aid) 
{
    int i = 0;
    while (i < root->numKeys && aid > root->keys[i]->groupId) 
    {
        i++;
    }
    if (i < root->numKeys && aid == root->keys[i]->groupId) 
    {
        return root->keys[i];
    }
    else if (root->isLeaf) 
    {
        return NULL;
    }
    else 
    {
        return AssignmentGroup_search(root->children[i], aid);
    }
}

/*void print_assignmentGroup(AssignmentGroup* assignment) 
{
    printf("Assignment ID: %d\n", assignment->groupId);
    printf("Topic: %s\n", assignment->topic);
    printf("Deadline: %d\n", assignment->deadline);
    printf("Status: ");
    switch (assignment->status) 
    {
        case declared2:
            printf("Not started\n");
            break;
        case submitted2:
            printf("In progress\n");
            break;
        case evaluated2:
            printf("Completed\n");
            break;
    }
    printf("Groups: ");
    AssignmentGroup* group = assignment->groups;
    while (group != NULL) 
    {
        printf("%s ", group->name);
        group = group->next;
    }
    printf("\n");
}*/

void AssignmentGroup_inorder_traversal(AssignmentGroup_BTreeNode* root) 
{
    if(root != NULL) 
    {
        for(int i = 0; i < root->numKeys; i++) 
        {
            printAssignmentGroupDetails(root->keys[i]);
        }
        printf("\n");
        if(root->isLeaf == 0) 
        {
            for(int i = 0; i < root->numKeys+1; i++) 
            {
                AssignmentGroup_inorder_traversal(root->children[i]);
            }
        }
    }
}

void printAssignmentGroupDetails(AssignmentGroup* group) {
    printf("Group ID: %d\n", group->groupId);
    printf("Student 1 ID: %d\n", group->studentId1);
    printf("Student 2 ID: %d\n", group->studentId2);
    switch(group->asgpstatus) 
        {
            case declared3:
                printf("Status: Declared\n");
                break;
            case due_date_overdue3:
                printf("Status: due date overloaded\n");
                break;
            case evaluated3:
                printf("Status: evaluated\n");
                break;
        }
    printf("Marks: %d\n", group->marks);
}




typedef struct Assignment_BTreeNode {
    Assignment* keys[MAX_GROUP_SIZE];
    int num_keys;
    struct Assignment_BTreeNode* children[MAX_GROUP_SIZE+1];
    //int num_children;
    int is_leaf;
} Assignment_BTreeNode;

Assignment_BTreeNode* Assignment_create_node() {
    Assignment_BTreeNode* node = (Assignment_BTreeNode*) malloc(sizeof(Assignment_BTreeNode));
    node->num_keys = 0;
    //node->num_children = 0;
    node->is_leaf = 1;
    for (int i = 0; i < MAX_GROUP_SIZE + 1; i++) 
    {
        node->children[i] = NULL;
    }
    return node;
}


typedef struct student_btree {
    Student* keys[MAX_GROUP_SIZE];
    struct student_btree* children[MAX_GROUP_SIZE + 1];
    int num_keys;
    int is_leaf;
} student_btree;

student_btree* student_create_node() {
    student_btree* new_node = (student_btree*) malloc(sizeof(student_btree));
    new_node->num_keys = 0;
    new_node->is_leaf = 1;
    for (int i = 0; i < MAX_GROUP_SIZE + 1; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

void student_insert_in_node(student_btree* node, Student* key, int index) 
{
    for (int i = node->num_keys; i > index; i--) 
    {
        node->keys[i] = node->keys[i-1];
    }
    node->keys[index] = key;
    node->num_keys++;
}

void student_split_child(student_btree* parent, int index) {
    student_btree* new_node = student_create_node();
    student_btree* old_node = parent->children[index];
    new_node->is_leaf = old_node->is_leaf;
    new_node->num_keys = MAX_GROUP_SIZE / 2;
    for (int i = 0; i < MAX_GROUP_SIZE / 2; i++) {
        new_node->keys[i] = old_node->keys[i + MAX_GROUP_SIZE / 2];
    }
    if (!old_node->is_leaf) {
        for (int i = 0; i < MAX_GROUP_SIZE / 2 + 1; i++) {
            new_node->children[i] = old_node->children[i + MAX_GROUP_SIZE / 2];
        }
    }
    old_node->num_keys = MAX_GROUP_SIZE / 2;
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = new_node;
    student_insert_in_node(parent, old_node->keys[MAX_GROUP_SIZE / 2], index);
}

student_btree* student_insert_into_node(student_btree* node, Student* key) {
    int index = node->num_keys - 1;
    if (node->is_leaf) {
        while (index >= 0 && node->keys[index]->group_id > key->group_id) {
            node->keys[index+1] = node->keys[index];
            index--;
        }
        node->keys[index+1] = key;
        node->num_keys++;
    }
    else {
        while (index >= 0 && node->keys[index]->group_id > key->group_id) {
            index--;
        }
        index++;
        if (node->children[index]->num_keys == MAX_GROUP_SIZE) {
            student_split_child(node, index);
            if (node->keys[index]->group_id < key->group_id) {
                index++;
            }
        }
        student_insert_into_node(node->children[index], key);
    }
    return node;
}

student_btree* student_insert(student_btree* root, Student* key) 
{
    if (root == NULL) 
    {
        student_btree* new_node = student_create_node();
        new_node->keys[0] = key;
        new_node->num_keys = 1;
        return new_node;
    }
    else 
    {
        if (root->num_keys == MAX_GROUP_SIZE) 
        {
            student_btree* new_node = student_create_node();
            new_node->is_leaf = 0;
            new_node->children[0] = root;
            student_split_child(new_node, 0);
            int i = 0;
            if (new_node->keys[0]->group_id < key->group_id) 
            {
                i++;
            }
            student_insert_into_node(new_node->children[i], key);
            return new_node;
        }
        else   
        {
            student_insert_into_node(root, key);
            return root;
        }
    }
}

Student* student_search_groupid(student_btree* root, int group_id) 
{
    int i = 0;
    while (i < root->num_keys && group_id > root->keys[i]->group_id) 
    {
        i++;
    }
    if (i < root->num_keys && group_id == root->keys[i]->group_id) 
    {
        return root->keys[i];
    }
    else if (root->is_leaf) 
    {
        return NULL;
    }
    else 
    {
        return student_search_groupid(root->children[i], group_id);
    }
}


Student* student_search_stuid(student_btree* root, int sid) 
{
    int i = 0;
    while (i < root->num_keys && sid > root->keys[i]->student_id) 
    {
        printf("\n hhhhhh");

        i++;
    }
    if (i < root->num_keys && sid == root->keys[i]->student_id) 
    {
        printf("\n kkkkkkk");
        return root->keys[i];
    }
    else if (root->is_leaf) 
    {
        printf("\n NULLNULLNULL");
        return NULL;
    }
    else 
    {
        printf("\n rrrrrrrrrr");
        return student_search_stuid(root->children[i], sid);
    }
}

void print_student(Student* student) 
{
    printf("Student ID: %d\n", student->student_id);
    printf("Name: %s\n", student->name);
    if (student->group_partner) {
        printf("Group Partner: %s\n", student->group_partner->name);
    }
    else {
        printf("Group Partner: None\n");
    }
    printf("Group ID: %d\n", student->group_id);
    printf("Assignment Name: %s\n", student->assignment_name);
    printf("Deadline: %ld\n", student->deadline);
    printf("Status: ");
    switch (student->stustatus) 
    {
        case not_submitted1:
            printf("not_submitted1\n");
            break;
        case submitted1:
            printf("submitted1\n");
            break;
        case evaluated1:
            printf("evaluated1\n");
            break;
    }
    printf("Evaluation Marks: %d\n", student->evaluation_marks);
    printf("Viva Marks: %d\n", student->viva_marks);
}

void student_inorder_traversal(student_btree* root) 
{
    if (root) 
    {
        student_inorder_traversal(root->children[0]);
        for (int i = 0; i < root->num_keys; i++) 
        {
            print_student(root->keys[i]);
            printf("\n");
            student_inorder_traversal(root->children[i+1]);
        }
    }
}

void Assignment_insert_in_node(Assignment_BTreeNode* node, Assignment* key, int index) 
{
    for (int i = node->num_keys; i > index; i--) 
    {
        node->keys[i] = node->keys[i-1];
    }
    node->keys[index] = key;
    node->num_keys++;
}
void Assignment_split_child(Assignment_BTreeNode* parent, int index) {
    Assignment_BTreeNode* new_node = Assignment_create_node();
    Assignment_BTreeNode* old_node = parent->children[index];
    new_node->is_leaf = old_node->is_leaf;
    new_node->num_keys = MAX_GROUP_SIZE / 2;
    for (int i = 0; i < MAX_GROUP_SIZE / 2; i++) {
        new_node->keys[i] = old_node->keys[i + MAX_GROUP_SIZE / 2];
    }
    if (!old_node->is_leaf) 
    {
        for (int i = 0; i < MAX_GROUP_SIZE / 2 + 1; i++) {
            new_node->children[i] = old_node->children[i + MAX_GROUP_SIZE / 2];
        }
    }
    old_node->num_keys = MAX_GROUP_SIZE / 2;
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = new_node;
    Assignment_insert_in_node(parent, old_node->keys[MAX_GROUP_SIZE / 2], index);
}
Assignment_BTreeNode* Assignment_insert_into_node(Assignment_BTreeNode* node, Assignment* key) 
{
    int index = node->num_keys - 1;
    if (node->is_leaf) 
    {
        while (index >= 0 && node->keys[index]->id > key->id) 
        {
            node->keys[index+1] = node->keys[index];
            index--;
        }
        node->keys[index+1] = key;
        node->num_keys++;
    }
    else 
    {
        while (index >= 0 && node->keys[index]->id > key->id) 
        {
            index--;
        }
        index++;
        if (node->children[index]->num_keys == MAX_GROUP_SIZE) 
        {
            Assignment_split_child(node, index);
            if (node->keys[index]->id < key->id) 
            {
                index++;
            }
        }
        Assignment_insert_into_node(node->children[index], key);
    }
    return node;
}

Assignment_BTreeNode* Assignment_insert(Assignment_BTreeNode* root, Assignment* key) 
{
    if (root == NULL) 
    {
        Assignment_BTreeNode* new_node = Assignment_create_node();
        new_node->keys[0] = key;
        new_node->num_keys = 1;
        return new_node;
    }
    else 
    {
        if (root->num_keys == MAX_GROUP_SIZE) 
        {
            Assignment_BTreeNode* new_node = Assignment_create_node();
            new_node->is_leaf = 0;
            new_node->children[0] = root;
            Assignment_split_child(new_node, 0);
            int i = 0;
            if (new_node->keys[0]->id < key->id) 
            {
                i++;
            }
            Assignment_insert_into_node(new_node->children[i], key);
            return new_node;
        }
        else   
        {
            Assignment_insert_into_node(root, key);
            return root;
        }
    }
}
Assignment* Assignment_search(Assignment_BTreeNode* root, int aid) 
{
    int i = 0;
    while (i < root->num_keys && aid > root->keys[i]->id) 
    {
        i++;
    }
    if (i < root->num_keys && aid == root->keys[i]->id) 
    {
        return root->keys[i];
    }
    else if (root->is_leaf) 
    {
        return NULL;
    }
    else 
    {
        return Assignment_search(root->children[i], aid);
    }
}

Assignment* Assignment_search_group(Assignment_BTreeNode* root, int aid) 
{
    int i = 0;
    while (i < root->num_keys && aid > root->keys[i]->groups->groupId) 
    {
        i++;
    }
    if (i < root->num_keys && aid == root->keys[i]->groups->groupId) 
    {
        return root->keys[i];
    }
    else if (root->is_leaf) 
    {
        return NULL;
    }
    else 
    {
        return Assignment_search_group(root->children[i], aid);
    }
}

void print_assignment(Assignment* assignment) 
{
    //time_t  now  = time(NULL);
    printf("Assignment ID: %d\n", assignment->id);
    printf("Topic: %s\n", assignment->topic);
    //printf("Deadline: %ld\n", (long) difftime(assignment->deadline,  10 ));
    printf("Deadline: %ld\n",assignment->deadline );
    printf("Status: ");
    switch (assignment->status) 
    {
        case declared2:
            printf("declared\n");
            break;
        case not_submitted2:
            printf("not_submitted\n");
            break;
        case submitted2:
            printf("submitted\n");
            break;
        case evaluated2:
            printf("evaluated\n");
            break;
    }
    printf("Groups: ");
    /*AssignmentGroup* group = assignment->groups;
    while (group != NULL) 
    {
        printf("%s ", group->name);
        group = group->next;
    }*/
    printf("\n");
}
void Assignment_inorder_traversal(Assignment_BTreeNode* root) 
{
    if (root) 
    {
        printf("\np%dp\n",root->num_keys);
        Assignment_inorder_traversal(root->children[0]);
        //printf("\np%dp",root->num_keys);
        for (int i = 0; i < root->num_keys; i++) 
        {
            print_assignment(root->keys[i]);
            printf("\n");
            Assignment_inorder_traversal(root->children[i+1]);
        }
    }
}

// void Q3(Assignment_BTreeNode* node) {
//     if (node == NULL) {
//         return;
//     }
//     int i;
//     for (i = 0; i < node->num_keys; i++) {
//         Assignment* assignment = node->keys[i];
//         if (assignment->status == submitted2) {
//             int j;
//             for (j = 0; j < MAX_GROUP_SIZE - 1; j++) {
//                 AssignmentGroup* group = assignment->groups[j];
//                 if (group == NULL) {
//                     break;
//                 }
//                 if (group->asgpstatus == submitted2) {
//                     printf("Student Group %d and %d submitted the assignment '%s', but it has not been evaluated yet.\n", group->studentId1, group->studentId2, assignment->topic);
//                 }
//             }
//         }
//         if (!node->is_leaf) {
//             Q3(node->children[i]);
//         }
//     }
//     if (!node->is_leaf) {
//         Q3(node->children[i]);
//     }
// }
//////////////////Q3////////////////////////////////////
void submit_assignment(Student* student) 
{
    if(student->stustatus==not_submitted1)
    {
        student->stustatus=submitted1;
        //printf("\n subm");
    }
}
void change_assignment_status(Assignment* assignment, AssignmentStatus newStatus) 
{
    assignment->status = newStatus;
    //printf("\n assignment submitted");
}
void change_student_assignment_status(Student* student, StudentStatus newStatus, int marks) 
{
    student->stustatus=newStatus;
    student->viva_marks=marks;
    student->evaluation_marks=marks;
    //printf("\n asgrp updated");
}
void change_assignment_group_status(AssignmentGroup* group, AssignmentGroupStatus newStatus, int marks) 
{
    group->asgpstatus = newStatus;
    group->marks = marks;
    //printf("p");
}



void Q4(student_btree* head,AssignmentGroup_BTreeNode* asgp) 
{
    AssignmentGroup_BTreeNode* curr=asgp;
    student_btree* h1=head;
    if(curr != NULL) 
    {
        for(int i = 0; i <= curr->numKeys; i++) 
        {
            printf("\nvv\n");
            //printAssignmentGroupDetails(curr->keys[i]);
            if(curr->keys[i]->asgpstatus==declared3)
            {
                Student* s1=student_search_stuid(h1,curr->keys[i]->studentId1);
                if(s1==NULL)
                {
                    printf("\n failure 1");
                }
                Student* s2=student_search_stuid(h1,curr->keys[i]->studentId2);
                if(s2==NULL)
                {
                    printf("\n failure 2");
                }
                printf("\n declared but not evaluated: \n");
                if(s1->evaluation_marks==-1)
                {
                    printf("Group ID: %d Student Id: %d for Assignment Name: %d has not been evaluated1.\n", curr->keys[i]->groupId,s1->student_id, s1->assignment_name);
                }
                if(s1->viva_marks==-1)
                {
                    printf("Group ID: %d Student Id: %d for Assignment Name: %d has not conducted viva.\n", curr->keys[i]->groupId,s1->student_id, s1->assignment_name);
                }
                if(s2->evaluation_marks==-1)
                {
                    printf("Group ID: %d Student Id: %d for Assignment Name: %d has not been evaluated1.\n", curr->keys[i]->groupId,s2->student_id, s2->assignment_name);
                }
                if(s2->viva_marks==-1)
                {
                    printf("Group ID: %d Student Id: %d for Assignment Name: %d has not conducted viva.\n", curr->keys[i]->groupId,s2->student_id, s2->assignment_name);
                }
            }
        }
        printf("\n");
        if(curr->isLeaf == 0) 
        {
            for(int i = 0; i < curr->numKeys+1; i++) 
            {
                Q4(head,asgp->children[i]);
            }
        }
    }
}



//evaluate marks
void evaluate_student(Student* stu,int marks)
{
    if(stu->stustatus==submitted1)
    {
        stu->evaluation_marks=marks;
        stu->stustatus=evaluated1;
    }
    else
    {
        printf("\n student has not submitted the assignment");
    }
}
void evaluate_assignment(Assignment* assignment) 
{
    //printf("\n m1 %s",assignment->status);
    if(assignment->status != evaluated2)
    {
        assignment->status = evaluated2;
    }
    //printf("\n m2");
    assignment->groups->asgpstatus= evaluated3;
    //printf("\n m3");
}

void evaluate_assignment_group(AssignmentGroup* group, Assignment** head, int marks1,int gid) 
{
    group->asgpstatus = evaluated2;
    group->marks = marks1;
    /*Assignment* head1=*head;
    while(head1->groups->groupId!=gid)
    {
        head1->groups=head1->groups->next;
    }
    head1->groups->asgpstatus = evaluated2;
    head1->groups->marks=marks1;*/
}

//viva students
void viva_student(Student* stu,int marks)
{
    if(stu->stustatus==evaluated1)
    {
        stu->viva_marks=marks;
        printf("p%dp",stu->viva_marks);
    }
    else
    {
        printf("\n assignment has not been evaluated.\n evaluate it and later viva will be taken");
    }
}



void print_overdue_assignments(AssignmentGroup_BTreeNode* root, student_btree* ptr) 
{
    Student* stu;
    if(root != NULL) 
    {
        for(int i = 0; i < root->numKeys; i++) 
        {
            AssignmentGroup_BTreeNode* curr = root;
            if(curr->keys[i]->asgpstatus==due_date_overdue3)
            {
                printf("\n Group ID: %d is overdue.", curr->keys[i]->groupId);
                //Student* stu=ptr;
                /*while(stu->group_id!=curr->keys[i]->groupId)
                {
                    stu=stu->next;
                }*/
                stu = student_search_groupid(ptr, curr->keys[i]->groupId);
                printf("\n StudenAssignmentStatus Id:%d , StudenAssignmentStatus Name- %s", stu->student_id,stu->name);
                printf("\n Student2 Id:%d , Student2 Name- %s", stu->group_partner->student_id,stu->group_partner->name);
            }
            //printAssignmentGroupDetails(root->keys[i]);
        }
        printf("\n");
        if(root->isLeaf == 0) 
        {
            for(int i = 0; i < root->numKeys+1; i++) 
            {
                print_overdue_assignments(root->children[i], ptr);
            }
        }
    }
}


/*void findStudentGroups(AssignmentGroup_BTreeNode* root, AssignmentGroup* assignmentGroup, AssignmentGroup** studentGroups, int* numStudentGroups) 
{
    if (root != NULL) 
    {
        for (int i = 0; i < root->numKeys; i++) 
        {
            AssignmentGroup* curr = root->keys[i];
            if (curr->groupId == assignmentGroup->groupId) 
            {
                studentGroups[(*numStudentGroups)++] = curr;
            }
            findStudentGroups(root->children[i], assignmentGroup, studentGroups, numStudentGroups);
        }
        findStudentGroups(root->children[root->numKeys], assignmentGroup, studentGroups, numStudentGroups);
    }
}*/

/*void print_student_groups_by_marks(AssignmentGroup_BTreeNode* root, int assignmentId) 
{
    AssignmentGroup* assignmentGroup = findAssignmentGroup(root, assignmentId);
    if (assignmentGroup == NULL) 
    {
        printf("Assignment group not found.\n");
        return;
    }

    // Traverse the B-tree to find all student-groups belonging to the assignment.
    AssignmentGroup** studentGroups = (AssignmentGroup**) malloc(sizeof(AssignmentGroup*) * MAX_GROUP_SIZE);
    int numStudentGroups = 0;
    findStudentGroups(root, assignmentGroup, studentGroups, &numStudentGroups);

    // Sort the student-groups by marks in descending order.
    qsort(studentGroups, numStudentGroups, sizeof(AssignmentGroup*), compareByMarks);

    // Print the details of each student-group.
    printf("Details of student-groups for assignment ID %d:\n", assignmentId);
    for (int i = 0; i < numStudentGroups; i++) 
    {
        AssignmentGroup* studentGroup = studentGroups[i];
        printf("Group ID: %d\n", studentGroup->groupId);
        printf("Student 1 ID: %d\n", studentGroup->studentId1);
        printf("Student 2 ID: %d\n", studentGroup->studentId2);
        printf("Marks: %d\n\n", studentGroup->marks);
    }

    free(studentGroups);
}*/

// Helper function to compare two assignment groups by marks.
int compareByMarks(const void* a, const void* b) 
{
    AssignmentGroup* ag1 = *(AssignmentGroup**) a;
    AssignmentGroup* ag2 = *(AssignmentGroup**) b;
    return ag2->marks - ag1->marks;
}

// Helper function to traverse the B-tree and find all student-groups belonging to a given assignment.


// Helper function to find an assignment group by assignment ID.
/*AssignmentGroup* findAssignmentGroup(AssignmentGroup_BTreeNode* root, int assignmentId) 
{
    if (root == NULL) 
    {
        return NULL;
    }

    int i = 0;
    while (i < root->numKeys && assignmentId > root->keys[i]->groupId) 
    {
        i++;
    }

    if (i < root->numKeys && assignmentId == root->keys[i]->groupId) 
    {*/

Student* qsp6(student_btree* root,Assignment* aq) 
{
    if (root) 
    {
        Student* st= qsp6(root->children[0],aq);
        for (int i = 0; i < root->num_keys; i++) 
        {
            //print_student(root->keys[i]);
            //printf("\n %s , %s ",aq->topic,root->keys[i]->name);
            if(strcmp(root->keys[i]->assignment_name,aq->topic)==0)
            {
                //printf("%d",dd);
                if(root->keys[i]->stustatus==submitted1&&root->keys[i]->evaluation_marks==-1)
                {
                    printf("\n student id: %d assignment id: %d is not evaluated",root->keys[i]->student_id,aq->id);
                }
                if((root->keys[i]->stustatus==submitted1||root->keys[i]->stustatus==evaluated1)&&root->keys[i]->viva_marks==-1)
                {
                    printf("\n student id: %d assignment id: %d has not conducted viva",root->keys[i]->student_id,aq->id);
                }
            }
            printf("\n");
            Student* st=qsp6(root->children[i+1],aq);
        }

    }
}




void qs6(Assignment_BTreeNode* root,student_btree* s) 
{
    if (root == NULL) 
    {
        printf("No assignments found.\n");
        return;
    }

    printf("Details of student-groups with top marks in each assignment:\n");

    for (int i = 0; i < root->num_keys; i++) 
    {
        Assignment* assignment = root->keys[i];
        qsp6(s,assignment);

    }
}




void find_assignment_group(AssignmentGroup_BTreeNode* root, int assignment_id, int* count)
{
    if(root != NULL) 
    {
        int i;
        for(i = 0; i < root->numKeys; i++) 
        {
            AssignmentGroup* group = root->keys[i];
            if(group->groupId == assignment_id) {
                // Print the details of student-groups in the decreasing order of marks they received
                printf("Assignment Group ID: %d\n", group->groupId);
                printf("Student 1 ID: %d, Marks: %d\n", group->studentId1,  group->marks);
                printf("Student 2 ID: %d, Marks: %d\n", group->studentId2,  group->marks);
                (*count)++;
                return;
            }
            else if(assignment_id < group->groupId) {
                // recursively search in the left child
                find_assignment_group(root->children[i], assignment_id, count);
                return;
            }
        }
        // recursively search in the right child
        find_assignment_group(root->children[i], assignment_id, count);
    }
}

void print_top_groupssp(AssignmentGroup_BTreeNode* root,student_btree* head,int aid,AssignmentGroup** asgp,int top_marks) 
{
    if(root != NULL) 
    {
        for(int i = 0; i < root->numKeys; i++) 
        {
            //printAssignmentGroupDetails(root->keys[i]);
            if(root->keys[i]->marks>top_marks)
            {
                top_marks=root->keys[i]->marks;
                *asgp=root->keys[i];
            }
        }
        printf("\n");
        if(root->isLeaf == 0) 
        {
            for(int i = 0; i < root->numKeys+1; i++) 
            {
                print_top_groupssp(root->children[i],head,aid,asgp,top_marks);
            }
        }
    }
}


void print_top_groups(Assignment_BTreeNode* root,student_btree* head) 
{
    if (root) 
    {
        //printf("\np%dp\n",root->num_keys);
        print_top_groups(root->children[0],head);
        //printf("\np%dp",root->num_keys);
        for (int i = 0; i < root->num_keys; i++) 
        {
            AssignmentGroup* asgp=NULL;
            print_top_groupssp(root->keys[i],head,root->keys[i]->id,&asgp,-1);
            printf("\n top performing group details: ");
            printf("\n group id: %d",asgp->groupId);
            printf("\n marks : %d",asgp->marks);
            printf("\n student1 id: %d",asgp->studentId1);
            printf("\n student2 id: %d",asgp->studentId2);
            
            printf("\n");
            print_top_groups(root->children[i+1],head);
        }
    }
}


void q9(Assignment_BTreeNode* root,int start,int end) 
{
    if (root) 
    {
        //printf("\np%dp\n",root->num_keys);
        q9(root->children[0],start,end);
        //printf("\np%dp",root->num_keys);
        for (int i = 0; i < root->num_keys; i++) 
        {
            if(root->keys[i]->id>=start&&root->keys[i]->id<=end)
            {
                print_assignment(root->keys[i]);
            }
            printf("\n");
            q9(root->children[i+1],start,end);
        }
    }
}

void student_search_aname(student_btree* root,Assignment* aq,AssignmentGroup_BTreeNode* agroot) 
{
    if (root) 
    {
        student_search_aname(root->children[0],aq,agroot);
        for (int i = 0; i < root->num_keys; i++) 
        {
            //print_student(root->keys[i]);
            //printf("\n %s , %s ",aq->topic,root->keys[i]->name);
            if(strcmp(root->keys[i]->assignment_name,aq->topic)==0)
            {
                //printf("%d",dd);
                root->keys[i]->deadline=aq->deadline;
                if(aq->deadline<=0)
                {
                    AssignmentGroup* aw=AssignmentGroup_search(agroot,root->keys[i]->group_id);
                    aw->asgpstatus=due_date_overdue3;
                }
            }
            printf("\n");
            student_search_aname(root->children[i+1],aq,agroot);
        }
    }
}

Student* student_search_aname2(student_btree* root,Assignment* aq,int* top_marks,Student** ans) 
{
    if (root) 
    {
        Student* st= student_search_aname2(root->children[0],aq,&top_marks,&ans);
        for (int i = 0; i < root->num_keys; i++) 
        {
            //print_student(root->keys[i]);
            //printf("\n %s , %s ",aq->topic,root->keys[i]->name);
            if(strcmp(root->keys[i]->assignment_name,aq->topic)==0)
            {
                //printf("%d",dd);
                if(root->keys[i]->evaluation_marks+root->keys[i]->viva_marks+root->keys[i]->group_partner->evaluation_marks+root->keys[i]->group_partner->viva_marks>=top_marks)
                {
                    top_marks=root->keys[i]->evaluation_marks+root->keys[i]->viva_marks+root->keys[i]->group_partner->evaluation_marks+root->keys[i]->group_partner->viva_marks;
                    //ans=root->keys[i];
                    return root->keys[i];
                }
            }
            printf("\n");
            Student* st=student_search_aname2(root->children[i+1],aq,&top_marks,&ans);
        }

    }
}




void print_student_groups_top_marks(Assignment_BTreeNode* root,student_btree* s) 
{
    if (root == NULL) 
    {
        printf("No assignments found.\n");
        return;
    }

    printf("Details of student-groups with top marks in each assignment:\n");

    for (int i = 0; i < root->num_keys; i++) 
    {
        Assignment* assignment = root->keys[i];
        Student* topStudentGroup = NULL;
        int topMarks = INT_MIN;

        // Iterate through the groups of the assignment to find the one with top marks.
        /*for (int j = 0; j < MAX_GROUP_SIZE; j++) 
        {
            AssignmentGroup* studentGroup = AssignmentGroup_search();
            Student* student Group = Stu
            if ( studentGroup->marks > topMarks) 
            {
                topMarks = studentGroup->marks;
                topStudentGroup = studentGroup;
            }
        }*/
        topStudentGroup=student_search_aname2(s,assignment,&topMarks,&topStudentGroup);

        // Print the details of the student group with top marks for the current assignment.
        if (topStudentGroup != NULL) 
        {
            printf("Assignment ID: %d\n", assignment->id);
            printf("Assignment Topic: %s\n", assignment->topic);
            printf("Group ID: %d\n", topStudentGroup->group_id);
            printf("Student 1 ID: %d\n", topStudentGroup->student_id);
            printf("Student 2 ID: %d\n", topStudentGroup->group_partner->student_id);
            printf("Marks: %d\n\n", topMarks);
        }
        else 
        {
            printf("Assignment ID: %d\n", assignment->id);
            printf("Assignment Topic: %s\n", assignment->topic);
            printf("No student group with completed assignment found.\n\n");
        }
    }
}


void change_deadline(Assignment_BTreeNode* head1, student_btree* head2, AssignmentGroup_BTreeNode* head3)
{
    int assignmentID;
    int days;
    int flag = 1;
    Assignment_BTreeNode* current = head1;
    student_btree* current_student = head2;

    Assignment_BTreeNode* current_prev;
    printf("\nEnter assignment ID:");
    scanf("%d", &assignmentID);
    printf("\nEnter the no of days by which deadline should be changed :");
    scanf("%d",&days);
    
    // Update Assignment B-tree
    while (current != NULL && flag == 1) 
    {
        for(int i = 0; i < current->num_keys && flag == 1; i++)
        {
            if(current->keys[i]->id == assignmentID)
            {
                current->keys[i]->deadline += days;
                flag = 0;
            }
        }
        if(flag == 1)
        {
            current_prev = current;
            current = current->children[current->num_keys];
        }
    }

    // Update Student B-tree
    while(current_student != NULL)
    {
        for(int i = 0; i < current_student->num_keys; i++)
        {
            if(strcmp(current_student->keys[i]->assignment_name, current_prev->keys[current_prev->num_keys-1]->topic) == 0)
            {
                
                
                AssignmentGroup* c = AssignmentGroup_search(head3, current_student->keys[i]->group_id);
                Student* st1 = student_search_stuid(head2, c->studentId1);
                st1->deadline = st1->deadline + days;
                Student* st2 = student_search_stuid(head2, c->studentId2);
                st2->deadline = st2->deadline + days;
                current_student->keys[i]->deadline += days;
                if(current_student->keys[i]->deadline <= 0)
                {
                    current_prev->keys[current_prev->num_keys-1]->status = due_date_overdue3;
                    //AssignmentGroup_BTreeNode* c = AssignmentGroup_search(head3, current_student->keys[i]->group_id);
                    
                    //c->keys[c->numKeys-1]->asgpstatus = due_date_overdue3;
                    c->asgpstatus = due_date_overdue3;
                    /*Student* st1 = student_search_stuid(head2, c->studentId1);
                    st1->deadline = st1->deadline + days;
                    Student* st2 = student_search_stuid(head2, c->studentId2);
                    st2->deadline = st2->deadline + days;*/
                }
            }
        }
        current_student = current_student->children[current_student->num_keys];
    }
}


void print_menu()
{
    printf("\n\n------------------------\n");
    printf("Assignment Management System\n");
    printf("------------------------\n");
    printf("1. Add Student\n");
    printf("2. Add Assignment\n");
    printf("3. Change Assignment Status\n");
    printf("4. Print Unsubmitted Assignments\n");
    printf("5. Print Overdue Assignments\n");
    printf("6. Print Pending Evaluations\n");
    printf("7. Print Student Groups by Marks\n");
    printf("8. Print Top Scorers\n");
    printf("9. Change Deadline\n");
    printf("10. Make student group\n");
    printf("11. Print ALL details:\n");
    printf("12, Evaluate assignment:\n");
    printf("13. Add Viva marks:\n");
    printf("14. Assign Assignment to a group\n");
    printf("16. q9\n");
    printf("0. Exit\n");
}


int main()  
{
    

    FILE* fstu;
    fstu = fopen("student.txt","r");
    if(fstu==NULL)
    {
        printf("\n error in opening student file");
    }
    FILE* fasgn;
    fasgn = fopen("assignment.txt","r");
    if(fasgn==NULL)
    {
        printf("\n error in opening assignment file");
    }

    student_btree* root = NULL;
    Assignment_BTreeNode* a_root = NULL;
    AssignmentGroup_BTreeNode *agroot = AssignmentGroup_createNode();

    //time_t  now  = time(NULL);
    int i=0,k=0,l=0;
    int choice,st1,ed;
    int assignmentId;
    int rno,assno,ano;
    int groupId,m1=0,m2=0,m3=0,m4=0;
    int studentId1,studentId2;
    //Student* s[MAX_GROUP_SIZE] = (Student*) malloc(sizeof(Student));
    Student** s = (Student**) malloc(MAX_SIZE * sizeof(Student*));
    for (int j = 0; j < MAX_SIZE; j++) 
    {
        s[j] = (Student*) malloc(sizeof(Student));
    }

    Assignment** a=(Assignment**) malloc(MAX_SIZE * sizeof(Assignment*));
    for (int j = 0; j < MAX_SIZE; j++) 
    {
        a[j] = (Assignment*) malloc(sizeof(Assignment));
    }
    AssignmentGroup *agt = (AssignmentGroup*)malloc(sizeof(AssignmentGroup));
    do 
    {
        print_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);


        switch (choice) 
        {
            case 1:
                // Insert a student record & Group id
                //Student* s = (Student*) malloc(sizeof(Student));
                /*printf("Enter student ID:");
                scanf("%d", &s->student_id);
                printf("Enter the Name:");
                scanf("%s", s->name);*/
                fscanf(fstu,"%d %s",&s[i]->student_id,&s[i]->name);
                //strcpy(s->name, "Shubham");
                s[i]->group_partner = NULL;
                //printf("Enter Group Id:");
                s[i]->group_id=-1;
                //printf("Enter Assignment Name:");
                //scanf("%s", &s->assignment_name);
                strcpy(s[i]->assignment_name,"");
                //strcpy(s->assignment_name, "Assignment 1");
                //printf("Enter the deadline:");
                //scanf("%d", &s->deadline);
                //s->deadline =  10 +s->deadline;
                s[i]->deadline =  10 ;
                s[i]->stustatus = not_submitted1;
                s[i]->evaluation_marks = -1;
                s[i]->viva_marks = -1;
                root = student_insert(root, s[i]);
                i++;
                break;
            case 2:
                // Insert assignment record
                //Assignment *a=(Assignment*) malloc(sizeof(Assignment));
                //printf("Enter Assignment ID:");
                fscanf(fasgn,"%d %s", &a[k]->id,&a[k]->topic);
                //printf("Enter the deadline:");
                //scanf("%d", &a->deadline);
                //a->deadline= 10 +a->deadline;
                a[k]->deadline= 10 ;
                //printf("Enter the topic:");
                //fscanf("%s", a->topic);
                //strcpy(a->topic,"Math");
                a[k]->status=declared2;
                a[k]->groups=NULL;
                a[k]->left=NULL;
                a[k]->right=NULL;
                a_root = Assignment_insert( a_root, a[k]);
                k++;
                break;
            case 3:
                // q3
                
                printf("\n enter student id and assno: ");
                scanf("%d%d",&rno,&assno);
                Student* st=student_search_stuid(root,rno);
                Assignment* as=Assignment_search(a_root,assno);
                AssignmentGroup* asgrp=AssignmentGroup_search(agroot,st->group_id);
                if(st==NULL||as==NULL||asgrp==NULL)
                {
                    printf("\n failure to open");
                }
                submit_assignment(st);
                change_assignment_status(as, submitted2);
                change_student_assignment_status(st, submitted1, -1);
                change_assignment_group_status(asgrp,declared2,-1);
                // Submit assignment
                //submit_assignment(s);
                break;
            case 4:
                // Print declared but not evaluated assignments
                Q4(root,agroot);
                break;
            case 5:
                // Print list of student groups who have not submitted the assignment even if due date is over
                print_overdue_assignments(agroot, root);
                break;
            case 6:
                // Print submitted but not evaluated assignments
                printf("\n Print submitted but not evaluated assignments");
                //Q6(agroot,root);
                qs6(a_root,root);
                break;
            case 7:
                
                printf("\n enter assignment id: ");
                scanf("%d",&assignmentId);
                //print_student_groups_by_marks(root, assignmentId);
                break;
            case 8:
                // Print Top Scorers
                //print_top_groupssp(agroot,root,aid,ag,top_marks);
                ///////////////print_top_groups(a_root, root);
                print_student_groups_top_marks(a_root,root);
                break;
            case 9:
                // Change Deadline
                printf("\n enter assignment no: ");
                scanf("%d",&ano);
                Assignment* aq=Assignment_search(a_root,ano);
                if(aq==NULL)
                {
                    printf("\n error");
                    break;
                }
                printf("\n enter the number of days by which deadline should be changed: ");
                scanf("%d",&rno);
                aq->deadline=aq->deadline+rno;
                student_search_aname(root,aq,agroot);

                //change_deadline(a_root, root, agroot);
                break;
            case 10:
                //
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                printf("\n enter assignment number to be assigned: ");
                scanf("%d",&ano);
                Student* submitStudent7 = student_search_groupid(root, groupId);
                Student* submitStudent8 = student_search_stuid(root, submitStudent7->group_partner->student_id);        
                Assignment* current1 = Assignment_search(a_root,ano);

                strcpy(submitStudent7->assignment_name,current1->topic);
            
                strcpy(submitStudent8->assignment_name,current1->topic);
                agt->groupId = groupId;
                agt->studentId1 = submitStudent7->student_id;
                agt->studentId2 = submitStudent8->student_id;
                agt->asgpstatus = declared2;
                agt->marks = -1;
                agroot=AssignmentGroup_insert(agroot, agt);
                //current1->groups=AssignmentGroup_insert(a_root->keys[]->groups, agt);
                break;
            case 11:
                // Print All Details
                student_inorder_traversal(root);
                Assignment_inorder_traversal(a_root);
                AssignmentGroup_inorder_traversal(agroot);
                break;
            case 12:
                //evaluated marks
                
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                Student* submitStudent3 = student_search_groupid(root, groupId);
                if(submitStudent3==NULL)
                {
                    printf("\n failed");
                }
                printf("\n evaluated marks for student id %d: ",submitStudent3->student_id);
                scanf("%d",&m1);
                evaluate_student(submitStudent3,m1);
                Assignment* asgn = Assignment_search_group(a_root,groupId);
                if(asgn==NULL)
                {
                    printf("\n failed");
                }
                if(asgn->status != evaluated2)
                {
                    evaluate_assignment(asgn);
                }
                Student* submitStudent4 = student_search_stuid(root, submitStudent3->group_partner->student_id);
                printf("\n evaluated marks for student id %d: ",submitStudent4->student_id);
                scanf("%d",&m2);
                
                evaluate_student(submitStudent4,m2);
                
                AssignmentGroup* asgp1 = AssignmentGroup_search(agroot,groupId);
                //AssignmentGroup* asgp8 = find_assignment_group_structure(assignmentList->groups,groupId);
                Assignment* asgp8 = Assignment_search_group(a_root,groupId);
                evaluate_assignment_group(asgp1,&asgn,m1+m2+m3+m4,groupId);
                evaluate_assignment_group(asgp8->groups,&asgn,m1+m2+m3+m4,groupId);
                //print_students(studentList) ;
                //print_assignments(assignmentList);
                //print_assignment_groups(assignmentgrouplist);
                change_assignment_status(asgn, evaluated3);
                break;
            case 13:
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                Student* submitStudent5 = student_search_groupid(root, groupId);
                printf("\n viva marks for student id %d: ",submitStudent5->student_id);
                scanf("%d",&m3);
                viva_student(submitStudent5,m3);
                printf("\nr1");
                Assignment* asgn1 = Assignment_search_group(a_root,groupId);
                printf("\nr2");
                //evaluate_assignment(asgn1);
                //if(asgn1->status != evaluated3)
                //{
                //    evaluate_assignment(asgn1);
                //}
                printf("\nr3");
                Student* submitStudent6 = student_search_stuid(root, submitStudent3->group_partner->student_id);
                printf("\nr4");
                printf("\n viva marks for student id %d: ",submitStudent6->student_id);
                scanf("%d",&m4);
                viva_student(submitStudent4,m4);
                AssignmentGroup* asgp2 = AssignmentGroup_search(agroot,groupId);
                //AssignmentGroup* asgp8 = find_assignment_group_structure(assignmentList->groups,groupId);
                Assignment* asgp9 = Assignment_search_group(a_root,groupId);
                m1=submitStudent5->evaluation_marks;
                m2=submitStudent6->evaluation_marks;
                //AssignmentGroup* asgp9 = find_assignment_group_structure(assignmentList->groups,groupId);
                evaluate_assignment_group(asgp2,&asgn1,m1+m2+m3+m4,groupId);
                evaluate_assignment_group(asgp9->groups,&asgn1,m1+m2+m3+m4,groupId);
                break;
            case 14:
                
                printf("Enter group id,studentId1,studentId2: ");
                scanf("%d%d%d", &groupId,&studentId1,&studentId2);
                Student* submitStudenAssignmentStatus = student_search_stuid(root, studentId1);
                printf("\nppppppppp\n");
                if(submitStudenAssignmentStatus==NULL)
                {
                    printf("\n error1");
                }
                submitStudenAssignmentStatus->group_id=groupId;
                Student* submitStudent2 = student_search_stuid(root, studentId2);
                if(submitStudent2==NULL)
                {
                    printf("\n error2");
                }
                submitStudent2->group_id=groupId;
                submitStudenAssignmentStatus->group_partner=submitStudent2;
                submitStudent2->group_partner=submitStudenAssignmentStatus;

                //student_inorder_traversal(root);
                break;
            case 15:
                // Submit assignment
                //submit_assignment(s);
                break;
            case 16:
                printf("\n enter range of assignment ids:");
                scanf("%d%d",&st1,&ed);
                q9(a_root,st1,ed);
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 0);


    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_NAME_LENGTH 50
#define MAX_TOPIC_LENGTH 100
#define MAX_ASSIGNMENT_LENGTH 100
#define MAX_GROUP_LENGTH 10

// Enumerations
typedef enum {not_submitted1, submitted1, evaluated1} StudentStatus;
typedef enum {declared2, due_date_overdue2, evaluated2} AssignmentGroupStatus;
typedef enum {declared3,submitted3,not_submitted3,evaluated3} AssignmentStatus;


// Structures
typedef struct Student 
{
    int student_id;
    char name[MAX_NAME_LENGTH];
    struct Student* group_partner;
    int group_id;
    char assignment_name[MAX_ASSIGNMENT_LENGTH];
    int deadline;
    StudentStatus stustatus;
    int evaluation_marks;
    int viva_marks;
    struct Student* next;
} Student;

typedef struct AssignmentGroup 
{
    int groupId;
    int studentId1;
    int studentId2;
    AssignmentGroupStatus asgpstatus;
    int marks;
    struct AssignmentGroup* next;
} AssignmentGroup;

typedef struct Assignment 
{
    int id;
    int deadline;
    char topic[MAX_TOPIC_LENGTH];
    AssignmentStatus status;
    AssignmentGroup* groups;
    struct Assignment* next;
} Assignment;

// Function declarations
void print_menu();
void print_students(Student* head);
void print_assignments(Assignment* head);
void print_assignment_groups(AssignmentGroup* head);
Student* find_student(Student* head, int id);
Student* find_studentgroup(Student* head, int id);
Assignment* find_assignment(Assignment* head, int id);
void insert_student(Student** head, int id, char* name);
void insert_assignment(Assignment** head, int id, char* topic);
void insert_assignment_group(AssignmentGroup** head, int groupId, int studentId1, int studentId2);
void change_assignment_status(Assignment* assignment, AssignmentStatus newStatus);
void change_assignment_group_status(AssignmentGroup* group, AssignmentGroupStatus newStatus, int marks);
void change_student_assignment_status(Student* student, StudentStatus newStatus, int marks);
void submit_assignment(Student* student);
void evaluate_assignment_group(AssignmentGroup* group, Assignment** head, int marks1,int gid);
void evaluate_assignment(Assignment* assignment);
void evaluate_student(Student* stu,int marks);
void viva_student(Student* stu,int marks);
void print_overdue_assignments(AssignmentGroup* head, Student* ptr);
void print_groups_not_submitted(struct Student *head, char now[]);
void print_unevaluated_groups(Assignment* head,Student* stu);
void print_assignment_details(Assignment* head, int id);
void assign_assignments(Assignment* head,Student* submitStudenAssignmentStatus,Student* submitStudent2,int gid,int ass_no);
Assignment* find_assignment_group(Assignment* assignmentList,int groupId);
AssignmentGroup* find_assignment_group_structure(AssignmentGroup* head,int gid);
void change_deadline(Student* head1, Assignment* head2, AssignmentGroup* head3);
void print_unevaluated_groups_case6(Assignment* head, Student* stu); 
void print_in_order(Student* head1, Assignment* head2);

void print_top_scoring_groups(Assignment* assignments);
void print_top_groups(Assignment* head,Student* ptr);


///sorting functions
void printAssignmentGroupsInDescendingOrderOfMarks(Assignment* assignment);
int compareAssignmentGroupsByMarks(const void* a, const void* b);






// Main function
int main() 
{
    // Initialize linked lists
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
    Student* studentList = NULL;
    Assignment* assignmentList = NULL;
    AssignmentGroup* assignmentgrouplist=NULL;
    int stuId;
    int groupId;
    char studentName[MAX_NAME_LENGTH];
    int assignmentId;
    char topic[MAX_TOPIC_LENGTH];
    int submitStudentId, submitAssignmentId;
    int ano,m1=0,m2=0,m3=0,m4=0;
    int studentId1;
    int studentId2;

    // Show menu
    int choice;
    do 
    {
        print_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                fscanf(fstu,"%d %s",&stuId,&studentName);
                insert_student(&studentList, stuId, studentName);
                
                break;
            case 2:
                fscanf(fasgn,"%d %s",&assignmentId,&topic);
                insert_assignment(&assignmentList, assignmentId, topic);
                break;
            case 3:
                printf("\n Enter group id, assignment id: ");
                scanf("%d %d", &groupId, &submitAssignmentId);
                // Find student and assignment
                Student* submitStudent = find_studentgroup(studentList, groupId);
                Assignment* submitAssignment = find_assignment(assignmentList, submitAssignmentId);
                AssignmentGroup* asgp = find_assignment_group_structure(assignmentgrouplist,groupId);
                if (submitStudent == NULL || submitAssignment == NULL || asgp == NULL) 
                {
                    printf("Invalid student or assignment id.\n");
                    break;
                }
    
                // Submit the assignment
                submit_assignment(submitStudent);
                change_assignment_status(submitAssignment, submitted3);
                change_student_assignment_status(submitStudent, submitted1, -1);
                change_assignment_group_status(asgp,declared2,-1);
                Student* submitStudenAssignmentStatus0 = find_student(studentList, submitStudent->group_partner->student_id);
                submit_assignment(submitStudenAssignmentStatus0);
                break;
            case 4:
                printf("\n declared but not evaluated assignments: \n");
                print_unevaluated_groups(assignmentList, studentList);
                printf("\n");
                break;
            case 5:
                printf("\n list of student groups who have not submitted the assignment even if due date is over: ");
                print_overdue_assignments(assignmentgrouplist, studentList);
                break;
            case 6:
                printf("\n submitted but not evaluated assignments:\n");
                print_unevaluated_groups_case6(assignmentList, studentList);
                printf("\n");
                break;
            case 7:
                //print_in_order(studentList, assignmentList);
                printf("\n enter assignment no: ");
                scanf("%d",&ano);
                Assignment* current = assignmentList;
                while(current->id!=ano)
                {
                    current=current->next;
                }
                printAssignmentGroupsInDescendingOrderOfMarks(current);
                //printGroupsByMarks(assignmentList);
                //print_groups_by_marks(1,assignmentList);
                //printAssignmentDetails(assignmentList,1);
                //printStudentGroupsForAssignment(assignmentList,1);
                break;
            case 8:
                print_top_groups(assignmentList,studentList);
                print_top_scoring_groups(assignmentList);
                break;
            case 9:
                change_deadline(studentList, assignmentList, assignmentgrouplist);
                break;
            case 10:
                printf("Enter group id,studentId1,studentId2: ");
                scanf("%d%d%d", &groupId,&studentId1,&studentId2);
    
                Student* submitStudenAssignmentStatus = find_student(studentList, studentId1);
                submitStudenAssignmentStatus->group_id=groupId;
                Student* submitStudent2 = find_student(studentList, studentId2);
                submitStudent2->group_id=groupId;
                submitStudenAssignmentStatus->group_partner=submitStudent2;
                submitStudent2->group_partner=submitStudenAssignmentStatus;
                break;
            case 11:
                printf("\n student details: ");
                print_students(studentList);
                printf("\n assignment details: ");
                print_assignments(assignmentList);
                printf("\n assignment group details: ");
                print_assignment_groups(assignmentgrouplist);
                break;
            case 12:
                //evaluated marks
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                Student* submitStudent3 = find_studentgroup(studentList, groupId);
                if(submitStudent3==NULL)
                {
                    printf("\n failed");
                }
                printf("\n evaluated marks for student id %d: ",submitStudent3->student_id);
                scanf("%d",&m1);
                evaluate_student(submitStudent3,m1);
                Assignment* asgn = find_assignment_group(assignmentList,groupId);
                if(asgn==NULL)
                {
                    printf("\n failed");
                }
                if(asgn->status != evaluated3)
                {
                    evaluate_assignment(asgn);
                }
                Student* submitStudent4 = find_student(studentList, submitStudent3->group_partner->student_id);
                printf("\n evaluated marks for student id %d: ",submitStudent4->student_id);
                scanf("%d",&m2);
                
                evaluate_student(submitStudent4,m2);
                
                AssignmentGroup* asgp1 = find_assignment_group_structure(assignmentgrouplist,groupId);
                //AssignmentGroup* asgp8 = find_assignment_group_structure(assignmentList->groups,groupId);
                AssignmentGroup* asgp8 = find_assignment_group_structure(asgn->groups,groupId);
                evaluate_assignment_group(asgp1,&asgn,m1+m2+m3+m4,groupId);
                evaluate_assignment_group(asgp8,&asgn,m1+m2+m3+m4,groupId);
                //print_students(studentList) ;
                //print_assignments(assignmentList);
                //print_assignment_groups(assignmentgrouplist);
                change_assignment_status(submitAssignment, evaluated3);
                break;
            case 13:
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                Student* submitStudent5 = find_studentgroup(studentList, groupId);
                printf("\n viva marks for student id %d: ",submitStudent5->student_id);
                scanf("%d",&m3);
                viva_student(submitStudent5,m3);
                printf("\nr1");
                Assignment* asgn1 = find_assignment_group(assignmentList,groupId);
                printf("\nr2");
                //evaluate_assignment(asgn1);
                //if(asgn1->status != evaluated3)
                //{
                //    evaluate_assignment(asgn1);
                //}
                printf("\nr3");
                Student* submitStudent6 = find_student(studentList, submitStudent5->group_partner->student_id);
                printf("\nr4");
                printf("\n viva marks for student id %d: ",submitStudent6->student_id);
                scanf("%d",&m4);
                viva_student(submitStudent4,m4);
                AssignmentGroup* asgp2 = find_assignment_group_structure(assignmentgrouplist,groupId);
                m1=submitStudent5->evaluation_marks;
                m2=submitStudent6->evaluation_marks;
                AssignmentGroup* asgp9 = find_assignment_group_structure(assignmentList->groups,groupId);
                evaluate_assignment_group(asgp2,&asgn1,m1+m2+m3+m4,groupId);
                evaluate_assignment_group(asgp8,&asgn,m1+m2+m3+m4,groupId);
                //print_students(studentList);
                //print_assignments(assignmentList);
                //print_assignment_groups(assignmentgrouplist);
                break;
            case 14:
                printf("\n enter group id: ");
                scanf("%d",&groupId);
                printf("\n enter assignment number to be assigned: ");
                scanf("%d",&ano);
                Student* submitStudent7 = find_studentgroup(studentList, groupId);
                Student* submitStudent8 = find_student(studentList, submitStudent7->group_partner->student_id);
        
                Assignment* current1 = assignmentList;
                while(current1->id!=ano)
                {
                    current1=current1->next;
                }
                strcpy(submitStudent7->assignment_name,current1->topic);
            
                strcpy(submitStudent8->assignment_name,current1->topic);
                
                insert_assignment_group(&current1->groups, groupId, submitStudent7->student_id, submitStudent8->student_id);
                insert_assignment_group(&assignmentgrouplist, groupId, submitStudent7->student_id, submitStudent8->student_id);
                break;
            default:
                printf("\n default exit");
                break;
        }
 }while(choice!=15);
 fclose(fstu);
 fclose(fasgn);
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
    printf("15. Exit\n");
}

void print_students(Student* head) 
{
    FILE* fstu1;
    fstu1 = fopen("write_student_details.txt","w");
    if(fstu1==NULL)
    {
        printf("\n error");
        return;
    }
    Student* current = head;
    while (current != NULL) 
    {
        printf("Student ID: %d\n", current->student_id);
        fprintf(fstu1,"Student ID: %d\n", current->student_id);
        printf("Name: %s\n", current->name);
        fprintf(fstu1,"Name: %s\n", current->name);
        if(current->group_partner!=NULL)
        {
            printf("Group Partner: %s\n", current->group_partner->name);
            fprintf(fstu1,"Group Partner: %s\n", current->group_partner->name);
        }
        printf("Group ID: %d\n", current->group_id);
        fprintf(fstu1,"Group ID: %d\n", current->group_id);
        printf("Assignment Name: %s\n", current->assignment_name);
        fprintf(fstu1,"Assignment Name: %s\n", current->assignment_name);
        printf("Deadline: %d\n", current->deadline);
        fprintf(fstu1,"Deadline: %d\n", current->deadline);
        switch(current->stustatus) 
        {
            case not_submitted1:
                printf("Status: Not submitted\n");
                fprintf(fstu1,"Status: Not submitted\n");
                break;
            case submitted1:
                printf("Status: submitted\n");
                fprintf(fstu1,"Status: submitted\n");
                break;
            case evaluated1:
                printf("Status: evaluated1\n");
                fprintf(fstu1,"Status: evaluated1\n");
                break;
        }
        printf("Evaluation Marks: %d\n", current->evaluation_marks);
        fprintf(fstu1,"Evaluation Marks: %d\n", current->evaluation_marks);
        printf("Viva Marks: %d\n", current->viva_marks);
        fprintf(fstu1,"Evaluation Marks: %d\n", current->evaluation_marks);
        printf("\n");
        fprintf(fstu1,"\n");
        current = current->next;
    }
    fclose(fstu1);
}

void assign_assignments(Assignment* head,Student* submitStudent1,Student* submitStudent2,int gid,int ass_no)
{
    Assignment* current = head;
    while(current->id!=ass_no)
    {
        current=current->next;
    }
    strcpy(submitStudent1->assignment_name,current->topic);
    strcpy(submitStudent2->assignment_name,current->topic);
    insert_assignment_group(&current->groups,gid,submitStudent1->student_id,submitStudent2->student_id);

}

void print_assignments(Assignment* head)
{
    FILE* fass1;
    fass1 = fopen("write_assignment_details.txt","w");
    if(fass1==NULL)
    {
        printf("\n error");
        return;
    }
    Assignment* current = head;
    while (current != NULL)
    {
        printf("Assignment ID: %d\n", current->id);
        fprintf(fass1,"Assignment ID: %d\n", current->id);
        printf("Topic: %s\n", current->topic);
        fprintf(fass1,"Topic: %s\n", current->topic);
        switch(current->status) 
        {
            case declared3:
                printf("Status: Declared\n");
                fprintf(fass1,"Status: Declared\n");
                break;
            case not_submitted3:
                printf("Status: Not submitted\n");
                fprintf(fass1,"Status: Not submitted\n");
                break;
            case submitted3:
                printf("Status: submitted\n");
                fprintf(fass1,"Status: submitted\n");
                break;
            case evaluated3:
                printf("Status: evaluated\n");
                fprintf(fass1,"Status: evaluated\n");
                break;
        }
        printf("Assignment deadline: %d\n", current->deadline);
        fprintf(fass1,"Assignment deadline: %d\n", current->deadline);
        printf("\n");
        fprintf(fass1,"\n");
        current = current->next;
    }
    fclose(fass1);
}

void print_assignment_groups(AssignmentGroup* head)
{
    FILE* fasgp1;
    fasgp1 = fopen("write_assignmentgroup_details.txt","w");
    if(fasgp1==NULL)
    {
        printf("\n error");
        return;
    }
    AssignmentGroup* current = head;
    while (current != NULL)
    {
        printf("Group ID: %d\n", current->groupId);
        fprintf(fasgp1,"Group ID: %d\n", current->groupId);
        printf("Student ID 1: %d\n", current->studentId1);
        fprintf(fasgp1,"Student ID 1: %d\n", current->studentId1);
        printf("Student ID 2: %d\n", current->studentId2);
        fprintf(fasgp1,"Student ID 2: %d\n", current->studentId2);
        switch(current->asgpstatus) 
        {
            case declared2:
                printf("Status: Declared\n");
                fprintf(fasgp1,"Status: Declared\n");
                break;
            case due_date_overdue2:
                printf("Status: Not submitted\n");
                fprintf(fasgp1,"Status: Not submitted\n");
                break;
            case evaluated2:
                printf("Status: evaluated\n");
                fprintf(fasgp1,"Status: evaluated\n");
                break;
        }
        printf("Marks: %d\n", current->marks);
        fprintf(fasgp1,"Marks: %d\n", current->marks);
        printf("\n");
        fprintf(fasgp1,"\n");
        current = current->next;
    }
    fclose(fasgp1);
}

Student* find_student(Student* head, int id)
{
    Student* current = head;
    while (current != NULL)
    {
        if (current->student_id == id)
        {
        return current;
        }
        current = current->next;
    }
    return NULL;
}
Student* find_studentgroup(Student* head, int id)
{
    Student* current = head;
    while (current != NULL)
    {
        if (current->group_id == id)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
Assignment* find_assignment(Assignment* head, int id)
{
    Assignment* current = head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
Assignment* find_assignment_group(Assignment* assignmentList,int groupid)
{
    Assignment* current = assignmentList;
    while (current != NULL)
    {
        AssignmentGroup* curr = current->groups;
        while(curr!=NULL)
        {
            //printf("\n w1");
            if (curr->groupId == groupid)
            {
                //printf("\n w2");
                return current;
            }
            curr = curr -> next;
        }
        current = current->next;
    }
    return NULL;
}

AssignmentGroup* find_assignment_group_structure(AssignmentGroup* head,int gid)
{
    AssignmentGroup* current = head;
    while (current != NULL)
    {
        if (current->groupId == gid)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void insert_student(Student** head, int id, char* name)
{
    // Check if student with the same ID already exists
    Student* current = *head;
    while (current != NULL)
    {
        if (current->student_id == id)
        {
            printf("\nError: A student with the same ID already exists.\n");
            return;
        }
        current = current->next;
    }
    // Create new student node
    Student* new_student = (Student*) malloc(sizeof(Student));
    new_student->student_id = id;
    strcpy(new_student->name, name);
    new_student->group_partner = NULL;
    //strcpy(new_student->group_partner->name,"") ;
    new_student->group_id = -1;
    strcpy(new_student->assignment_name, "");
    //strcpy(new_student->deadline, "");
    new_student->deadline=10;
    new_student->stustatus = not_submitted1;
    new_student->evaluation_marks = -1;
    new_student->viva_marks = -1;
    new_student->next = NULL;
    // Add student to the end of the list
    if (*head == NULL)
    {
        *head = new_student;
    }
    else
    {
        Student* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_student;
    }
}

void insert_assignment(Assignment** head, int id, char* topic)
{
    // Check if assignment with the same ID already exists
    Assignment* current = *head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            printf("\nError: An Assignment with the same ID already exists.\n");
            return;
        }
        current = current->next;
    }
    // Create new assignment node
    Assignment* new_assignment = (Assignment*) malloc(sizeof(Assignment));
    new_assignment->id = id;
    strcpy(new_assignment->topic, topic);
    new_assignment->status = declared3;
    new_assignment->groups = NULL;
    new_assignment->deadline=10;
    new_assignment->next = NULL;
    // Add assignment to the end of the list
    if (*head == NULL)
    {
        *head = new_assignment;
    }
    else   
    {
        Assignment* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_assignment;
    }
}

void insert_assignment_group(AssignmentGroup** head, int groupId, int studentId1, int studentId2) 
{
    // Check if group with the same ID already exists
    AssignmentGroup* curr = *head;
    while (curr != NULL) 
    {
        if (curr->groupId == groupId) 
        {
        printf("\nError: A group with the same ID already exists.\n");
        return;
        }
        curr = curr->next;
    }
    // Allocate memory for the new group
    AssignmentGroup* newGroup = (AssignmentGroup*)malloc(sizeof(AssignmentGroup));
    newGroup->groupId = groupId;
    newGroup->studentId1 = studentId1;
    newGroup->studentId2 = studentId2;
    newGroup->asgpstatus= declared2;
    newGroup->marks=-1;
    newGroup->next = NULL;
    printf("\n successfully grouped");
    // If the list is empty, make the new group the head
    if (*head == NULL) {
        printf("\n null");
        *head = newGroup;
        return;
    }
    /*else
    {
        AssignmentGroup* current = *head;
        while (current->next != NULL) 
        {
            printf("\n next");
            current = current->next;
        }
        current->next = newGroup;
    }*/
    // Traverse the list to find the last group
    AssignmentGroup* current = *head;
    while (current->next != NULL) {
        printf("\n next");
        current = current->next;
    }

    // Add the new group to the end of the lis  U7t
    current->next = newGroup;
    return;
    //printf("\n successfully grouped");
}

void change_assignment_status(Assignment* assignment, AssignmentStatus newStatus) 
{
    assignment->status = newStatus;
    //printf("\n assignment submitted");
}

void change_assignment_group_status(AssignmentGroup* group, AssignmentGroupStatus newStatus, int marks) 
{
    group->asgpstatus = newStatus;
    group->marks = marks;
    //printf("p");
}

void change_student_assignment_status(Student* student, StudentStatus newStatus, int marks) 
{
    student->stustatus=newStatus;
    student->viva_marks=marks;
    student->evaluation_marks=marks;
    //printf("\n asgrp updated");
}

void submit_assignment(Student* student) 
{
    if(student->stustatus==not_submitted1)
    {
        student->stustatus=submitted1;
        //printf("\n subm");
    }
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

void evaluate_assignment(Assignment* assignment) 
{
    //printf("\n m1 %s",assignment->status);
    if(assignment->status != evaluated3)
    {
        assignment->status = evaluated3;
    }
    //printf("\n m2");
    assignment->groups->asgpstatus= evaluated2;
    //printf("\n m3");
}
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
void print_overdue_assignments(AssignmentGroup* head,Student* ptr) 
{
    AssignmentGroup* curr=head;
    while(curr!=NULL)
    {
        if(curr->asgpstatus==due_date_overdue2)
        {
            printf("\n Group ID: %d is overdue.", curr->groupId);
            Student* stu=ptr;
            while(stu->group_id!=curr->groupId)
            {
                stu=stu->next;
            }
            printf("\n StudenAssignmentStatus Id:%d , StudenAssignmentStatus Name- %s", stu->student_id,stu->name);
            printf("\n Student2 Id:%d , Student2 Name- %s", stu->group_partner->student_id,stu->group_partner->name);
        }
        curr=curr->next;
    }
}

void print_unsubmitted_groups(Assignment* head) 
{
    Assignment* currentAssignment = head;
    while (currentAssignment != NULL) 
    {
        AssignmentGroup* currentGroup = currentAssignment->groups;
        while (currentGroup != NULL) 
        {
            if (currentGroup->asgpstatus == not_submitted3) 
            {
                printf("Group ID: %d for Assignment ID: %d has not been submitted.\n", currentGroup->groupId, currentAssignment->id);
            }
            currentGroup = currentGroup->next;
        }
        currentAssignment = currentAssignment->next;
    }
}
void print_declared_assignments(Assignment *assignments_head) 
{
    Assignment *current = assignments_head;
    while (current != NULL) 
    {
        Assignment *assignment = (Assignment *) current->groups;
        //if (strcmp(assignment->status, "declared") == 0 && assignment->offline_evaluation_marks == 0 && assignment->viva_marks == 0) 
        if(assignment->status==declared2 && current->status==declared3)
        {
            print_assignments(assignment);
        }
        current = current->next;
    }
}

void print_unevaluated_groups(Assignment* head, Student* stu) 
{
    Assignment* currentAssignment = head;
    while (currentAssignment != NULL) 
    {
        Student* curr=stu;
        AssignmentGroup* currentGroup = currentAssignment->groups;
        while (currentGroup != NULL) 
        {
            //int f=0;
            //printf("\n tyu1");
            if (currentGroup->asgpstatus == declared2 && currentAssignment->status == declared3 ) 
            {
                //printf("\n tyu2");
                while(curr->group_id!=currentGroup->groupId&&curr!=NULL)
                {
                    //f=1;
                    curr=curr->next;
                    //printf("\n tyu3");
                }
                if(curr->evaluation_marks==-1)//&&f==1)
                {
                    printf("Group ID: %d for Assignment ID: %d has not been evaluated1.\n", currentGroup->groupId, currentAssignment->id);
                }
                else if(curr->viva_marks==-1)//&&f==1)
                {
                    printf("Group ID: %d for Assignment ID: %d has not conducted viva.\n", currentGroup->groupId, currentAssignment->id);
                }
                //printf("\n tyu4");
            }
            currentGroup = currentGroup->next;
            //printf("\n tyu5");
        }
        currentAssignment = currentAssignment->next;
    }
}

void print_unevaluated_groups_case6(Assignment* head, Student* stu) 
{
    Assignment* currentAssignment = head;
    while (currentAssignment != NULL) 
    {
        //Student* curr=stu;
        AssignmentGroup* currentGroup = currentAssignment->groups;
        while (currentGroup != NULL) 
        {
            Student* curr=stu;
            while(stu->group_id!=currentGroup->groupId&&stu!=NULL)
            {
                stu=stu->next;
            }
            if(stu->evaluation_marks==-1&&stu->stustatus==submitted1)
            {
                printf("Group ID: %d for Assignment ID: %d has not been evaluated1.\n", currentGroup->groupId, currentAssignment->id);
            }
            if(stu->viva_marks==-1&&(stu->stustatus==evaluated1||stu->stustatus==submitted1))
            {
                printf("Group ID: %d for Assignment ID: %d has not conducted viva.\n", currentGroup->groupId, currentAssignment->id);
            }
            currentGroup = currentGroup->next;
        }
        currentAssignment = currentAssignment->next;
    }
}


void change_deadline( Student* head1, Assignment* head2,AssignmentGroup *head3)
{
    int assignmentID;
    int days;
    int flag = 1;
    Student* current = head1;
    Assignment* curr = head2;

    Assignment* curr_prev;
    printf("\nEnter assignment ID:");
    scanf("%d", &assignmentID);
    printf("\nEnter the no of days by which deadline should be changed :");
    scanf("%d",&days);
    while (curr != NULL && flag == 1) 
    {
        if(curr->id == assignmentID)
        {
            curr -> deadline = curr->deadline + days;
            flag = 0;
        }
        curr_prev = curr;
        curr = curr->next;
    }
    while(current != NULL)
    {
        if(strcmp(current -> assignment_name , curr_prev -> topic ) == 0)
        {
            current -> deadline = current->deadline + days;
            if(current->deadline <= 0)
            {
                curr_prev -> groups -> asgpstatus = due_date_overdue2;
                AssignmentGroup* c = find_assignment_group_structure(head3,current->group_id);
                c->asgpstatus = due_date_overdue2;
            }
        }
        current = current ->next;
    }
}

void print_in_order(Student* head1, Assignment* head2)
{
    Student* current = head1;
    Assignment* curr  = head2;

    Assignment* curr_need;
    int assign_id;
    printf("Enter the Assignment ID: \n");
    scanf("%d",&assign_id);

    while(curr != NULL)
    {
        if(curr -> id == assign_id)
        {
            curr_need = curr;
        }
        curr = curr -> next;
    }
    //curr_need->groups->marks;

    AssignmentGroup* temp1;
    AssignmentGroup* temp2;
    AssignmentGroup* head = curr_need->groups;
    while(head != NULL)
    {
        if(head->marks > head->next->marks)
        {
            temp2 = head->next->next;
            temp1 = head;
            head= head-> next;
            head-> next = temp1;
            head->next->next = temp2;
        }
        head = head -> next;
    }
    head = curr_need->groups;
    while(head != NULL)
    {
        print_assignment_groups(head);
        head = head -> next;
    }
}

/*
// Function to merge two sorted linked lists into a single sorted linked list
struct Student* merge(struct Student *lisAssignmentStatus, struct Student *list2) {
    struct Student *merged_list = NULL;
    if (lisAssignmentStatus == NULL) {
        return list2;
    }
    else if (list2 == NULL) {
        return lisAssignmentStatus;
    }
    if (lisAssignmentStatus->marks <= list2->marks) {
        merged_list = lisAssignmentStatus;
        merged_list->next = merge(lisAssignmentStatus->next, list2);
    }
    else {
        merged_list = list2;
        merged_list->next = merge(lisAssignmentStatus, list2->next);
    }
    return merged_list;
}

// Function to split a linked list into two halves
void split(struct student *head, struct student **front, struct student **back) {
    struct student *fast, *slow;
    slow = head;
    fast = head->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = head;
    *back = slow->next;
    slow->next = NULL;
}

// Function to sort the linked list in ascending order based on marks
void merge_sort(struct student **head_ref) {
    struct student *head = *head_ref;
    struct student *lisAssignmentStatus, *list2;
    if (head == NULL || head->next == NULL) {
        return;
    }
    split(head, &lisAssignmentStatus, &list2);
    merge_sort(&lisAssignmentStatus);
    merge_sort(&list2);
    *head_ref = merge(lisAssignmentStatus, list2);
}*/


/*merge sort

    void printAssignmentGroupsInDescendingOrderOfMarks(AssignmentGroup* head) {
    // Sort the AssignmentGroup list by marks using merge sort
    AssignmentGroup* sortedList = mergeSortAssignmentGroups(head);

    // Traverse the sorted list and print the groups in descending order of marks
    printf("Groups in descending order of marks:\n");
    AssignmentGroup* curr = sortedList;
    while (curr != NULL) {
        printf("Group %d: Student %d, Student %d, Marks: %d\n", 
               curr->groupId, curr->studentId1, curr->studentId2, curr->marks);
        curr = curr->next;
    }

    // Free the memory allocated for the sorted list
    freeAssignmentGroupList(sortedList);
}

AssignmentGroup* mergeSortAssignmentGroups(AssignmentGroup* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    AssignmentGroup* mid = getMiddleAssignmentGroup(head);
    AssignmentGroup* nextOfMid = mid->next;
    mid->next = NULL;

    AssignmentGroup* left = mergeSortAssignmentGroups(head);
    AssignmentGroup* right = mergeSortAssignmentGroups(nextOfMid);

    return mergeAssignmentGroups(left, right);
}

AssignmentGroup* getMiddleAssignmentGroup(AssignmentGroup* head) {
    AssignmentGroup* slow = head;
    AssignmentGroup* fast = head->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    return slow;
}

AssignmentGroup* mergeAssignmentGroups(AssignmentGroup* left, AssignmentGroup* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    AssignmentGroup* result = NULL;
    if (left->marks >= right->marks) {
        result = left;
        result->next = mergeAssignmentGroups(left->next, right);
    } else {
        result = right;
        result->next = mergeAssignmentGroups(left, right->next);
    }

    return result;
}

void freeAssignmentGroupList(AssignmentGroup* head) {
    AssignmentGroup* curr = head;
    while (curr != NULL) {
        AssignmentGroup* temp = curr;
        curr = curr->next;
        free(temp);
    }
}
*/

void print_top_groups(Assignment* head,Student* ptr)
{
    Assignment* curr=head;
    while(curr!=NULL)
    {
        AssignmentGroup* currentGroup = curr->groups;
        int marksmax=-1,gid;
        while(currentGroup!=NULL)
        {
            printf("\np1");
            if(currentGroup->marks>=marksmax)
            {
                marksmax=currentGroup->marks;
                gid=currentGroup->groupId;
                printf("\n p2 %d %d",marksmax,gid);
            }
            currentGroup=currentGroup->next;
        }
        printf("\n p3 %d %d",marksmax,gid);
        printf("\n group with maximum marks in assignment %d is %d",curr->id,gid);
        Student* currstu=ptr;
        while(currstu->group_id!=gid)
        {
            currstu=currstu->next;
        }
        printf("\n Student1 Id:%d , Student1 Name- %s", currstu->student_id,currstu->name);
        printf("\n Student2 Id:%d , Student2 Name- %s", currstu->group_partner->student_id,currstu->group_partner->name);
        curr=curr->next;
    }
}

void printAssignmentGroupsInDescendingOrderOfMarks(Assignment* assignment) {
    AssignmentGroup* group = assignment->groups;
    int count = 0;
    while (group != NULL) {
        count++;
        group = group->next;
    }

    AssignmentGroup* groupArray = (AssignmentGroup*) malloc(count * sizeof(AssignmentGroup));
    group = assignment->groups;
    int i = 0;
    while (group != NULL) {
        groupArray[i] = *group;
        group = group->next;
        i++;
    }

    qsort(groupArray, count, sizeof(AssignmentGroup), compareAssignmentGroupsByMarks);

    printf("Groups in descending order of marks:\n");
    for (i = 0; i < count; i++) {
        printf("Group %d: Student %d, Student %d, Marks: %d\n", 
               groupArray[i].groupId, groupArray[i].studentId1, groupArray[i].studentId2, groupArray[i].marks);
    }

    free(groupArray);
}

int compareAssignmentGroupsByMarks(const void* a, const void* b) {
    AssignmentGroup* groupA = (AssignmentGroup*) a;
    AssignmentGroup* groupB = (AssignmentGroup*) b;
    return (groupB->marks - groupA->marks);
}




void print_top_scoring_groups(Assignment* assignments) 
{
    for (Assignment* assignment = assignments; assignment != NULL; assignment = assignment->next) 
    {
        printf("\n Assignment %d - Topic: %s, Deadline: %d", assignment->id, assignment->topic, assignment->deadline);
        AssignmentGroup* top_group = NULL;
        int top_marks = -1;
        for (AssignmentGroup* group = assignment->groups; group != NULL; group = group->next) 
        {
            if (group->marks > top_marks) 
            {
                top_marks = group->marks;
                top_group = group;
            }
        }
        if (top_group != NULL) 
        {
            printf("\n  Top-scoring group: %d ,Student Id: %d, %d - Marks: %d", top_group->groupId, top_group->studentId1, top_group->studentId2, top_marks);
        }
        else 
        {
            printf("\n  No groups found.");
        }
    }
}


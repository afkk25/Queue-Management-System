#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CSC4352_QUEUE_SIZE 5

typedef struct{
    char student_name[30];
    char student_id[7];
}student_info;

typedef struct nd{
    student_info student;
    struct nd* next;
}node;

void Menu(void){
    printf("\n\t\t------------------- MENU-------------------\n");
    printf("\n\t\t1.Reserve a seat\n");
    printf("\n\t\t2.Cancel reservation\n");
    printf("\n\t\t3.Check the content of a queue\n");
    printf("\n\t\t4.Pre-registration is over\n");
    printf("\n\t\t5.QUIT\n");
    printf("\n\t\t--------------------------------------------\n");
    printf("\n\t\tYour choice please: ");
}


int enqueue(student_info CSC4352_queue[], student_info ToEnqueue,int N, int*rear){
    if(*rear == N-1){
        return 0; // overflow
    }else{
        CSC4352_queue[++(*rear)] = ToEnqueue;
        return 1;
    }  
}

void enqueue_LL(node**front_LL, node**rear, node*ToEnqueue){ 
    if((*rear) == NULL)
        *front_LL = ToEnqueue;
    else
        (*rear)->next = ToEnqueue;
    *rear = ToEnqueue;
}

student_info collect_info(){   
    student_info ToEnqueue;
    printf("\n\t\tEnter the name of the student: ");
    getchar();
    gets(ToEnqueue.student_name);
    printf("\n\t\tEnter the student ID: ");
    gets(ToEnqueue.student_id);

    return(ToEnqueue);
}

node* Create_Fill_node(student_info ToEnqueue){
    node*ptr = (node*)malloc(sizeof(node));
    strcpy(ptr->student.student_name, ToEnqueue.student_name);
    strcpy(ptr->student.student_id, ToEnqueue.student_id);
    ptr->next = NULL;
    return ptr;
}

int cancel_registration(char*id, student_info CSC4352_queue[], int size, int*rear, node*Dequeued_node, node**front_LL, node**rear_LL, int n){
    int i, j, flag;
    if(n == 1){
        for(i = 0; i < size; i++){
            if(strcmp(id, CSC4352_queue[i].student_id) == 0){ 
                for(j = i; j < size - 1; j++)
                    CSC4352_queue[j] = CSC4352_queue[j+1]; 
                
                if(Dequeued_node != NULL){
                    strcpy(CSC4352_queue[j].student_name, Dequeued_node->student.student_name);
                    strcpy(CSC4352_queue[j].student_id, Dequeued_node->student.student_id);
                    flag = 1;
                    break;
                }else{
                    (*rear)--;
                    flag = -1; // -1 means that no student from the waiting list was added!
                }
            }
        }
        
    }else if(n == 2){
        node*temp, *walker;
        if(strcmp(id, (*front_LL)->student.student_id) == 0){
            temp = (*front_LL);
            if((*front_LL) == (*rear_LL))
               *rear_LL = NULL;
            *front_LL = (*front_LL)->next;
            free(temp);
        }

        else{
            walker = (*front_LL);
            while(walker->next != NULL){
                if(strcmp(id, walker->next->student.student_id) == 0){
                    temp = walker->next;
                    walker->next = temp->next;
                    *rear_LL = walker;
                    free(temp);
                    break;
                }else{
                    walker = walker->next;
                }
            }
        }
    }
    return flag;
}

int check_student_existance(student_info CSC4352_queue[], int size, char*id_tolookfor, node*front_LL){  // This function checks a student existance in both the main queue and the waiting queue.
    int i, flag = 0;
    for(i = 0; i < size; i++){
        if(strcmp(id_tolookfor, CSC4352_queue[i].student_id) == 0){
            flag = 1;  //student found in the main queue
            break;
        }
    }
    node* walker = front_LL;
    while(walker != NULL){
        if(strcmp(id_tolookfor, walker->student.student_id) == 0){
        flag = 2;  //student found in the waiting list
        break;
        }else{
            walker = walker->next;
        }
    }

    return flag;  /* flag == 0 if student does not exist, else flag == 1 if student exists in the main queue & flag == 2 if student
                     exists in the waiting list */
}

node* dequeue_LL(node**front_LL, node**rear){ 
    node* temp;
    if(*front_LL == NULL){ 
        return NULL;
    }else{
        temp = *front_LL;
        *front_LL = (*front_LL)->next;    
        if(*front_LL == NULL)
          *rear = NULL;
        return temp;
    }    
}

void Main_Queue_content(student_info CSC4352_queue[], int rear){
    int i;
    printf("\n\t\t--------------------------------------------\n");
    for(i = 0; i < (rear + 1); i++)
        printf("\n\t\t%s\t%s\n",CSC4352_queue[i].student_name, CSC4352_queue[i].student_id);
    
}  

void Waiting_Queue_content(node*front_LL){  
    node*walker;
        walker = front_LL;
        printf("\n\t\t--------------------------------------------\n");
        while(walker != NULL){
            printf("\n\t\t%s\t%s\n", walker->student.student_name, walker->student.student_id);
            walker = walker->next;
        }
    
}

void generate_file(student_info CSC4352_queue[], int*rear){
    FILE*outfp;
    outfp = fopen("CSC4352_Fall2023_List.txt", "w"); 

    for(int i = (*rear);i >= 0 ;i--){
        fprintf(outfp,"%s\t\t%s\n",CSC4352_queue[0].student_name, CSC4352_queue[0].student_id);
        for(int j = 0; j < i; j++)
            CSC4352_queue[j] = CSC4352_queue[j+1];
        
        (*rear)--;
    }

    fclose(outfp);
}

void destroy_waitlist(node**front_LL, node**rear_LL){
node*walker = (*front_LL);
node*temp;
    while(walker != NULL){
        temp = walker;
        walker = walker->next;
        free(temp);
    }
    *front_LL = NULL;
    *rear_LL = NULL;
}


int main(void){

    student_info CSC4352_queue[5];
    student_info ToEnqueue;
    node*dequeued_student = NULL;
    node* student;
    node*front_LL = NULL, *rear_LL = NULL;
    int rear = -1, front = 0;
    int choice, status;
    
    do
    {
        Menu();
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            ToEnqueue = collect_info();
            if(enqueue(CSC4352_queue, ToEnqueue, CSC4352_QUEUE_SIZE, &rear) == 0){
                node* student = Create_Fill_node(ToEnqueue);
                enqueue_LL(&front_LL, &rear_LL, student);
                printf("\n\t\tQueue is full! %s was added to the CSC 4352 waiting list queue!\n", student->student.student_name);
            }else{
                printf("\n\t\t%s was successfully added to the CSC 4352 queue!\n", ToEnqueue.student_name);
            }
            break;
        case 2:
            int index;
            char id_from_user[7];
            if(rear == -1)
                printf("\n\t\tSorry, The CSC4352 main queue is empty.\n");
            else{
                printf("\n\t\tPlease enter the ID to cancel registration for: "); 
                getchar();
                gets(id_from_user);
                status = check_student_existance(CSC4352_queue, CSC4352_QUEUE_SIZE, id_from_user, front_LL);
                if(status != 0){
                    if(status == 1)
                        dequeued_student = dequeue_LL(&front_LL, &rear_LL);
                    index = cancel_registration(id_from_user, CSC4352_queue, CSC4352_QUEUE_SIZE, &rear, dequeued_student, &front_LL, &rear_LL, status);
                    if(status == 1){
                        printf("\n\t\tStudent successfully removed from the CSC4352 Main Queue!\n");
                        if(index == 1)
                            printf("\n\t\tInfo about the student who just joined the queue:\n\n\t\tName: %s\n\n\t\tID: %s\n", dequeued_student->student.student_name, dequeued_student->student.student_id);
                        else if(index == -1)
                            printf("\n\t\tNo student was added to the CSC4352 queue.\n\n\t\tReason: Empty waiting list\n");                       
                    }
                    else if(status == 2)
                        printf("\n\t\tStudent successfully removed from the CSC4352 waiting list Queue!\n");           
                }else
                    printf("\n\t\tStudent not found!\n");
                
            }
            break;
        case 3: 
            int user_choice;
            printf("\n\t\tWhich queue do you want to display?\n\n\t\t1. Main Queue      2. Waiting Queue\n\n\t\t");
            scanf("%d", &user_choice);
            if(user_choice == 1){
                if(rear == -1)
                    printf("\n\t\tCSC4352 Main Queue is EMPTY!\n");
                else
                    Main_Queue_content(CSC4352_queue, rear);           
                break; 
            }
            else if(user_choice == 2){
                if(front_LL == NULL)
                    printf("\n\t\tWaiting list is EMPTY!\n"); 
                else
                    Waiting_Queue_content(front_LL);
                break;
            }
            else{
                printf("\n\t\tPlease choose a valid option!\n");
            }
            break;
        case 4:
            if(rear != -1){     
                generate_file(CSC4352_queue, &rear);
                printf("\n\t\tPre-registration is over! A file with students' names and IDs has been generated.\n");
                destroy_waitlist(&front_LL, &rear_LL);
            }else
                printf("\n\t\tNo student is registred for this course!\n");
            break;
        case 5:
            printf("\n\t\tYou decided to quit! Bye!\n");
            break;
        default:
            printf("\n\t\tInvalid choice.\n");
            break;
        }
    }while (choice != 5);

    return 0;
}
/*
Project by Afaf Bentakhou
A system to manage students special request to enroll in courses using data structures (heap), strings operations, and files
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 10

typedef struct
{
    char student_name[50];
    int student_ID;
    int priority;
}student_info;

void Menu(void){
    printf("\n\t\t------------------- MENU-------------------\n");
    printf("\n\t\ta. Construct the initial heap\n");
    printf("\n\t\tb. Add a new element to the heap\n");
    printf("\n\t\tc. Print the heap content\n");
    printf("\n\t\td. Heap sort\n");
    printf("\n\t\te. Calculate the height of the heap\n");
    printf("\n\t\tf. Special request is over\n");
    printf("\n\t\tg. Quit\n");
    printf("\n\t\t--------------------------------------------\n");
    printf("\n\t\tYour choice please: ");
}

int evaluate_priority(char*string){
    
    if(strcmp(string, "If I don't take the course, I will not be able to graduate on time") == 0) // The Apostrophe in the file is different than the one in Vs Code
        return 20;
    else if(strcmp(string, "I am on probation and that's the only course I can add to my schedule") == 0) // The Apostrophe in the file is different than the one in Vs Code
        return 15;
    else if(strcmp(string, "I want to take the course as elective") == 0)
        return 10;
    else if(strcmp(string, "I want to take the course as audit") == 0)
        return 5;
    else
        return 0; // Any other reason other than the ones above are given a priority of 0

}

void swap(student_info*S1, student_info*S2){
    student_info temp;
    temp = *S1;
    *S1 = *S2;
    *S2 = temp;
}

int find_largest_index(student_info *max_heap, int index1, int index2){
    if(max_heap[index1].priority < max_heap[index2].priority)
        return index2;
    else
        return index1;
}

void heapify(student_info *max_heap, int parent_index, int last_index){

    int right_index, left_index, largest_index;
    if(parent_index > (last_index-1)/2)
        return;                             // Base case
    else{
        left_index = (2*parent_index)+1;
        right_index = (2*parent_index)+2;
        if(right_index > last_index)
            largest_index = find_largest_index(max_heap, parent_index, left_index);
        else{
            largest_index = find_largest_index(max_heap, parent_index, left_index);
            largest_index = find_largest_index(max_heap, largest_index, right_index);
        }
        if(largest_index != parent_index){
            swap(&max_heap[largest_index], &max_heap[parent_index]);  
            heapify(max_heap, largest_index, last_index);
        }
        
    }
}

int construct_heap(FILE*infp, student_info *max_heap){

    int i = 0, last_index, n_students;
    char seperation_line[80];
    char temp[100];
    fseek(infp, 0, 0);
    while(!feof(infp)){
        fgets(max_heap[i].student_name, 50, infp);
        max_heap[i].student_name[strlen(max_heap[i].student_name)-1] = '\0';
        fscanf(infp, "%d\n", &max_heap[i].student_ID);
        fgets(temp, 100, infp);
        temp[strlen(temp)-1] = '\0';
        max_heap[i].priority = evaluate_priority(temp);
        fgets(seperation_line, 80, infp);
        i++;
    }

    n_students = i;
    last_index = n_students - 1;

    for(i = ((last_index+1)/2)-1; i>=0; i--)
        heapify(max_heap, i, last_index);

    return(n_students);
}

student_info collect_info(){
    student_info new_student;
    char reason[100];
        
    printf("\n\t\tPlease enter your name: ");
    gets(new_student.student_name);
    printf("\n\t\tPlease enter your ID: ");
    scanf("%d",&(new_student.student_ID));
    getchar();
    printf("\n\t\tWhy do you want to go for a special request? ");
    gets(reason);
    new_student.priority = evaluate_priority(reason);

    return new_student;
}

int add_to_heap(student_info *max_heap, int *n_students){

    int index, parent_index;

    if(*n_students == SIZE)
        return -1;
    else{
        student_info ToAdd = collect_info();
        index = (*n_students)++;
        max_heap[index] = ToAdd;
        parent_index = (index-1)/2;
        while(index >= 1 && max_heap[index].priority > max_heap[parent_index].priority){
            swap(&max_heap[index], &max_heap[parent_index]);
            index = parent_index;
            parent_index = (index-1)/2;
        }
        return index;
    }
}


void print_heap(student_info*max_heap, int n_students){
    int i;

    for(i = 0; i < n_students; i++)
        printf("\n\t\tName: %s\tID: %d\tPriority: %d\n",max_heap[i].student_name, max_heap[i].student_ID, max_heap[i].priority);
}

student_info delete(student_info*max_heap, int*n_students, student_info ToServe){
    int root_index = 0, last_index;
    int i;
    if((*n_students) == 0)
        strcpy(ToServe.student_name, "EMPTY");
    else{
        ToServe = max_heap[root_index];
        max_heap[root_index] = max_heap[--(*n_students)];
        last_index = (*n_students)-1;
        if(last_index < 0)  
            return;
        else{
            for(i = ((last_index+1)/2)-1; i>=0; i--)
                heapify(max_heap, i, last_index);
        }
    }
    return(ToServe);
}

void sort_heap(student_info*max_heap, int n_student){
    student_info temp[SIZE];
    student_info ToServe;
    int n_student_temp = n_student, i;

    // Back up the content of the file
    for(i = 0; i < n_student; i++)
        temp[i] = max_heap[i];

    while(n_student_temp != 0){
        ToServe = temp[0];
        delete(temp, &n_student_temp, ToServe);
        printf("\n\t\tName: %s\tID: %d\tPriority: %d\n",ToServe.student_name, ToServe.student_ID, ToServe.priority); //should we print priority??
    }
}

int calculate_height(student_info*max_heap, int n_students){
    int last_index = n_students-1;
    int parent_index;
    int cnt = 0;

    do{
        parent_index = (last_index-1)/2;
        last_index = parent_index;
        cnt++;
    }while(parent_index > 0 );
    
    return cnt;
}

void Generate_File(student_info*max_heap,int n_students, int number){
        FILE*outfp = fopen("Joined_Students.txt", "w");
        int i;
        student_info temp[SIZE];
        student_info ToServe = max_heap[0];
        for(i = 0; i < n_students; i++)
            temp[i] = max_heap[i];
        for(i = 0; i < number; i++){
            ToServe = delete(temp, &n_students, ToServe);
            fprintf(outfp, "Name:%s  ID:%d", ToServe.student_name, ToServe.student_ID);
            fprintf(outfp, "\n");
        }
        fclose(outfp);
}

int main(void){
    FILE*infp = fopen("SpecialRequests.txt", "r");
    student_info max_heap[SIZE];
    int flag = 0; // Flag to check if the heap is constructed or not
    char choice;
    int n_students, index_of_student, number_closed_section;
    if(infp == NULL){
        printf("\n\t\tThe File could not be opened...Program terminating\n");
        exit(0); // Terminate the program if the file could not be opened!
    }
    do{
        Menu();
        scanf("%c", &choice);
        getchar();
        switch(choice){
            case 'a':
                if(!flag){
                    n_students = construct_heap(infp, max_heap);
                    fclose(infp);
                    printf("\n\t\tHeap was successfully constructed!\n");
                    printf("\n\t\tNumber of students is %d\n",n_students);
                    flag = 1; //Heap constructed
                }else
                    printf("\n\t\tHeap already constructed\n");
                break;

            case 'b':
                if(flag){
                    index_of_student = add_to_heap(max_heap, &n_students);  //Collect info was called in the function to ensure that we won't ask the user for it's informations unless we are sure that we can add more
                    if(index_of_student != -1)
                        printf("\n\t\tThe slot index where the new element was saved is: %d\n", index_of_student); 
                    else{
                        printf("\n\t\tSorry! Unable to add the student\n");
                    } 
                }else
                    printf("\n\t\tPlease construct the heap first!\n");
                break;

            case 'c':
                if(flag)
                    print_heap(max_heap, n_students); 
                else
                    printf("\n\t\tPlease construct the heap first\n");
                break; 

            case 'd':
                if(flag)
                    sort_heap(max_heap, n_students);
                else
                    printf("\n\t\tPlease construct the heap first\n");
                break;

            case 'e':
                if(flag){
                    int height = calculate_height(max_heap, n_students);
                    printf("\n\t\tHeight of the Heap is %d\n",height);
                }else
                    printf("\n\t\tPlease construct the heap first\n");
                break;

            case 'f':
                if(flag){
                    printf("\n\t\tHow many students to add to the closed section: ");
                    scanf("%d", &number_closed_section);
                    getchar();
                    if(number_closed_section != 0 && number_closed_section < n_students){
                        Generate_File(max_heap, n_students, number_closed_section);
                        printf("\n\t\tFile was successfully generated!\n");
                    }else if(number_closed_section == 0){
                        printf("\n\t\tFile was not generated. Reason: No students were accepted to join the closed section");
                    }else{
                        printf("\n\t\tNumber entered exceeds the number of students who requested special request.\n\n\t\tA file was generated with only %d students\n", n_students);
                        Generate_File(max_heap, n_students, n_students);
                    }
                }else{ 
                    printf("\n\t\tPlease construct the heap first\n");
                }
                break;
            case 'g':
                printf("\n\t\tYou decided to quit...BYE!\n");
                break;
            default:
                printf("\n\t\tInvalid choice.\n");
                break;   
        }
    }while(choice != 'g');
    return 0;
}
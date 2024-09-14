#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  char name[30];
  int office_number;
  char office_building[5];
  char office_hours[5][30];
} faculty_info;


void menu(void);
int check_faculty(faculty_info[20], int, char*);
int display_file(FILE *);
void load_to_array(FILE *, faculty_info[20], int );
void get_office_hrs(faculty_info[20], int, char[30], char[5][30]);
int cancel_office_hrs(faculty_info[20], int, char[30]);
void sort_array(FILE*,faculty_info[20], int);
void update_office_hours(faculty_info[20], int, char*, int, char*);


int main() {
  int choice, faculty_number;
  char faculty_name[30], hrs[5][30];
  int flag = 0, flag_2 = 0; //to check if the array is filled or not
  char weekdays[5][30] = {"Monday","Tuesday","Wednesday","Thursday","Friday"};
  faculty_info aui_faculty[20];
  FILE *infp = fopen("office_hours.txt", "r");
  FILE *outfp;

  if(infp != NULL){
    do {
      menu();
      scanf("%d", &choice);

      switch (choice) {


       case 1:

         faculty_number = display_file(infp);
         printf("\n\t\tThe number of faculty is %d\n", faculty_number);
         flag_2 = 1;

        break;


       case 2: 

         if(!flag && flag_2 != 0){
           load_to_array(infp, aui_faculty, faculty_number);
           fclose(infp);
           flag = 1;

         }else if(flag && flag_2 != 0)
           printf("\n\t\tArray is already filled!\n");
          else if(flag_2 == 0){
            printf("\n\t\tSorry! You should open the file first.\n");
          }

        break;


       case 3: 

        if(flag){   //check if the array is filled or not
          getchar();
          printf("\n\t\tPlease enter the faculty's name: ");    
          gets(faculty_name);

          get_office_hrs(aui_faculty,faculty_number,faculty_name,hrs);

          if(strcmp(hrs[0],"null") == 0)
            printf("\n\t\tFaculty not found.\n");

          else{ 
           int answer,day;  //answer is for week or particular day    //day is for the specific weekday

           do{ 
             printf("\n\t\tIf want to look for office hours for the whole week: Choose 0\n");
             printf("\n\t\tIf you want to look for office hours for a particular day: Choose 1\n");
             scanf("%d",&answer);

            
             switch(answer){

               case 0: //User choose to see office hours for the whole week
                
                for(int i = 0;i < 5;i++)
                  printf("\n\t\t%s:%s\n",weekdays[i],hrs[i]); 

               break;
  
               case 1:  //User choose to see office hours for a particular day

                do{
                  printf("\n\t\tWhich day?\n\n\t\t1.Monday\n\n\t\t2.Tuesday\n\n\t\t3.Wednesday\n\n\t\t4.Thursday\n\n\t\t5.Friday\n");
                  scanf("%d",&day);

                  if(day > 5 || day < 1)
                    printf("\n\t\tPlease choose a valid choice\n");

                }while(day > 5 || day < 1);
           
                if(strcmp(hrs[day-1],"None") == 0) //Check if the faculty doesn't have office hours on that day
                  printf("\n\t\tFaculty %s has no office hours on %s\n",faculty_name,weekdays[day-1]);

                else
                  printf("\n\t\t%s:%s\n",weekdays[day-1],hrs[day-1]);

               break;

               default:
                printf("\n\t\tPlease choose a valid choice\n");

             }

           }while(answer != 0 && answer != 1);
          }

        }else  
          printf("\n\t\tPlease fill the array first!\n"); 

       break;


       case 4:

         if(flag){  //check if the array is filled or not

           getchar();
           printf("\n\t\tPlease enter the faculty's name: ");    
           gets(faculty_name);
           int flag1 = cancel_office_hrs(aui_faculty,faculty_number,faculty_name); //flag1 to check if the operation was successful or not

              if(flag1){
                faculty_number--;
                printf("\n\t\tSuccessful operation\n");
              }else
                printf("\n\t\tFailed operation\n");

         }else
           printf("\n\t\tPlease fill the array first!\n"); 

        break;


       case 5:

         if(flag){
           outfp = fopen("office_hours_SortedbyName.txt","w");
           sort_array(outfp,aui_faculty,faculty_number);

           //Printing the array in the file
           for(int i = 0;i < faculty_number;i++){
             fprintf(outfp,"%s\n",aui_faculty[i].name); 
             fprintf(outfp,"%d\n",aui_faculty[i].office_number);
             fprintf(outfp,"%s\n",aui_faculty[i].office_building);
              for(int j = 0;j < 5;j++)
                fprintf(outfp,"%s\n",aui_faculty[i].office_hours[j]);
             fprintf(outfp,"-----------------------------\n");  
           }

           //closing the file
           fclose(outfp);

         }else
           printf("\n\t\tPlease fill the array first!\n");

        break;


       case 6:

         if(flag){
           char change[30];
           int day;
           printf("\n\t\tPlease enter the full name of the faculty you want to update: ");
           getchar();
           gets(faculty_name);
           if(check_faculty(aui_faculty, faculty_number, faculty_name)){
            printf("\n\t\tWhich day?\n\n\t\t1.Monday\n\n\t\t2.Tuesday\n\n\t\t3.Wednesday\n\n\t\t4.Thursday\n\n\t\t5.Friday\n");
            scanf("%d", &day);
            while (day <= 0 || day > 5){
              printf("\n\t\tInvalid choice!");
              printf("\n\t\tWhich day?\n\n\t\t1.Monday\n\n\t\t2.Tuesday\n\n\t\t3.Wednesday\n\n\t\t4.Thursday\n\n\t\t5.Friday\n");
              scanf("%d", &day);
            }
            printf("\n\t\tPlease enter the change: ");
            getchar();
            gets(change);

            update_office_hours(aui_faculty, faculty_number, faculty_name, day, change);

           }else{
            printf("\n\t\tSorry! Faculty not found.\n");
           }

         }else
           printf("\n\t\tPlease fill the array first!\n");

       break;


       case 7:
          printf("\n\t\tYou decided to quit! Bye.\n");
        break;


       default:
         printf("\n\t\tThat was an invalid choice!\n");
      }

    } while (choice != 7);

  }else 
    printf("\n\t\tCouldn't open the File!\n");

return 0;
}

void menu(void) {
  printf("\n\t\t1. View File Content\n");
  printf("\n\t\t2. Load File Data to an Array\n");
  printf("\n\t\t3. Get a Faculty Member Office Hours\n");
  printf("\n\t\t4. Cancel all Office Hours\n");
  printf("\n\t\t5. Sort File Content\n");
  printf("\n\t\t6. Update Office Hours\n");
  printf("\n\t\t7. Quit\n");
}


int display_file(FILE *infp) {

  char line[50];
  int count = 0;
  while (fgets(line, 50, infp) != NULL) {
    printf("%s", line);
    count++;
  }
  printf("\n");
  return (count / 9);
}


void load_to_array(FILE *infp, faculty_info aui_faculty[20], int N) {
  fseek(infp, 0, 0);
  char seperation[50];
  int i;
  int j;

  for (i = 0; i < N; i++) {
    fgets(aui_faculty[i].name, 30, infp);
    aui_faculty[i].name[strlen(aui_faculty[i].name)-1]='\0';  

    fscanf(infp,"%d\n",&(aui_faculty[i].office_number));

    fgets(aui_faculty[i].office_building,4,infp);
    aui_faculty[i].office_building[strlen(aui_faculty[i].office_building)-1]='\0';  

    for (j = 0; j < 5; j++) {
      fgets(aui_faculty[i].office_hours[j], 30, infp);
      aui_faculty[i].office_hours[j][strlen(aui_faculty[i].office_hours[j])-1]='\0';
    }

    fgets(seperation, 50, infp);
  }


}

void get_office_hrs(faculty_info aui_faculty[20], int N, char name[30], char hrs[5][30]){
int i;

  for(i = 0;i < N;i++){
    if(strcasecmp(aui_faculty[i].name,name) == 0){
      for(int j = 0; j < 5; j++)
        strcpy(hrs[j],aui_faculty[i].office_hours[j]);
    break;
    }
  }

  if(i == N)
    strcpy(hrs[0],"null");

}


int cancel_office_hrs(faculty_info aui_faculty[20],int N,char name[30]){
int i,j;

 for(i = 0;i < N;i++){
  if(strcmp(aui_faculty[i].name,name) == 0){  
    for(j = i;j < N;j++){
      aui_faculty[j] = aui_faculty[j+1];
    }
  return(1);
  }
 }

  if(i == N)
   return(0);

}


void sort_array(FILE* outfp, faculty_info aui_faculty[20],int N){
 int i,j,min_index;
 faculty_info temp;

 for(i = 0;i < N-1;i++){
   min_index=i;
     for(j = i+1;j < N;j++){
       if(strcmp(aui_faculty[j].name,aui_faculty[min_index].name) < 0)
           min_index = j;
     }
      if(min_index != i){
      temp = aui_faculty[i];
      aui_faculty[i] = aui_faculty[min_index];
      aui_faculty[min_index] = temp;  
      }   
 }

}


void update_office_hours(faculty_info aui_faculty[20], int N, char* faculty_name, int day, char* change) {
    int i;
    for (i = 0; i < N; i++) {
        if (strcasecmp(aui_faculty[i].name, faculty_name) == 0) {
            strcpy(aui_faculty[i].office_hours[day-1], change);
            break;
        }
    }
}

int check_faculty(faculty_info aui_faculty[], int N, char* faculty_name){

  int i;
  for(i = 0; i < N; i++){
    if(strcasecmp(aui_faculty[i].name, faculty_name) == 0){
      return 1;
    }
  }
  return 0;
}
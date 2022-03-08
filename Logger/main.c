#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>


// please enter here the list of all the data file you want to save
char* data_files[] = {"IMU.bin","WIMDA.bin","HCHDG.bin","GPRMC.bin"};
int len = sizeof(data_files)/sizeof(data_files[0]);

int main(int argc, char const *argv[]){

  printf("%d\n",len );

  if (argc!=3){
    printf("Please enter sensor data directory and path to save log\n");
    return 0;
  }



  char *data_path = malloc(50);
  char *saving_path = malloc(50);

  strcpy(data_path, argv[1]);
  strcpy(saving_path, argv[2]);


  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  strftime(s, sizeof(s), "%F__%H-%M-%S", tm);
  // strftime(s, sizeof(s), "%c", tm);



  char logname[200];
  strcpy(logname,saving_path);
  strcat(logname,s);
  strcat(logname,".log");



  FILE *logptr;
  logptr = fopen(logname,"w");

  if (logptr==NULL){
    return 0;
  }



  while (1){


    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%D;%T", tm));
    fprintf(logptr,"%s", s);
    printf("log saved at %s\n", s);

    for (int i = 0; i < len; i++) {
      FILE * fptr;
      char * filename  = malloc(50);
      strcat(filename,data_path);
      strcat(filename,data_files[i]);
      fptr = fopen(filename,"rb");

      if (fptr==NULL){

      }else{
        float data[20];

        for (int   j = 0; j < 20; j++) {
          data[j] = NAN;
        }

        fread(data,sizeof(float),20,fptr);
        fclose(fptr);


        for (int j = 0; j < 20; j++) {
          if (!isnan(data[j])){
            fprintf(logptr,";%f",data[j]);
          }
        }
      }

      free(filename);

    }
    fprintf(logptr, "\n");
    fflush(logptr);
    sleep(1);
  }
  return 0;
}

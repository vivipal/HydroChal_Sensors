#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>



int main(int argc, char const *argv[]){


  if (argc<3){
    printf("Please enter path to save log and at least one sensor data file \n");
    return 0;
  }

  const int len_argc = argc ;


  char *saving_path = malloc(50);
  strcpy(saving_path, argv[1]);

  int len = len_argc - 2;

  char* data_files[len];
  for (int i = 0; i < len; i++) {
    data_files[i] = (char*) argv[i+2];
  }


  printf(" number of files : %d\n",len );

  for (size_t i = 0; i < len; i++) {
    printf("%s\n", data_files[i]);
  }



  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  strftime(s, sizeof(s), "%F__%H-%M-%S", tm);



  char logname[200];
  strcpy(logname,saving_path);
  strcat(logname,s);
  strcat(logname,".log");



  FILE *logptr;
  logptr = fopen(logname,"w");



  if (logptr==NULL){
    return 0;
  }

  for (int i = 0; i < len; i++) {
    fprintf(logptr,"%s  ", data_files[i]);
  }
  fprintf(logptr, "\n");

  while (1){


    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%D;%T", tm));
    fprintf(logptr,"%s", s);
    printf("log saved at %s\n", s);

    for (int i = 0; i < len; i++) {
      FILE * fptr;
      char * filename  = data_files[i];
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

    }
    fprintf(logptr, "\n");
    fflush(logptr);
    sleep(1);
  }
  return 0;
}

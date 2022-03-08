#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


// nombre d'info a recup pour chaque trame NMEA
#define WIMDA_len 2
#define HCHDG_len 3
#define GPRMC_len 9

// indices des infos a recup pour chaque trame NMEA
int WIMDA_indx[WIMDA_len] = {14,18};
int HCHDG_indx[HCHDG_len] = {0,3,4};
int GPRMC_indx[GPRMC_len] = {0,1,2,3,4,5,6,7,8};


bool inarray(int array[], int element, int len){
  // regarde si element appartient bien a array de taille len
  for (int i = 0; i < len; i++) {
      if (array[i] == element) {return true;}
    }
  return false;
}


int open_port(char *device){

  int fd; /* File descriptor for the port */


  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1){
    printf("Unable to open %s", device);
  }else{
    fcntl(fd, F_SETFL, 0);
  }
  return (fd);
}

void set_baudrate(int fd){
  struct termios options;

  //Get the current options for the port...
  tcgetattr(fd, &options);

  //Set the baud rates to 4800...
  cfsetispeed(&options, B4800);
  cfsetospeed(&options, B4800);

  // Enable the receiver and set local mode...
  options.c_cflag |= (CLOCAL | CREAD);

  // Set 8N1
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  // Read data as raw
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  // Set the new options for the port...
  tcsetattr(fd, TCSANOW, &options);
}

void wait_message_begin(int fd){
  char buffer[2];
  do {
    read(fd,buffer,1);
  } while(*buffer!='$');
}

float get_data(char ** msg,float *data,int len,int indxToSelect[]){

  // recupere uniquement les donnes des indices de indxToSelect
  // les stocks dans data de taille len=nombreDeDonneesASelectionner

  char * substr; // this is used by strtok() as an index
  int c = 0;


  for (int i=0;i<20;i++){
    substr = strsep(msg,","); // on recup le nom de la trame
    if (inarray(indxToSelect,i,len)){
      float val;
      if (*substr=='E' || *substr=='N' || *substr=='A'){ // east, north, valid GPS data
        val = 1.0;
      }else if (*substr=='W' || *substr=='S' || *substr=='V'){ // west, south or not valid GPS Data
        val = -1.0;
      }else{
        val = atof(substr);
      }
      data[c++] = val;
    }
  }
}

void receive_message(int fd,char *receivedMessage){
  bool end_message = false;
  char buffer[2];
  while (!end_message){
    read(fd,buffer,1);
    if (*buffer=='*'){
      end_message = true;
    }else{
      strcat(receivedMessage,buffer);
    }
  }
}

char * get_sentence(char **msg){

  char * trame; // this is used by strtok() as an index
  trame = strsep(msg,","); // on recup le nom de la trame


  return trame;
}

int get_len(char *trameIndicator){
  int len=0;
  if (strcmp(trameIndicator,(char*)"WIMDA")==0){
    len = WIMDA_len;
  }else if (strcmp(trameIndicator,(char*)"GPRMC")==0){
    len = GPRMC_len;
  }else if (strcmp(trameIndicator,(char*)"HCHDG")==0){
    len = HCHDG_len;
  }
  return len;
}

float * process_data(char **msg, char *trameIndicator,int len){


  // initialisation dynamique du tableau des indices a selectionner
  int *toselect;
  toselect = (int *) malloc(0);

  if (strcmp(trameIndicator,(char*)"WIMDA")==0){
    // trame WIMDA
    // creation du tableau des indices a selectionner pour cette trame
    toselect = (int *) realloc(toselect,WIMDA_len*sizeof(int));
    for (int i=0; i<WIMDA_len; i++){
      *(toselect+i)= WIMDA_indx[i];
    }

  }else if (strcmp(trameIndicator,(char*)"GPRMC")==0){
    // trame GPRMC
    // creation du tableau des indices a selectionner pour cette trame
    toselect = (int *) realloc(toselect,GPRMC_len*sizeof(int));
    for (int i=0; i<GPRMC_len; i++){
      *(toselect+i)= GPRMC_indx[i];
    }

  }else if (strcmp(trameIndicator,(char*)"HCHDG")==0){
    // trame HCHDG
    // creation du tableau des indices a selectionner pour cette trame
    toselect = (int *) realloc(toselect,HCHDG_len*sizeof(int));
    for (int i=0; i<HCHDG_len; i++){
      *(toselect+i)= HCHDG_indx[i];
    }
  }

  // printf("%s\n", (msg+1));

  // float data[len]; // creation des données de la trame recue
  float *data = malloc (sizeof (float) * len);
  get_data(msg,data,len,toselect);  // recuperation des donnees dans ce tableau


  // // affichage des donnes de la trame recue
  // printf("data : ");
  // for (int i=0;i<len;i++){
  //   printf("%f",data[i]);
  //   printf(" | ");
  // }
  // printf("\n");



  free(toselect);    // on désalloc le tableaux des indices a selectionner

  return data;

}




int main(int argc, char const *argv[]){                     // run over and over again


  char *serial_device = malloc(30);
  char *binary_file_path = malloc(50);
  if (argc<2){
    serial_device = "/dev/ttyUSB0";
    binary_file_path = "./";
  }else if(argc==2){
    strcpy(serial_device, argv[1]);
    binary_file_path = "./";
  } else {
    strcpy(serial_device, argv[1]);
    strcpy(binary_file_path, argv[2]);
  }
  // open serial communication
  int fd = open_port(serial_device);
  set_baudrate(fd);

  while (1){


    wait_message_begin(fd);

    char receivedMessage[200] = {0};
    receive_message(fd,receivedMessage);

    char * pTempMessage = receivedMessage;
    char * trameIndicator = get_sentence(&pTempMessage);
    int len = get_len(trameIndicator);

    if (len>0){

      float *data;
      data = process_data(&pTempMessage,trameIndicator,len);


      printf("%s data : ", trameIndicator);
      for (int i=0;i<len;i++){
          printf("%f",*(data+i));
          printf(" | ");
      }
      printf("\n");


      // clock_t start = clock();
      char * new_filename = malloc(100);
      strcpy(new_filename,binary_file_path);
      strcat(new_filename,trameIndicator);
      strcat(new_filename,".bin");


      FILE * fptr;
      fptr = fopen(new_filename,"wb");
      fwrite(data,sizeof(float),len,fptr);
      fclose(fptr);
      // clock_t end = clock();

      // double time_taken = (double)((end - start) / (CLOCKS_PER_SEC));
      // printf("%f s\n", time_taken );

    }


    printf("------------------\n");

  }
}

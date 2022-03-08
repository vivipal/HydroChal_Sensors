#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <math.h>







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
  cfsetispeed(&options, B57600);
  cfsetospeed(&options, B57600);

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

void wait_new_message(int fd){
  char buffer[2];

  // wait for begin of a sentence ie '#'
  do {
    read(fd,buffer,1);
  } while(*buffer!='#');

  // read the "YPR="
  for (size_t i = 0; i < 4; i++) {
    read(fd,buffer,1);
  }

}

void receive_message(int fd,char *receivedMessage){
  bool end_message = false;
  char buffer[2];
  while (!end_message){
    read(fd,buffer,1);
    if (*buffer=='\n'){
      end_message = true;
    }else{
      strcat(receivedMessage,buffer);
    }
  }
}

void get_data(char ** msg,float *data){

  // recupere uniquement les donnes des indices de indxToSelect
  // les stocks dans data de taille len=nombreDeDonneesASelectionner

  char * substr; // this is used by strtok() as an index


  for (int i=0;i<3;i++){
    substr = strsep(msg,","); // on recup le nom de la trame
    float val;
    val = atof(substr);
    data[i] = val;
  }
}

float * process_data(char **msg){


  // initialisation dynamique du tableau des indices a selectionner

  float *data = malloc (sizeof (float) * 3);
  get_data(msg,data);  // recuperation des donnees dans ce tableau

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


  wait_new_message(fd);
  char buffer[2];
  while (1){


    wait_new_message(fd);

    char receivedMessage[200] = {0};
    receive_message(fd,receivedMessage);

    char * pTempMessage = receivedMessage;
    float *data;
    data = process_data(&pTempMessage);


    char * new_filename = malloc(100);
    strcpy(new_filename,binary_file_path);
    strcat(new_filename,"IMU.bin");


    FILE * fptr;
    fptr = fopen(new_filename,"wb");
    fwrite(data,sizeof(float),3,fptr);
    fclose(fptr);

    printf("data written: ");
    for (int i=0;i<3;i++){
      printf("%f",*(data+i));
      printf(" | ");
    }



    printf("\n");
    printf("------------------\n");

  }
  free(serial_device);
}

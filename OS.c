#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


float calculate_variance(float a, float b, float c) {
  float mean = (a + b + c) / 3;
  float variance = ((a - mean) * (a - mean) + (b - mean) * (b - mean) + (c - mean) * (c - mean)) / 3;
  return variance;
}

int main() {

  // pid_t ch2, ch2, ch3, ch21, ch22;
  // pid_t ch21, ch22;
  // pid_t P = getpid();
  pid_t ch2;

  float a, b, c;

  FILE* fp;
  char output_file[] = "output.txt";

  scanf("%f", &a);
  scanf("%f", &b);
  scanf("%f", &c);

  // build a shared memory space
  int shmid;
  key_t key = 1234;
  int* shared_data;

  shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  shared_data = (int*)shmat(shmid, NULL, 0);
  if (shared_data == (int*)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  // ch1 process starts here

  pid_t ch1 = fork();
  if (ch1 == -1) {
    perror("ch1 process creation failed\n");
    exit(EXIT_FAILURE);
  }

  if (ch1 == 0) {// ch1 process



    // start ch11 process
    pid_t ch11 = fork();

    if (ch11 < 0) {
      perror("ch11 process creation failed\n");
      exit(EXIT_FAILURE);
    }
    if (ch11 == 0) {// start ch11 process


      int i, sum = 0;
      for (i = 1; i <= a; i++)
        sum += (i * i);

      *shared_data = sum;
      // exit for ch12
      exit(EXIT_SUCCESS);
    }

    int status;
    waitpid(ch11, &status, 0);

    int sum_square = *shared_data;


    pid_t ch12 = fork();

    if (ch12 < 0) {
      perror("ch12 process creation failed\n");
      exit(EXIT_FAILURE);
    }
    if (ch12 == 0) {// start ch12 process

      int i, fact = 1;
      int temp = 1;
      int temp2;
      for (i = 2; i < a; i++) {
        temp2 = fact;
        fact += temp;
        temp = temp2;
      }

      *shared_data = fact;
      // exit for ch12
      exit(EXIT_SUCCESS);
    }

    waitpid(ch12, &status, 0);

    int factorial = *shared_data;

    fp = fopen(output_file, "w");

    if (fp == NULL) {
      printf("failed to open output file for ch1 process named %s", output_file);
      return(EXIT_FAILURE);
    }

    int input_ch1;
    printf("inter input of ch1 process :");
    scanf("%d", &input_ch1);

    int ch1_value = factorial + sum_square + input_ch1;

    printf("this is ch1_value: %d\n", ch1_value);

    fprintf(fp, "%d", ch1_value);

    fclose(fp);

    // exit for ch1 
    exit(EXIT_SUCCESS);
  }

  int status1;
  waitpid(ch1, &status1, 0);

  // todo : return the achived var to parent process
  // ch2 process starts here 
  ch2 = fork();

  if (ch2 < 0) {
    printf("ch2 creation process faild");

  }
  else if (ch2 == 0) {// ch2 process

    float variance;
    float avrage;

    //Note: creation pipe ch2, ch21
    int pipels2_21[2];
    // pipels1-11[0] : read
    // pipels1-11[1] : write

    if (pipe(pipels2_21) == -1) {
      printf("pipe ch2 ,ch21 cration faild");
      exit(21);
    }

    //Note: go into ch21 process
    pid_t ch21 = fork();

    if (ch21 < 0) {
      printf("ch21 creation process faild");
    }
    else if (ch21 == 0) {

      //Note: ch21 process

      close(pipels2_21[0]); // close the unused read side of pipe

      float var = calculate_variance(a, b, c);


      write(pipels2_21[1], &var, sizeof(float));
      close(pipels2_21[1]);

      exit(21);
    }
    else {
      // ch2 process

      close(pipels2_21[1]); // close the unused read side of pipe 


      read(pipels2_21[0], &variance, sizeof(float));
      printf("this is the variance : %f\n", variance);
      close(pipels2_21[0]);
      int status;

      waitpid(ch21, &status, 0);
    }

    int pipels2_22[2];

    if (pipe(pipels2_22) == -1) {
      printf("pipe ch2 ,ch22 cration faild");
      exit(22);
    }

    //Note: go into ch22 process

    pid_t ch22 = fork();

    if (ch22 < 0) {
      printf("ch22 creation process faild");
    }
    else if (ch22 == 0) {

      //Note: ch22 process

      close(pipels2_22[0]); // close the unused read side of pipe


      float avr = (a + b + c) / 3;

      write(pipels2_22[1], &avr, sizeof(float));
      close(pipels2_22[1]);

      exit(22);
    }
    else {
      // ch2 process

      close(pipels2_22[1]); // close the unused read side of pipe 


      read(pipels2_22[0], &avrage, sizeof(float));
      printf("this is the avrage : %f\n", avrage);
      close(pipels2_22[0]);

      int status;
      waitpid(ch22, &status, 0);
    }

    printf("ch2 output: %f\n", (variance + avrage));


    exit(2);
  }

  int status2;

  waitpid(ch2, &status2, 0);
  // end of ch2

  // start ch3 process
  pid_t ch3 = fork();

  if (ch3 < 0){
    perror("the craetion of ch3 process falid \n");
    exit(EXIT_FAILURE);
  }
  if(ch3 == 0){// ch3 process
    int min = a;
    if (b < a) min = b;
    if (c < min) min = c;
    *shared_data = min ;

    exit(EXIT_SUCCESS);
  }

  int status3 ;
  waitpid(ch3, &status3, 0);
  // end ch3 process

  int ch3_value = *shared_data ;

  printf("value of ch3 : %d\n", ch3_value);


  return 0;
}

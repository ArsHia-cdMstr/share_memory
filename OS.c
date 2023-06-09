#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


float calculate_variance(float a, float b, float c) {
    float mean = (a + b + c) / 3;
    float variance = ((a - mean) * (a - mean) + (b - mean) * (b - mean) + (c - mean) * (c - mean)) / 3;
    return variance;
}

int main() {

  // pid_t ch2, ch2, ch3, ch21, ch22;
  // pid_t ch21, ch22;
  // pid_t P = getpid();
  pid_t ch2 ;

  float a, b, c ;
  scanf("%f", &a);
  scanf("%f", &b);
  scanf("%f", &c);

  // ch1 process starts here
  


  // ch2 process starts here
  ch2 = fork();

  if (ch2 < 0){
    printf("ch2 creation process faild");
  
  }else if(ch2 == 0){// ch2 process

  float variance ;
  float avrage ;

    //Note: creation pipe ch2, ch21
    int pipels2_21[2];
    // pipels1-11[0] : read
    // pipels1-11[1] : write

    if(pipe(pipels2_21) == -1){
      printf("pipe ch2 ,ch21 cration faild");
      exit(21);
    }

    //Note: go into ch21 process
    pid_t ch21  = fork();

    if (ch21 < 0){
      printf("ch21 creation process faild");
    }else if (ch21 == 0){
      
      //Note: ch21 process

      close(pipels2_21[0]); // close the unused read side of pipe

      float var = calculate_variance(a, b, c);
      // int i, sum = 0;
      // for(i = 1; i <= a; i++)
      //   sum += (i*i);
      
      write(pipels2_21[1], &var, sizeof(float)); 
      close(pipels2_21[1]);

      exit(21);
    }else {
      // ch2 process

      close(pipels2_21[1]); // close the unused read side of pipe 
      

      read(pipels2_21[0], &variance, sizeof(float));
      printf("this is the variance : %f\n", variance);
      close(pipels2_21[0]);
      int status;

      waitpid(ch21, &status, 0);
    }

  int pipels2_22[2];

  if(pipe(pipels2_22) == -1){
    printf("pipe ch2 ,ch22 cration faild");
    exit(22);
  }

    //Note: go into ch22 process

  pid_t ch22 = fork();
    
  if (ch22 < 0){
    printf("ch22 creation process faild");
    }else if (ch22 == 0){
      
      //Note: ch22 process

      close(pipels2_22[0]); // close the unused read side of pipe


      float avr = (a + b + c) / 3;

      // int i, factorial = 1;
      // int temp = 1 ;
      // int temp2 ;
      // for(i = 2; i < a; i++){
      //   temp2 = factorial;
      //   factorial += temp ;
      //   temp = temp2 ;
      // }

      write(pipels2_22[1], &avr, sizeof(float)); 
      close(pipels2_22[1]);

      exit(22);
    }else {
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

  int status;

  waitpid(ch2, &status, 0);

  

  // }else{ // parent process
  //   // wait(NULL);
  //   int x = 3;
  //   printf("%d\n", x);
  // }

  return 0;
}

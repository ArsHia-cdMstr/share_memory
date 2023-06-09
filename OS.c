#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  // pid_t ch1, ch2, ch3, ch11, ch12;
  // pid_t ch21, ch22;
  // pid_t P = getpid();
  pid_t ch1 ;

  int a, b, c ;
  scanf("%d", &a);
  scanf("%d", &b);
  scanf("%d", &c);


  ch1 = fork();

  if (ch1 < 0){
    printf("ch1 creation process faild");
  
  }else if(ch1 == 0){// ch1 process

  int factorial ;
  int square_sum ;

    //Note: creation pipe ch1, ch11
    int pipels1_11[2];
    // pipels1-11[0] : read
    // pipels1-11[1] : write

    if(pipe(pipels1_11) == -1){
      printf("pipe ch1 ,ch11 cration faild");
      exit(11);
    }

    //Note: go into ch11 process
    pid_t ch11  = fork();

    if (ch11 < 0){
      printf("ch11 creation process faild");
    }else if (ch11 == 0){
      
      //Note: ch11 process

      close(pipels1_11[0]); // close the unused read side of pipe

      int i, sum = 0;
      for(i = 1; i <= a; i++)
        sum += (i*i);
      
      write(pipels1_11[1], &sum, sizeof(int)); 
      close(pipels1_11[1]);

      exit(11);
    }else {
      // ch1 process

      close(pipels1_11[1]); // close the unused read side of pipe 
      

      read(pipels1_11[0], &square_sum, sizeof(int));
      printf("this is the square sums : %d\n", square_sum);
      close(pipels1_11[0]);
      int status;

      waitpid(ch11, &status, 0);
    }

  int pipels1_12[2];

  if(pipe(pipels1_12) == -1){
    printf("pipe ch1 ,ch12 cration faild");
    exit(12);
  }

    //Note: go into ch12 process

  pid_t ch12 = fork();
    
  if (ch12 < 0){
    printf("ch12 creation process faild");
    }else if (ch12 == 0){
      
      //Note: ch12 process

      close(pipels1_12[0]); // close the unused read side of pipe

      int i, factorial = 1;
      int temp = 1 ;
      int temp2 ;
      for(i = 2; i < a; i++){
        temp2 = factorial;
        factorial += temp ;
        temp = temp2 ;
      }

      write(pipels1_12[1], &factorial, sizeof(int)); 
      close(pipels1_12[1]);

      exit(12);
    }else {
      // ch1 process

      close(pipels1_12[1]); // close the unused read side of pipe 
      

      read(pipels1_12[0], &factorial, sizeof(int));
      printf("this is the factorial : %d\n", factorial);
      close(pipels1_12[0]);

      int status;
      waitpid(ch12, &status, 0);
    }

    int inputch1;

    scanf("%d", &inputch1);

    printf("ch1 output: %d\n", (inputch1 + factorial + square_sum));


    exit(1);
  }

  int status;

  waitpid(ch1, &status, 0);

  

  // }else{ // parent process
  //   // wait(NULL);
  //   int x = 3;
  //   printf("%d\n", x);
  // }

  return 0;
}

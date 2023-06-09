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
      int square_sum ;

      read(pipels1_11[0], &square_sum, sizeof(int));
      printf("this is the square sums : %d\n", square_sum);
      close(pipels1_11[0]);

      exit(1);
    }

    exit(1);
  }

  // }else{ // parent process
  //   // wait(NULL);
  //   int x = 3;
  //   printf("%d\n", x);
  // }

  return 0;
}

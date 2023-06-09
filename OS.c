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

  int x = 0 ;


  ch1 = fork();

  if(ch1 == 0){
    *sum_squares = 1 ;
    exit(0);
  }else{ 
    wait(NULL);
    int x = 
    printf("%d", x);
  }

  printf("x");
}

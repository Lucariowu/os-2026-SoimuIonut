#include "kernel/types.h"
#include "user/user.h"

void
sieve(int pleft[2])
{
  while(1){
    close(pleft[1]);
    int prime;
    if(read(pleft[0], &prime, sizeof(prime)) != sizeof(prime)){
      close(pleft[0]);
      exit(0);
    }
    printf("prime %d\n", prime);

    int pright[2];
    if(pipe(pright) < 0){
      fprintf(2, "pipe failed\n");
      exit(1);
    }

    int pid = fork();
    if(pid < 0){
      fprintf(2, "fork failed\n");
      exit(1);
    }
    if(pid == 0){
      close(pleft[0]);
      pleft[0] = pright[0];
      pleft[1] = pright[1];
      continue;
    } else {
      close(pright[0]);
      int n;
      while(read(pleft[0], &n, sizeof(n)) == sizeof(n)){
        if(n % prime != 0){
          write(pright[1], &n, sizeof(n));
        }
      }
      close(pleft[0]);
      close(pright[1]);
      wait(0);
      exit(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  if(pipe(p) < 0){
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  }
  if(pid == 0){
    sieve(p);
  } else {
    close(p[0]);
    for(int i = 2; i <= 35; i++){
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}

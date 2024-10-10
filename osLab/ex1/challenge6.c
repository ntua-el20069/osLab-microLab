# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>

int main(int argc, char **argv){

  int pipefd[2], pipefd2[2];

  if(pipe(pipefd)==1){perror("Problem with pipe");  exit(1); }

   dup2(pipefd[0],33);
  dup2(pipefd[1],34);

  if(pipe(pipefd2)==1){perror("Problem with pipe");  exit(1); }


   dup2(pipefd2[0],53);
   dup2(pipefd2[1],54);

  char* array[] ={"riddle",NULL};
  execv("riddle",array);

}

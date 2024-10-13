#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

	pid_t pid = getpid();
	pid_t desired_pid = 32767;
		
	char pathname[]="./riddle";
        char *argv[]={pathname, NULL};
        char *envp[]={NULL};

	if(pid == desired_pid){
		execve(pathname, argv, envp);
	}

	while(pid != desired_pid){
        	
		if((pid=fork())<0) {
                	perror("fork");
                	exit(1);
        	}

		if(pid==0){ // the child
			if(getpid() == desired_pid)
				execve(pathname, argv, envp);
			else
				exit(0);
		}
	}

return 0;

}

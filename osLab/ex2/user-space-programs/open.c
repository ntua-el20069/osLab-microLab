#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){

	char *path = "/dev/lunix5-batt";
	int fd = open(path, O_RDONLY);
	printf("\nFile %s opened with file descriptor fd = %d\n", path, fd);
	sleep(7);
	printf("\nExiting main ...\n\n");
	return 0;
}

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>


  int read_memory(pid_t pid, unsigned long address,char c,size_t size ){
 
    char mem_path[64];
    int fd;
    ssize_t bytes_write;
    char *buffer=malloc(size);
    
    buffer[0]=c;
    
    snprintf(mem_path,sizeof(mem_path),"/proc/%d/mem",pid);
    
    fd=open(mem_path,O_RDWR);
    
    if(fd<0){perror("Faild to open meme path"); exit(1);}
    
    if(lseek(fd,address,SEEK_SET)==-1){
    
    perror("Faild to open meme path"); 
    close(fd);
    exit(1);} 
  
  bytes_write=write(fd,buffer,size);
  
  if(bytes_write==-1){perror("FAiled to read mem"); exit(1);}
  
  close(fd);
   return (int)bytes_write; 

 }

 int main(int argc, char** argv){
 
 int pid=fork();
 
  if(pid==0){
   char* array[]={"riddle",NULL};
   execv("riddle",array); 
  
  }
 
 
 
   
  
 
  char str[17];
  char *end;
  char c;
  int npid;
  scanf("%s %c",str,&c);
  unsigned long address =strtoul(str,&end,16);
  

  int result=read_memory(pid,address,c,1);
 
  
  printf("%d",result);
 
 
 
 
 }     
 
 
  

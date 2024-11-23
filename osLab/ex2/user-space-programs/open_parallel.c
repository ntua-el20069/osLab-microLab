#include <stdio.h>
#include <unistd.h>
#include<string.h>
#include<fcntl.h>
#include <stdlib.h>
#include <pthread.h>


// Compile as : gcc open_parallel.c -o open_parallel -lpthread
// because linker can't find pthread library functions

typedef struct{

char filename[20];

char sensor_number[2];


} thread_data;


char *type[3]={"temp","batt","light"};


void* read_file(void* arg){

   char buffer[20]; 
   
   char path[25];

    thread_data* tdata = (thread_data*)arg;

     for(int i=0; i<3; i++){

      
     snprintf(path,sizeof(path),"%s",tdata->filename);
     snprintf(path + strlen(path) ,sizeof(path)-strlen(path),"%s",type[i]);

   int  fd= open(path,O_RDONLY);
   
    if(fd<0)  {
               

                 
               fprintf(stderr, "Problem with open file:%s\n" ,path) ;
               continue;
             
    }

   else   
   {       
             int res=read(fd,buffer,sizeof(buffer));
              
              if(res<0) {    
              fprintf(stderr, "Problem with read file:%s\n" ,path) ;
               continue;
                                  }
            
            printf("Sensor %s has %s\n",tdata->sensor_number,buffer);

            memset(buffer,'\0', sizeof(buffer));  //clear buffer


            if(close(fd)<0) fprintf(stderr, "Problem with close file:%s\n" ,path) ;
  
   }

}
}

  int main(){

  pthread_t Threads[2];

  thread_data data[2];

  //Print values for sensor 0 and sensor 1
  //Every thread prints a sensor

 
  snprintf(data[0].filename, sizeof(data[0].filename),"/dev/lunix0-");
  snprintf(data[0].sensor_number, sizeof(data[0].sensor_number),"%c",'0');
  
  snprintf(data[1].filename, sizeof(data[1].filename),"/dev/lunix1-");
  snprintf(data[1].sensor_number, sizeof(data[1].sensor_number),"%c",'1');


   for(int j=0; j<=1; j++)
{

    if(pthread_create(&Threads[j], NULL, read_file, &data[j]) != 0){  //data[j] are the arguments for read_file

          fprintf(stderr, "Problem in creating thread %d",j);
          return 1;
    }
}
   for (int j = 0; j< 2; j++) {
        if (pthread_join(Threads[j], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", j);
            return 1;
        }

}





}
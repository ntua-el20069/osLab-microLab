#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
# include <string.h>
#include <stdlib.h>
int main(){

    //Print values for sensor 0 and sensor 1

  char buffer[20];

  char path[35];  

  char num[2];

  char* type[3]={"-temp","-batt","-light"};

  //Open and read Temperature files:


  for(int i=0 ; i<=1 ; i++){

    snprintf(num,sizeof(num),"%d",i); //convert i to string and save it in num

      for(int j=0 ; j<3; j++){
     memset(buffer, '\0', sizeof(buffer)); //clear buffer data
 
   snprintf(path ,sizeof(path),"%s","/dev/lunix");  //path="/dev/lunix"

   snprintf(path + strlen(path), sizeof(path) - strlen(path), "%s", num);  // Use snprintf for safe concatenation
  
   snprintf(path + strlen(path), sizeof(path) - strlen(path), "%s",type[j]); // Use snprintf for safe concatenation


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
            
            printf("Sensor %s has %s\n",num,buffer);

            if(close(fd)<0) fprintf(stderr, "Problem with close file:%s\n" ,path) ;
   }

      }

  }





}
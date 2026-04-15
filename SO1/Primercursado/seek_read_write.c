#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/*
  condicion de carrera: es una cateroria de error de progamacion donde varios proceso manpulan y acceden
  los mismos datos concurrentemente

*/

void characterAtTime(char* str){
  setbuf(stdout, NULL); //no bufferea
  char *ptr;
  int c;
  for(ptr = str; (c = *ptr++)!= 0;){
      putc(c, stdout);
  }

}

char seek_read(int fd, off_t x)
{
  char resC;

  lseek(fd,x,SEEK_SET); // pos es absoluta
  
  int res =  read(fd, &resC, 1);

  return resC;
}
void seek_write(int fd, off_t x,const char c){

  lseek(fd,x,SEEK_SET); // pos es absoluta
  
  int res =  write(fd, &c, 1);

 return; 
}

int main(){
    int fd= open("./file", O_RDWR);

      pid_t pid = fork();
      // int fdes[2];
      // pipe(fdes);
  
  
      if (pid == 0){
          char c= seek_read(fd,13);
          printf("child leyo: %c\n",c ) ;
          seek_write(fd ,13 ,'f');
          c= seek_read(fd,13);
          printf("child escribio: %c\n",c ) ;
      }
      else{

        char c= seek_read(fd,13);
        printf("parent leyo:%c\n",c ) ;
        seek_write(fd ,13 ,'b');
        c= seek_read(fd,13);
        printf("parent escrbio:%c\n",c ) ;
          
      }
  close(fd);
      
  
      return 0;
  }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
  int fdread = open("toChild", O_RDONLY);
  int fdwrite = open("toParent", O_WRONLY);
  // printf("CHILD OPENED PIPE\n");

  while (1) {
    char *array = calloc(50, sizeof(char));
    while (read(fdread,array, 50)) {
      // rot13
      int i = 0;
      int boundleft;
      int boundright;
      for (; i < strlen(array); i++){
        // used to exceed the char size limit of 127
        int target = array[i];
        int charater = 0;
        if (65 <= target && target <= 90){
          // cap charaters
          boundleft = 65;
          boundright = 90;
          charater = 1;

        }else if(97 <= target && target <= 122){
          // lower charaters
          boundleft = 97;
          boundright = 122;
          charater = 1;
        }
        target = target + 13;
        if (charater){
          if (boundright - target < 0){
            // pass the alphabet
            // wrap around
            target = boundleft + target - boundright - 1;
          }
        }

        array[i] = target;
      }
      write(fdwrite, array, 50);
    }
  }

  return 0;
}

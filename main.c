#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char const *argv[]) {

    int p_to_c[2];
    int c_to_p[2];
    pipe(p_to_c);
    pipe(c_to_p);
    if (fork()){
      // parent
      // close unused pipe ends
      close(p_to_c[0]);
      close(c_to_p[1]);
      while (1) {
        printf("what is your input (lower or uppercase)\n");
        char *array = calloc(50, sizeof(char));
        fgets(array, 50, stdin);

        if (array[49] != '\0' && array[49] != '\n'){
          printf("%s\n", "sorry input too large");
          break;
        }

        if (array[strlen(array) -1 ] == '\n'){
          array[strlen(array) -1] = '\0';
        }

        // printf("%s\n", array);
        // send to child
        write(p_to_c[1], array, 50);
        // recieve from child
        read(c_to_p[0],array, 50);
        printf("child rot13: %s\n", array);
      }

    }else{
      // child
      // close unused pipe ends
      close(p_to_c[1]);
      close(c_to_p[0]);

      char *array = calloc(50, sizeof(char));
      while (read(p_to_c[0],array, 50)) {
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
        write(c_to_p[1], array, 50);
      }

    }

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
  mkfifo("toChild", 0644);
  mkfifo("toParent", 0644);
  int fdwrite = open("toChild", O_WRONLY);
  int fdread = open("toParent", O_RDONLY);
  // printf("PARENT OPENED PIPE\n");
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
    write(fdwrite, array, 50);
    // recieve from child
    read(fdread,array, 50);
    printf("child rot13: %s\n", array);
  }

  return 0;
}

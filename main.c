
#include "objloader.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
  char *path = "cube.obj";

  read_file(path);

  printf("%s\n", path);

  return 1;
}

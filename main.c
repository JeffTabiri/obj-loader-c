#include "objloader.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *path = "cube.obj";

  read_file(path);

  return 1;
}

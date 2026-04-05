#include "objloader.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *path = "cube.obj";
  object_attrib_t object;

  int result = read_obj_file(path, &object);

  for (size_t i = 0; i < object.num_indices; i++) {
    printf("%d\n", object.indices[i]);
  }

  return 1;
}

#include "objloader.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *path = "cube.obj";
  object_attrib_t object;

  int result = read_obj_file(path, &object);

  assert(object.num_v == 8);
  assert(object.num_f == 12);
  assert(object.num_vn == 6);

  return 1;
}

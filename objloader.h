#ifndef VIOLA_OBJ_LOADER_H
#define VIOLA_OBJ_LOADER_H

#include <stdio.h>
static void read_file(char *path) {
  int n = 100;
  char line[n];

  FILE *fptr;

  fptr = fopen(path, "r");

  while (fgets(line, n, fptr)) {

    fclose(fptr);
  }
}

#endif // !VIOLA_OBJ_LOADER_H

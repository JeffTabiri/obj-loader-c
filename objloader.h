#ifndef VIOLA_OBJ_LOADER_H
#define VIOLA_OBJ_LOADER_H

#include <stdio.h>

float vertices[4096] = {};

static int read_file(char *path, int *vertices) {
  int n = 4096;
  char line[n];

  FILE *fptr;

  fptr = fopen(path, "r");

  if (fptr == NULL) {
    printf("Error reading file.");
  }

  int vertex = 0;

  while (fgets(line, n, fptr)) {
    // Process vertex normals
    if (line[0] == 'v' && line[0] == 'n') {
    }

    // Handle vertex
    if (line[0] == 'v' && line[1] == ' ') {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      sscanf((line + 1), "%f%f%f", &x, &y, &z);
      vertices[vertex] = x;
      vertices[vertex] = y;
      vertices[vertex] = z;
      vertex += 3;
    }
  }

  fclose(fptr);

  printf("vertex: %d", vertex);
}

#endif // !VIOLA_OBJ_LOADER_H

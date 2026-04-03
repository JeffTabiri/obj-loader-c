#ifndef VIOLA_OBJ_LOADER_H
#define VIOLA_OBJ_LOADER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE (4096)
#define JADE_SUCCESS (0)
#define JADE_ERROR (-1)

typedef struct {
  size_t num_f;
  size_t num_v;
  size_t num_vn;
  float *vertices;
  int *indices;
  float *normals;
} object_attrib_t;

// Make index zero-base and support relative index
static int fix_index(int idx, int n) {
  if (idx > 0)
    return idx - 1;
  return n + idx;
}

static int read_obj_file(char *path, object_attrib_t *vertices) {

  char line[LINE_SIZE];
  FILE *fptr;
  fptr = fopen(path, "r");

  if (fptr == NULL) {
    printf("FILE_READING_ERROR\n");
    return JADE_ERROR;
  }

  size_t num_f = 0;
  size_t num_v = 0;
  size_t num_vn = 0;

  while (fgets((line), 4, fptr)) {
    if (line[0] == 'v' && line[1] == 'n') {
      num_vn++;
    } else if (line[0] == 'v' && line[1] == ' ') {
      num_v++;
    } else if (line[0] == 'f' && line[1] == ' ') {
      num_f++;
    }
  }

  rewind(fptr); // we are going to read again, so start from the top.

  vertices->num_f = num_f;
  vertices->num_v = num_v;
  vertices->num_vn = num_vn;
  vertices->vertices = malloc(num_v * 3 * sizeof(float));
  vertices->normals = malloc(num_vn * 3 * sizeof(float));
  vertices->indices = malloc(num_f * 3 * sizeof(unsigned int));

  size_t current_v = 0;
  size_t current_vn = 0;
  size_t current_indice = 0;

  while (fgets(line, LINE_SIZE, fptr)) {
    // Skip the comments
    if (line[0] == '#')
      continue;

    // Check if there is a vertex
    if (line[0] == 'v' && line[1] == ' ') {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      sscanf((line + 1), "%f%f%f", &x, &y, &z);

      vertices->vertices[(current_v)] = x;
      vertices->vertices[(current_v + 1)] = y;
      vertices->vertices[(current_v + 2)] = z;
      current_v += 3;
    }

    // Process vertex normals
    if (line[0] == 'v' && line[0] == 'n') {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      sscanf((line + 1), "%f%f%f", &x, &y, &z);

      vertices->normals[(current_vn)] = x;
      vertices->normals[(current_vn + 1)] = y;
      vertices->normals[(current_vn + 2)] = z;
      current_vn += 3;
    }

    // Handle faces
    if (line[0] == 'f' && line[1] == ' ') {
      int v1 = 0;
      int v2 = 0;
      int v3 = 0;
      sscanf((line + 1), "%d//%*d %d//%*d %d//%*d", &v1, &v2, &v3);
      vertices->indices[(current_indice)] = fix_index(v1, num_f * 3);
      vertices->indices[(current_indice + 1)] = fix_index(v2, num_f * 3);
      vertices->indices[(current_indice + 2)] = fix_index(v3, num_f * 3);
      current_indice += 3;
    }
  }

  fclose(fptr);

  return JADE_SUCCESS;
}

#endif // !VIOLA_OBJ_LOADER_H

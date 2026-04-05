#ifndef VIOLA_OBJ_LOADER_H
#define VIOLA_OBJ_LOADER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE (4096)
#define JADE_SUCCESS (0)
#define JADE_ERROR (-1)

typedef struct {
  size_t num_faces;
  size_t num_vertices;
  size_t num_vertex_normals;
  size_t num_indices;
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

static int read_obj_file(char *path, object_attrib_t *attrib) {
  char line[LINE_SIZE];
  FILE *fptr;
  fptr = fopen(path, "r");

  if (fptr == NULL) {
    printf("FILE::READ::ERROR\n");
    return JADE_ERROR;
  }

  // First pass collects info for memory allocation
  while (fgets((line), LINE_SIZE, fptr)) {
    if (line[0] == 'v' && line[1] == 'n') {
      attrib->num_vertex_normals++;
    } else if (line[0] == 'v' && line[1] == ' ') {
      attrib->num_vertices++;
    } else if (line[0] == 'f' && line[1] == ' ') {
      attrib->num_faces++;
    }
  }

  rewind(fptr);

  attrib->vertices = malloc(attrib->num_vertices * 3 * sizeof(float));
  attrib->normals = malloc(attrib->num_vertex_normals * 3 * sizeof(float));
  attrib->indices = malloc(attrib->num_faces * 3 * sizeof(unsigned int));
  attrib->num_indices = attrib->num_faces * 3;

  size_t vertex_count = 0;
  size_t vertex_normal_count = 0;
  size_t indice_count = 0;

  while (fgets(line, LINE_SIZE, fptr)) {

    // Skip the comments
    if (line[0] == '#')
      continue;

    // Check if there is a vertex
    if (line[0] == 'v' && line[1] == ' ') {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;

      int result = sscanf((line + 1), "%f%f%f", &x, &y, &z);
      if (result != 3) {
        return JADE_ERROR;
      }

      attrib->vertices[(vertex_count)] = x;
      attrib->vertices[(vertex_count + 1)] = y;
      attrib->vertices[(vertex_count + 2)] = z;
      vertex_count += 3;
    }

    // Process vertex normals
    if (line[0] == 'v' && line[1] == 'n') {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;

      int result = sscanf((line + 2), "%f%f%f", &x, &y, &z);
      if (result != 3) {
        return JADE_ERROR;
      }

      attrib->normals[(vertex_normal_count)] = x;
      attrib->normals[(vertex_normal_count + 1)] = y;
      attrib->normals[(vertex_normal_count + 2)] = z;
      vertex_normal_count += 3;
    }

    // Handle faces
    if (line[0] == 'f' && line[1] == ' ') {
      int v1 = 0;
      int v2 = 0;
      int v3 = 0;

      int result = sscanf((line + 1), "%d//%*d %d//%*d %d//%*d", &v1, &v2, &v3);
      if (result != 3) {
        return JADE_ERROR;
      }
      size_t max_idx_size = attrib->num_faces * 3;
      attrib->indices[(indice_count)] = fix_index(v1, max_idx_size);
      attrib->indices[(indice_count + 1)] = fix_index(v2, max_idx_size);
      attrib->indices[(indice_count + 2)] = fix_index(v3, max_idx_size);
      indice_count += 3;
    }
  }

  fclose(fptr);

  return JADE_SUCCESS;
}

#endif // !VIOLA_OBJ_LOADER_H

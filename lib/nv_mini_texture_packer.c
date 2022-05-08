#include "nv_mini_texture_packer.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int nv_atlas_get_texture_count(const char * file_path) {
  int count = 0;
  FILE * fd = fopen(file_path, "rb");
  fread(&count, sizeof(int), 1, fd);
  fclose(fd);
  return count;
}

void nv_atlas_get_positions(nv_atlas_positions * positions, const char * file_path) {
  FILE * file = fopen(file_path, "rb");
  int size = 0;
  fread(&size, sizeof(int), 1, file);
  for (int i = 0; i < size; i++) {
    fread(&positions[i], sizeof(nv_atlas_positions), 1, file);
  }
  fclose(file);
}

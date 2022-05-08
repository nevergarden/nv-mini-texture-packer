#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nv_mini_texture_packer.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct width_height {
  int width;
  int height;
} width_height;

static int file_list_count;
static char** file_list_paths;

static int first_fit(nv_atlas_positions * positions, width_height * images_data, int image_data_count, int max_size) {
  uint16_t row_height = 0;
  uint16_t pos_x = 0;
  uint16_t pos_y = 0;
  
  width_height data;

  for (int i = 0; i < image_data_count; i++) {
    // hard_copy_data
    data.width = images_data[i].width;
    data.height = images_data[i].height;

    // fit it inside
    if(data.width > max_size || data.height > max_size)
        return -1;
    if(pos_x+data.width >= max_size) {
      pos_x = 0;
      pos_y += row_height;
      row_height = 0;
      if(pos_y+data.height >= max_size)
        return -1;
    }
    if(row_height < data.height)
      row_height = data.height;
    positions[i].id = i;
    positions[i].x = pos_x;
    positions[i].y = pos_y;
    positions[i].w = data.width;
    positions[i].h = data.height;
    pos_x+=data.width;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s size [files...]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int size = atoi(argv[1]);
  if (size == 0) {
    printf("Error: size should be decimal.\n");
    exit(EXIT_FAILURE);
  }

  switch (size) {
    case 32:
    case 64:
    case 128:
    case 256:
    case 512:
    case 1024:
    case 2048:
    case 4096:
    case 8192:
      break;
    default:
      if (size > 8192) {
        printf("Error: does not support texture size bigger than 8192.\n");
      } else {
        printf("Error: size should be power of two.\n");
      }
      exit(EXIT_FAILURE);
  }

  file_list_count = argc - 2;
  file_list_paths = &argv[2];
  width_height images_data[file_list_count];

  for (int i = 0; i < file_list_count; i++)
  {
    images_data[i].width = 0;
    images_data[i].height = 0;
    stbi_info(file_list_paths[i], &images_data[i].width, &images_data[i].height, NULL);
  }

  // Initialize positions and set values
  nv_atlas_positions positions[file_list_count];
  memset(&positions, 0, sizeof(nv_atlas_positions)*file_list_count);
  
  int status = first_fit(positions, images_data, file_list_count, size);
  if(status != 0) {
    printf("Error: pack size is not enough for all images to fit.\n");
    exit(EXIT_FAILURE);
  }
  // open texture to write to
  unsigned char * image = malloc(size*size*4);
  memset(image, 0, size*size*4);
  unsigned char * open_image;
  for (int i = 0; i < file_list_count; i++) {
    int index = positions[i].id;
    int width = images_data[index].width;
    int height = images_data[index].height;
    unsigned char * open_image = stbi_load(file_list_paths[index], &width, &height, NULL, 4);
    for(int x = 0; x < width; x++) {
      for(int y = 0; y < height; y++) {
        unsigned int lil_pos = (x*4)+(y*width*4);
        unsigned int pos = ( (positions[i].x + x) * 4) + ( (positions[i].y + y) * 4 * size);
        memcpy(image+pos, open_image+lil_pos, 4);
      }
    }
    stbi_image_free(open_image);
  }
  // write it to file
  stbi_write_png("packed.png", size, size, 4, image, size*4);
  // free it
  free(image);
  
  // Now write the atlas
  FILE * atlas_fd = fopen("packed.atlas", "wb");
  fwrite(&file_list_count, sizeof(file_list_count), 1, atlas_fd);
  fwrite(&positions, sizeof(nv_atlas_positions), file_list_count, atlas_fd);
  fflush(atlas_fd);
  fclose(atlas_fd);

  // Now write id -> filename into a text file
  atlas_fd = fopen("packed.txt", "w");
  for (int i = 0; i<file_list_count; i++) {
    fprintf(atlas_fd, "%d: %s\r\n", i, file_list_paths[i]);
  }
  fflush(atlas_fd);
  fclose(atlas_fd);

  /* 
  // A test to read atlas and report it's sizes
  int psize = nv_atlas_get_texture_count("packed.atlas");
  nv_atlas_positions p[psize];
  nv_atlas_get_positions(p, "packed.atlas");

  for (int i = 0; i < psize; i++) {
    printf("%d: %d,%d\n", p[i].id, p[i].x, p[i].y);
  }
  */

  exit(EXIT_SUCCESS);
}

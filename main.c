#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct width_height {
  int width;
  int height;
} width_height;

// TODO: align this
typedef struct bin_positions {
  uint8_t id;
  uint16_t x;
  uint16_t y;
} bin_positions;

static int file_list_count;
static char** file_list_paths;

static int first_fit(bin_positions * positions, width_height * images_data, int image_data_count, int max_size) {
  uint16_t row_height = 0;
  uint16_t pos_x = 0;
  uint16_t pos_y = 0;
  int fitted = 0;
  
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
    stbi_info(file_list_paths[i], &images_data[i].width, &images_data[i].height,
              NULL);

  bin_positions positions[file_list_count];
  first_fit(positions, images_data, file_list_count, size);
}

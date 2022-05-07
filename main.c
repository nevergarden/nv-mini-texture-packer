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

static int file_list_count;
static char** file_list_paths;

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
}

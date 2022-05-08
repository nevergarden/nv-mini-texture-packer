# Minimal Buggy Texture Packer
A minimal texture packer with restriction of `UINT8_MAX` count of textures in a single file.
Didn't handle some stuff and it's using a naive first fit algorithm for packing textures. This consists of two parts:
* __tool__: packs textures into .png, .atlas, .txt
* __library__: parses .atlas to get textures info

## Tool
To use the packer you must specify POT number as resault texture size and pass images as arguments:
```sh
nv-mini-texture-packer-tool size files...
```
If run successfuly creates 3 files:
* __packed.png__: packed texture.
* __packed.atlas__: texture atlas x, y, width and height.
* __packed.txt__: id of images cause assets must be retrive with id.

## Library
Pass `packed.atlas` files into it and get data.

```c
nv_atlas_positions {
  uint8_t id;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
}

// first get count of textures in atlas:
int count = nv_atlas_get_texture_count("packed.atlas");
// create a atals positions data struct (can be done use malloc too)
nv_atlas_positions positions[count];
memset(&positions, 0, sizeof(nv_atlas_positions)*count); // can be ignored if don't care about safety.
// get the data
nv_atlas_get_positions(&positions, "packed.atlas");
```

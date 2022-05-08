#ifndef NV_MINI_TEXTURE_PACKER_H
#define NV_MINI_TEXTURE_PACKER_H

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#ifndef PACK
#define PACK( __Declaration__ ) __Declaration__
#endif

#include <stdint.h>
#include <stdio.h>

typedef PACK(struct nv_atlas_positions {
    uint8_t id;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
}) nv_atlas_positions;

int nv_atlas_get_texture_count(const char * file_path);
void nv_atlas_get_positions(nv_atlas_positions * positions, const char * file_path);

#undef PACK
#endif

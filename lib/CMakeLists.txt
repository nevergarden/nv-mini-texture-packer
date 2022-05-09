add_library(nv-mini-texture-packer SHARED nv_mini_texture_packer.c)
set_target_properties(nv-mini-texture-packer PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  PUBLIC_HEADER nv_mini_texture_packer.h
)
install(
  TARGETS nv-mini-texture-packer
)

include_directories(../lib)
add_executable(nv-mini-texture-packer-tool main.c)
target_link_libraries(nv-mini-texture-packer-tool PUBLIC m nv-mini-texture-packer)

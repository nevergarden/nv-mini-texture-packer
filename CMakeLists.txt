cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

project(libnv-mini-texture-packer
  LANGUAGES C
  HOMEPAGE_URL "https://github.com/nevergarden/nv-mini-texture-packer"
  DESCRIPTION "A tiny buggy minimal texture packer"
  VERSION 0.0.2
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/share/pkgconfig/${PROJECT_NAME}.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION "${INSTALL_PKGCONFIG_DIR}")

add_subdirectory(lib)
add_subdirectory(tool)

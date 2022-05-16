project(stb C CXX)

add_library(stb STATIC)

target_include_directories(stb
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/stb"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_c_lexer.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_connected_components.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_divide.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_ds.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_dxt.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_easy_font.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_herringbone_wang_tile.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_hexwave.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_resize.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_write.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_include.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_leakcheck.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_rect_pack.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_sprintf.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_textedit.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_tilemap_editor.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_truetype.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_voxel_render.h"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_vorbis.c"
)

target_sources(stb
PUBLIC
  ${inc}
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/stb" PREFIX "include" FILES ${inc})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/stb" PREFIX "source" FILES ${src})

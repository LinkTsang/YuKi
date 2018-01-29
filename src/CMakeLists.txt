add_subdirectory(samples)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(YUKI_SOURCE_LIST
  "core/typedef.h"
  "core/app.cpp"
  "core/app.h"
  "core/logger.cpp"
  "core/logger.h"
  "core/object.cpp"
  "core/object.h"
  "core/string.hpp"

  "graphics/bitmap.cpp"
  "graphics/bitmap.h"
  "graphics/brush.cpp"
  "graphics/brush.h"
  "graphics/color.cpp"
  "graphics/color.h"
  "graphics/font.cpp"
  "graphics/font.h"
  "graphics/geometry.cpp"
  "graphics/geometry.h"
  "graphics/painter.cpp"
  "graphics/painter.h"

  "ui/uielement.cpp"
  "ui/uielement.h"
  "ui/view.cpp"
  "ui/view.h"
  "ui/weight.cpp"
  "ui/weight.h"
  "ui/window.cpp"
  "ui/window.h"

  "platforms/windows/nativeapp.cpp"
  "platforms/windows/nativeapp.h"
  "platforms/windows/window.cpp"
  "platforms/windows/window.h"
  "platforms/windows/direct2d.cpp"
  "platforms/windows/direct2d.h"
)

add_library(yuki
  ${YUKI_SOURCE_LIST}
)

foreach(source IN LISTS YUKI_SOURCE_LIST)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
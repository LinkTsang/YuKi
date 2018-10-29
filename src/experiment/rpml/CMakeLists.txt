set(RPML_SOURCE_LIST
  "rpml_reader.h"
  "rpml_reader.cc"
  "rpml_loader.h"
  "rpml_loader.cc"
)

add_library(yuki-rpml ${RPML_SOURCE_LIST})

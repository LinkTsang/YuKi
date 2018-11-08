set(SOURCE_LIST
  "property_unittest.cc"
  "property.h"
)

add_executable(yuki_property_test ${SOURCE_LIST})
target_link_libraries(yuki_property_test gtest_main)
add_test(NAME yuki_property_test COMMAND yuki_property_test)

set(TEST_SOURCE_LIST
  "property_unittest.cc"
)

add_executable(yuki_core_test ${TEST_SOURCE_LIST})
target_link_libraries(yuki_core_test yuki)
target_link_libraries(yuki_core_test gtest_main)
set_target_properties(yuki_core_test PROPERTIES FOLDER "Testing")
add_test(NAME yuki_core_test COMMAND yuki_core_test)

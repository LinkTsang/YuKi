include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(helloworld main.cpp)
target_link_libraries(helloworld yuki)
set_target_properties(helloworld PROPERTIES FOLDER "Examples")

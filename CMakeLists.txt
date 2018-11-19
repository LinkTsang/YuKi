cmake_minimum_required (VERSION 3.10)

include(ExternalProject)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

### Boost ###
find_package(Boost 1.68 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

### LLVM ###
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
set_target_properties(intrinsics_gen PROPERTIES FOLDER "ThirdParty")

### Google Test ###
set(GOOGLETEST_VERSION 1.9.0)
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()
include(CMakeDependentOption)
include(GNUInstallDirs)
add_subdirectory(thirdparty/googletest)
set_target_properties(gtest PROPERTIES FOLDER "ThirdParty")
set_target_properties(gtest_main PROPERTIES FOLDER "ThirdParty")

### RTTR ###
set(RTTR_BUILD_COMMAND ${CMAKE_COMMAND} --build . --config $(CONFIGURATION) --target)
set(RTTR_INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install)
ExternalProject_Add(rttr
  PREFIX ${CMAKE_SOURCE_DIR}/thirdparty/rttr
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/rttr
  BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty/rttr
  BUILD_COMMAND ${RTTR_BUILD_COMMAND} RTTR::Core_Lib
  CMAKE_CACHE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_RTTR_DYNAMIC=ON
    -DBUILD_STATIC=ON
    -DBUILD_INSTALLER=OFF

  INSTALL_DIR ${CMAKE_BINARY_DIR}/thirdparty/install/rttr
  INSTALL_COMMAND ${RTTR_INSTALL_COMMAND}
)

set(RTTR_DIR ${CMAKE_BINARY_DIR}/thirdparty/install/rttr)
# find_package(RTTR CONFIG REQUIRED Core)

project(YuKi)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(src)
add_subdirectory(test)

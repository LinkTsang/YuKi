include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(llvm_fibonacci_test fibonacci.cpp)

llvm_map_components_to_libnames(llvm_libs
  Core
  ExecutionEngine
  Interpreter
  MC
  MCJIT
  Support
  nativecodegen
  )

target_link_libraries(llvm_fibonacci_test ${llvm_libs})
set_target_properties(llvm_fibonacci_test PROPERTIES FOLDER "Testing")

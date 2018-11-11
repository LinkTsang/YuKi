include_directories(${PROJECT_SOURCE_DIR}/src/experiment)
set(SOURCE_LIST
  "rpml_loader_test.cc"
)

antlr4cpp_process_grammar(yuki-rpml-test antlrxmlparser
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar/XMLLexer.g4
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar/XMLParser.g4)
source_group("antlr4cpp" FILES ${antlr4cpp_src_files_antlrxmlparser})
foreach(generated_file ${antlr4cpp_src_files_antlrxmlparser})
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set_source_files_properties(
      ${generated_file}
      PROPERTIES
      COMPILE_FLAGS -wd4251)
  endif ()
endforeach(generated_file)
include_directories(${antlr4cpp_include_dirs_antlrxmlparser})

add_executable(yuki-rpml-test ${SOURCE_LIST} ${antlr4cpp_src_files_antlrxmlparser})
add_dependencies(yuki-rpml-test antlr4cpp antlr4cpp_generation_antlrxmlparser)
target_link_libraries(yuki-rpml-test antlr4-runtime)

ADD_CUSTOM_COMMAND(TARGET yuki-rpml-test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANTLR4CPP_LIBS}/antlr4-runtime.dll ${CMAKE_BINARY_DIR}/$<CONFIG>/
  COMMENT "Copying antlr4cpp binaries" VERBATIM)

### Antlr4 ###
set(CMAKE_CXX_STANDARD 11)
add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR4_WITH_STATIC_CRT OFF)#
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.1-complete.jar)
find_package(ANTLR REQUIRED)

set_target_properties(antlr4_runtime PROPERTIES FOLDER "ThirdParty")
set_target_properties(antlr4_runtime-build_shared PROPERTIES FOLDER "ThirdParty")
set_target_properties(antlr4_runtime-build_static PROPERTIES FOLDER "ThirdParty")
set_target_properties(antlr4_runtime-update_repo PROPERTIES FOLDER "ThirdParty")

antlr_target(XMLGrammarLexer grammar/XMLLexer.g4
             LEXER PACKAGE antlrxmlparser)
antlr_target(XMLGrammarParser grammar/XMLParser.g4
             PARSER LISTENER VISITOR
             PACKAGE antlrxmlparser
             DEPENDS_ANTLR XMLGrammarLexer)

message (status "${ANTLR_XMLGrammarLexer_OUTPUT_DIR}")
message (status "${ANTLR_XMLGrammarParser_OUTPUT_DIR}")
message (status "${ANTLR_XMLGrammarParser_CXX_OUTPUTS}")
message (status "${ANTLR_XMLGrammarParser_CXX_OUTPUTS}")

include_directories(${ANTLR_XMLGrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_XMLGrammarParser_OUTPUT_DIR})
source_group("antlr4cpp" FILES
  ${ANTLR_XMLGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_XMLGrammarParser_CXX_OUTPUTS}
)
 
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/src/experiment)
set(SOURCE_LIST
  "rpml_loader_test.cc"
)

add_executable(yuki_rpml_test ${SOURCE_LIST}
  ${ANTLR_XMLGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_XMLGrammarParser_CXX_OUTPUTS}
)
set_target_properties(yuki_rpml_test PROPERTIES FOLDER "Testing")
target_link_libraries(yuki_rpml_test antlr4_static)

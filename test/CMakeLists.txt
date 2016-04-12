set(BIN_NAME jit_test)

include(../cmake/ucm.cmake)
ucm_set_runtime(STATIC)

find_package(Angelscript REQUIRED COMPONENTS scriptbuilder)

source_group(Script FILES ${Angelscript_HEADER} ${Angelscript_SOURCE})

add_executable(${BIN_NAME} test.cpp ${Angelscript_HEADER} ${Angelscript_SOURCE})

#Include our dependencies
target_include_directories(${BIN_NAME} PUBLIC ../include angeljit ${Angelscript_ADDON_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.as DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Link our dependencies
target_link_libraries(${BIN_NAME} angeljit ${Angelscript_LIBRARIES})
set(LIB_NAME angeljit)

include(../cmake/ucm.cmake)
ucm_set_runtime(STATIC)

#Find Angelscript
find_package(Angelscript REQUIRED)

#Embed asmjit
set(ASMJIT_EMBED TRUE)
add_Definitions(-DASMJIT_STATIC )
set(ASMJIT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/asmjit)
include(${ASMJIT_DIR}/CMakeLists.txt)

add_library(${LIB_NAME} angeljit.cpp
                        ../include/angeljit.hpp
                        arch_x86.hpp
                        arch_x86.cpp
                        ${ASMJIT_SRC})

#Include our dependencies
target_include_directories(${LIB_NAME} PUBLIC ${ASMJIT_INCLUDE_DIR} ${Angelscript_INCLUDE_DIRS})

#Link our dependencies
target_link_libraries(${LIB_NAME} ${ASMJIT_DEPS} ${Angelscript_LIBRARIES})

set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 11)
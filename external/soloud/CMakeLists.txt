# helped by Nigel Huens 
file(GLOB_RECURSE 
_SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SOLOUD_SRC_C ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(SOLOUD_SRC ${SOLOUD_SRC_CPP} ${SOLOUD_SRC_C})
add_library(soloud OBJECT ${SOLOUD_SRC})

include(CMakePrintHelpers)
cmake_print_variables(SOLOUD_SRC)

target_compile_features(soloud PUBLIC cxx_std_20)

target_include_directories(soloud PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
cmake_minimum_required (VERSION 3.0)
project (yottabyte_symbolizer)

add_compile_options(-std=c++14)

include_directories(third_party/libelfin include)
add_executable(yottabyte_symbolizer src/symbolizer.cpp)

add_executable(simple examples/simple.cpp)
set_target_properties(simple
                      PROPERTIES COMPILE_FLAGS "-g -O0")


add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/third_party/libelfin
)
target_link_libraries(yottabyte_symbolizer
                      ${PROJECT_SOURCE_DIR}/third_party/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/third_party/libelfin/elf/libelf++.so)
add_dependencies(yottabyte_symbolizer libelfin)

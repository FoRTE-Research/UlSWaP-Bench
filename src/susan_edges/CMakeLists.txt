add_executable(susan_edges ${CMAKE_CURRENT_LIST_DIR}/susan.c)

target_compile_options(susan_edges PRIVATE -Wno-array-bounds)

target_link_libraries(susan_edges c m)

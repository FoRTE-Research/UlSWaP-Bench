add_executable(susan ${CMAKE_CURRENT_LIST_DIR}/susan.c)

target_compile_options(susan PRIVATE -Wno-array-bounds)

target_link_libraries(susan c m)

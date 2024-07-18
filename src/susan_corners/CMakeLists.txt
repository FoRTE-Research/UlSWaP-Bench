target_sources(susan_corners PRIVATE ${CMAKE_CURRENT_LIST_DIR}/susan.c)

target_compile_options(susan_corners PRIVATE -Wno-array-bounds)

target_link_libraries(susan_corners c m)

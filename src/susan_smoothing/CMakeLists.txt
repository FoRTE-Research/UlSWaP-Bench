target_sources(susan_smoothing PRIVATE ${CMAKE_CURRENT_LIST_DIR}/susan.c)

target_compile_options(susan_smoothing PRIVATE -Wno-array-bounds)

target_link_libraries(susan_smoothing c m)

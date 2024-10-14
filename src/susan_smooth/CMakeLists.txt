target_sources(susan_smooth PRIVATE ${CMAKE_CURRENT_LIST_DIR}/susan.c)

target_compile_options(susan_smooth PRIVATE -Wno-array-bounds)

target_link_libraries(susan_smooth c m)

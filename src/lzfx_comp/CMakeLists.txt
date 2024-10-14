target_sources(lzfx_comp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lzfx_compress_test.c
                                     ${CMAKE_CURRENT_LIST_DIR}/lzfx.c)

target_link_libraries(lzfx_comp c)

target_sources(lzfx_compress PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lzfx_compress_test.c
                                     ${CMAKE_CURRENT_LIST_DIR}/lzfx.c)

target_link_libraries(lzfx_compress c)

target_sources(lzfx_decompress PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lzfx_decomp_test.c
                                       ${CMAKE_CURRENT_LIST_DIR}/lzfx.c)

target_link_libraries(lzfx_decompress c)

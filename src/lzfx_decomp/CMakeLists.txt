target_sources(lzfx_decomp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lzfx_decomp_test.c
                                       ${CMAKE_CURRENT_LIST_DIR}/lzfx.c)

target_link_libraries(lzfx_decomp c)

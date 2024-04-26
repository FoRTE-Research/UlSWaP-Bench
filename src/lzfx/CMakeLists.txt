target_sources(lzfx PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lzfx_decomp_test.c
                            ${CMAKE_CURRENT_LIST_DIR}/lzfx_decomp.c)

target_link_libraries(lzfx c)

target_sources(lorawan_down PRIVATE ${CMAKE_CURRENT_LIST_DIR}/aes.c
                                    ${CMAKE_CURRENT_LIST_DIR}/cmac.c
                                    ${CMAKE_CURRENT_LIST_DIR}/lw.c
                                    ${CMAKE_CURRENT_LIST_DIR}/lorawan_down_test.c
                                    ${CMAKE_CURRENT_LIST_DIR}/inputs.c)

target_link_libraries(lorawan_down c)

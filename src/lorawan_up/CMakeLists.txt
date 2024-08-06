target_sources(lorawan_up PRIVATE ${CMAKE_CURRENT_LIST_DIR}/aes.c
                                  ${CMAKE_CURRENT_LIST_DIR}/cmac.c
                                  ${CMAKE_CURRENT_LIST_DIR}/lw.c
                                  ${CMAKE_CURRENT_LIST_DIR}/lorawan_up_test.c)

target_link_libraries(lorawan_up c)

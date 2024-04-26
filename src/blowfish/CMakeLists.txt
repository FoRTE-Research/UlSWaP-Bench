target_sources(blowfish PRIVATE ${CMAKE_CURRENT_LIST_DIR}/blowfish.c
                                ${CMAKE_CURRENT_LIST_DIR}/blowfish_test.c)

target_link_libraries(blowfish c)

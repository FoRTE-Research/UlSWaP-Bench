target_sources(limits PRIVATE ${CMAKE_CURRENT_LIST_DIR}/limit_test.c
                              ${CMAKE_CURRENT_LIST_DIR}/limit_test.stuff.c)

target_link_libraries(limits c)

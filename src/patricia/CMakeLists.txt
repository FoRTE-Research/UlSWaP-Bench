target_sources(patricia PRIVATE ${CMAKE_CURRENT_LIST_DIR}/patricia.c
                                ${CMAKE_CURRENT_LIST_DIR}/patricia_test.c)

target_link_libraries(patricia c)

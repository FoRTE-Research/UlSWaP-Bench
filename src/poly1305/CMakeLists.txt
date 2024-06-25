target_sources(poly1305 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                                ${CMAKE_CURRENT_LIST_DIR}/poly1305.c)

target_link_libraries(poly1305 c)

target_sources(sha256 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                              ${CMAKE_CURRENT_LIST_DIR}/sha256.c)

target_link_libraries(sha256 c)

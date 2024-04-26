target_sources(rsa PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                           ${CMAKE_CURRENT_LIST_DIR}/rsa_test.c
                           ${CMAKE_CURRENT_LIST_DIR}/bn.c)

target_link_libraries(rsa c)

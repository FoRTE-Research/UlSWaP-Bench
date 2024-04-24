add_executable(bitcount ${CMAKE_CURRENT_LIST_DIR}/bitcnt_1.c
                        ${CMAKE_CURRENT_LIST_DIR}/bitcnt_2.c
                        ${CMAKE_CURRENT_LIST_DIR}/bitcnt_3.c
                        ${CMAKE_CURRENT_LIST_DIR}/bitcnt_4.c
                        ${CMAKE_CURRENT_LIST_DIR}/bitcnts.c)

target_link_libraries(bitcount c)

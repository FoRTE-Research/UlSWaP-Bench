add_executable(aes ${CMAKE_CURRENT_LIST_DIR}/main.c
                   ${CMAKE_CURRENT_LIST_DIR}/aes.c)

target_link_libraries(aes c)

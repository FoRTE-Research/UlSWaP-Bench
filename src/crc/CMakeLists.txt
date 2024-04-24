add_executable(crc ${CMAKE_CURRENT_LIST_DIR}/main.c
                   ${CMAKE_CURRENT_LIST_DIR}/crc.c)

target_link_libraries(crc c)

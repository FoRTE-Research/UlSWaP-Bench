target_sources(mp3_encode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/shine_mp3.c
                                  ${CMAKE_CURRENT_LIST_DIR}/mp3_encode_test.c)

target_link_libraries(mp3_encode c m)

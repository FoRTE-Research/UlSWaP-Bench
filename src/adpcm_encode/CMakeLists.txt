add_executable(adpcm_encode ${CMAKE_CURRENT_LIST_DIR}/rawcaudio.c
                            ${CMAKE_CURRENT_LIST_DIR}/adpcm.c)

target_link_libraries(adpcm_encode c)

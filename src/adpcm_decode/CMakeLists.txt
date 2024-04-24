add_executable(adpcm_decode ${CMAKE_CURRENT_LIST_DIR}/rawdaudio.c
                            ${CMAKE_CURRENT_LIST_DIR}/adpcm.c)

target_link_libraries(adpcm_decode c)

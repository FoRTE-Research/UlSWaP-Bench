target_sources(adpcm_encode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/rawcaudio.c
                                    ${CMAKE_CURRENT_LIST_DIR}/adpcm.c)

target_link_libraries(adpcm_encode c)

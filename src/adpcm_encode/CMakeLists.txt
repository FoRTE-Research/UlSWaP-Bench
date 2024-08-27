target_sources(adpcm_encode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/adpcm_encode_test.c
                                    ${CMAKE_CURRENT_LIST_DIR}/adpcm-lib.c)

target_compile_options(adpcm_encode PRIVATE -Wno-stringop-truncation)

target_link_libraries(adpcm_encode c m)

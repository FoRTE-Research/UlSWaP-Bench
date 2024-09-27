target_sources(adpcm_encode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/adpcm_encode_test.c
                                    ${CMAKE_CURRENT_LIST_DIR}/adpcm-lib.c)

if(NOT CMAKE_C_COMPILER MATCHES "clang")
    target_compile_options(adpcm_encode PRIVATE -Wno-stringop-truncation)
endif()

target_link_libraries(adpcm_encode c m)

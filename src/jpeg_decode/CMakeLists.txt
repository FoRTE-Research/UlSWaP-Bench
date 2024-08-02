target_sources(jpeg_decode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picojpeg.c
                                ${CMAKE_CURRENT_LIST_DIR}/jpeg_decode_test.c)

target_compile_options(jpeg_decode PRIVATE -Wno-shift-negative-value)

target_link_libraries(jpeg_decode c)

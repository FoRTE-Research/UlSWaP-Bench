<<<<<<< HEAD
target_sources(jpeg_decode PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picojpeg.c
                                ${CMAKE_CURRENT_LIST_DIR}/jpeg_decode_test.c)

target_compile_options(jpeg_decode PRIVATE -Wno-shift-negative-value)

target_link_libraries(jpeg_decode c)
=======
target_sources(picojpeg PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picojpeg.c
                                ${CMAKE_CURRENT_LIST_DIR}/picojpegtest.c)

target_compile_options(picojpeg PRIVATE -Wno-shift-negative-value)

target_link_libraries(picojpeg c)
>>>>>>> ef7a95f (Add jpeg_encode)

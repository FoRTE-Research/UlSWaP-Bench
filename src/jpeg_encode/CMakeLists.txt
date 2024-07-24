target_sources(jpeg_encode PRIVATE  ${CMAKE_CURRENT_LIST_DIR}/buf.c
                                    ${CMAKE_CURRENT_LIST_DIR}/conf.c
                                    ${CMAKE_CURRENT_LIST_DIR}/enc.c
                                    ${CMAKE_CURRENT_LIST_DIR}/huff.c
                                    ${CMAKE_CURRENT_LIST_DIR}/jpeg_encode_test.c)

target_compile_options(jpeg_encode PRIVATE -Wno-shift-negative-value)

# Uncomment the following line when using a new input image in order to determine the output buffer size
# target_compile_definitions(jpeg_encode PRIVATE NEW_INPUT_IMAGE=1)

target_link_libraries(jpeg_encode c)

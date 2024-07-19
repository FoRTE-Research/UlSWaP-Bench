target_sources(picojpeg PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picojpeg.c
                                ${CMAKE_CURRENT_LIST_DIR}/picojpegtest.c)

target_compile_options(picojpeg PRIVATE -Wno-shift-negative-value)

target_link_libraries(picojpeg c)

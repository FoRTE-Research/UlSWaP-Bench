# TODO: FIXME
# Custom commands should work according to docs, but they don't. Run manually for now.

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_bintoc
                   COMMAND cc -O2 bintoc.c -o g_bintoc
                   DEPENDS ${CMAKE_CURRENT_LIST_DIR}/bintoc.c
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "Building g_bintoc")

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_jpegdata.h
                   COMMAND ./g_bintoc
                   DEPENDS ${CMAKE_CURRENT_LIST_DIR}/bintoc.c ${CMAKE_CURRENT_LIST_DIR}/g_bintoc
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "Generating g_jpegdata.h")

target_sources(picojpeg PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picojpeg.c
                                ${CMAKE_CURRENT_LIST_DIR}/picojpegtest.c
                                ${CMAKE_CURRENT_LIST_DIR}/g_jpegdata.h)

target_compile_options(picojpeg PRIVATE -Wno-shift-negative-value)

# For native builds only, uncomment the following line to write the decoded raw image to a file
# target_compile_definitions(picojpeg PRIVATE HOST_TEST=1)

target_link_libraries(picojpeg c)

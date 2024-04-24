add_executable(fft ${CMAKE_CURRENT_LIST_DIR}/main.c
                   ${CMAKE_CURRENT_LIST_DIR}/fftmisc.c
                   ${CMAKE_CURRENT_LIST_DIR}/fourierf.c)

target_link_libraries(fft c m)

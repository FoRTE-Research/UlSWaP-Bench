target_sources(basicmath PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                                 ${CMAKE_CURRENT_LIST_DIR}/cubic.c
                                 ${CMAKE_CURRENT_LIST_DIR}/isqrt.c
                                 ${CMAKE_CURRENT_LIST_DIR}/rad2deg.c)

# if (${ARCH} STREQUAL "riscv")

target_link_libraries(basicmath m c)

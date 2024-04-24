add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_randmath
    COMMAND cc -o g_randmath ${CMAKE_CURRENT_LIST_DIR}/randmath.c
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/randmath.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Building g_randmath")

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_abcmath.c
    COMMAND ./g_randmath
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/g_randmath
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generating g_abcmath.c")

add_executable(randmath ${CMAKE_CURRENT_LIST_DIR}/main.c
                        ${CMAKE_CURRENT_LIST_DIR}/g_abcmath.c)

target_link_libraries(randmath c)

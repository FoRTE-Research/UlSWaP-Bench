# TODO: FIXME
# Custom commands should work according to docs, but they don't. Run manually for now.
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_randmath
    COMMAND cc ARGS -o g_randmath ${CMAKE_CURRENT_LIST_DIR}/randmath.c
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/randmath.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Building g_randmath")

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/g_abcmath.c
    COMMAND ./g_randmath
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/g_randmath
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generating g_abcmath.c")

target_sources(randmath PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                                ${CMAKE_CURRENT_LIST_DIR}/g_abcmath.c)

define_property(TARGET PROPERTY LOWER_OPT_LEVEL)
set_property(TARGET randmath PROPERTY LOWER_OPT_LEVEL 1)

target_link_libraries(randmath c)

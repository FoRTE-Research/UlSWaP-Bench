target_sources(qsort PRIVATE ${CMAKE_CURRENT_LIST_DIR}/qsort.c)

target_link_libraries(qsort c m)

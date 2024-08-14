target_sources(ar PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                          ${CMAKE_CURRENT_LIST_DIR}/svm.c)

target_link_libraries(ar c m)

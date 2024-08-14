target_sources(anomaly PRIVATE  ${CMAKE_CURRENT_LIST_DIR}/main.c
                                ${CMAKE_CURRENT_LIST_DIR}/svm.c)

target_link_libraries(anomaly c m)

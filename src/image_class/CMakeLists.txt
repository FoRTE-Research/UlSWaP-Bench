target_sources(image_class PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                                  ${CMAKE_CURRENT_LIST_DIR}/tm_layers.c
                                  ${CMAKE_CURRENT_LIST_DIR}/tm_model.c
                                  ${CMAKE_CURRENT_LIST_DIR}/tm_stat.c
                                )

target_link_libraries(image_class c m)

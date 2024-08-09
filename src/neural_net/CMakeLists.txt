target_sources(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                                  ${CMAKE_CURRENT_LIST_DIR}/core/nnom.c
                                  ${CMAKE_CURRENT_LIST_DIR}/core/nnom_layers.c
                                  ${CMAKE_CURRENT_LIST_DIR}/core/nnom_tensor.c
                                  ${CMAKE_CURRENT_LIST_DIR}/core/nnom_utils.c
                                  ${CMAKE_CURRENT_LIST_DIR}/backends/nnom_local.c
                                  ${CMAKE_CURRENT_LIST_DIR}/backends/nnom_local_q15.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_conv2d.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_activation.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_maxpool.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_flatten.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_dense.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_softmax.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_output.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_input.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_baselayer.c
                                  ${CMAKE_CURRENT_LIST_DIR}/layers/nnom_dw_conv2d.c
                                  )

target_include_directories(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/core)
target_include_directories(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/backends)
target_include_directories(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc)
target_include_directories(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/layers)
target_include_directories(neural_net PRIVATE ${CMAKE_CURRENT_LIST_DIR}/port)

target_link_libraries(neural_net c)

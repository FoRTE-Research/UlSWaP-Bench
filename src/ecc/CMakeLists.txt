target_sources(ecc PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c
                           ${CMAKE_CURRENT_LIST_DIR}/ecdh.c)

target_compile_options(ecc PRIVATE -DECC_CURVE=NIST_B571)

target_link_libraries(ecc c)

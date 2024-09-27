target_sources(activity_recognition PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.c)

target_link_libraries(activity_recognition c m)

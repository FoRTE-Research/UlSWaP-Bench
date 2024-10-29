function (set_native_config)
    message(STATUS "Setting native configuration")

    set(CMAKE_C_COMPILER "cc" PARENT_SCOPE)

    if (HOST_TEST)
        message(STATUS "Building for host testing")
        list(APPEND OPTIONAL_FLAGS "-DHOST_TEST=1")
    else()
        list(APPEND OPTIONAL_FLAGS "-DHOST_TEST=0")
    endif()

    set(GENERAL_FLAGS "-static;-Wall;-g;-Og")
    set(ARCH_FLAGS "${GENERAL_FLAGS};${OPTIONAL_FLAGS}" PARENT_SCOPE)

    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c" PARENT_SCOPE)
endfunction()

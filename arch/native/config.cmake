function (set_native_config)
    message(STATUS "Setting native configuration")
    set(CMAKE_C_COMPILER "cc" PARENT_SCOPE)
    set(ARCH_FLAGS "-O3;-static;-Wall;-g" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c" PARENT_SCOPE)
endfunction()
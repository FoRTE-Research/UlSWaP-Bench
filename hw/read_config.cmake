# read_config.cmake
#
# Reads a config.env (shell-style KEY=VALUE) and defines one CMake variable per
# key in the including scope. Values may reference keys defined earlier in the
# file, or environment variables, as $VAR or ${VAR}.
#
# Each hw target keeps its own config.env beside its config.cmake, so point at
# it explicitly before including:
#     set(CONFIG_ENV_FILE "${CMAKE_CURRENT_LIST_DIR}/config.env")
#     include(${CMAKE_CURRENT_LIST_DIR}/../read_config.cmake)
# That way the location of config.env does not depend on where cmake was
# invoked, nor on where this shared helper happens to live. Falling back to a
# config.env next to this file preserves the single-target layout.

if(NOT DEFINED CONFIG_ENV_FILE)
    set(CONFIG_ENV_FILE "${CMAKE_CURRENT_LIST_DIR}/config.env")
endif()

set(ULSWAP_CONFIG_ENV "${CONFIG_ENV_FILE}")

if(NOT EXISTS "${ULSWAP_CONFIG_ENV}")
    message(FATAL_ERROR "Cannot find config.env at ${ULSWAP_CONFIG_ENV}")
endif()

file(STRINGS "${ULSWAP_CONFIG_ENV}" _config_lines)

foreach(_line IN LISTS _config_lines)
    string(STRIP "${_line}" _line)
    if(_line STREQUAL "" OR _line MATCHES "^#")
        continue()
    endif()
    string(FIND "${_line}" "=" _eq)
    if(_eq EQUAL -1)
        continue()
    endif()
    string(SUBSTRING "${_line}" 0 ${_eq} _key)
    math(EXPR _valstart "${_eq} + 1")
    string(SUBSTRING "${_line}" ${_valstart} -1 _val)
    string(STRIP "${_key}" _key)
    string(STRIP "${_val}" _val)
    # strip surrounding quotes if present
    string(REGEX REPLACE "^\"(.*)\"$" "\\1" _val "${_val}")
    string(REGEX REPLACE "^'(.*)'$" "\\1" _val "${_val}")

    # expand $VAR / ${VAR}, preferring keys already read from config.env and
    # falling back to the environment (this is how $HOME resolves)
    while(_val MATCHES "\\$\\{?([A-Za-z_][A-Za-z0-9_]*)\\}?")
        set(_ref "${CMAKE_MATCH_1}")
        if(DEFINED ${_ref})
            set(_sub "${${_ref}}")
        else()
            set(_sub "$ENV{${_ref}}")
        endif()
        string(REGEX REPLACE "\\$\\{?${_ref}\\}?" "${_sub}" _val "${_val}")
    endwhile()

    # drop any trailing slash so "${VAR}/file" never doubles up
    string(REGEX REPLACE "/+$" "" _val "${_val}")

    set(${_key} "${_val}")
endforeach()

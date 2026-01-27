if (NOT TARGET faster)
    message(STATUS "Finding faster")

    set(faster_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
    find_library(faster_LIBRARY NAMES faster PATHS "${CMAKE_CURRENT_LIST_DIR}")
    message(STATUS "faster_LIBRARY: ${faster_LIBRARY}")

    add_library(faster_bin SHARED IMPORTED)
    set_target_properties(faster_bin PROPERTIES
        IMPORTED_IMPLIB "${faster_LIBRARY}"
    )

    add_library(faster INTERFACE)
    target_link_libraries(faster INTERFACE faster_bin)
    target_include_directories(faster INTERFACE ${faster_INCLUDE_DIR})
endif()

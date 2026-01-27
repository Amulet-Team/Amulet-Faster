cmake_minimum_required(VERSION 3.13)

project(amulet_faster LANGUAGES CXX)

set(amulet_faster_DIR ${CMAKE_CURRENT_LIST_DIR}/src/amulet/faster CACHE PATH "")
set(BUILD_AMULET_FASTER_TESTS OFF CACHE BOOL "Should tests be built?")

# Set C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set platform variables
if (WIN32)
	# set windows 8 as the minimum version
	add_definitions(-D_WIN32_WINNT=0x0602)
elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
elseif(UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
else()
    message( FATAL_ERROR "Unsupported platform. Please submit a pull request to support this platform." )
endif()

if (MSVC)
    add_definitions("/MP")
endif()

# Find libraries
if (NOT TARGET pybind11::module)
    find_package(pybind11 CONFIG REQUIRED)
endif()
if (NOT TARGET amulet_pybind11_extensions)
    find_package(amulet_pybind11_extensions CONFIG REQUIRED)
endif()

# Add implementation
#include(FetchContent)
#set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
#FetchContent_Declare(
#  faster
#  GIT_REPOSITORY https://github.com/Amulet-Team/FASTER.git
#  GIT_TAG        7f71289fc1e5a188e90a50f26994fb0d88657dc0
#  SOURCE_SUBDIR cc/src
#)
#FetchContent_MakeAvailable(faster)
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#target_include_directories(faster PUBLIC "${amulet_faster_DIR}/include")
#set_target_properties(faster PROPERTIES FOLDER "CPP")

# Build the FASTER library.
set (FASTER_HEADERS
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/address.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/alloc.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/async.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/async_result_types.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/auto_ptr.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/checkpoint_locks.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/checkpoint_state.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/checkpoint_state_f2.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/config.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/constants.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/f2.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/faster.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/gc_state.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/grow_state.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/guid.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/internal_contexts.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/internal_contexts_f2.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/key_hash.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/light_epoch.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/lss_allocator.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/malloc_fixed_page_size.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/native_buffer_pool.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/persistent_memory_malloc.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/phase.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/read_cache.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/read_cache_utils.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/record.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/recovery_status.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/state_transitions.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/status.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/thread.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/utility.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/device/file_system_disk.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/device/null_disk.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file_common.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/cold_index.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/cold_index_contexts.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/hash_bucket.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/hash_table.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/index.h
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/index/mem_index.h
)

if (MSVC)
set (FASTER_HEADERS ${FASTER_HEADERS}
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file_windows.h
)
else()
set (FASTER_HEADERS ${FASTER_HEADERS}
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file_linux.h
)
endif()

set (FASTER_SOURCES
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/address.cc
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/lss_allocator.cc
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core/thread.cc
)

if (MSVC)
set (FASTER_SOURCES ${FASTER_SOURCES}
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file_windows.cc
)
else()
set (FASTER_SOURCES ${FASTER_SOURCES}
  D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/environment/file_linux.cc
)
endif()

# Find C++ files
file(REAL_PATH src SOURCE_PATH)
file(GLOB_RECURSE EXTENSION_SOURCES LIST_DIRECTORIES false ${SOURCE_PATH}/amulet/*.py.cpp)
file(GLOB_RECURSE EXTENSION_HEADERS LIST_DIRECTORIES false ${SOURCE_PATH}/amulet/*.py.hpp)
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_PATH}/amulet/*.cpp)
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false ${SOURCE_PATH}/amulet/*.hpp)
list(REMOVE_ITEM SOURCES ${EXTENSION_SOURCES})
list(REMOVE_ITEM HEADERS ${EXTENSION_HEADERS})

# Add implementation
add_library(faster SHARED)
set_target_properties(faster PROPERTIES FOLDER "CPP")
target_compile_definitions(faster PRIVATE ExportAmuletFaster)
target_include_directories(faster PUBLIC ${SOURCE_PATH})
target_include_directories(faster PRIVATE "D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src/core")
target_include_directories(faster PRIVATE "D:/Github/Amulet-Faster/build/_deps/faster-src/cc/src")
target_sources(faster PRIVATE ${FASTER_HEADERS} ${FASTER_SOURCES} ${SOURCES} ${HEADERS})
foreach(FILE ${SOURCES} ${HEADERS})
    file(RELATIVE_PATH REL_PATH ${SOURCE_PATH} ${FILE})
    get_filename_component(GROUP ${REL_PATH} DIRECTORY)
    string(REPLACE "/" "\\" GROUP ${GROUP})
    source_group(${GROUP} FILES ${FILE})
endforeach()

# Add python extension
pybind11_add_module(_faster)
set_target_properties(_faster PROPERTIES FOLDER "Python")
target_link_libraries(_faster PRIVATE amulet_pybind11_extensions)
target_link_libraries(_faster PRIVATE faster)
target_compile_definitions(_faster PRIVATE PYBIND11_DETAILED_ERROR_MESSAGES)
target_compile_definitions(_faster PRIVATE PYBIND11_VERSION="${pybind11_VERSION}")
target_compile_definitions(_faster PRIVATE COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
target_compile_definitions(_faster PRIVATE COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
target_sources(_faster PRIVATE ${EXTENSION_SOURCES} ${EXTENSION_HEADERS})
foreach(FILE ${EXTENSION_SOURCES} ${EXTENSION_HEADERS})
    file(RELATIVE_PATH REL_PATH ${SOURCE_PATH} ${FILE})
    get_filename_component(GROUP ${REL_PATH} DIRECTORY)
    string(REPLACE "/" "\\" GROUP ${GROUP})
    source_group(${GROUP} FILES ${FILE})
endforeach()

if(NOT DEFINED AMULET_FASTER_EXT_DIR)
    set(AMULET_FASTER_EXT_DIR ${amulet_faster_DIR})
endif()

# Install
install(TARGETS faster DESTINATION ${amulet_faster_DIR})
install(TARGETS _faster DESTINATION ${AMULET_FASTER_EXT_DIR})

if (BUILD_AMULET_FASTER_TESTS)
    add_subdirectory(tests)
endif()

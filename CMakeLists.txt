# 3.13 as minimum to use `target_link_directories` feature
cmake_minimum_required(VERSION 3.13)

set(C_LIBTORCH_BUILD_TARGET "clibtorch") # also used for libname

project(${C_LIBTORCH_BUILD_TARGET} C CXX)

# options
option(C_LIBTORCH_USE_CACHE "Use ccache for faster recompile." ON)
option(C_LIBTORCH_PYTHON_ENABLED "Set ON if you link with libtorch included in pytorch package(compiled with python module), OFF for standalone libtorch(no python dependency) " ON)
option(C_LIBTORCH_BUILD_TESTS "Build uint tests" ON)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizers)
find_package(Sanitizers) # Address sanitizer (-DSANITIZE_ADDRESS=ON)

find_package(Torch REQUIRED)

if (C_LIBTORCH_PYTHON_ENABLED)
  find_package(Python REQUIRED COMPONENTS Development)
endif (C_LIBTORCH_PYTHON_ENABLED)


set(CMAKE_C_STANDARD 11)

# libtorch 1.8.0 or later requires C++14
# NOTE: C++17 build will fail on VS2019.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE On)


# [ccache]
if(C_LIBTORCH_USE_CCACHE)
  if(MSVC)
    # No ccache support
  else()
    find_program(CCACHE_EXE ccache)
    if(CCACHE_EXE)

      message(STATUS "Use ccache : " ${CCACHE_EXE})
      # CMAKE_C_COMPILER_LAUNCHER = available from cmake 3.4+)
      if(CMAKE_C_COMPILER_LAUNCHER)
        set(CMAKE_C_COMPILER_LAUNCHER "${CMAKE_C_COMPILER_LAUNCHER}"
                                      "${CCACHE_EXE}")
      else()
        set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_EXE}")
      endif()

      if(CMAKE_CXX_COMPILER_LAUNCHER)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER}"
                                        "${CCACHE_EXE}")
      else()
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXE}")
      endif()

    endif(CCACHE_EXE)
  endif()
endif()

# Add custom build type DebugOpt
message("* Adding build types...")
IF (MSVC)
  SET(CMAKE_CXX_FLAGS_DEBUGOPT
      "-DDEBUG /DEBUG /O3"
      CACHE STRING "Flags used by the C++ compiler during coverage builds."
      FORCE )
  SET(CMAKE_C_FLAGS_DEBUGOPT
      "-DDEBUG /DEBUG /O3"
      CACHE STRING "Flags used by the C compiler during coverage builds."
      FORCE )
ELSE () # Assume gcc, clang, etc
  SET(CMAKE_CXX_FLAGS_DEBUGOPT
      "-g -O3 -fno-omit-frame-pointer"
      CACHE STRING "Flags used by the C++ compiler during coverage builds."
      FORCE )
  SET(CMAKE_C_FLAGS_DEBUGOPT
      "-g -O3 -fno-omit-frame-pointer"
      CACHE STRING "Flags used by the C compiler during coverage builds."
      FORCE )
ENDIF()

SET(CMAKE_EXE_LINKER_FLAGS_DEBUGOPT
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_DEBUGOPT
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_DEBUGOPT
    CMAKE_C_FLAGS_DEBUGOPT
    CMAKE_EXE_LINKER_FLAGS_DEBUGOPT
    CMAKE_SHARED_LINKER_FLAGS_DEBUGOPT )

add_library(${C_LIBTORCH_BUILD_TARGET} SHARED
    src/c_libtorch.cc)
add_sanitizers(${C_LIBTORCH_BUILD_TARGET})

set_target_properties(${C_LIBTORCH_BUILD_TARGET}
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Increase warning level for clang.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

  include(CheckCXXCompilerFlag)

  # -Wno-delete-non-abstract-non-virtual-dtor
  # -Wno-delete-non-virtual-dtor
  #  workaround for pbrtParser https://github.com/ingowald/pbrt-parser/issues/39
  check_cxx_compiler_flag("-Wno-delete-non-abstract-non-virtual-dtor" HAS_NO_DELETE_NON_ABSTRACT_NON_VIRTUAL_DTOR)
  if (HAS_NO_DELETE_NON_ABSTRACT_NON_VIRTUAL_DTOR)
    string(APPEND OPTIONAL_CXX_FLAGS " -Wno-delete-non-abstract-non-virtual-dtor ")
  endif ()

  check_cxx_compiler_flag("-Wno-delete-non-virtual-dtor" HAS_NO_DELETE_NON_VIRTUAL_DTOR)
  if (HAS_NO_DELETE_NON_VIRTUAL_DTOR)
    string(APPEND OPTIONAL_CXX_FLAGS " -Wno-delete-non-virtual-dtor ")
  endif ()

  set(CLANG_COMPILE_FLAGS "-Weverything -Werror -Wno-padded -Wno-c++98-compat-pedantic -Wno-documentation -Wno-documentation-unknown-command -Wno-reserved-id-macro -Wno-unused-variable  -Wno-exit-time-destructors -Wno-global-constructors ")

  string(APPEND CLANG_COMPILE_FLAGS ${OPTIONAL_CXX_FLAGS})

  if(MINGW)
    # TODO(LTE): increase warning level for llvm-mingw.
  else()
    set_source_files_properties(
      ${C_LIBTORCH_SOURCES}
      PROPERTIES
      COMPILE_FLAGS
      ${CLANG_COMPILE_FLAGS}
      )
  endif()
endif()


target_include_directories(${C_LIBTORCH_BUILD_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/pybind11/include)

# for dllexport on WIN32
target_compile_definitions(${C_LIBTORCH_BUILD_TARGET} PRIVATE "C_TORCH_COMPILE_LIBRARY")

# TORCH_INSTALL_PREFIX should be available after find_package(Torch)
# Required for -ltorch_python
target_link_directories(${C_LIBTORCH_BUILD_TARGET} PRIVATE ${TORCH_INSTALL_PREFIX}/lib)

if (C_LIBTORCH_PYTHON_ENABLED)
  target_link_libraries(${C_LIBTORCH_BUILD_TARGET} PUBLIC "${TORCH_LIBRARIES}"
      torch_python Python::Python)
else()
  target_link_libraries(${C_LIBTORCH_BUILD_TARGET} PUBLIC "${TORCH_LIBRARIES}")
endif()


if (C_LIBTORCH_BUILD_TESTS)
  enable_testing()

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(CsSignal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CsSignal_INCLUDES "${PROJECT_SOURCE_DIR}/include")
set(CsSignal_DEFINITIONS
  CS_SIGNAL_EXPORTS=
  CS_SIGNAL_NS=cs_signal
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CsSignal_CONFIG_DIRECTORY debug)
else()
  set(CsSignal_CONFIG_DIRECTORY release)
endif()

add_library(CsSignal STATIC IMPORTED GLOBAL)

if (MSVC)
  set_target_properties(CsSignal PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CsSignal_INCLUDES}"
    INTERFACE_COMPILE_DEFINITIONS "${CsSignal_DEFINITIONS}"
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/windows-cl-${CsSignal_CONFIG_DIRECTORY}/CsSignal.lib"
  )
else()
endif()

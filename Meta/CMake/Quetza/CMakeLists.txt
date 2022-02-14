cmake_minimum_required(VERSION 3.16)

project(
    UtopiaOSQuetza
    DESCRIPTION "Orchestrate UtopiaOS builds and deployments"
    LANGUAGES NONE
)

get_filename_component(
    UTOPIA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../.."
    ABSOLUTE CACHE
)

set(UTOPIA_ARCH "x86_64" CACHE STRING "Target Architecture for UtopiaOS")
# Ironic now that GNU is the only one that isn't supported
#set(UTOPIA_TOOLCHAIN "GNU" CACHE STRING "Compiler and set of utils to use for Utopia (For now only GNU is supported)")
# trip is hardcoded for now
#set(UTOPIA_VENDOR "trip" CACHE STRING "The company that is distributing Utopia, default trip")

set(UTOPIA_BUILD_DIR "${PROJECT_BINARY_DIR}/../${UTOPIA_ARCH}")

if (CMAKE_HOST_SYSTEM_NAME MATCHES "UtopiaOS")
    message(STATUS "Thank you for using Utopia :D")
else()
    configure_file("${UTOPIA_SOURCE_DIR}/Factory/Toolchain/CMake/${UTOPIA_TOOLCHAIN}UtopiaStageOne.txt.in" "${UTOPIA_BUILD_DIR}/CMakeToolchain.txt" @ONLY)
    set(UTOPIA_TOOLCHAIN_FILE "${UTOPIA_BUILD_DIR}/CMakeToolchain.txt" CACHE PATH "Toolchain file to use for cross-compilation")
    # Support non-cross builds by stuffing this in a variable
    set(UTOPIA_TOOLCHAIN_FILE_ARG "-DCMAKE_TOOLCHAIN_FILE:STRING=${UTOPIA_TOOLCHAIN_FILE}")
endif()

# Allow the Ninja generators to output messages as they happen by assigning
# these jobs to the 'console' job pool
set(console_access "")
if(CMAKE_GENERATOR MATCHES "^Ninja")
  set(
      console_access
      USES_TERMINAL_CONFIGURE YES
      USES_TERMINAL_BUILD YES
      USES_TERMINAL_INSTALL YES
  )
endif()

include(ExternalProject)

ExternalProject_Add(
    utopia
    SOURCE_DIR "${UTOPIA_SOURCE_DIR}"
    BINARY_DIR "${UTOPIA_BUILD_DIR}"
    CMAKE_CACHE_ARGS
    "-DUTOPIA_ARCH:STRING=${UTOPIA_ARCH}"
    "${UTOPIA_TOOLCHAIN_FILE_ARG}"
    ${console_access}
)
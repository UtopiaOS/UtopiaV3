# By default musl generates this on its own makefile, we can use the power of cmake to do it ourselves
set(LIBSYSTEM_VERSION "1.2.2")

# === Build libSystem shared library ===

# A copy of the C_FLAGS musl makefile uses
set(CMAKE_C_FLAGS 
    "-std=c99 -nostdinc -ffreestanding -fexcess-precision=standard \
    -frounding-math -Wa,--noexecstack \
    -Os -pipe -fomit-frame-pointer -fno-unwind-tables \
    -fno-asynchronous-unwind-tables -ffunction-sections \
    -fdata-sections -Wno-pointer-to-int-cast \
    -Werror=implicit-function-declaration -Werror=implicit-int \
    -Werror=pointer-sign -Werror=pointer-arith \
    -Werror=int-conversion -Werror=incompatible-pointer-types \
    -Werror=discarded-qualifiers -Werror=discarded-array-qualifiers \
    -Waddress -Warray-bounds -Wchar-subscripts -Wduplicate-decl-specifier \
    -Winit-self -Wreturn-type -Wsequence-point -Wstrict-aliasing \
    -Wunused-function -Wunused-label -Wunused-variable")

# The pain of writing this is on my soul and will be forever remebered
# The challenge of getting musl to build with cmake
# Written originally by Diego Magdaleno on 19 January 2022

# Get all our base sources
file(GLOB BASE_GLOBS
        src/*/*.c
        src/malloc/mallocng/*.c
        ldso/*.c)

# Get architecture specific sources
file(GLOB ARCH_GLOBS
        src/*/${UTOPIA_ARCH}/*.[csS])

# Create a new list of arch specific globs but without extension
set(NO_EXT_GLOBS "")
foreach(ARCH_SOURCE IN LISTS ARCH_GLOBS)
    string(REGEX REPLACE "\\.[^.]*$" "" NO_EXTENSION_ARCH ${ARCH_SOURCE})
    get_filename_component(NO_PATH_ARCH ${NO_EXTENSION_ARCH} NAME)
    list(APPEND NO_EXT_GLOBS ${NO_PATH_ARCH})
endforeach()

# The logic here is simple: we iterate over our base_globs we remove the path and extension
# in order to get the filename, then we check if said filename is on the ARCH list we created
# earlier, if thats the case, then we remove it from the original glob.
# Why? If we didn't do this, we would get symbol redefitions complains from ld and we don't want 
# that, if we have an arch specific file, we let that file define the symbol
foreach(SOURCE IN LISTS BASE_GLOBS)
    string(REGEX REPLACE "\\.[^.]*$" "" NO_EXTENSION ${SOURCE})
    get_filename_component(NO_PATH ${NO_EXTENSION} NAME)
    if(NO_PATH IN_LIST NO_EXT_GLOBS)
        list(REMOVE_ITEM BASE_GLOBS ${SOURCE})
    endif()
endforeach()

# This is important, because it contains some defitions for some symbols
file(GLOB ADDTIONAL_ARCH_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/*.[hcS])

# Concatate the sources
set(SYSTEM_OBJECTS_SOURCES
    ${ADDTIONAL_ARCH_SOURCES}
    ${BASE_GLOBS}
    ${ARCH_GLOBS})

# Same strategy as musl, why is it an OBJECT library? in the original makefile
# musl creates non-linked objects that then links into a shared library 
# together with libgcc
add_library(system_objects OBJECT ${SYSTEM_OBJECTS_SOURCES})

#! THIS IS EXTREMALY IMPORTART
#! THE OTHER OF THESE FILES IS CRITICAL
#! IF THE THE ARCH SPECIFIC FILES ARENT IN THE INCLUDE FIRST
#! LD WILL COMPLAIN. DO NOT TOUCH THIS FILES UNLESS YOU KNOW WHAT YOU 
#! ARE DOING.
target_include_directories(system_objects PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_BINARY_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/generic)
# Set our definitions
target_compile_definitions(system_objects PRIVATE
            VERSION="${LIBSYSTEM_VERSION}"
            _XOPEN_SOURCE=700
)
# First we need to create our alltypes and syscall headers
add_dependencies(system_objects alltypes.h syscall.h)

# This is important or LD will complain it cannot realocate the symbols
set_property(TARGET system_objects PROPERTY POSITION_INDEPENDENT_CODE ON)

# Here we do the same strategy, create a shared library that uses the objects that we
# build previously
add_library(System SHARED $<TARGET_OBJECTS:system_objects>)
# Link the gcc library or ld will complain about missing simbols like: __muldc3, __mulsc3
target_link_libraries(System gcc)
# Do not link stdlib or we segfault
# the rest of the flags are taken from musl
target_link_options(System PRIVATE "-nostdlib"
                                    "-Wl,--sort-section,alignment"
                                    "-Wl,--sort-common"
                                    "-Wl,--gc-sections"
                                    "-Wl,--hash-style=both"
                                    "-Wl,--no-undefined"
                                    "-Wl,--exclude-libs=ALL"
                                    "-Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/dynamic.list"
                                    "-Wl,-e,_dlstart")

# === Build crt.o and friends ===

set(GENERIC_CRT_DEFINTIONS CRT _XOPEN_SOURCE=700)
set(COMMON_CRT_INCLUDE_DIRECTORIES                 
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_BINARY_DIR}/include
            )

add_library(crt1 STATIC crt/crt1.c)
set_property(TARGET crt1 PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(crt1 PRIVATE
            ${GENERIC_CRT_DEFINTIONS})
target_include_directories(crt1 PRIVATE ${COMMON_CRT_INCLUDE_DIRECTORIES}
                )
add_dependencies(crt1 alltypes.h syscall.h)
add_custom_command(
    TARGET crt1
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crt1> ${CMAKE_INSTALL_PREFIX}/Core/Libraries/crt1.o
)

if ("${UTOPIA_ARCH}" STREQUAL "x86_64")
    set(CRTI_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/crt/x86_64/crti.s")
    set(CRTN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/crt/x86_64/crtn.s")
endif()

add_library(crti STATIC ${CRTI_SOURCE})
set_property(TARGET crti PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(crti PRIVATE ${GENERIC_CRT_DEFINITIONS})
target_include_directories(crti PRIVATE ${COMMON_CRT_INCLUDE_DIRECTORIES})
add_dependencies(crti alltypes.h syscall.h)

add_custom_command(
    TARGET crti
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crti> ${CMAKE_INSTALL_PREFIX}/Core/Libraries/crti.o   
)

add_library(crtn STATIC ${CRTN_SOURCE})
set_property(TARGET crtn PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(crtn PRIVATE ${GENERIC_CRT_DEFINITIONS})
target_include_directories(crtn PRIVATE ${COMMON_CRT_INCLUDE_DIRECTORIES})
add_dependencies(crtn alltypes.h syscall.h)

add_custom_command(
    TARGET crtn
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crtn> ${CMAKE_INSTALL_PREFIX}/Core/Libraries/crtn.o   
)

# These are the translation of the makefile targets that replace types
# you shouldn't need to touch these
add_custom_target(alltypes.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND sed -f ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkalltypes.sed
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/alltypes.h.in
        > ${CMAKE_CURRENT_BINARY_DIR}/include/bits/alltypes.h
    COMMENT include/bits/alltypes.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/alltypes.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/alltypes.h.in
)

add_custom_target(syscall.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    COMMAND sed -n -e s/__NR_/SYS_/p < ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
        >> ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    COMMENT include/bits/syscall.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
)
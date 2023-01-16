
function(utopia_install_headers target_name)
    file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
    foreach(header ${headers})
        get_filename_component(subdirectory ${header} DIRECTORY)
        install(FILES ${header} DESTINATION Core/Headers/${target_name}/${subdirectory} OPTIONAL)
    endforeach()
endfunction()

function(utopia_install_sources)
    string(LENGTH ${CMAKE_SOURCE_DIR} root_source_dir_length)
    string(SUBSTRING ${CMAKE_CURRENT_SOURCE_DIR} ${root_source_dir_length} -1 current_source_dir_relative)
    file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.c")
    foreach(source ${sources})
        get_filename_component(subdirectory ${source} DIRECTORY)
        install(FILES ${source} DESTINATION System/Sources/${current_source_dir_relative}/${subdirectory} OPTIONAL)
    endforeach()
endfunction()


function(utopia_generated_sources target_name)
    if(DEFINED GENERATED_SOURCES)
        set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED 1)
        foreach(generated ${GENERATED_SOURCES})
            get_filename_component(generated_name ${generated} NAME)
            add_dependencies(${target_name} generate_${generated_name})
            add_dependencies(all_generated generated_${generated_name})
        endforeach()
    endif()
endfunction()

function(utopia_covenant_component covenant_sublib_name)
    utopia_install_headers("covenant")
    install(TARGETS ${covenant_sublib_name} DESTINATION Core/Libraries/covenant)
endfunction()

function(utopia_covenant_component_static covenant_sublib_name fs_name)
    utopia_install_headers("covenant")
    utopia_install_sources("Core/Libraries/${covenant_sublib_name}")
    add_library(${covenant_sublib_name} STATIC ${SOURCES})
    set_target_properties(${covenant_sublib_name} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    install(TARGETS ${covenant_sublib_name} ARCHIVE DESTINATION Core/Libraries/covenant)
    set_target_properties(${covenant_sublib_name} PROPERTIES OUTPUT_NAME ${fs_name})
    utopia_generated_sources(${covenant_sublib_name})
endfunction()

function(utopia_component_interface interface_name)
    add_library(${interface_name} INTERFACE)
    target_include_directories("${interface_name}" INTERFACE "${CMAKE_SOURCE_DIR}/Core/Libraries/${interface_name}/include")
endfunction()

function(utopia_dummy_component dummy_component)
    add_library(${dummy_component} STATIC ${SOURCES})
endfunction()

function(utopia_covenant_component_dynamic covenant_sublib_name fs_name)
    # Don't install the headers again, because all covenant libraries have an static
    # version that we compile before these.
    # Same with the sources
    add_library(${covenant_sublib_name} SHARED ${SOURCES})
    set_target_properties(${covenant_sublib_name} PROPERTIES OUTPUT_NAME ${fs_name})
    set(install_path /Core/Libraries/covenant/lib${fs_name}.dylib) # TODO: This is hacky, use output name in the future

    # 100.1.0 is the default version of all Covenant components
    if (NOT DEFINED COVENANT_DYLIB_VERSION)
        set(COVENANT_DYLIB_VERSION 100.1.0)
    endif()

    set_target_properties(${covenant_sublib_name} PROPERTIES
        PREFIX "lib"
        SUFFIX ".dylib"
        OUTPUT "${fs_name}"
    )

    # Asume we don't need compat
    if (NOT DEFINED COVENANT_DYLIB_COMPAT_VERSION)
        set(COVENANT_DYLIB_COMPAT_VERSION ${COVENANT_DYLIB_VERSION})
    endif()

    target_link_options(${covenant_sublib_name} PRIVATE
        -Wl,-dylib
		-Wl,-dylib_install_name,${install_path}
        -Wl,-dylib_current_version,${COVENANT_DYLIB_VERSION}
        -Wl,-dylib_compatibility_version,${COVENANT_DYLIB_COMPAT_VERSION}
    )

endfunction()

function(utopia_support_component_dynamic libname fs_name)
    add_library(${libname} SHARED ${SOURCES})
    set_target_properties(${libname} PROPERTIES OUTPUT_NAME ${fs_name})
    set(install_path /Core/Support/lib${fs_name}.dylib)
    if (NOT DEFINED SUPPORT_DYLIB_VERSION)
        set(SUPPORT_DYLIB_VERSION 100.1.0)
    endif()

    set_target_properties(${libname} PROPERTIES
        PREFIX "lib"
        SUFFIX ".dylib"
        OUTPUT "${fs_name}"
    )

    if (NOT DEFINED SUPPORT_DYLIB_COMPAT_VERSION)
        set(SUPPORT_DYLIB_COMPAT_VERSION ${SUPPORT_DYLIB_VERSION})
    endif()

    target_link_options(${libname} PRIVATE
        -Wl,-dylib
        -Wl,-dylib_install_name,${install_path}
        -Wl,-dylib_current_version,${SUPPORT_DYLIB_VERSION}
        -Wl,-dylib_compatibility_version,${SUPPORT_DYLIB_COMPAT_VERSION}
    )
endfunction()


function(utopia_library_static libname fs_name)
    utopia_install_headers("")
    utopia_install_sources("Core/Libraries/${libname}")
    add_library(${libname} STATIC ${SOURCES})
    set_target_properties(${libname} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    install(TARGETS ${libname} ARCHIVE DESTINATION Core/Libraries/)
    set_target_properties(${libname} PROPERTIES OUTPUT_NAME ${fs_name})
    utopia_generated_sources(${libname})
endfunction()

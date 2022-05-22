
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


function(utopia_covenant_component_dynamic covenant_sublib_name fs_name)
    # Don't install the headers again, because all covenant libraries have an static
    # version that we compile before these.
    # Same with the sources
    add_library(${covenant_sublib_name} SHARED ${SOURCES})

    set(install_path Core/Libraries/covenant/${fs_name})

    message(${install_path})

    # 100.1.0 is the default version of all Covenant components
    if (NOT DEFINED COVENANT_DYLIB_VERSION)
        set(COVENANT_DYLIB_VERSION 100.1.0)
    endif()

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
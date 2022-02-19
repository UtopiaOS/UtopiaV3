# Routines for libCovenant components


function(covenant_install_headers)
    ## Get anything that is arch/generic first
    file(GLOB_RECURSE
        GENERIC_ARCH_HEADERS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        arch/generic/*/*.h)
    

    foreach(generic_arch_header ${GENERIC_ARCH_HEADERS})
        string(REGEX REPLACE "(arch/generic/)" "" cleaned_header ${generic_arch_header})
        get_filename_component(filename ${cleaned_header} NAME)
        STRING(REPLACE ${filename} "" subdir ${cleaned_header})
        message(${subdir})
        install(FILES ${generic_arch_header} DESTINATION Core/Headers/${subdir})
    endforeach()

    ## Arch specific now
    file(GLOB_RECURSE
        ARCH_SPECIFIC_HEADERS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        arch/${UTOPIA_ARCH}/*/*.h)

    message(${UTOPIA_ARCH})
    foreach(arch_specific_header ${ARCH_SPECIFIC_HEADERS})
        if (${UTOPIA_ARCH} STREQUAL "x86_64")
            string(REGEX REPLACE "(arch/x86_64/)" "" cleaned_header ${arch_specific_header})
            get_filename_component(filename ${cleaned_header} NAME)
            STRING(REPLACE ${filename} "" subdir ${cleaned_header})
            message(${cleaned_header})

            install(FILES ${arch_specific_header} DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Headers/${subdir})
        endif()
    endforeach()

    ## Clean and install our headers now
    file(GLOB_RECURSE
        MAIN_HEADERS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
            include/*/*.h)
        
    foreach(header ${MAIN_HEADERS})
        string(REGEX REPLACE "(include/)" "" cleaned_header ${header})
        get_filename_component(filename ${cleaned_header} NAME)
        STRING(REPLACE ${filename} "" subdir ${cleaned_header}  )
        install(FILES ${header} DESTINATION Core/Headers/${subdir})
    endforeach()
endfunction()


function(covenant_bootstrap_all_include_directories covenant_libraries)
    foreach(covenant_sublib ${covenant_libraries})
        include_directories(Core/Libraries/${covenant_sublib}/include)
        include_directories(Core/Libraries/${covenant_sublib}/internal)
        include_directories(Core/Libraries/${covenant_sublib}/arch/generic)
        include_directories(Core/Libraries/${covenant_sublib}/arch/${UTOPIA_ARCH})
    endforeach()
endfunction()

function(covenant_static_component covenant_sublib_name fs_name)
    covenant_install_headers()
    add_library(${covenant_sublib_name} STATIC ${SOURCES})
    #set_target_properties(${covenant_sublib_name} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    install(TARGETS ${covenant_sublib_name} ARCHIVE DESTINATION Core/Libraries/covenant)
    set_target_properties(${covenant_sublib_name} PROPERTIES OUTPUT_NAME ${fs_name})
endfunction()

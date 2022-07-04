

function(libcompat_configure_types)
    add_custom_target(alltypes.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/Core/Headers/bits
    COMMAND sed -f ${PROJECT_META_DIR}/sed/mkalltypes.sed
        ${PROJECT_SOURCE_DIR}/Core/Libraries/libCompat/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
        ${PROJECT_SOURCE_DIR}/Core/Libraries/libCompat/include/alltypes.h.in
        > ${CMAKE_CURRENT_BINARY_DIR}/Core/Headers/bits/alltypes.h
    COMMENT Core/Headers/alltypes.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/Core/Headers/bits/alltypes.h
    SOURCES ${PROJECT_SOURCE_DIR}/Core/Libraries/libCompat/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
    ${PROJECT_SOURCE_DIR}/Core/Libraries/libCompat/include/alltypes.h.in
    )
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Core/Headers/bits/alltypes.h DESTINATION Core/Headers/bits/)
endfunction()
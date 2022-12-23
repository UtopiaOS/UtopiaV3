

add_executable(test
    test.c)

target_link_libraries(test PRIVATE libC)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/test 
    PERMISSIONS OWNER_READ OWNER_EXECUTE
    DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Binaries
)
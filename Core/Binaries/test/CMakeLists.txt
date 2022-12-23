

add_executable(test
    test.c)

target_link_libraries(test PRIVATE libC)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/test DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Binaries)
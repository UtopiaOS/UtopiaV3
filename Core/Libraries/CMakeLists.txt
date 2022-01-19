# Why did we add libSystem as an external project?
# well since its based on Musl it isnt really designed
# for cmake, and trying to move it to cmake was a never
# ending pain.
# If someone tries to move it to cmake it would be apreciated

include(ExternalProject)

set(LIBSYSTEM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSystem)
set(LIBSYSTEM_BIN ${CMAKE_CURRENT_BINARY_DIR}/libSystem)
set(LIBSYSTEM_LIB ${LIBSYSTEM_LIB}/lib/libSystem.so)


ExternalProject_Add(
    libSystem
    PREFIX ${LIBSYSTEM_BIN}
    SOURCE_DIR ${LIBSYSTEM_DIR}
    CONFIGURE_COMMAND 
    ${CMAKE_COMMAND} -E env PATH=${TOOLCHAIN_PATH}:/usr/bin:/bin 
    ${CMAKE_COMMAND} -E env AR=ar
    ${LIBSYSTEM_DIR}/configure --srcdir=${MODBUS_DIR} --prefix=/Core/Libraries --disable-gcc-wrapper --disable-static --bindir=/Core/Binaries --syslibdir=/Core/Libraries --libdir=/Core/Libraries --includedir=/Core/Headers --exec-prefix=/Core/Binaries --sysconfdir=/System/Settings --localstatedir=/Data/Variable --target=${UTOPIA_ARCH}-trip-linux-utopia
    BUILD_COMMAND ${CMAKE_COMMAND} -E env PATH=${TOOLCHAIN_PATH}:/usr/bin:/bin AR=ar make -j8
    INSTALL_COMMAND ${CMAKE_COMMAND} -E env DESTDIR=${CMAKE_SYSROOT} make install
)


set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
set(CMAKE_EXTRA_SHARED_LIBRARY_SUFFIXES ".tbd" ".so")
set(CMAKE_SHARED_MODULE_PREFIX "lib")
set(CMAKE_SHARED_MODULE_SUFFIX ".so")

# the magic of libCovenant...
set(CMAKE_DL_LIBS "")

set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "-Wl,-rpath,")
#set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP ":")
set(CMAKE_SHARED_LIBRARY_RPATH_ORIGIN_TOKEN "\$ORIGIN")
#set(CMAKE_SHARED_LIBRARY_RPATH_LINK_C_FLAG "-Wl,-rpath-link,")
set(CMAKE_SHARED_LIBRARY_SONAME_C_FLAG "-install_name")
#set(CMAKE_EXE_EXPORTS_C_FLAG "-Wl,--export-dynamic")

# Shared libraries with no builtin soname may not be linked safely by
# specifying the file path.
#set(CMAKE_PLATFORM_USES_PATH_WHEN_NO_SONAME 1)

# Initialize C link type selection flags.  These flags are used when
# building a shared library, shared module, or executable that links
# to other libraries to select whether to use the static or shared
# versions of the libraries.
#foreach(type SHARED_LIBRARY SHARED_MODULE EXE)
 # set(CMAKE_${type}_LINK_STATIC_C_FLAGS "-Wl,-Bstatic")
  #set(CMAKE_${type}_LINK_DYNAMIC_C_FLAGS "-Wl,-Bdynamic")
#endforeach()

include(Platform/UtopiaOSPaths)
include(Platform/UnixPaths)
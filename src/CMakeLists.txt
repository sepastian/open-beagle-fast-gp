# 20091112, sebastian: Replaced 'fdj' by 'gp'.
# 20090622, sebastian: Initial version
#
# The file can be used as a template for other builds.
# OpenBEAGLE should be installed system-wide, normally in /usr/local.
#
# This file sets
#
#   + the CMAKE_BUILD_TYPE (defaults to "Debug")
#   + the OPEN_BEAGLE_DIR, OPEN_BEAGLE_INCLUDE_DIR, and OPEN_BEAGLE_LIB_DIR
#   + the PACC_DIR, PACC_INCLUDE_DIR, PACC_LIB_DIR
#   + BEAGLE_HAVE_DEV_URANDOM
#   + BEAGLE_HAVE_LIBZ
#
# To build another project, adapt the last paragraph in this file.

cmake_minimum_required(VERSION 2.4)

# Set debug variable
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)
message("++ Building in ${CMAKE_BUILD_TYPE} mode")
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DPACC_NDEBUG)
	set(BEAGLE_NDEBUG 1)
	message("++ PACC_NDEBUG variable set")
endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

# Check for OpenBEAGLE.
# Per default, use a system-wide installation.
# Set include and library paths.
if(NOT OPEN_BEAGLE_DIR)
	set(OPEN_BEAGLE_DIR "/usr/local")
	message("++ OPEN_BEAGLE_DIR set to ${OPEN_BEAGLE_DIR}")
endif(NOT OPEN_BEAGLE_DIR)
if(NOT OPEN_BEAGLE_INCLUDE_DIR)
	set(OPEN_BEAGLE_INCLUDE_DIR "${OPEN_BEAGLE_DIR}/include")
	message("++ OPEN_BEAGLE_INCLUDE_DIR set to ${OPEN_BEAGLE_INCLUDE_DIR}")
endif(NOT OPEN_BEAGLE_INCLUDE_DIR)
if(NOT OPEN_BEAGLE_LIB_DIR)
	set(OPEN_BEAGLE_LIB_DIR "${OPEN_BEAGLE_DIR}/lib")
	message("++ OPEN_BEAGLE_LIB_DIR set to ${OPEN_BEAGLE_LIB_DIR}")
endif(NOT OPEN_BEAGLE_LIB_DIR)
include_directories(${OPEN_BEAGLE_INCLUDE_DIR})
link_directories(${OPEN_BEAGLE_INCLUDE_DIR})

# Check for /dev/urandom
if(EXISTS /dev/urandom)
	set(BEAGLE_HAVE_DEV_URANDOM 1)
endif(EXISTS /dev/urandom)

# Check for PACC
if(NOT PACC_DIR)
    set(PACC_DIR "/usr/local")
    message("++ PACC_DIR set to default /usr/local")
endif(NOT PACC_DIR)
if(NOT PACC_INCLUDE_DIR)
    set(PACC_INCLUDE_DIR "${PACC_DIR}/include")
    message("++ PACC_INCLUDE_DIR set to default ${PACC_DIR}/include")
endif(NOT PACC_INCLUDE_DIR)
if(NOT PACC_LIB_DIR)
    set(PACC_LIB_DIR "${PACC_DIR}/lib")
    message("++ PACC_LIB_DIR set to default ${PACC_DIR}/lib")
endif(NOT PACC_LIB_DIR)
include_directories(${PACC_INCLUDE_DIR})
link_directories(${PACC_LIB_DIR})

# Check for zlib
find_package(ZLIB)
if(ZLIB_FOUND)
    add_definitions(-DPACC_ZLIB)
    message("++ PACC_ZLIB variable set")
    include_directories(${ZLIB_INCLUDE_DIR})
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/gzstream.h DESTINATION include)
	set(BEAGLE_HAVE_LIBZ 1)
else(ZLIB_FOUND)
    message("++ Cannot find the zlib library") 
endif(ZLIB_FOUND)

# GP engine
file(GLOB GP_SRC  *.cpp)
file(GLOB GP_DATA *.conf spambase.data ReadMe.txt)
add_executable(gp ${GP_SRC})
add_dependencies(gp openbeagle-GP openbeagle-GA openbeagle pacc)
target_link_libraries(gp openbeagle-GP openbeagle-GA openbeagle pacc dl)
install(TARGETS gp DESTINATION bin/openbeagle/gp)
install(FILES ${gp_DATA} DESTINATION bin/openbeagle/gp)


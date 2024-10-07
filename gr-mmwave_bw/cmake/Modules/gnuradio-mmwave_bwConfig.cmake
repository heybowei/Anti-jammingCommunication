find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_MMWAVE_BW gnuradio-mmwave_bw)

FIND_PATH(
    GR_MMWAVE_BW_INCLUDE_DIRS
    NAMES gnuradio/mmwave_bw/api.h
    HINTS $ENV{MMWAVE_BW_DIR}/include
        ${PC_MMWAVE_BW_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_MMWAVE_BW_LIBRARIES
    NAMES gnuradio-mmwave_bw
    HINTS $ENV{MMWAVE_BW_DIR}/lib
        ${PC_MMWAVE_BW_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-mmwave_bwTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_MMWAVE_BW DEFAULT_MSG GR_MMWAVE_BW_LIBRARIES GR_MMWAVE_BW_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_MMWAVE_BW_LIBRARIES GR_MMWAVE_BW_INCLUDE_DIRS)

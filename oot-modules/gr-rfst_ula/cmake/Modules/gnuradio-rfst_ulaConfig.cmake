find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_RFST_ULA gnuradio-rfst_ula)

FIND_PATH(
    GR_RFST_ULA_INCLUDE_DIRS
    NAMES gnuradio/rfst_ula/api.h
    HINTS $ENV{RFST_ULA_DIR}/include
        ${PC_RFST_ULA_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_RFST_ULA_LIBRARIES
    NAMES gnuradio-rfst_ula
    HINTS $ENV{RFST_ULA_DIR}/lib
        ${PC_RFST_ULA_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-rfst_ulaTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_RFST_ULA DEFAULT_MSG GR_RFST_ULA_LIBRARIES GR_RFST_ULA_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_RFST_ULA_LIBRARIES GR_RFST_ULA_INCLUDE_DIRS)

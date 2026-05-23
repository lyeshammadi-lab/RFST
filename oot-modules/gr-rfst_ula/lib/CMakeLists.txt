########################################################################
# Setup library
########################################################################
include(GrPlatform)

list(APPEND rfst_ula_sources
    adaptive_doa_ula_impl.cc
)

set(rfst_ula_sources "${rfst_ula_sources}" PARENT_SCOPE)
if(NOT rfst_ula_sources)
    message(STATUS "No C++ sources... skipping lib/")
    return()
endif(NOT rfst_ula_sources)

add_library(gnuradio-rfst_ula SHARED ${rfst_ula_sources})
target_link_libraries(gnuradio-rfst_ula
    gnuradio::gnuradio-runtime
)

target_include_directories(gnuradio-rfst_ula
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

set_target_properties(gnuradio-rfst_ula PROPERTIES DEFINE_SYMBOL "gnuradio_rfst_ula_EXPORTS")

# ── Optimisations agressives pour la boucle Gazor ────────────────────
target_compile_options(gnuradio-rfst_ula PRIVATE
    -O3
    -march=native
    -ffast-math
    -funroll-loops
    -fomit-frame-pointer
)

if(APPLE)
    set_target_properties(gnuradio-rfst_ula PROPERTIES INSTALL_NAME_DIR
        "${CMAKE_INSTALL_PREFIX}/lib")
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-rfst_ula)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

# Pas de tests C++ pour ce bloc
list(APPEND test_rfst_ula_sources)

if(NOT test_rfst_ula_sources)
    message(STATUS "No C++ unit tests... skipping")
    return()
endif(NOT test_rfst_ula_sources)
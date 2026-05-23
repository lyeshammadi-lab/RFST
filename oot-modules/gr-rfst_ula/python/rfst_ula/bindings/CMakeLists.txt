# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT rfst_ula_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT rfst_ula_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND rfst_ula_python_files
    adaptive_doa_ula_python.cc python_bindings.cc)

gr_pybind_make_oot(rfst_ula ../../.. gr::rfst_ula "${rfst_ula_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
    TARGET rfst_ula_python
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rfst_ula_python>
            ${PROJECT_BINARY_DIR}/test_modules/gnuradio/rfst_ula/)

install(
    TARGETS rfst_ula_python
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/rfst_ula
    COMPONENT pythonapi)

# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT mmwave_bw_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT mmwave_bw_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND mmwave_bw_python_files
    tx_qam_source_rand_python.cc
    tx_freq_frame_python.cc
    tx_ifft_python.cc
    tx_time_frame_python.cc
    rx_frame_detect_python.cc
    rx_fft_python.cc
    rx_frame_equalization_csi_python.cc
    rx_deqam_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(mmwave_bw
   ../../..
   gr::mmwave_bw
   "${mmwave_bw_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET mmwave_bw_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mmwave_bw_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/mmwave_bw/
)

install(TARGETS mmwave_bw_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/mmwave_bw COMPONENT pythonapi)

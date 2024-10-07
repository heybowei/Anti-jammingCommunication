/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_tx_qam_source_rand(py::module& m);
    void bind_tx_freq_frame(py::module& m);
    void bind_tx_ifft(py::module& m);
    void bind_tx_time_frame(py::module& m);
    void bind_rx_frame_detect(py::module& m);
    void bind_rx_fft(py::module& m);
    void bind_rx_frame_equalization_csi(py::module& m);
    void bind_rx_deqam(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(mmwave_bw_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_tx_qam_source_rand(m);
    bind_tx_freq_frame(m);
    bind_tx_ifft(m);
    bind_tx_time_frame(m);
    bind_rx_frame_detect(m);
    bind_rx_fft(m);
    bind_rx_frame_equalization_csi(m);
    bind_rx_deqam(m);
    // ) END BINDING_FUNCTION_CALLS
}
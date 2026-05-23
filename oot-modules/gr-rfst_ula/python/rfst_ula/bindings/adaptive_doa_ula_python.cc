/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>

namespace py = pybind11;

#include <gnuradio/rfst_ula/adaptive_doa_ula.h>

void bind_adaptive_doa_ula(py::module& m)
{
    using adaptive_doa_ula = gr::rfst_ula::adaptive_doa_ula;

    py::class_<adaptive_doa_ula,
               gr::sync_block,
               gr::block,
               std::shared_ptr<adaptive_doa_ula>>(m, "adaptive_doa_ula")
        .def_static("make", &adaptive_doa_ula::make,
                    py::arg("mu") = 0.05f,
                    py::arg("eta") = 1e-5f,
                    py::arg("initial_theta") = 0.0f)
        .def("set_mu", &adaptive_doa_ula::set_mu)
        .def("set_eta", &adaptive_doa_ula::set_eta)
        .def("get_theta", &adaptive_doa_ula::get_theta);
}
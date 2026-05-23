########################################################################
# Bindings pybind11
########################################################################
include(GrPybind)

GR_PYBIND_MAKE_OOT(rfst_ula
   ../../..
   gnuradio::gnuradio-rfst_ula
   adaptive_doa_ula)

# Copy bindings from build directory to source directory
install(TARGETS rfst_ula_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/rfst_ula COMPONENT pythonapi)
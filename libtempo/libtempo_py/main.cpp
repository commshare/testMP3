#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "module_libtempo.h"
#include "module_audio_io.h"
#include "module_signal_processing.h"
#include <curve_utils.h>
#include "pycast.h"

namespace py = pybind11;

PYBIND11_MODULE(libtempo_py, m) {
    m.doc() = R"pbdoc(
        libtempo_py
        -----------------------
        .. currentmodule:: libtempo_py
        .. autosummary::
            :toctree: _generate

            audio_to_novelty_curve
            novelty_curve_to_tempogram
            tempogram_to_cyclic_tempogram
            smoothen_tempogram
            tempogram_to_tempo_curve
            correct_tempo_curve
            curve_to_sections
            sections_extract_offset
            Section
            audio.AudioFile
            audio.annotation.generate_click
            audio.annotation.click_track_from_positions
            audio.annotation.click_track_from_tempo
            signal.stft
            signal.stft_noc
            signal.compute_fourier_coefficients
    )pbdoc";

    register_libtempo(m);

    auto audio_module = m.def_submodule("audio");
    register_audio_io(audio_module);

    auto signal_module = m.def_submodule("signal");
    register_signal_processing(signal_module);

    /*m.def("test_fn", []() {
        auto test = Mat<double>(10, 1, fill::ones);
        return test;
    });

    m.def("test_fn2", []() {
        vec test = vec(10, fill::ones);
        return std::make_tuple(test,  mat(10, 10, fill::ones));
    });*/
};
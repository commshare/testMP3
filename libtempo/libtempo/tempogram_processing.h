//
// Created by egordm on 28-7-2018.
//

#ifndef PROJECT_TEMPO_ESTIMATION_H
#define PROJECT_TEMPO_ESTIMATION_H

#include <armadillo>
#include "defines.h"

using namespace arma;

namespace libtempo { namespace tempogram_processing {
    /**
     * Computes a novelty curve (onset detection function) for the input audio signal. This implementation is a
     * variant of the widely used spectral flux method with additional bandwise processing and a logarithmic intensity
     * compression. This particularly addresses music with weak onset information (e.g., exhibiting string instruments.)
     * @param feature_rate_ret
     * @param signal: wavefrom of audio signal
     * @param sr: sampling rate of the audio (Hz)
     * @param window_length: window length for STFT (in samples)
     * @param hop_length: stepsize for the STFT
     * @param compression_c: constant for log compression
     * @param log_compression: enable/disable log compression
     * @param resample_feature_rate: feature rate of the resulting novelty curve (resampled, independent of stepsize)
     * @return novelty_curve
     */
    vec audio_to_novelty_curve(int &feature_rate_ret, const fvec &signal, int sr, int window_length = -1, int hop_length = -1,
                                                double compression_c = 1000, bool log_compression = true,
                                                int resample_feature_rate = 200);

    /**
     * Computes a complex valued fourier tempogram for a given novelty curve
     * indicating note onset candidates in the form of peaks.
     * This implementation provides parameters for chosing fourier
     * coefficients in a frequency range corresponding to musically meaningful
     * tempo values in bpm.
     *
     * @param t
     * @param novelty_curve: a novelty curve indicating note onset positions
     * @param bpms: vector containing BPM values to compute
     * @param feature_rate: feature rate of the novelty curve (Hz). This needs to be set to allow for setting other parameters in seconds!
     * @param tempo_window: Analysis window length in seconds
     * @param hop_length: window hop length in frames (of novelty curve)
     * @return tempogram
     */
    cx_mat novelty_curve_to_tempogram_dft(vec &t, const vec &novelty_curve, const vec &bpms, double feature_rate,
                                                                int tempo_window, int hop_length = -1);


    /**
     * Computes a cyclic tempogram representation of a tempogram by identifying octave equivalences, simnilar as for
     * chroma features.
     * @param y_axis
     * @param tempogram: a tempogram representation
     * @param bpm: tempo axis of the tempogram (in bpm)
     * @param octave_divider: number of tempo classes used for representing a tempo octave. This parameter controls the
     * dimensionality of cyclic tempogram
     * @param ref_tempo: reference tempo defining the partition of BPM into tempo octaves
     * @return cyclic_tempogram, cyclic_axis
     */
    mat tempogram_to_cyclic_tempogram(vec &y_axis, const cx_mat &tempogram, const vec &bpm, int octave_divider = 120,
                                                       int ref_tempo = DEFAULT_REF_TEMPO);


}};


#endif //PROJECT_TEMPO_ESTIMATION_H

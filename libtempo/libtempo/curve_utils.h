//
// Created by egordm on 29-8-2018.
//

#ifndef PROJECT_CURVE_UTILS_H
#define PROJECT_CURVE_UTILS_H

#include <armadillo>
#include <ostream>
#include "defines.h"

using namespace arma;

namespace libtempo { namespace curve_utils {
    /**
     * Splits curve in segments with all the uniform value.
     * A returend segments contains indices of the points on the curve.
     *
     * @param curve
     * @return
     */
    std::vector<uvec> split_curve(const vec &curve);

    /**
     * Joins adjacent segments
     *
     * @param segments
     * @return
     */
    std::vector<uvec> join_adjacent_segments(const std::vector<uvec> &segments);

    /**
     * Correct curve by removing short value changes and thus removing small sudden spikes.
     *
     * @param measurements
     * @param min_length: maximum section length in seconds after which section is split in half
     * @return
     */
    vec correct_curve_by_length(const vec &measurements, int min_length);

    /**
     * Correct curve by ignoring measurements which have low confidence
     *
     * TODO: Lots of room for optimization.
     * @param measurements
     * @param confidence
     * @param threshold
     * @return
     */
    vec correct_curve_by_confidence(const vec &measurements, const vec &confidence, float threshold = 0.85);

    struct Section {
        double start, end, bpm, offset;

        Section() = default;

        Section(double start, double end, double bpm, double offset = 0)
                : start(start), end(end), bpm(bpm), offset(offset) {}
    };

    std::ostream &operator<<(std::ostream &os, const Section &section);

    /**
     * Converts segments with indices of a curve to section which holds all the data
     * @param segments
     * @param curve
     * @param t
     * @param bpm_reference
     * @return
     */
    std::vector<Section> tempo_segments_to_sections(const std::vector<uvec> &segments, const vec &curve, const vec &t,
                                                    double bpm_reference = DEFAULT_REF_TEMPO);

    /**
     * Split sections into 2 smaller sections if their length surpasses the maximum. Sectionms will be appended
     * to given list
     * @param section
     * @param sections
     * @param max_section_len
     */
    void split_section(const Section &section, std::vector<Section> &sections, double max_section_len = 60);

    /**
     * Merges adjacent section if delta bpm is lower then the threshold
     * @param sections
     * @param threshold
     * @return
     */
    std::vector<Section> merge_sections(const std::vector<Section> &sections, float threshold = 0.5f);

    /**
     * Merges given sections by averaging bpm and offset
     * @param sections
     * @return
     */
    Section average_sections(const std::vector<Section> &sections);

    /**
     * Finds the optimal offset for given section. Also tunes the bpm to a more fitting value
     * @param novelty_curve
     * @param section
     * @param tempo_multiples: Tempo multiples to consider when searchin for correct offset
     * @param feature_rate
     * @param bpm_doubt_window: Window around candidate bpm which to search for a more fine and correct bpm
     * @param bpm_doubt_step: Steps which to take inside the doubt window to fine tune the bpm
     */
    void extract_offset(const vec &novelty_curve, Section &section, const std::vector<int> &tempo_multiples,
                        int feature_rate, float bpm_doubt_window = 2, double bpm_doubt_step = 0.1);

    /**
     * Correct offset by shifting the offset as far as possible to the beginning of the section by using
     * bar length steps
     * @param section
     * @param smallest_fraction_shift
     */
    void correct_offset(Section &section, int smallest_fraction_shift = 4);
}}


#endif //PROJECT_CURVE_UTILS_H

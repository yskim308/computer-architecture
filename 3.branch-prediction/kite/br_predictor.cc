#include <cstring>
#include "br_predictor.h"

br_predictor_t::~br_predictor_t() {
    // Deallocate the PHT.
    delete [] pht;
}

br_target_buffer_t::~br_target_buffer_t() {
    // Deallocate the target address array.
    delete [] buffer;
}



/**********************************
* Assignment 4: Branch Prediction *
***********************************/

// Branch predictor
br_predictor_t::br_predictor_t(unsigned m_hist_len, unsigned m_pht_bits) :
    bhr(0),
    pht(0),
    h(m_hist_len),
    p(m_pht_bits) {
    // Create a pattern history table (PHT).
    pht = new uint8_t[(1 << h) * (1 << p)];
}
// Is a branch predicted to be taken?
bool br_predictor_t::is_taken(inst_t *m_inst) {
    // Predict always not taken.
    return false;
}

// Update a prediction counter.
void br_predictor_t::update(inst_t *m_inst) {
    bhr = 0;
}



// Branch target buffer
br_target_buffer_t::br_target_buffer_t(uint64_t m_size) :
    num_entries(m_size),
    buffer(0) {
    // Create a direct-mapped branch target buffer (BTB).
    buffer = new uint64_t[num_entries];
}
// Get a branch target address.
uint64_t br_target_buffer_t::get_target(uint64_t m_pc) {
    // Always return PC = 0.
    return 0;
}

// Update the branch target buffer.
void br_target_buffer_t::update(uint64_t m_pc, uint64_t m_target_addr) {
}


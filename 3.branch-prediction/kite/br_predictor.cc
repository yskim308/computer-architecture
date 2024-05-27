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
    //initialize pht to 1 (weakly not taken)
    for (unsigned i = 0; i < h; i++){
        for (unsigned j = 0; j < p; j++){
            pht[i* (1 << p) + j] = 0;
        }
    }
}
// Is a branch predicted to be taken?
bool br_predictor_t::is_taken(inst_t *m_inst) {
    m_inst->pre_update_bhr = bhr; 
    uint64_t shifted_pc = (m_inst->pc >> 2); 
    int row_index = (bhr ^ shifted_pc) & 15;
    int column_index = shifted_pc & 15;
    
    //return true if pht at index is 2 or 3
    if (pht[row_index * (1 << p) + column_index] >= 2){
        return true;
    }
    else{
        return false;
    }
}

// Update a prediction counter.
void br_predictor_t::update(inst_t *m_inst) {
    uint64_t shifted_pc = (m_inst->pc >> 2);        //shift and get lowest 4 bits
    //get row and column index according to shifting logic
    uint8_t pre_update_bhr = m_inst->pre_update_bhr;
    int row_index = (pre_update_bhr ^ shifted_pc) & 15;
    int column_index = shifted_pc & 15;

    pre_update_bhr <<= 1; 

    if (m_inst->branch_taken){
        pre_update_bhr += 1;
        if (pht[row_index * (1<<p) + column_index] !=3 ){
           pht[row_index * (1<<p) + column_index] += 1; 
        }
    }
    else{
        if (pht[row_index * (1<<p) + column_index] != 0){
            pht[row_index * (1<<p) + column_index] -= 1;
        }
    }
    
    bhr = pre_update_bhr & 15; 
}



// Branch target buffer
br_target_buffer_t::br_target_buffer_t(uint64_t m_size) :
    num_entries(m_size),
    buffer(0) {
    // Create a direct-mapped branch target buffer (BTB).
    buffer = new uint64_t[num_entries];
    //initialize btb values to zero
    for (unsigned i = 0; i < num_entries; i++){
        buffer[i] = 0;
    }
}
// Get a branch target address.
uint64_t br_target_buffer_t::get_target(uint64_t m_pc) {
    // Always return PC = 0.
    unsigned buffer_address = (m_pc >> 2) & 15; //shift and get lowest 4 bits 
    return buffer[buffer_address];
}

// Update the branch target buffer.
void br_target_buffer_t::update(uint64_t m_pc, uint64_t m_target_addr) {
    unsigned buffer_address = (m_pc >> 2) & 15; //shift and get lowest 4 bits 
    buffer[buffer_address] = m_target_addr;
}


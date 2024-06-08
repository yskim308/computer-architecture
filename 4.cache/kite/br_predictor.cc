#include <cstring>
#include "br_predictor.h"
#include <stdio.h>

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
    for (unsigned i = 0; i < unsigned(1 << h) ; i++){
        for (unsigned j = 0; j < unsigned(1 << p); j++){
            pht[i* (1 << p) + j] = 1;
        }
    }
}
// Is a branch predicted to be taken?
bool br_predictor_t::is_taken(inst_t *m_inst) {
    m_inst->pre_update_bhr = bhr;               //save bhr to pre_update_bhr 
    uint64_t shifted_pc = (m_inst->pc >> 2);    //shift pc by 2  

    int row_index = (bhr ^ shifted_pc) & 15;// row index = last h-bits of bhr XOR pc >> 2 
    int column_index = shifted_pc & 15;     // column index = = last p-bits of pc >> 2 
    
    int pht_index = row_index * (1 << p) + column_index; //find the index from row and column variables
    //return true if pht at index is 2 or 3
    if (pht[pht_index] >= 2){
        return 1;
    }
    return 0; 
}

// Update a prediction counter.
void br_predictor_t::update(inst_t *m_inst) {
    uint64_t shifted_pc = (m_inst->pc >> 2);            //shift PC right by 2  
    uint8_t pre_update_bhr = m_inst->pre_update_bhr;    //get the saved bhr from is_taken() stage

    int row_index = (pre_update_bhr ^ shifted_pc) & 15; //last 4 bits of bhr XOR shifted PC
    int column_index = shifted_pc & 15;                 //last 4 bits of shifted PC

    int pht_index = row_index * (1<<p) + column_index;  //get index from row and column variables
    //pre_update_bhr = (pre_update_bhr << 1) & 15;        //shift left by one and extract last 4 digits

    bhr <<= 1;                                          //shift bhr by 1 
    if (m_inst->branch_taken){                          //if branch taken, increment as long as its less than 3
        if (pht[pht_index] < 3 ){
           pht[pht_index]++; 
        }
        bhr++;                                          //increment bhr if branch taken 
    }
    else{
        if (pht[pht_index] > 0){                        //otherwise, decrement as long as the count is more than zero
            pht[pht_index] -= 1;
        }
    }
    
}



// Branch target buffer
br_target_buffer_t::br_target_buffer_t(uint64_t m_size) :
    num_entries(m_size),
    buffer(0) {
    // Create a direct-mapped branch target buffer (BTB).
    buffer = new uint64_t[num_entries];
    //initialize btb values to zero
    for (unsigned i = 0; i < num_entries; i++){             //intiialize btb to 0
        buffer[i] = 0;
    }
}
// Get a branch target address.
uint64_t br_target_buffer_t::get_target(uint64_t m_pc) {
    unsigned buffer_address = (m_pc >> 2) & 15;             //shift pc right by 2 and extract last 4 bits for buffer address 
    return buffer[buffer_address];                          //return the address at the buffer_address
}

// Update the branch target buffer.
void br_target_buffer_t::update(uint64_t m_pc, uint64_t m_target_addr) {
    unsigned buffer_address = (m_pc >> 2) & 15;             //shift pc right by 2 and extract last 4 bits for buffer address 
    buffer[buffer_address] = m_target_addr;                 //at buffer address, store the actual target address
}


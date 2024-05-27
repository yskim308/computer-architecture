#ifndef __KITE_INST_H__
#define __KITE_INST_H__

#include <cstdint>
#include "defs.h"

// Kite instruction
class inst_t {
public:
    inst_t();
    inst_t(const inst_t &m_inst);
    ~inst_t();

    uint64_t pc;                        // Program counter
    kite_opcode op;                     // Instruction operation
    int rd_num, rs1_num, rs2_num;       // Register operand indices
    int64_t rd_val, rs1_val, rs2_val;   // Register operand values
    int64_t imm;                        // Immediate
    std::string label;                  // Label
    uint64_t memory_addr;               // Data memory address
    unsigned alu_latency;               // ALU latency
    bool rd_ready;                      // Is the rd value ready?
    uint64_t branch_target;             // Actual branch target
    uint64_t pred_target;               // Predicted branch target
    bool branch_taken;                  // Is a branch actually taken?
    bool pred_taken;                    // Is a branch predicted to be taken?
    uint8_t pre_update_bhr;             // bhr BEFORE update  
};

// Convert Kite instruction to string format.
std::string get_inst_str(inst_t *inst, bool details = false);

#endif


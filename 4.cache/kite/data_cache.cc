#include <cstdlib>
#include <iostream>
#include "data_cache.h"

using namespace std;

data_cache_t::data_cache_t(uint64_t *m_ticks, uint64_t m_cache_size,
                           uint64_t m_block_size, uint64_t m_ways,
                           uint64_t m_victim_size) :
    memory(0),
    ticks(m_ticks),
    blocks(0),
    victim_cache(0),
    cache_size(m_cache_size),
    block_size(m_block_size),
    num_sets(0),
    num_ways(m_ways),
    block_offset(0),
    set_offset(0),
    block_mask(0),
    set_mask(0),
    num_accesses(0),
    num_misses(0),
    num_loads(0),
    num_stores(0),
    missed_inst(0) {
    // Calculate the block offset.
    uint64_t val = block_size;
    while(!(val & 0b1)) {
        val = val >> 1; block_offset++;
        block_mask = (block_mask << 1) | 0b1;
    }
    // Check if the block size is a multiple of doubleword.
    if((block_size & 0b111) || (val != 1)) {
        cerr << "Error: cache block size must be a multiple of doubleword" << endl;
        exit(1);
    }

    // Check if the number of ways is a power of two.
    val = num_ways;
    while(!(val & 0b1)) { val = val >> 1; }
    if(val != 1) {
        cerr << "Error: number of ways must be a power of two" << endl;
        exit(1);
    }

    // Calculate the number of sets.
    num_sets = cache_size / block_size / num_ways;
    // Calculate the set offset and mask.
    val = num_sets;
    while(!(val & 0b1)) {
        val = val >> 1; set_offset++;
        set_mask = (set_mask << 1) | 0b1;
    }
    set_offset += block_offset;
    set_mask = set_mask << block_offset;
    // Check if the number of sets is a power of two.
    if(val != 1) {
        cerr << "Error: number of sets must be a power of two" << endl;
        exit(1);
    }
    
    // Allocate cache blocks.
    blocks = new block_t*[num_sets]();
    for(uint64_t i = 0; i < num_sets; i++) { blocks[i] = new block_t[num_ways](); }

    // Create a victim cache.
    victim_cache = new victim_cache_t(m_victim_size);
}

data_cache_t::~data_cache_t() {
    // Deallocate the cache blocks.
    for(uint64_t i = 0; i < num_sets; i++) { delete [] blocks[i]; }
    delete [] blocks;

    // Destruct the victim cache.
    delete victim_cache;
}

// Connect to the lower-level memory.
void data_cache_t::connect(data_memory_t *m_memory) { memory = m_memory; }

// Is cache free?
bool data_cache_t::is_free() const { return !missed_inst; }

// Read data from cache.
void data_cache_t::read(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    if(addr & 0b111) {
        cerr << "Error: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

    // Calculate the set index and tag.
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Check direct-mapped cache entry.
    block_t *block = &blocks[set_index][0];
    if(!block->valid || (block->tag != tag)) { block = 0; }

    if(block) { // Cache hit
        // Update the last access time.
        block->last_access = *ticks;
        // Read a doubleword in the block.
        m_inst->rd_val = *(block->data + ((addr & block_mask) >> 3));
#ifdef DATA_FWD
        m_inst->rd_ready = true;
#endif
        num_accesses++;
        num_loads++;
    }
    else { // Cache miss
        block_t victimBlock = victim_cache->remove(addr);
        if (victimBlock.valid){
            victimBlock.tag = tag;
            blocks[set_index][0] = victimBlock; 
            read(m_inst);
            return; 
        }
        missed_inst = m_inst;
        memory->load_block(addr & ~block_mask, block_size);
        num_misses++;
#ifdef DEBUG
        cout << *ticks << " : cache miss : addr = " << addr
             << " (tag = " << tag << ", set = " << set_index << ")" << endl;
#endif
    }
}

// Write data in memory.
void data_cache_t::write(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    if(addr & 0b111) {
        cerr << "Error: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

    // Calculate the set index and tag.
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Check the direct-mapped cache entry.
    block_t *block = &blocks[set_index][0];
    if(!block->valid || (block->tag != tag)) { block = 0; }

    if(block) { // Cache hit
        // Update the last access time and dirty flag.
        block->last_access = *ticks;
        block->dirty = true;
        // Write a doubleword in the block.
        *(block->data + ((addr & block_mask) >> 3)) = m_inst->rs2_val;
        num_accesses++;
        num_stores++;
    }
    else { // Cache miss
        block_t victimBlock = victim_cache->remove(addr);
        if (victimBlock.valid){
            victimBlock.tag = tag;
            blocks[set_index][0] = victimBlock;
            write(m_inst);
            return;
        }
        missed_inst = m_inst;
        memory->load_block(addr & ~block_mask, block_size);
        num_misses++;
#ifdef DEBUG
        cout << *ticks << " : cache miss : addr = " << addr
             << " (tag = " << tag << ", set = " << set_index << ")" << endl;
#endif
    }
}

// Handle a memory response.
void data_cache_t::handle_response(int64_t *m_data) {
    // Calculate the set index and tag.
    uint64_t addr = missed_inst->memory_addr;
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Block replacement
    block_t *victim = &blocks[set_index][0];
    if(victim->valid) {
        victim->tag = addr >> 5;
        victim_cache->insert(*victim);
        /*
        cout << *ticks << " : cache block eviction : addr = " << addr
             << " (tag = " << tag << ", set = " << set_index << ")" << endl;
             */
    }
    // Place the missed block.
    *victim = block_t(tag, m_data, /* valid */ true);

    // Replay the cache access.
    if(missed_inst->op == op_ld) { read(missed_inst); }
    else { write(missed_inst); }
    // Clear the missed instruction so that the cache becomes free.
    missed_inst = 0;
}

// Run data cache.
bool data_cache_t::run() {
    memory->run();          // Run the data memory.
    return missed_inst;     // Return true if the cache is busy.
}

// Print cache stats.
void data_cache_t::print_stats() {
    cout << endl << "Data cache stats:" << endl;
    cout.precision(3);
    cout << "    Number of loads = " << num_loads << endl;
    cout << "    Number of stores = " << num_stores << endl;
    cout << "    Miss rate = " << fixed
         << (num_accesses ? double(num_misses) / double(num_accesses) : 0)
         << " (" << num_misses << "/" << num_accesses << ")" << endl;
    cout.precision(-1);

    // Print victim cache stats.
    victim_cache->print_stats();
}

// Victim cache
victim_cache_t::victim_cache_t(uint64_t m_size) :
    num_entries(0),
    size(m_size),
    num_accesses(0),
    num_hits(0),
    num_writebacks(0),
    blocks(0) {
    blocks = new block_t[size]();
}

victim_cache_t::~victim_cache_t() {
    delete [] blocks;
}

block_t victim_cache_t::remove(uint64_t m_addr) {
    block_t block;  // Invalid block 
    num_accesses++; //increment access
    
    uint64_t m_tag = m_addr >> 5;  //tag is address shifted by block offset 
    for (uint64_t i = 0; i < size; i++){ //iterate through the size of the victim caceh
        if (blocks[i].tag == m_tag && blocks[i].valid){ //if the tag matches and the block is valid it is a hit
            num_hits++;
            block = blocks[i]; 
            //at index i shift everything forward
            for (uint64_t j = i + 1; j < size; j++){
                blocks[j-1] = blocks[j];
            }
            num_entries--; //decrease the number of entries after shifting
            return block;  //early return of block 
        }
    }

    /* Search locks[] if there's a matching entry. The matching entry should
       become the returning block, such as:

       block = blocks[i];

       Otherwise, the invalid block is returned to the data cache's query to
       indicate that there's no matching entry in the victim cache. */

    return block;
}

void victim_cache_t::insert(block_t m_block) {
    /* If the victim cache is full, the oldest entry is evicted and written
       back to the memory if dirty, such as: */
    if(num_entries == size) { //if victim is full 
        if(blocks[0].dirty) { num_writebacks++; } //write back if the [0] is dirty (FIFO)
        //iterate from 1 to end and shift 
        for (uint64_t k = 1; k < size; k++){
            blocks[k - 1] = blocks[k];
        }
        num_entries--; //decreement the number of entriesj
    }

    /* The remaining entries are shifted forward, and the new victim block is
       placed at the end of FIFO queue, such as: */
    //place new block at next available index
    blocks[num_entries++] = m_block;

}

void victim_cache_t::print_stats() {
    /* Do not modify this function */
    cout << endl << "Victim cache stats:" << endl;
    cout.precision(3);
    cout << "    Number of writebacks = " << num_writebacks << endl;
    cout << "    Hit rate = " << fixed
         << (num_accesses ? double(num_hits) / double(num_accesses) : 0)
         << " (" << num_hits << "/" << num_accesses << ")" << endl;
    cout.precision(-1);
}


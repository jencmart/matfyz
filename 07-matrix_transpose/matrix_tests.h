#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

/* A matrix stored in a simulated cache */

class CachedMatrix {
    unsigned B;             // Block size
    unsigned mem_blocks;    // Memory size in blocks
    unsigned cache_blocks;  // Cache size in blocks
    unsigned cache_used;    // How many blocks of cache we already used

    // We store the matrix as a one-dimensional array
    // Matrix is 1D array !!!!
    vector<unsigned> items;
    unsigned pos(unsigned i, unsigned j) {
        return i*N + j;
    }

    /*
     *  For each memory block, we keep the following structure.
     *  If the block is currently cached, we set cached == true
     *  and lru_prev/next point to neighboring blocks in the cyclic LRU list.
     *  Otherwise, cached == false and the block is not in the LRU.
     */

    class MemoryBlock {
      public:
        unsigned lru_prev, lru_next;
        bool cached;
        MemoryBlock()
        {
            lru_prev = lru_next = 0;
            cached = false;
        }
    };

    vector<MemoryBlock> blocks;

    // One block at the end of "blocks" serves as a head of the LRU list.
    unsigned lru_head;

  public:
    // Number of rows and columns of the matrix
    unsigned N;

    int debug_level;        // Verbosity

    // Constructor
    CachedMatrix(unsigned N, unsigned M, unsigned B, int debug_level=0)
    {
        EXPECT(N > 0, "CachedMatrix must be non-empty.");
        EXPECT(B > 0, "Blocks must be non-empty.");
        EXPECT(!(M % B), "Cache size must be divisible by block size.");
        EXPECT(M >= 2*B, "Cache must have at least 2 blocks.");

        unsigned NN = N*N;
        items.resize(NN, 0);

        this->N = N;
        this->B = B;
        this->debug_level = debug_level;
        mem_blocks = (NN+B-1) / B;
        cache_blocks = M / B;
        cache_used = 0;

        // Initialize the LRU list
        blocks.resize(mem_blocks + 1);
        lru_head = mem_blocks;
        blocks[lru_head].lru_prev = lru_head;
        blocks[lru_head].lru_next = lru_head;

        if (debug_level > 0)
            cout << "\tMemory: " << mem_blocks << " blocks of " << B << " items, " << cache_blocks << " cached\n";
    }

    // Read value at position (i,j), used only in testing code
    unsigned read(unsigned i, unsigned j)
    {
        EXPECT(i < N && j < N, "Read out of range: " + coord_string(i, j) + ".");
        unsigned addr = pos(i, j);
        access(addr);
        return items[addr];
    }

    // Write value at position (i,j), used only in testing code
    void write(unsigned i, unsigned j, unsigned data)
    {
        EXPECT(i < N && j < N, "Write out of range: " + coord_string(i, j) + ".");
        unsigned addr = pos(i, j);
        access(addr);
        items[addr] = data;
    }

    // Swap items (i1,j1) and (i2,j2)
    // == swap two values !!!
    void swap(unsigned i1, unsigned j1, unsigned i2, unsigned j2)
    {
        EXPECT(i1 < N && j1 < N && i2 < N && j2 < N, "Swap out of range: " + coord_string(i1, j1) + " with " + coord_string(i2, j2) + ".");
        if (debug_level > 1)
            cout << "\tSwap " << coord_string(i1, j1) << " " << coord_string(i2, j2) << endl;
        unsigned addr1 = pos(i1, j1), addr2 = pos(i2, j2);
        access(addr1);
        access(addr2);
        std::swap(items[addr1], items[addr2]);
    }

    unsigned stat_cache_misses;
    unsigned stat_accesses;

    // Reset statistic counters.
    void reset_stats()
    {
        stat_cache_misses = 0;
        stat_accesses = 0;
    }

    static string coord_string(unsigned i, unsigned j)
    {
        return "(" + to_string(i) + "," + to_string(j) + ")";
    }

#include "matrix_transpose.h"



//    void transposeAndSwap(unsigned a_i, unsigned a_j, unsigned b_i, unsigned b_j, unsigned size, bool skip){
//
//        unsigned new_size_a_d = (size + 1) / 2; // a1 and a3 can be overlapping ...
//        unsigned new_size_b_c = size - new_size_a_d;
//
//
//        if(size == 1){
//            if(! skip)
//                swap(a_i, a_j, b_i, b_j);
//            return;
//        }
//
//        bool skip_corner = false;
//        unsigned shift = 0;
//        if (size%2 == 1){
//            skip_corner = true;
//            shift = 1;
//        }
//
//
//        transposeAndSwap(a_i, a_j, b_i, b_j, new_size_a_d, skip_corner); // a1 b1 -- right bottom corner possibly skipped
//        transposeAndSwap(a_i+new_size_a_d-shift, a_j+new_size_a_d-shift, b_i+new_size_a_d-shift, b_j+new_size_a_d-shift, new_size_a_d, skip); // a4 b4
//        transposeAndSwap(a_i, a_j+new_size_a_d, b_i + new_size_a_d, b_j, new_size_b_c, false); // a2 b3
//        transposeAndSwap(a_i + new_size_a_d, a_j, b_i, b_j + new_size_a_d, new_size_b_c, false); // a3 b2
//
//
//    }
//    void transposeRec(unsigned s_i, unsigned s_j, unsigned size){
//        unsigned new_size_a_d = (size + 1) / 2; // a1 and a3 can be overlapping ...
//        unsigned new_size_b_c = size - new_size_a_d;
//        if(size == 1)
//            return;
//        // A1
//        transposeRec(s_i, s_j, new_size_a_d);
//
//        // A4
//        if (size%2 == 0)
//            transposeRec(s_i+new_size_a_d, s_j+new_size_a_d, new_size_a_d);
//        else
//            transposeRec(s_i+new_size_a_d-1, s_j+new_size_a_d-1, new_size_a_d);
//
//        // A2, A3
//        transposeAndSwap(s_i, s_j + new_size_a_d,   s_i+ new_size_a_d, s_j,new_size_b_c, false);
//
//    }
//    void transpose(){
//        transposeRec(0,0,N);
//    }


  private:
    // Bring the given address to the cache.
    void access(unsigned addr)
    {
        int i = addr / B;           // Which block to bring

        if (blocks[i].cached) {
            lru_remove(i);
        }
        else {

            // We still have room in the cache.
            if (cache_used < cache_blocks)
                cache_used++;

            else {
                // We need to evict the least-recently used block to make space.
                unsigned replace = blocks[lru_head].lru_prev;
                lru_remove(replace);
                blocks[replace].cached = false;
            }

            blocks[i].cached = true;
            stat_cache_misses++;
        }


        lru_add_after(i, lru_head);
        stat_accesses++;
    }

    // Remove block from the LRU list.
    void lru_remove(unsigned i)
    {
        unsigned prev = blocks[i].lru_prev;
        unsigned next = blocks[i].lru_next;
        blocks[prev].lru_next = next;
        blocks[next].lru_prev = prev;
    }

    // Add block at the given position in the LRU list.
    void lru_add_after(unsigned i, unsigned after)
    {
        unsigned next = blocks[after].lru_next;
        blocks[next].lru_prev = i;
        blocks[after].lru_next = i;
        blocks[i].lru_next = next;
        blocks[i].lru_prev = after;
    }
};

/* A cached matrix extended by methods for testing */

class TestMatrix : public CachedMatrix {
  public:
    TestMatrix(unsigned N, unsigned M, unsigned B, int debug_level = 0) : CachedMatrix(N, M, B, debug_level) { }

    // Fill matrix with a testing pattern.
    void fill_matrix()
    {
        if (debug_level > 1)
            cout << "\tInitializing\n";
        for (unsigned i = 0; i < N; i++)
            for (unsigned j = 0; j < N; j++)
                write(i, j, i*N + j);
    }

    // Check that the pattern corresponds to the properly transposed matrix.
    void check_result()
    {
        if (debug_level > 1)
            cout << "\tChecking\n";
        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < N; j++) {
                unsigned want = j*N + i;
                unsigned found = read(i, j);
                unsigned found_i = found / N;
                unsigned found_j = found % N;
                EXPECT(found == want,
                       "Mismatch at position " + coord_string(i, j) +
                       ": expected element from " + coord_string(j, i) +
                       ", found element from " + coord_string(found_i, found_j) +
                       ".");
            }
        }
    }

    // Transpose the matrix naively.
    void naive_transpose()
    {
        for (unsigned i=0; i<N; i++)
            for (unsigned j=0; j<i; j++)
                swap(i, j, j, i);
    }
};

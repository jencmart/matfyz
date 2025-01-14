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
    vector<unsigned> items;
    unsigned pos(unsigned i, unsigned j) { return i*N + j; }

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

// #include "matrix_transpose.h" todo


// split A and B to A1 A2 A3 A3 A4 and B1 B2 B3 B4
// transpose and swap A1/B1   A4/B4   A2/B3   A3/B2
    void transposeAndSwap(unsigned Ai, unsigned Aj, unsigned Bi, unsigned Bj, unsigned size, bool skip_right_bottom){
        // new dimensions
        unsigned size_1_4 = (size + 1) / 2; // a1 and a3 can be overlapping ...
        unsigned size_2_3 = size - size_1_4;

        // stopping criterion
        if(size == 1 && ! skip_right_bottom ) swap(Ai, Aj, Bi, Bj);  // prevent duplicate swapping
        if(size == 1) return;

        // for the overlapping case ... matrix A4 have +1 in both dimensions
        unsigned shift = size%2 == 0 ? 0 : 1;
        // for the overlapping case ...  we want to swap middle value only once !
        bool skip_corner = size % 2 != 0;


        // A1 B1 -- right bottom corner of A1 is not swapped (when matrices A1(B1) and A4(B4) are overlapping)
        transposeAndSwap(Ai, Aj, Bi, Bj, size_1_4, skip_corner);
        // A4 B4 -- right corner of A4(B4) skipped (in case when this matrix A(B) was at some point right bottom sub-matrix of A*1(B*1) matrix which was overlapped with A*4(B*4) in upper level of recursion
        transposeAndSwap(Ai + size_1_4 - shift, Aj + size_1_4 - shift, Bi + size_1_4 - shift, Bj + size_1_4 - shift, size_1_4, skip_right_bottom);
        // A2 B3
        transposeAndSwap(Ai, Aj + size_1_4, Bi + size_1_4, Bj, size_2_3, false);
        // A3 B2
        transposeAndSwap(Ai + size_1_4, Aj, Bi, Bj + size_1_4, size_2_3, false);
    }

// split A to A1 A2 A3 A3 A4
// transpose A1, A4
// transpose and swap A2 with A3
    void transposeRec(unsigned Ai, unsigned Aj, unsigned size){
        // new dimensions
        unsigned size_1_4 = (size + 1) / 2; // A1 and A3 can be overlapping ...
        unsigned size_2_3 = size - size_1_4;

        // stopping criterion
        if(size == 1) return;

        // for the overlapping case ... matrix A4 have +1 in both dimensions
        unsigned shift = size%2 == 0 ? 0 : 1;

        // A1
        transposeRec(Ai, Aj, size_1_4);
        // A4
        transposeRec(Ai + size_1_4 - shift, Aj + size_1_4 - shift, size_1_4);
        // A2, A3
        transposeAndSwap(Ai, Aj + size_1_4, Ai + size_1_4, Aj, size_2_3, false);
    }

// initialize recursive transposition
    void transpose(){
        transposeRec(0,0,N);
    }

  private:
    // Bring the given address to the cache.
    void access(unsigned addr)
    {
        int i = addr / B;           // Which block to bring
        if (blocks[i].cached) {
            lru_remove(i);
        } else {
            if (cache_used < cache_blocks) {
                // We still have room in the cache.
                cache_used++;
                if (debug_level > 1)
                  cout << "\t\tLoading block " << i << endl;
            } else {
                // We need to evict the least-recently used block to make space.
                unsigned replace = blocks[lru_head].lru_prev;
                lru_remove(replace);
                EXPECT(blocks[replace].cached, "Internal error: Buggy LRU list.");
                blocks[replace].cached = false;
                if (debug_level > 1)
                  cout << "\t\tLoading block " << i << ", replacing " << replace << endl;
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

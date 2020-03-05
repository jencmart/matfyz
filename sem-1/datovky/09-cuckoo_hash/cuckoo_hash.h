#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include "random.h"

#include <cmath>

using namespace std;

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)

void expect_failed(const string& message);

// ok - make sense
// constructor init the tables --- creates always 4 tables of 8bit values (we hash 32 bit integers)
// num buckets - for the modulo ...
class TabulationHash {
    /*
     * Hash function for hashing by tabulation.
     *
     * The 32-bit key is split to four 8-bit parts. Each part indexes
     * a separate table of 256 randomly generated values. Obtained values
     * are XORed together.
     */

    unsigned num_buckets;
    uint32_t tables[4][256];

public:
    TabulationHash(unsigned num_buckets, RandomGen *random_gen)
    {
        // init the tables using the random values
      this->num_buckets = num_buckets;
      for (int i=0; i<4; i++)
          for (int j=0; j<256; j++)
              tables[i][j] = random_gen->next_u32();
    }

    // create the hash from key using the tables and return the hash
    uint32_t hash(uint32_t key)
    {
        unsigned h0 = key & 0xff;
        unsigned h1 = (key >> 8) & 0xff;
        unsigned h2 = (key >> 16) & 0xff;
        unsigned h3 = (key >> 24) & 0xff;
        return (tables[0][h0] ^ tables[1][h1] ^ tables[2][h2] ^ tables[3][h3]) % num_buckets;
    }
};

class CuckooTable {
    /*
     * Hash table with Cuckoo hashing.
     * We have two hash functions, which map 32-bit keys to buckets of a common
     * hash table. Unused buckets contain 0xffffffff.
     */
    const uint32_t UNUSED = 0xffffffff;

    // The array of buckets
    vector<uint32_t> table;
    unsigned num_buckets;

    // Hash functions and the random generator used to create them
    TabulationHash *hashes[2];
    RandomGen *random_gen;

    // number of the items in the hash table
    unsigned n;

    // insertion timeout
    unsigned timeout;

public:

    CuckooTable(unsigned num_buckets)
    {
        // Initialize the table with the given number of buckets.
	    // The number of buckets is expected to stay constant.
        this->num_buckets = num_buckets;
        table.resize(num_buckets, UNUSED);

        // Obtain two fresh hash functions.
        random_gen = new RandomGen(42);
        for (auto & hash_function : hashes)
            hash_function = new TabulationHash(num_buckets, random_gen);

        // no items in the table
        this->n = 0;

        // timeout is equal to zero
        this->timeout = 0;
    }

    ~CuckooTable()
    {
        for (auto & hash_function : hashes)
            delete hash_function;
        delete random_gen;
    }

    bool lookup(uint32_t key)
    {
        // Check if the table contains the given key. Returns True or False.
        unsigned h0 = hashes[0]->hash(key);
        unsigned h1 = hashes[1]->hash(key);
        return (table[h0] == key || table[h1] == key);
    }

    void rehash_all(uint32_t key){
        // clear the buckets & save the keys
        vector<uint32_t> keys_from_hashtable;
        keys_from_hashtable.emplace_back(key);
        for(unsigned i = 0; i < num_buckets ; i++){
            if(table[i] != UNUSED){
                keys_from_hashtable.emplace_back(table[i]);
                table[i] = UNUSED;
            }
        }

        while(true) {
            // delete old hashes && create new
            for (auto & hash_function : hashes) {
                delete hash_function;
                hash_function = new TabulationHash(num_buckets, random_gen);
            }

            // try to insert all the values again
            bool failure = false;
            for(auto & k : keys_from_hashtable){
                if(this->insert_wrapper(k) != UNUSED){
                    // inserting value k took too long ...
                    failure = true;
                    break;
                }
            }

            // we are done after successful insertion of all keys
            if(! failure)
                break;

            // we did not succeed even with different hashes -> clear what we have hashed so far & start again
            for(unsigned i = 0; i < num_buckets ; i++)
                if(table[i] != UNUSED)
                    table[i] = UNUSED;
        }
    }

    uint32_t insert_cuckoo_way(uint32_t key, int counter, int hash_fce_id){
        // stopping criterion -- unsuccessful
        if(counter > this->timeout)
            return key;

        // stopping criterion -- successful
        uint32_t hash = this->hashes[hash_fce_id]->hash(key);
        if (this->table[hash] == UNUSED) {
            table[hash] = key;
            return UNUSED;
        }

        // bucket full -> kick out the value and set this value instead
        uint32_t kicked_key = this->table[hash];
        this->table[hash] = key;

        // determine which hash function was used for kicked_key and use the latter
        int not_used_hash_fce = 0;
        if(this->hashes[not_used_hash_fce]->hash(kicked_key) == hash) // use the other hash
            not_used_hash_fce = 1;

        // finally insert kicked out value
        return insert_cuckoo_way(kicked_key, counter+1, not_used_hash_fce);
    }

    uint32_t insert_wrapper(uint32_t key){
        // try to insert the key to the empty bucket
        for(auto & hash_fce : this->hashes){
            uint32_t hash = hash_fce->hash(key);
            if (this->table[hash] == UNUSED) {
                table[hash] = key;
                return UNUSED;
            }
        }

        // unsuccessful - we need to use the cuckoo strategy
        return  insert_cuckoo_way(key, 1, 1); // we always start with the latter hash function
    }

    void insert(uint32_t key) {
        EXPECT(key != UNUSED, "Keys must differ from UNUSED.");
        uint32_t res = insert_wrapper(key);

        // res != if insertion was unsuccessful & contains not inserted key
        if( res != UNUSED)
            this->rehash_all(res);

        // one more item in the data structure
        this->n += 1;
        // n=0 -> timeout=0 , n=1 -> timeout=0, n=2 -> timeout=6, ...
        timeout =  (int) (6 * log2 (this->n) + 0.5); // 0.5 for the ceil
    }
};
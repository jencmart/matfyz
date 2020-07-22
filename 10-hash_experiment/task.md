## Goal
evaluate Linear probing hash table with different systems of hash functions.
program which implements everything needed to perform the following experiments.

- Grow experiment: 
for different sizes $N$ of the hash table:
1. inserts small keys in random order until 60% of the table is used
2. then it performs lookup operation for keys $0, \ldots, N-1$.
 
- Usage experiment: 
hash table of size $2^{20}$. 
1. It performs insertions to increase usage of the table by 1%  && reports efficiency of the insert operation
2. repeats the same until usage of the table reaches 90%

Both measure number of probed buckets per operation
Both are repeated 40 times and report average and standard deviation. 
Note: even with 40 repetitions the reported numbers still depend quite a lot on the random seed used 


perform both for 5 different classes of hash functions –
tabulation
multiply-shift which uses top bits of 32-bit word (`ms-low`),
multiply-shift which uses low bits of upper half of 64-bit word (`ms-high`),
and polynomial hash function of degree 1 and 2 

write a report, which contains two plots of the measured data for each experiment. 
first plot: average complexity of operations
second: standard deviation

x axis: 
- size of the hash table (the grow experiment) 
- usage of the hash table (the usage experiment)

## Test program
two arguments:
- The name of the test `{grow,usage}-{ms-low,ms-high,poly-1,poly-2,tab}`
- random seed: last 2 digits of your student ID 

one line per experiment, which consists of set size and the average number of structural changes.
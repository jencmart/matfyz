#### run these experiments and write a report, which contains:
* one plot of the measured data for each cache type, 
* showing dependency of the average number of misses on the matrix size. 
* two curves in each plot: one for your algorithm, another for the trivial one.
* discuss observed behavior (including any strange anomalies)  using theory from the lectures.
* If you want, you can carry out further experiments to gain better understanding of the algorithm 

## Test program
The test program is given two arguments:
- Cache type:
    - `m1024b16` – cache of 1024 items organized in 16-item blocks
    - `m8192b64` – cache of 8192 items organized in 64-item blocks
    - `m65536b256` – cache of 65536 items organized on 256-item blocks
    - `m65536b4096` – cache of 65536 items organized in 4096-item blocks
- The implementation to test (`smart` or `naive`).
* one line per experiment - matrix size and the average number of cache misses. (the diagonal items not counted). 
* you can try `make -j`to run the tests in parallel.

## Optional: Tests on real hardware (for 5 extra points)
* using the `matrix_experiment_real` 
* The matrix is stored in row-major order, each item takes 4 bytes.
The program takes one parameter, the implementation to test: `smart` or `naive`.
*  one line per experiment, matrix size, average time per item in nanoseconds.
* include the configuration of caches in your machine
* try `machinfo`  https://gitlab.kam.mff.cuni.cz/mj/aim.git

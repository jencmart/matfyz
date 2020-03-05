####Star test:
* Specific sequence of operations on which naive implementation
  creates $Θ(\sqrt{n})$ stars of size 1 up to $Θ(\sqrt{n})$.
  
####Random test:
* Test of size $n$ first inserts $2n$ elements and then it does
  $n$ operations of which 
  * one third are insertions
  * one third are decreases
  * one third are extract\_mins in random order.
  
####Biased tests:
* Like random test but the weights of operations are different.
  * First the test inserts $2n$ elements then it does random $n$ operations.
  * Number of operations of each type is proportional to their weight.
    * Weight of insert is 10
    * weight of extract\_min 20 and 
    * weight of decrease == number in the name of test (10, 40, 70 and 100)

* The program tries each experiment with different values of $n$. 
* In each try, it prints the average number of structural changes 
    * (adding and removing a child of a node) per one operation (insert, decrease and extract\_min).


## PLOTS
* write a report ... one plot of data for each test.

- Star test:    One plot with 2 curves – one for standard and the other one for naive
- Random test:  One plot with 2 curves – one for standard and the other one for naive
- Biased tests: One plot with 4 curves – one for each value of bias, all of them tested with standard implementation. 
    * (Note: no biased test for naive)

####Each plot should:
* show the dependence of the avg number of structural changes on the test size $n$.

####The report:
* evaluate your implementation of Fibonacci heap experimentally 
* compare it with a "naive" implementation which does not mark vertices after cutting a child.
* discuss the experimental results
* try to explain the observed behavior using theory from the lectures. 
* (If you want, you can carry out further experiments)


## Hints Plots
A quick checklist for plots:
- Is there a caption explaining what is plotted?
- Are the axes clearly labelled? Do they have value ranges and units?
- Have you mentioned that this axis has logarithmic scale? (Logarithmic graphs
  are more fitting in some cases, but you should tell.)
- Is it clear which curve means what?
- Is it clear what are the measured points and what is an interpolated curve between them?
- Are there any overlaps? (E.g., the most interesting part of the curve hidden underneath a label?)

## Hints discussion
It should be always clear which is which:
- Experimental results == the raw data you obtained from the experiments
- Theoretical facts ==  claims we have proved mathematically
- Your hypotheses == you claim that the graph looks like something is true, but you are not able to prove it

################################
## Test program
three arguments:
- The name of the test (`star`, `random`, `biased-10`, `biased-40`, `biased-70`, `biased-100`).
- The random seed: 18
- The implementation to test (`std` or `naive`).

 1 line per experiment
- set size , average number of structural changes

## Your implementation
Please use your implementation from the previous exercise. If you used C++,
make sure that the `add_child()`,
`remove()` and `max_rank()` methods are declared virtual (they will
be overridden by the test program). 

Also, if you are performing any structural changes
without using the `add_child()` and `remove()` helper methods, you need to adjust
the `BenchmarkingHeap` class accordingly. ! ! !
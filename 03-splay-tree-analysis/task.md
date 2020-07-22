#### Sequential :
* Insert _n_  sequentially
* Find them sequentially
 
####Random test:
* Insert _n_ elements in random order
* find _5n_ random elements
  
#### Subset test:
* Insert _n_ elements, which contains arithmetic progressions interspersed with random elements. (Insert operations not counted!)
* Repeatedly (10 000x) access a small subset of these elements in random order. 
* Try this with subsets of different cardinalities.

print average number of rotations per splay operation. (== rotations / splays)

## PLOTS
(Each plot should show the dependence of the average
number of rotations on the set size _n_.)

- The sequential test: one curve for the standard implementation, one for the naive one.
- The random test: one curve for the standard implementation, one for the naive one.
- The subset test: three curves for the standard implementation with different sizes
  of the subset, three for the naive implementation with the same sizes.

The report discuss the experimental results 
 try to explain the observed behavior using theory from the lectures. 
 
 ( you can carry out further
experiments to gain better understanding of the data structure and include these
in the report. )

## Test program
- The name (`sequential`, `random`, `subset`).
- The random seed: 18 
- The implementation to test (`std` or `naive`).

 1 line per experiment
- For the sequential and random test: the set size and the average number of rotations.

- For the subset test:  subset size,  set size, average num rotations per find. ( initial insertions of the full set are not counted. )

## Hints Plots
checklist for plots:
- Is there a caption explaining what is plotted?
- Are the axes clearly labelled? Do they have value ranges and units?
- Have you mentioned that this axis has logarithmic scale? (Logarithmic graphs
  are more fitting in some cases, but you should tell.)
- Is it clear which curve means what?
- Is it clear what are the measured points and what is an interpolated
  curve between them?
- Are there any overlaps? (E.g., the most interesting part of the curve
  hidden underneath a label?)

## Hints discussion
In your discussion, please distinguish the following kinds of claims.
It should be always clear which is which:
- Experimental results (i.e., the raw data you obtained from the experiments)
- Theoretical facts (i.e., claims we have proved mathematically)
- Your hypotheses (e.g., when you claim that the graph looks like something is true,
  but you are not able to prove rigorously that it always holds)

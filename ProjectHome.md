## Latest version 1.02 : Released on Tuesday, March 02, 2010. ##

![http://help-cspsol.googlegroups.com/web/csp-paperroll.png](http://help-cspsol.googlegroups.com/web/csp-paperroll.png)

### Overview ###
[Cutting Stock Problem](http://neos.mcs.anl.gov/CaseStudies/cutting/index.html) (CSP) consists of finding the best way of cutting a set of large objects into smaller items. CSPs are encountered in a wide variety of industrial applications such as in steel, wood, glass and paper industries.

Imagine a physical object (For example : a steel bar or a paper roll) of fixed width. There are number of such objects (all with same width) to be cut into smaller items. Also there are orders/demands from customers for different number of items of various widths.

Solving CSP involves satisfying all customer orders by cutting minimum number of objects.
Each object is to be cut into a pattern of smaller items/orders. Each pattern is made of one or more orders/items of different widths. Solving CSP can be thought as finding all patterns such that all orders/demand are satisfied and total number of patterns are minimized.

Figure at the top shows a steel rod of width 100 Units is being cut into pattern
(45, 45, 10) which can also be written as:

```
Pattern count 1: 45 x 2, 10 x 1.
```

### Objectives ###
Objectives of this project are :
  * To solve CSP and other similar problems.
  * To demonstrate column generation (CG) or decomposition technique.
  * To use, test and demonstrate use of [GLPK](http://www.gnu.org/software/glpk/) solver API.

### Solution ###

Custom branch & bound (BB) algorithm is written to solve the CSP integer model. Each node of BB tree has associated LP to be solved.

Following are some important steps used to solve the problem :
  * Root node (and if desired other nodes) LP is solved using column generation algorithm.
  * CG involves solving a sub-problem (which in this case is a integer knapsack problem).
  * Sub-problem makes use of dual values of master problem (i.e node lp). Basic idea is to generate column with most negative reduced cost.
  * Best pattern is generated and added to the master problem. The loop is repeated till no new patterns are generated.
  * At this point node LP is said to be solved completely. Node solution is checked.
  * Depending on solution status node is either fathomed or branched into child nodes.
  * Simple branching of a fractional variable is used to create child nodes.
  * BB tree is explored in Depth First Search or Breadth First Search manner.
  * Once BB tree is exhausted/empty, best integer solution is used to print solution report.

![http://help-cspsol.googlegroups.com/web/search-strategy.png](http://help-cspsol.googlegroups.com/web/search-strategy.png)

For more details, download design document from project "Downloads" section.

### Usage ###

Following is sample usage of the program.

```

~/projects/cspsol/src$ make
...

~/projects/cspsol/src$ ./cspsol --data ../data/case1.txt 

Reading order data from file ../data/case1.txt
Total orders read from file = 2
2000 4
1000 4
Add order demand constraints. Total rows, cols = 2, 0
Added initial patterns =  2
Node    1: ....
Patterns: Current =   2, New =   4, Obj Func Value = 2 Branch.
Node    2: 
Patterns: Current =   6, New =   0, Obj Func Value = 2 INTEGER ***
Node    3: 
Patterns: Current =   6, New =   0, LP worse than integer incumbent 2.33333 >= 2. Fathom node. 

Branch and bound tree exhausted.

# Total runtime = 0 Secs

 # Solution Report # 

Best integer obj. func. value = 2
Pattern count =    1:  2000 x  3, 
Pattern count =    1:  2000 x  1,  1000 x  4, 


```

### Bin Packing Problem ###
One dimensional bin packing problem is variant of CSP. Bin packing problem is defined as follows. Given a set of n objects, where size Si of the ith object satisfies 0 < Si <= max\_size. We want to pack all the objects into the minimum number of max\_size bins.

### References ###
  * http://neos.mcs.anl.gov/CaseStudies/cutting/index.html
  * http://mathworld.wolfram.com/Bin-PackingProblem.html



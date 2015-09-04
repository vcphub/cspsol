_Note: Hit Refresh button of your browser to get latest version of this page._

### Introduction ###

Following are results of test runs done for cspsol version 0.9.

### Computer Specification ###

Test were made on computer with following specification.

Desktop Computer Dell Vostro 400, 32 bits

Processor: Intel(R) Core(TM)2 Quad CPU    Q6600  @ 2.40GHz

RAM: 2 GB

### GLPK Version ###

GLPK Version 4.27

### CSP Benchmarks ###

```

vijay@vijay-hq:~/projects/cspsol/data$ cat allcases.txt 
6
case1.txt 2
case2.txt 32
case3.txt 16
case4.txt 18
case5.txt 9
case6.txt 5

vijay@vijay-hq:~/projects/cspsol/data$ cspsol --test allcases.txt 

Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    0 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   16, Runtime =    0 Secs. PASS.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    0 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    6, Runtime =    0 Secs. FAIL.

Finished all test cases in 0 Secs.
------------------------------------------------------------------------------------

vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O2 --test allcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    0 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   16, Runtime =    0 Secs. PASS.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    0 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    6, Runtime =    1 Secs. FAIL.

Finished all test cases in 1 Secs.
------------------------------------------------------------------------------------


vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O3 --test allcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    0 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   16, Runtime =    0 Secs. PASS.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    1 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    5, Runtime =    1 Secs. PASS.

Finished all test cases in 2 Secs.
------------------------------------------------------------------------------------

vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O3 --cgroot --test allcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    0 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   17, Runtime =    0 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    1 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    5, Runtime =    0 Secs. PASS.

Finished all test cases in 1 Secs.

```

## BPP Benchmarks ##

```

vijay@vijay-hq:~/projects/cspsol/data$ cat bpcases.txt 
6
N1C1W1_A.BPP 	25
N1C1W1_B.BPP 	31
N1C1W1_C.BPP 	20
N1C1W1_D.BPP 	28 
N1C1W1_E.BPP 	26 
N1C1W1_F.BPP 	27 

vijay@vijay-hq:~/projects/cspsol/data$ cspsol --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   25, Runtime =   50 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   32, Runtime =   19 Secs. FAIL.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =   81 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   28, Runtime =    1 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   27, Runtime =    8 Secs. FAIL.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   28, Runtime =    5 Secs. FAIL.

Finished all test cases in 164 Secs.
------------------------------------------------------------------------------------

vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O2 --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   25, Runtime =    3 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   31, Runtime =    2 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =   41 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   28, Runtime =    6 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   27, Runtime =    4 Secs. FAIL.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   27, Runtime =    1 Secs. PASS.

Finished all test cases in 57 Secs.
------------------------------------------------------------------------------------

vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O3 --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   25, Runtime =    2 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   31, Runtime =   14 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =   22 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   28, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   27, Runtime =    1 Secs. FAIL.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   27, Runtime =    1 Secs. PASS.

Finished all test cases in 40 Secs.
------------------------------------------------------------------------------------

vijay@vijay-hq:~/projects/cspsol/data$ cspsol -O3 --cgroot --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   27, Runtime =   28 Secs. FAIL.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   34, Runtime =   22 Secs. FAIL.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =   11 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   29, Runtime =    2 Secs. FAIL.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   30, Runtime = 1811 Secs. FAIL.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   28, Runtime =   17 Secs. FAIL.

Finished all test cases in 1892 Secs.
------------------------------------------------------------------------------------

```

Bin Packing Problem test cases were taken from :
http://www.wiwi.uni-jena.de/Entscheidung/binpp/bin1dat.htm

### Summary ###

From above tests, it can be concluded that column generation at every node of BB tree, improves the accuracy of solution produced by cspsol. Depending on the availability of RAM on you machine, you can choose optimization flags -O1 (lowest memory required) to -O3 (highest memory required). Flag -O3 requires highest memory because large number of optimal solutions to subproblem are stored in the memory.

More testing and source code changes are required.
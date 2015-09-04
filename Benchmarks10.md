_Note: Hit Refresh button of your browser to get latest version of this page._

### Introduction ###

Following are results of test runs done for cspsol version 1.0.

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
7
case1.txt 2
case2.txt 32
case3.txt 16
case4.txt 18
case5.txt 9
case6.txt 5
case7.txt 19

vijay@vijay-hq:~/projects/cspsol/data$ ./cspsol --test allcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    0 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   16, Runtime =    1 Secs. PASS.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    0 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    6, Runtime =    0 Secs. FAIL.

Solving testcase no. 7... Done. 
Testcase #7:  Expected =   19, Actual =   22, Runtime =  551 Secs. FAIL.

Finished all test cases in 552 Secs.

```

### BPP Benchmarks ###

```

vijay@vijay-hq:~/projects/cspsol/data$ cat bpcases.txt 
9
bpp/N1C1W1_A.BPP 25
bpp/N1C1W1_B.BPP 31
bpp/N1C1W1_C.BPP 20
bpp/N1C1W1_D.BPP 28 
bpp/N1C1W1_E.BPP 26 
bpp/N1C1W1_F.BPP 27 
bpp/N1C1W1_G.BPP 25 
bpp/N1C1W1_H.BPP 31 
bpp/N1C1W1_I.BPP 25 

vijay@vijay-hq:~/projects/cspsol/data$ ./cspsol --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   25, Runtime =   50 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   32, Runtime =   19 Secs. FAIL.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =   80 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   28, Runtime =    1 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   27, Runtime =    8 Secs. FAIL.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   28, Runtime =    4 Secs. FAIL.

Solving testcase no. 7... Done. 
Testcase #7:  Expected =   25, Actual =   26, Runtime =    8 Secs. FAIL.

Solving testcase no. 8... Done. 
Testcase #8:  Expected =   31, Actual =   32, Runtime =   12 Secs. FAIL.

Solving testcase no. 9... Done. 
Testcase #9:  Expected =   25, Actual =   25, Runtime =    1 Secs. PASS.

Finished all test cases in 183 Secs.

```
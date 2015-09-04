_Note: Hit Refresh button of your browser to get latest version of this page._

### Introduction ###

Following are results of test runs done for cspsol version 1.1.

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

vijay@vijay-hq:~/projects/cspsol/data$ cspsol --test allcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =    2, Actual =    2, Runtime =    1 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   16, Actual =   16, Runtime =    0 Secs. PASS.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   18, Actual =   18, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =    9, Actual =    9, Runtime =    0 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =    5, Actual =    5, Runtime =    0 Secs. PASS.

Solving testcase no. 7... Done. 
Testcase #7:  Expected =   19, Actual =   21, Runtime =  152 Secs. FAIL.

Finished all test cases in 153 Secs.

```

### BPP Benchmarks ###

```

vijay@vijay-hq:~/projects/cspsol/data$ cat bpcases.txt 
24
bpp/N1C1W1_A.BPP 25
bpp/N1C1W1_B.BPP 31
bpp/N1C1W1_C.BPP 20
bpp/N1C1W1_D.BPP 28 
bpp/N1C1W1_E.BPP 26 
bpp/N1C1W1_F.BPP 27 
bpp/N1C1W1_G.BPP 25 
bpp/N1C1W1_H.BPP 31 
bpp/N1C1W1_I.BPP 25 
bpp/N1C1W1_J.BPP 26 
bpp/N1C1W1_K.BPP 26 
bpp/N1C1W1_L.BPP 33
bpp/N1C1W1_M.BPP 30
bpp/N1C1W1_N.BPP 25
bpp/N1C1W1_O.BPP 32
bpp/N1C1W1_P.BPP 26
bpp/N1C1W1_Q.BPP 28
bpp/N1C1W1_R.BPP 25
bpp/N1C1W1_S.BPP 28
bpp/N1C1W1_T.BPP 28
bpp/N1C1W2_A.BPP 29
bpp/N1C1W2_B.BPP 30
bpp/N1C1W2_C.BPP 33
bpp/N1C1W2_D.BPP 31
bpp/N1C1W2_E.BPP 36
bpp/N1C1W2_H.BPP 33
bpp/N1C1W2_I.BPP 35
bpp/N1C1W2_J.BPP 34
bpp/N1C1W4_A.BPP 35
bpp/N1C1W4_B.BPP 40

vijay@vijay-hq:~/projects/cspsol/data$ cspsol --bpp --test bpcases.txt 
Solving testcase no. 1... Done. 
Testcase #1:  Expected =   25, Actual =   25, Runtime =    7 Secs. PASS.

Solving testcase no. 2... Done. 
Testcase #2:  Expected =   31, Actual =   31, Runtime =    0 Secs. PASS.

Solving testcase no. 3... Done. 
Testcase #3:  Expected =   20, Actual =   21, Runtime =    3 Secs. FAIL.

Solving testcase no. 4... Done. 
Testcase #4:  Expected =   28, Actual =   28, Runtime =    0 Secs. PASS.

Solving testcase no. 5... Done. 
Testcase #5:  Expected =   26, Actual =   26, Runtime =    0 Secs. PASS.

Solving testcase no. 6... Done. 
Testcase #6:  Expected =   27, Actual =   27, Runtime =    0 Secs. PASS.

Solving testcase no. 7... Done. 
Testcase #7:  Expected =   25, Actual =   25, Runtime =    0 Secs. PASS.

Solving testcase no. 8... Done. 
Testcase #8:  Expected =   31, Actual =   31, Runtime =    0 Secs. PASS.

Solving testcase no. 9... Done. 
Testcase #9:  Expected =   25, Actual =   25, Runtime =    4 Secs. PASS.

Solving testcase no. 10... Done. 
Testcase #10:  Expected =   26, Actual =   26, Runtime =    0 Secs. PASS.

Solving testcase no. 11... Done. 
Testcase #11:  Expected =   26, Actual =   26, Runtime =    0 Secs. PASS.

Solving testcase no. 12... Done. 
Testcase #12:  Expected =   33, Actual =   33, Runtime =    0 Secs. PASS.

Solving testcase no. 13... Done. 
Testcase #13:  Expected =   30, Actual =   30, Runtime =    0 Secs. PASS.

Solving testcase no. 14... Done. 
Testcase #14:  Expected =   25, Actual =   25, Runtime =    8 Secs. PASS.

Solving testcase no. 15... Done. 
Testcase #15:  Expected =   32, Actual =   32, Runtime =    0 Secs. PASS.

Solving testcase no. 16... Done. 
Testcase #16:  Expected =   26, Actual =   26, Runtime =    0 Secs. PASS.

Solving testcase no. 17... Done. 
Testcase #17:  Expected =   28, Actual =   28, Runtime =    0 Secs. PASS.

Solving testcase no. 18... Done. 
Testcase #18:  Expected =   25, Actual =   25, Runtime =    0 Secs. PASS.

Solving testcase no. 19... Done. 
Testcase #19:  Expected =   28, Actual =   28, Runtime =    0 Secs. PASS.

Solving testcase no. 20... Done. 
Testcase #20:  Expected =   28, Actual =   28, Runtime =    0 Secs. PASS.

Solving testcase no. 21... Done. 
Testcase #21:  Expected =   29, Actual =   29, Runtime =    2 Secs. PASS.

Solving testcase no. 22... Done. 
Testcase #22:  Expected =   30, Actual =   30, Runtime =    0 Secs. PASS.

Solving testcase no. 23... Done. 
Testcase #23:  Expected =   33, Actual =   33, Runtime =    0 Secs. PASS.

Solving testcase no. 24... Done. 
Testcase #24:  Expected =   31, Actual =   31, Runtime =    0 Secs. PASS.

Solving testcase no. 25... Done. 
Testcase #25:  Expected =   36, Actual =   36, Runtime =    0 Secs. PASS.

Solving testcase no. 26... Done. 
Testcase #26:  Expected =   33, Actual =   33, Runtime =    0 Secs. PASS.

Solving testcase no. 27... Done. 
Testcase #27:  Expected =   35, Actual =   35, Runtime =    0 Secs. PASS.

Solving testcase no. 28... Done. 
Testcase #28:  Expected =   34, Actual =   34, Runtime =    1 Secs. PASS.

Solving testcase no. 29... Done. 
Testcase #29:  Expected =   35, Actual =   35, Runtime =    0 Secs. PASS.

Solving testcase no. 30... Done. 
Testcase #30:  Expected =   40, Actual =   40, Runtime =    0 Secs. PASS.

Finished all test cases in 25 Secs.

```
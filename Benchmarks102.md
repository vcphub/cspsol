_Note: Hit Refresh button of your browser to get latest version of this page._

### Introduction ###

Following are results of test runs done for cspsol version 1.02.

### Computer Specification ###

Test were made on computer with following specification.

Desktop Computer Dell Vostro 400, 32 bits

Processor: Intel(R) Core(TM)2 Quad CPU    Q6600  @ 2.40GHz

RAM: 2 GB

### GLPK Version ###
GLPK Version 4.27

### CSP Benchmarks ###

```

vijay@vijay-hq:~/projects/cspsol/data$ cat csp-results.txt 
cspsol, version 1.02
Sr.No, Testcase, Expected, Actual, Heurtics, Runtime (Secs), Status
  1, case1.txt,    2,    2,    2,    0,  PASS.
  2, case2.txt,   32,   32,   36,    0,  PASS.
  3, case3.txt,   16,   16,   17,    0,  PASS.
  4, case4.txt,   18,   18,   22,    0,  PASS.
  5, case5.txt,    9,    9,   10,    0,  PASS.
  6, case6.txt,    5,    5,    5,    0,  PASS.
  7, case7.txt,   19,   21,   22,  167,  FAIL.

Following test cases have FAIL status : 

vijay@vijay-hq:~/projects/cspsol/data$ grep FAIL csp-results.txt 
  7, case7.txt,   19,   21,   22,  167,  FAIL.

```

### BPP Benchmarks ###

```

vijay@vijay-hq:~/projects/cspsol/data$ cat bpp-results.txt 
cspsol, version 1.02
Sr.No, Testcase, Expected, Actual, Heurtics, Runtime (Secs), Status
  1, bpp/N1C1W1_A.BPP,   25,   25,   25,    0,  PASS.
  2, bpp/N1C1W1_B.BPP,   31,   31,   31,    0,  PASS.
  3, bpp/N1C1W1_C.BPP,   20,   21,   21,    0,  FAIL.
  4, bpp/N1C1W1_D.BPP,   28,   28,   28,    0,  PASS.
  5, bpp/N1C1W1_E.BPP,   26,   26,   26,    0,  PASS.
  6, bpp/N1C1W2_A.BPP,   29,   29,   29,    0,  PASS.
  7, bpp/N1C1W2_B.BPP,   30,   30,   30,    0,  PASS.
  8, bpp/N1C1W2_C.BPP,   33,   33,   33,    0,  PASS.
  9, bpp/N1C1W2_D.BPP,   31,   31,   31,    0,  PASS.
 10, bpp/N1C1W2_E.BPP,   36,   36,   36,    0,  PASS.
 11, bpp/N1C1W4_A.BPP,   35,   35,   35,    0,  PASS.
 12, bpp/N1C1W4_B.BPP,   40,   40,   40,    0,  PASS.
 13, bpp/N1C1W4_C.BPP,   36,   36,   36,    0,  PASS.
 14, bpp/N1C1W4_D.BPP,   38,   38,   38,    0,  PASS.
 15, bpp/N1C1W4_E.BPP,   38,   38,   38,    0,  PASS.
 16, bpp/N1C2W1_A.BPP,   21,   21,   21,    0,  PASS.
 17, bpp/N1C2W1_B.BPP,   26,   26,   26,    0,  PASS.
 18, bpp/N1C2W1_C.BPP,   23,   23,   23,    0,  PASS.
 19, bpp/N1C2W1_D.BPP,   21,   21,   21,    0,  PASS.
 20, bpp/N1C2W1_E.BPP,   17,   17,   17,    1,  PASS.
 21, bpp/N1C2W2_A.BPP,   24,   24,   24,    0,  PASS.
 22, bpp/N1C2W2_B.BPP,   27,   27,   27,    0,  PASS.
 23, bpp/N1C2W2_C.BPP,   29,   29,   29,    0,  PASS.
 24, bpp/N1C2W2_D.BPP,   24,   24,   24,    0,  PASS.
 25, bpp/N1C2W2_E.BPP,   33,   33,   33,    0,  PASS.
 26, bpp/N1C2W4_A.BPP,   29,   29,   29,    0,  PASS.
 27, bpp/N1C2W4_B.BPP,   32,   32,   32,    0,  PASS.
 28, bpp/N1C2W4_C.BPP,   30,   30,   30,    0,  PASS.
 29, bpp/N1C2W4_D.BPP,   28,   28,   28,    0,  PASS.
 30, bpp/N1C2W4_E.BPP,   30,   30,   30,    0,  PASS.
 31, bpp/N1C3W1_A.BPP,   16,   16,   17,    4,  PASS.
 32, bpp/N1C3W1_B.BPP,   16,   16,   16,    0,  PASS.
 33, bpp/N1C3W1_C.BPP,   17,   17,   18,   63,  PASS.
 34, bpp/N1C3W1_D.BPP,   19,   19,   19,    0,  PASS.
 35, bpp/N1C3W1_E.BPP,   16,   16,   16,    0,  PASS.
 36, bpp/N1C3W2_A.BPP,   19,   19,   19,    0,  PASS.
 37, bpp/N1C3W2_B.BPP,   20,   20,   21,    2,  PASS.
 38, bpp/N1C3W2_C.BPP,   22,   22,   22,    0,  PASS.
 39, bpp/N1C3W2_D.BPP,   20,   20,   20,    0,  PASS.
 40, bpp/N1C3W2_E.BPP,   21,   21,   21,    0,  PASS.
 41, bpp/N1C3W4_A.BPP,   21,   21,   22,    4,  PASS.
 42, bpp/N1C3W4_B.BPP,   22,   22,   23,   10,  PASS.
 43, bpp/N1C3W4_C.BPP,   24,   24,   24,    0,  PASS.
 44, bpp/N1C3W4_D.BPP,   21,   22,   22,    4,  FAIL.
 45, bpp/N1C3W4_E.BPP,   23,   23,   23,    0,  PASS.
 46, bpp/N2C1W1_A.BPP,   48,   48,   48,    0,  PASS.
 47, bpp/N2C1W1_B.BPP,   49,   49,   49,    1,  PASS.
 48, bpp/N2C1W1_C.BPP,   46,   46,   46,    0,  PASS.
 49, bpp/N2C1W1_D.BPP,   50,   50,   50,    0,  PASS.
 50, bpp/N2C1W1_E.BPP,   58,   58,   58,    1,  PASS.
 51, bpp/N2C1W2_A.BPP,   64,   64,   64,    0,  PASS.
 52, bpp/N2C1W2_B.BPP,   61,   61,   61,    0,  PASS.
 53, bpp/N2C1W2_C.BPP,   68,   68,   68,    0,  PASS.
 54, bpp/N2C1W2_D.BPP,   74,   74,   74,    0,  PASS.
 55, bpp/N2C1W2_E.BPP,   65,   65,   65,    0,  PASS.
 56, bpp/N2C1W4_A.BPP,   73,   73,   73,    0,  PASS.
 57, bpp/N2C1W4_B.BPP,   71,   71,   71,    0,  PASS.
 58, bpp/N2C1W4_C.BPP,   77,   77,   77,    1,  PASS.
 59, bpp/N2C1W4_D.BPP,   82,   82,   82,    0,  PASS.
 60, bpp/N2C1W4_E.BPP,   73,   73,   73,    0,  PASS.
 61, bpp/N2C2W1_A.BPP,   42,   42,   42,    0,  PASS.
 62, bpp/N2C2W1_B.BPP,   50,   50,   50,    0,  PASS.
 63, bpp/N2C2W1_C.BPP,   40,   40,   40,    1,  PASS.
 64, bpp/N2C2W1_D.BPP,   42,   42,   42,    1,  PASS.
 65, bpp/N2C2W1_E.BPP,   40,   40,   40,    0,  PASS.
 66, bpp/N2C2W2_A.BPP,   52,   52,   52,    1,  PASS.
 67, bpp/N2C2W2_B.BPP,   56,   56,   56,    0,  PASS.
 68, bpp/N2C2W2_C.BPP,   53,   53,   53,    0,  PASS.
 69, bpp/N2C2W2_D.BPP,   51,   51,   51,    0,  PASS.
 70, bpp/N2C2W2_E.BPP,   54,   54,   54,    1,  PASS.
 71, bpp/N2C2W4_A.BPP,   57,   57,   57,    0,  PASS.
 72, bpp/N2C2W4_B.BPP,   60,   60,   60,    0,  PASS.
 73, bpp/N2C2W4_C.BPP,   65,   65,   65,    0,  PASS.
 74, bpp/N2C2W4_D.BPP,   61,   61,   61,    0,  PASS.
 75, bpp/N2C2W4_E.BPP,   60,   60,   60,    0,  PASS.
 76, bpp/N2C3W1_A.BPP,   35,   35,   35,    1,  PASS.
 77, bpp/N2C3W1_B.BPP,   35,   35,   35,    1,  PASS.
 78, bpp/N2C3W1_C.BPP,   35,   35,   35,    0,  PASS.
 79, bpp/N2C3W1_D.BPP,   37,   37,   37,    1,  PASS.
 80, bpp/N2C3W1_E.BPP,   34,   34,   34,    1,  PASS.
 81, bpp/N2C3W2_A.BPP,   41,   41,   42,  352,  PASS.
 82, bpp/N2C3W2_B.BPP,   43,   43,   43,    1,  PASS.

Following test cases have FAIL status : 

vijay@vijay-hq:~/projects/cspsol/data$ grep FAIL bpp-results.txt 
  3, bpp/N1C1W1_C.BPP,   20,   21,   21,    0,  FAIL.
 44, bpp/N1C3W4_D.BPP,   21,   22,   22,    4,  FAIL.


Next BPP test case in the list 'bpp/N2C3W2_B.BPP' takes too much time. I have not chance to complete all test cases.

```

### Summary ###

This release provides a systematic way of running the test cases and comparing results. Test case results are stored in file 'csp-results.txt' and 'bpp-results.txt' for CSP and BPP respectively.

For each test cases, the optimal solution obtained using branch & price method is compared with optimal solution obtained using First Fit Decreased (FFD) heuristic. We expect that branch & price method produces better solution.
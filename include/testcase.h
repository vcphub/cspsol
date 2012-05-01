#ifndef TESTCASE_H
#define TESTCASE_H

#include<ctime>

/* Result of a single problem or a test case. */
struct TestCaseSol 
{
	double obj_val; 
	/* Optimal integer solution. */
	time_t runtime; 
	/* Time taken to solve the case, in secs. */
};

void run_testcases();

#endif

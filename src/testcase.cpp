
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<cassert>
#include<cmath>
#include "testcase.h"
#include "extern.h"

using namespace std;

/* Global object to print BB progress info. */
ofstream fout("bb_log.txt");
extern TestCaseSol * solve_csp();
extern int heur_obj_val;

/*-------------------------------------------------------------------
 * Relevant options: cspsol --test filename.
 * The first line of the file should contain number of test cases.
 * Rest of the lines should follow FORMAT:
 * N
 * order-data-file-name1 expected_opt_value1
 * order-data-file-name2 expected_opt_value2
 * order-data-file-nameN expected_opt_valueN
 *
 * Run all testcases specified in the input file. Print test case
 * status PASS or FAIL.
-------------------------------------------------------------------*/
void run_testcases()
{
  ifstream ftc;  // READ from test cases specs file.
  ofstream fres; // WRITE to test run results file.
	assert(option->tc_file != NULL);

	ftc.open(option->tc_file);
  if(option->bpp == false)
		fres.open("csp-results.txt");
  else
	  fres.open("bpp-results.txt");

	time_t start_time, end_time;
	time(&start_time);

	/* Read test cases from specification file. */
  /* Read test case file name and optimal value. */
	double exp_opt_val;
	char filename[64];
	string buffer;
  int tc_index = 1;

  getline(ftc, buffer);
  istringstream ss(buffer);
	ss >> filename >> exp_opt_val;
        
  /* Print 'cspsol' version no. and header. */
  fres<<"cspsol, version "<< VERSION_NUMBER <<endl;
	fres<<"Sr.No, Testcase, Expected, Actual, Heurtics, Runtime (Secs), Status"<<endl;
	while(!ftc.eof()) {
  	if(buffer[0] == '#') {
    	getline(ftc, buffer);
    	continue;
    }
    istringstream ss(buffer);
	  ss >> filename >> exp_opt_val;

		option->data_file = filename;

		cout<<"Solving testcase no. "<< tc_index <<", '"<< option->data_file << "' ...";
		cout.flush();

		option->silent = true;
    // TODO : silent_cout
		option->redirect_cout();

		/* Solve this test case. */
		TestCaseSol * result = solve_csp();

		option->restore_cout();
		cout<<" Done. "<<endl;

		fres<< setw(3) << tc_index << ", ";
		fres<< setw(3) << option->data_file << ", ";
		fres<< setw(4) << exp_opt_val << ", ";
		fres<< setw(4) << result->obj_val << ", ";
		fres<< setw(4) << heur_obj_val << ", ";
		fres<< setw(4) << result->runtime << ", ";

		/* Compare obj. func. value with expected value. */
		if(fabs(result->obj_val - exp_opt_val) < 1e-7)
			fres<<" PASS."<<endl;
		else				
			fres<<" FAIL."<<endl;

		delete(result);
    // process next test case file.
    getline(ftc, buffer);
    tc_index++;
	}

	time(&end_time);
  ftc.close();
  fres.close();
	cout<<"Finished all test cases in "<<(end_time-start_time)<<" Secs."<<endl<<endl;
}


/* Program Description: Minimize total number of rolls/patterns used to 
 * satisfy order requirement. */

/* File: main.cpp
 * Description: Read order data from specified input file. 
 * Create master cutting stock problem.
 */

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<cassert>
#include<stack>
#include<queue>
#include<cmath>
#include<ctime>

#include "glpk.h"
#include "order_width.h"
#include "pattern.h"
#include "extern.h"
#include "model.h"
#include "bb_node.h"
#include "cmdline.h"

using namespace std;

/* Result of a test case. */
struct TestCaseSol 
{
	double obj_val; /* Optimal integer solution. */
	time_t runtime; /* Time taken in secs. */
};

/* Global object to print debug information into a log file. */
ofstream fout("log.txt");
CmdOption * option = NULL;
TestCaseSol * solve_csp();
void run_testcases();

/* Program entry point */
int main(int argc, char * argv[])
{		
	/* Command line arguments. */
	option = process_arguments(argc, argv);

	if(option->test == false)
		solve_csp();

	/* Run test cases from specified file. */
	else
		run_testcases();

	delete(option);
	return 0;
}


/*-------------------------------------------------------------------
 * Solve one instance of CSP. Get order data file name from object
 * 'option'.
 * Return value: Optimal integer solution's obj. value. 
-------------------------------------------------------------------*/
TestCaseSol * solve_csp()
{
	/* Container for storing all order objects. */
	OrderWidthContainer ow_set;	
	/* Container object represents branch and bound tree. */
	BBNodeContainer bbnode_set;	

	time_t start_time, end_time;
	time(&start_time);

	if(option->bpp == true)
		OrderWidth::read_item_data(ow_set, option->data_file);
	else
		OrderWidth::read_order_data(ow_set, option->data_file);

	OrderWidth::print_order_list(ow_set);

	/* Create master lp using GLPK API. */
	glp_prob * master_lp = NULL;
	glp_term_out(GLP_OFF);
	master_lp = glp_create_prob();
	glp_set_prob_name(master_lp, "MasterCutStock");
	/* Minimize total number of rolls/pattern used. */
	glp_set_obj_dir(master_lp, GLP_MIN);

	add_demand_constraints(master_lp, ow_set);

	/* Create root BB node. */
	BBNode::set_best_int_obj_val(Infinity);
	BBNode * root_node = new BBNode(master_lp, (long int)1);
	bbnode_set.push_back(root_node);
	add_init_patterns(root_node, master_lp, ow_set);

	/* While Loop: Branch and bound algorithm. */
	int solved_node_cnt = 1;
	while(bbnode_set.empty() == false) {

		/* Select next node from the tree. */
		BBNode * node = NULL;
		if(option->search == BFS) {
                        node = bbnode_set.front();
			bbnode_set.pop_front();
		} else if(option->search == DFS) {
                        node = bbnode_set.back();
			bbnode_set.pop_back();
		}

		cout<<"Node "<<setw(4)<<(solved_node_cnt)<<": ";
		/* Solve node LP using column generation. */
		node->solve(ow_set, bbnode_set);
		solved_node_cnt++;

		//cout<<": new patterns = "<<setw(4)<<node->get_pat_cnt()<<" ";
		
		if(node->get_lp_status() == REAL_INFEA)
			cout << "Infeasible LP. Fathom node. "<<endl;

		else if(node->get_lp_status() == OPT_NONINT) {

			if(node->get_opt_obj_val() >= BBNode::get_best_int_obj_val()-1.0) {
				cout << "LP worse than integer incumbent "<< node->get_opt_obj_val();
				cout <<" >= " << BBNode::get_best_int_obj_val();
				cout << ". Fathom node. "<<endl;

			} else {
				cout << "Obj Func Value = "<< node->get_opt_obj_val() << " Branch."<<endl;
				/* Branch on fractional pattern variable. Create child nodes. */
				node->branch(bbnode_set);
			}

		} else if(node->get_lp_status() == OPT_INT) 
			cout << "Obj Func Value = "<< node->get_opt_obj_val() <<" INTEGER ***"<<endl;

		node->remove_patterns();
                // ilpanda : This node in BB tree is no more needed.
                delete(node);
	} 

	/* Print solution report. */
	cout << endl << "Branch and bound tree exhausted." << endl;
	time(&end_time);
	cout << endl << "# Total runtime = "<< (end_time - start_time) << " Secs"<< endl;

	BestNode->print_text_report(cout, master_lp, ow_set);
	BestNode->print_solution(master_lp, ow_set);

	/* Store result and other info. into a object. */
	TestCaseSol * result = new TestCaseSol();
	result->obj_val = BBNode::get_best_int_obj_val();
	result->runtime = end_time-start_time;

	/* Memory cleanup and exit. */
	glp_delete_prob(master_lp);
	OrderWidth::clean_up(ow_set);
	Pattern::clean_up();
	BBNode::clean_up(bbnode_set);

	return result;
}

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
	ifstream ftc;
	int tc_count;

	assert(option->tc_file != NULL);
	ftc.open(option->tc_file);				
	ftc >> tc_count; /* test case count */

	time_t start_time, end_time;
	time(&start_time);

	/* For all test cases. */
	for(int tc = 1; tc <= tc_count; tc++) {
		double exp_opt_val;
		char filename[64];

		cout<<"Solving testcase no. "<< tc <<"...";
		cout.flush();
		option->silent = true;
		option->redirect_cout();

		ftc >> filename >> exp_opt_val;
		option->data_file = filename;

		/* Solve this test case. */
		TestCaseSol * result = solve_csp();

		option->restore_cout();
		cout<<" Done. "<<endl;

		cout<<"Testcase #"<< tc << ": ";
		cout<<" Expected = "<< setw(4) << exp_opt_val;
		cout<<", Actual = "<< setw(4) << result->obj_val;
		cout<<", Runtime = " << setw(4) << result->runtime << " Secs.";

		/* Compare obj. func. value with expected value. */
		if(fabs(result->obj_val - exp_opt_val) < 1e-7)
			cout<<" PASS."<<endl<<endl;
		else				
			cout<<" FAIL."<<endl<<endl;

		delete(result);
	}
	time(&end_time);
	cout<<"Finished all test cases in "<<(end_time-start_time)<<" Secs."<<endl<<endl;
}


/* Program Description: Minimize total number of rolls/patterns used to 
 * satisfy order requirement. */

/* File: main.cpp
 * Description: Read order data from specified input file. 
 * Create master cutting stock problem.
 */

#include<iostream>
#include<fstream>
#include<sstream>
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
#include "testcase.h"

using namespace std;

CmdOption * option = NULL;
TestCaseSol * solve_csp();

/* Program entry point */
int main(int argc, char * argv[])
{		
	/* Command line arguments. */
	option = new CmdOption();
	option->process_arguments(argc, argv);

	if(option->test == false)
		solve_csp();
	else
		run_testcases();  // Run test cases from specified file.

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

	time_t start_time, end_time, curr_time;
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
  /* Create and store initial pattern in the root node. */
	root_node->add_init_patterns(ow_set);

	/* While Loop: Branch and bound algorithm. */
	int solved_node_cnt = 1;
  bool tm_lim_flag = false;

	while(bbnode_set.empty() == false) {

  	/* Check for time limit. */
    time(&curr_time); 
    if(option->tm_lim && (curr_time - start_time) >= option->tm_lim) {
    	tm_lim_flag = true;
      break;
    }
                                 
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
		node->solve(ow_set);
		solved_node_cnt++;
		
		cout << " | Obj. value = " << setw(8) << node->get_opt_obj_val() << " | ";

		if(node->get_lp_status() == REAL_INFEA)
			cout << "Infeasible LP. Fathom node. "<<endl;
		else if(node->get_lp_status() == OPT_NONINT) {

			if(node->get_opt_obj_val() >= BBNode::get_best_int_obj_val()-1.0+EPSILON) {
				// LP worse than integer incumbent 
				cout << "Fathom node (w.r.t. " << BBNode::get_best_int_obj_val()-1.0 << ")" << endl;

			} else {
				cout << "Branch." << endl;
				/* Branch on fractional pattern variable. Create child nodes. */
				node->branch(bbnode_set);
			}

		} else if(node->get_lp_status() == OPT_INT) {
			//cout << "Obj Func Value = "<< node->get_opt_obj_val() <<" INTEGER ***"<<endl;
			cout << "INTEGER ***" << endl;
		}

		node->remove_patterns();
    // This node in BB tree is no more needed.
    delete(node);
	} 

	/* Print solution report. */
  if(tm_lim_flag == true)
		cout << endl << "Specified time limit exceeded." << endl;
  else
	  cout << endl << "Branch and bound tree exhausted." << endl;

	time(&end_time);
	cout << endl << "# Total runtime = "<< (end_time - start_time) << " Secs"<< endl;

  if(BestNode != NULL) {
		BestNode->print_text_report(cout, master_lp, ow_set);
	  BestNode->print_solution(master_lp, ow_set);
  } else {
		cout << "No integer solution found." << endl;
  }

	/* Store result and other info. into a object. */
  TestCaseSol * result = NULL;
	if(option->test == true) {
		result = new TestCaseSol();
	  result->obj_val = BBNode::get_best_int_obj_val();
	  result->runtime = end_time-start_time;
	}

	/* Memory cleanup and exit. */
	glp_delete_prob(master_lp);
	OrderWidth::clean_up(ow_set);
	Pattern::clean_up();
	BBNode::clean_up(bbnode_set);

	return result;
}



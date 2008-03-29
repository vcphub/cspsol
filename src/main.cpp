/* Program Description: Minimize total number of rolls/patterns used to 
 * satisfy order requirement. */

/* File: main.cpp
 * Description: Read order data from specified input file. 
 * Create master cutting stock problem.
 */

#include<iostream>
#include<string>
#include<vector>
#include<cassert>
#include<cstdio>
#include<stack>
#include<queue>

#include<fstream>
#include<iomanip>
#include<cstdlib>
#include "glpk.h"
#include "order_width.h"
#include "pattern.h"
#include "extern.h"
#include "model.h"
#include "bb_node.h"

using namespace std;

/* Global object to print debug information into a log file. */
ofstream fout("log.txt");

/* Program entry point */
int main(int argc, char * argv[])
{		
	/* Container for storing all order objects. */
	OrderWidthContainer ow_set;	
	/* Container object represents branch and bound tree. */
	BBNodeContainer bbnode_set;	
	time_t start_time, end_time;

	time(&start_time);

	if(argc != 2) {
		cout << "Usage: " << argv[0] << " filename" << endl << endl;
		cout << "Where filename contains orders data in following format." << endl;
		cout << "maximum_pattern_width" << endl;
		cout << "order_width_1 demand_1" << endl;
		cout << "order_width_2 demand_2" << endl;
		cout << "order_width_n demand_n" << endl << endl;
		cout << "All demand quantities are <= maximum_pattern_width." << endl;
	       	cout << endl;
		exit(-1);
	}
	/* Parse the argument */
	OrderWidth::read_order_data(ow_set, argv[1]);
	OrderWidth::print_order_list(ow_set);

	/* Create master lp using GLPK API. */
	glp_prob * master_lp = NULL;
	glp_term_out(GLP_OFF);
	master_lp = glp_create_prob();
	glp_set_prob_name(master_lp, "MasterCutStock");
	/* Minimize total number of rolls/pattern used. */
	glp_set_obj_dir(master_lp, GLP_MIN);

	add_demand_constraints(master_lp, ow_set);
	add_slack_variables(master_lp, ow_set);

	/* Create root BB node. */
	BBNode * node = new BBNode(master_lp, (long int)1);
	bbnode_set.push(node);

	/* While Loop: Branch and bound algorithm. Breadth First Search (BFS). */
	int solved_node_cnt = 0;
	while(bbnode_set.empty() == false) {

		/* Select next node from the tree. */
		BBNode * node = bbnode_set.front();
		bbnode_set.pop();

		/* Solve node LP using column generation. */
		node->solve(ow_set, bbnode_set);
		solved_node_cnt++;

		cout<<"Node "<<setw(5)<<(solved_node_cnt);
		cout<<": new patterns = "<<node->get_pat_cnt()<<" ";

		
		if(node->get_lp_status() == REAL_INFEA || 
				node->get_lp_status() == LOGIC_INFEA)
			cout << "Infeasible LP. Fathom node. "<<endl;

		else if(node->get_lp_status() == OPT_NONINT) {

			if(node->get_opt_obj_val() >= BBNode::get_best_int_obj_val()) {
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
	} 

	/* Print solution report. */
	cout << endl << "Branch and bound tree exhausted." << endl;
	Pattern::print_solution(fout, master_lp, ow_set);
	Pattern::print_solution(cout, master_lp, ow_set);

	/* Memory cleanup and exit. */
	glp_delete_prob(master_lp);
	OrderWidth::clean_up(ow_set);
	Pattern::clean_up();
	time(&end_time);
	cout << endl << "# Total runtime = "<< (end_time - start_time) << " Secs"<< endl;

	return 0;
}

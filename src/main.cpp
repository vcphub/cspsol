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
/* Global functions. */
void print_usage();
void process_arguments(int argc, char * argv[], char **file, SearchStrategy& search);

#define parse_cla(str) (strcmp(argv[i], str) == 0)

/* Program entry point */
int main(int argc, char * argv[])
{		
	/* Container for storing all order objects. */
	OrderWidthContainer ow_set;	
	/* Container object represents branch and bound tree. */
	BBNodeContainer bbnode_set;	
	time_t start_time, end_time;
	SearchStrategy search = DFS;
	char * order_data_file = NULL;

	time(&start_time);
	/* Command line arguments. */
	process_arguments(argc, argv, &order_data_file, search);

	OrderWidth::read_order_data(ow_set, order_data_file);
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
	bbnode_set.push_back(node);

	/* While Loop: Branch and bound algorithm. Breadth First Search (BFS). */
	int solved_node_cnt = 0;
	while(bbnode_set.empty() == false) {

		/* Select next node from the tree. */
		BBNode * node;
		if(search == DFS) {
	       		node = bbnode_set.front();
			bbnode_set.pop_front();
		} else if(search == BFS) {
	       		node = bbnode_set.back();
			bbnode_set.pop_back();
		}

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


/* Print program usage. */
void print_usage() 
{
	cout << endl;
	cout << "Usage: cspsol [options...]" << " --data filename" << endl << endl;
	cout << "Where filename contains orders data in following format." << endl;
	cout << "maximum_pattern_width" << endl;
	cout << "order_width_1 demand_1" << endl;
	cout << "order_width_2 demand_2" << endl;
	cout << "order_width_n demand_n" << endl << endl;
	cout << "All demand quantities are <= maximum_pattern_width." << endl;
	cout << endl;
	cout << "Options:"<<endl;
	cout << "--dfs		Process branch and bound tree in depth first manner."<<endl;
	cout << "--bfs		Process branch and bound tree in breadth first manner."<<endl;
	cout << "-h, --help 	Display this help information and exit."<<endl;
	cout << endl;

	exit(-1);
}

/* Process command line options. */
void process_arguments(int argc, char * argv[], char **file, SearchStrategy& search)
{

	/* Parse the argument */
	if(argc == 1)
		print_usage();
	int i;
	for(i = 1; i < argc; i++) {
		if(parse_cla("--help") || parse_cla("-h"))
			print_usage();
		if(parse_cla("--dfs"))
			search = DFS;
		else if(parse_cla("--bfs"))
			search = BFS;
		else if (parse_cla("-d") || parse_cla("--data"))
		{  
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				printf("cspsol rrror: Orders data file NOT specifed");
				print_usage();
			}    
			*(file) = argv[i];
		} 
	}

	if((i == argc)  && (*file == NULL)) {
		printf("cspsol rrror: Orders data file NOT specifed");
		print_usage();
	}
}


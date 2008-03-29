#ifndef BB_NODE_H
#define BB_NODE_H

#include<string>
#include<vector>
#include<queue>
#include "order_width.h"
#include "glpk.h"

enum NodeStatus {NOT_SOLVED, OPT_NONINT, OPT_INT, REAL_INFEA, LOGIC_INFEA};

class BBNode;	/* Forward declaration */

/* Queue is used to implement Breadth First Search */
typedef std::queue<BBNode*> BBNodeContainer;
typedef std::vector<int> IntContainer;
typedef std::vector<double> DblContainer;

/* class: Branch and Bound Tree Node */
class BBNode
{
	long int node_id;
	/* Pointer to master problem object. */
	glp_prob * master_lp;
	NodeStatus lp_status;	/* Status of master_lp after solving it. */

	/* Optimal objective function value of the lp. */
	double opt_obj_val;	
	static double best_int_obj_val;

	IntContainer fix_col_ind_list;
	DblContainer lb_list;
	DblContainer ub_list;
	/* Number of new patterns added while solving this node. */
	int pat_cnt;		

	//std::vector<FixType> fix_type_list;
	//std::vector<FixVar*> fix_var_list;	

public:

	BBNode(glp_prob * lp, long int node_id);
	BBNode(long int node_id, BBNode * parent_node);
	~BBNode(void);

	int get_pat_cnt() { return pat_cnt; } 
	long int get_node_id() { return node_id; }
	NodeStatus get_lp_status() { return lp_status; }
	double get_opt_obj_val() { return opt_obj_val; }
	static double get_best_int_obj_val() { return best_int_obj_val; }
	static void set_best_int_obj_val(double val) { best_int_obj_val = val; }

	void add_var_fix(int col_ind, double lb, double ub);
	int find_var_fix(int col_ind);
	void solve(OrderWidthContainer& ow_set, BBNodeContainer& bbnode_set);
	bool int_sol();
	void branch(BBNodeContainer& bbnode_set);


	void unfix_all_vars();
	void fix_vars();

	void print_lp_file(int iter_count);
};


#endif

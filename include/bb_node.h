#ifndef BB_NODE_H
#define BB_NODE_H

#include<string>
#include<vector>
#include<deque>
#include "order_width.h"
#include "pattern.h"
#include "glpk.h"

enum NodeStatus {NOT_SOLVED, OPT_NONINT, OPT_INT, REAL_INFEA};

class VariableFix {
	public:
		int col_ind; 
		/* column index of variable to be fixed. */
		double lb; 
		/* Lower bound on the variable. */
		double ub; 
		/* Upper bound on the variable. */
};

class BBNode;	/* Forward declaration */

/* Queue is used to implement Breadth First Search */
typedef std::deque<BBNode*> BBNodeContainer;
typedef std::deque<BBNode*>::iterator BBNodeIterator;
typedef std::vector<int> IntContainer;
typedef std::vector<double> DblContainer;

/* class: Branch and Bound Tree Node */
class BBNode
{
	public:
		PatternContainer pattern_list;
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
		void solve(OrderWidthContainer& ow_set);
		bool int_sol();
		static BBNode * get_next_node(BBNodeContainer & bbnode_set);
		void branch(BBNodeContainer& bbnode_set);

		Pattern * get_duplicate(Pattern * pattern);
		bool check_duplicate(Pattern * pattern);

		void add_init_patterns(OrderWidthContainer& ow_set);
		void add_exist_patterns();
		void remove_patterns();
		void unfix_all_vars();
		void fix_vars();

		void store_solution(glp_prob * master_lp);
		void print_solution(glp_prob* master_lp, OrderWidthContainer& ow_set);
		void print_xml_report(std::ostream&, glp_prob * master_lp, 
										OrderWidthContainer& ow_set); 
		void print_json_report(std::ostream&, glp_prob * master_lp, 
										OrderWidthContainer& ow_set); 
		void print_text_report(std::ostream&, glp_prob * master_lp, 
										OrderWidthContainer& ow_set);

		void print_lp_file(int iter_count);
		static void clean_up(BBNodeContainer& bbset);

	private:
		long int node_id;
		glp_prob * master_lp;
		/* Pointer to master problem object. */
		NodeStatus lp_status;	
		/* Status of master_lp after solving it. */

		double opt_obj_val;	
		/* Optimal objective function value of the lp. */
		static double best_int_obj_val;

		IntContainer fix_col_ind_list;
		DblContainer lb_list;
		DblContainer ub_list;
		int pat_cnt;		
		/* Number of new patterns added while solving this node. */

};

#endif

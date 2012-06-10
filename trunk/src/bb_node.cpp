#include<vector>
#include<string>
#include<algorithm>
#include<sstream>
#include<cmath>
#include<cassert>
#include<iomanip>

#include "glpk.h"
#include "bb_node.h"
#include "pattern.h"
#include "extern.h"
#include "model.h"

using namespace std;

const double Infinity = 100000.0;
double BBNode::best_int_obj_val = Infinity;
extern bool workaround_flag;
BBNode * BestNode = NULL;
int heur_obj_val = 0;

/*------------------------------------------------------------------------
Construct a BBNode object from lp pointer object.
------------------------------------------------------------------------*/
BBNode::BBNode(glp_prob * lp, long int node_id)
{
	assert(lp != NULL);
	this->master_lp = lp;
	this->node_id = node_id;
	this->lp_status = NOT_SOLVED;
}


/*------------------------------------------------------------------------
 * Create child node from given parent node.
------------------------------------------------------------------------*/
BBNode::BBNode(long int node_id, BBNode * parent_node)
{
	this->node_id = node_id;
	this->lp_status = NOT_SOLVED;
	/* Deep copy data from parent node */
	this->master_lp = parent_node->master_lp;
	this->pattern_list = parent_node->pattern_list;

	this->fix_col_ind_list = parent_node->fix_col_ind_list;
	this->lb_list = parent_node->lb_list;
	this->ub_list = parent_node->ub_list;
}

/*------------------------------------------------------------------------
 * Create initial patterns from given order widths.
 * Use First Fit Decreasing (FFD) logic.
 * Add these patterns to the master problem.
------------------------------------------------------------------------*/
void BBNode::add_init_patterns(OrderWidthContainer& ow_set)
{
	BBNode * temp_node = new BBNode(0, this);

	// Sort orders with decreasing width.
  sort(ow_set.begin(), ow_set.end(), CmpOrderWidth());

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {
  	OrderWidth * ow = *(ow_iter);

    // Explore existing patterns.
    int demand = ow->get_demand();
	  PatternIterator pat_iter = AllPatternList.begin();
	  for(; pat_iter != AllPatternList.end(); pat_iter++) {

    	Pattern * pat = *(pat_iter);

      if(demand == 0) break;
      pat->assign_order_width(ow, demand); 
      // else use next pattern;
    }

    // All existing patterns exhausted.
    // Loop for generating new patterns till demand is satisfied.
    while(demand != 0) {

    	Pattern * pattern = new Pattern();
      pattern->assign_order_width(ow, demand); 

      Pattern * dupe_pattern = this->get_duplicate(pattern);
			if(dupe_pattern) {
      	/*  Found same pattern. */
        delete pattern;
        pattern = dupe_pattern;
      } else {
      	/* Add new pattern to both lists. */
		    this->pattern_list.push_back(pattern);
		    temp_node->pattern_list.push_back(pattern);
		    AllPatternList.push_back(pattern);
      }
      /* Increment count. */
      double x = pattern->get_int_sol();
      pattern->set_int_sol(x + 1.0);

      if(demand == 0) break;
      // else use new pattern.
		}
	} // next order width.

  heur_obj_val = 0;
	PatternIterator pat_iter = AllPatternList.begin();
	for(; pat_iter != AllPatternList.end(); pat_iter++) {
  	heur_obj_val += (*pat_iter)->get_int_sol();
  }
  BBNode::set_best_int_obj_val(heur_obj_val);
  BestNode = temp_node;
}

/*------------------------------------------------------------------------
Add exiting columns to the master problem.
------------------------------------------------------------------------*/
void BBNode::add_exist_patterns()
{
	PatternIterator iter = pattern_list.begin();
	for(; iter != pattern_list.end(); iter++)
		add_pattern(master_lp, *iter);
}

/*------------------------------------------------------------------------
Remove all patterns in the pattern_list from master problem.
------------------------------------------------------------------------*/
void BBNode::remove_patterns()
{
	int * num = new int[pattern_list.size() + 1];

	PatternIterator iter = pattern_list.begin();
	for(int i = 1; iter != pattern_list.end(); iter++, i++)
		num[i] = (*iter)->get_master_col_num();

	glp_del_cols(master_lp, pattern_list.size(), num);

	delete [] num;
}
	

/*------------------------------------------------------------------------
Solve node LP associated with this BBNode object.
LP is solved using column/pattern generation.
------------------------------------------------------------------------*/
void BBNode::solve(OrderWidthContainer& ow_set)
{
	int curr_pat_cnt = pattern_list.size();

	if(option->test == false)
		fout<<endl<< "["<<(this->node_id)<<"]"<<" Solving BB Node"<<endl<<endl;

	/* Add columns to master problem for all patterns. */
	this->add_exist_patterns();

	/* Unfix all variables. */
	this->unfix_all_vars();
	/* Fix variables using node's variable fix data. */
	this->fix_vars();

	/* Column generation Loop, while new pattern is found. */
	int iter_count = 1, new_pat_cnt = 0;
	while (true) {

		/* Solve master problem associated with this BB node. */
		int lp_status;
		lpx_std_basis(master_lp);
	  lp_status = glp_simplex(master_lp, NULL);
		assert(lp_status == 0);

		if(glp_get_status(master_lp) != GLP_OPT) break;

	  if(option->test == false)
			fout << "Obj Func Value = " << glp_get_obj_val(master_lp) << endl;
		if((this->node_id != 1) && (option->cg_root_only)) break;

		/* Store dual values in OrderWidth objects. */
		store_dual_values(master_lp, ow_set);

		/* Generate best pattern by solving subproblem. */
		Pattern * pattern;
	  pattern = Pattern::get_new_pattern(this, ow_set, iter_count);

		/* Could be NULL, because of new pattern could not be generated. */
		if(pattern == NULL) break;

		this->pattern_list.push_back(pattern); // new code
		/* Add to global container AllPatternList. */
		AllPatternList.push_back(pattern);

		add_pattern(master_lp, pattern); 
		new_pat_cnt++;
		iter_count++;
		cout<<"."; cout.flush();
	} 

	if(glp_get_status(master_lp) != GLP_OPT)
		this->lp_status = REAL_INFEA;

	else if(this->int_sol() == true) {
		this->opt_obj_val = glp_get_obj_val(master_lp); 
		this->lp_status = OPT_INT;

		/* Imp: Store integer solution */
		if(this->opt_obj_val < BBNode::get_best_int_obj_val()) {
    	// Copy object to BestNode.
      if(BestNode != NULL)
      	delete(BestNode);
	    BestNode = new BBNode(this->node_id, this);

			BBNode::set_best_int_obj_val(this->opt_obj_val);
			store_solution(master_lp);
		}

	} else {
		this->opt_obj_val = glp_get_obj_val(master_lp); 
		this->lp_status = OPT_NONINT;
	}

	if(this->node_id == 1) {
		lpx_write_cpxlp(master_lp, "master-root.lp");
	}

	cout<<endl;
	cout<<"Patterns: Current = "<<setw(3)<<curr_pat_cnt;
	cout<<", New = "<<setw(3)<<new_pat_cnt<<", ";
}

/*------------------------------------------------------------------------
Description: Returns true if all pattern variables are integer else
returns false.
------------------------------------------------------------------------*/
bool BBNode::int_sol()
{
	double dblx, floor_x, ceil_x;
	bool integer_sol = true;

	PatternIterator pat_iter = pattern_list.begin();
	for(; pat_iter != pattern_list.end(); pat_iter++) {

		dblx = glp_get_col_prim(master_lp, (*pat_iter)->get_master_col_num());
		floor_x = floor(dblx);
		ceil_x = ceil(dblx);
		if((dblx - floor_x) <= EPSILON)
			(*pat_iter)->var_status = INTEGER;
		else if((ceil_x - dblx) <= EPSILON) 
			(*pat_iter)->var_status = INTEGER;
		else {			
			(*pat_iter)->var_status = FRACTIONAL;
			integer_sol = false;
		}
	}

	return integer_sol;
}

/*------------------------------------------------------------------------
Precondition: Call to function int_sol. 
Description: Find the column index of fractional variable.
Branch on the fractional variable. 
Create left and right child nodes in the BB tree.
------------------------------------------------------------------------*/
void BBNode::branch(BBNodeContainer& bbnode_set)
{
	/* Column index of fractional variable, to be branched. */
	int col_ind = -1;				
	BBNode * left_child = NULL;
	BBNode * right_child = NULL;
	double dblx, ceil_x, floor_x;

	/* Find first fractional variable 'col_ind' to be fixed. */
	PatternIterator pat_iter = pattern_list.begin();
	for(; pat_iter != pattern_list.end(); pat_iter++) {
		if((*pat_iter)->var_status == FRACTIONAL) {
			col_ind = (*pat_iter)->get_master_col_num();
			break;
		}
	}
	assert(col_ind != -1);
	/* Get the value of fractional variable. */
	dblx = glp_get_col_prim(master_lp, col_ind);
	ceil_x = ceil(dblx);
	floor_x = floor(dblx);

	/* Fix UP, ceil_x <= variable << Infinity */
	right_child = new BBNode(2*this->node_id + 1, this);
	right_child->add_var_fix(col_ind, ceil_x, Infinity);		
	bbnode_set.push_back(right_child);

	/* Fix DOWN, 0 <= variable <= floor_x */
	left_child = new BBNode(2*this->node_id, this);
	left_child->add_var_fix(col_ind, 0.0, floor_x);
	bbnode_set.push_back(left_child);

	if(option->test == false)
	        fout<<"Branching on pattern variable, col index = "<<col_ind<<endl;
}

/*------------------------------------------------------------------------
 * Process new tuple (col_ind, lb, ub). 
 * Check if variable already exist in the node's fix_col_ind_list array.
 * If it does exist then update the bounds. 
------------------------------------------------------------------------*/
void BBNode::add_var_fix(int col_ind, double lb, double ub)
{
	int ccnt;
	int list_index;

	ccnt = glp_get_num_cols(this->master_lp);
	assert(col_ind > 0);
	assert(col_ind <= ccnt);

	/* Check if tuple exists. */
	list_index = find_var_fix(col_ind);
	if(list_index == -1) {
		fix_col_ind_list.push_back(col_ind);
		lb_list.push_back(lb);
		ub_list.push_back(ub);
	} else {
		/* Update existing tuple. */
		//fout << "NOTE: Updating existing fix var tuple "<<endl;
		double lhs = max(lb, lb_list[list_index]);
		double rhs = min(ub, ub_list[list_index]);
		if(lhs > rhs)
			assert(0);

		lb_list[list_index] = max(lb, lb_list[list_index]);
		ub_list[list_index] = min(ub, ub_list[list_index]);
		
	}
}

/*------------------------------------------------------------------------
 * Check if given column index is already present in the list of variables
 * to be fixed.
------------------------------------------------------------------------*/
int BBNode::find_var_fix(int col_ind)
{
	for(size_t i = 0; i < fix_col_ind_list.size(); i++)
	{
		if(fix_col_ind_list[i] == col_ind)
			return i;
	}
	return -1;
}


/*------------------------------------------------------------------------
Unfix ALL pattern vars. 
------------------------------------------------------------------------*/
void BBNode::unfix_all_vars()
{
	int col_index;

	PatternIterator pat_iter = pattern_list.begin();
	for(; pat_iter != pattern_list.end(); pat_iter++) {

		col_index = (*pat_iter)->get_master_col_num();
		glp_set_col_bnds(master_lp, col_index, GLP_LO, 0.0, 0.0);
	}
}

/*------------------------------------------------------------------------
Description: Fix (actually change lower and upper bounds) of pattern 
variables using arrays fix_col_ind_list, lb_list, ub_list of this 
BBNode object.
------------------------------------------------------------------------*/
void BBNode::fix_vars()
{
	/* Sanity check on arrays. */
	assert(fix_col_ind_list.size() == lb_list.size());
	assert(lb_list.size() == ub_list.size());

	/* Only for certain pattern variables. */
	for(size_t i = 0; i < fix_col_ind_list.size(); i++)
	{
		int col_index = fix_col_ind_list[i];
		double lb = lb_list[i];
		double ub = ub_list[i];
		assert(lb <= ub);

		if(lb == ub)
			glp_set_col_bnds(master_lp, col_index, GLP_FX, lb, ub);
		else 
			glp_set_col_bnds(master_lp, col_index, GLP_DB, lb, ub);

		//fout<<"Fixing var "<<col_index<<" "<<lb<<" "<<ub<<endl;
	}
}


/*------------------------------------------------------------------------
Utility function
------------------------------------------------------------------------*/
void BBNode::print_lp_file(int iter_count)
{
	ostringstream lpfile;
	lpfile << "master.lp.n" << node_id << ".i" << iter_count;
	//lpx_write_cpxlp(lp, lpfile.str().c_str());
}

/*------------------------------------------------------------------------
Clean Up 
------------------------------------------------------------------------*/
BBNode::~BBNode(void)
{
}

/*------------------------------------------------------------------------
Clean up objects in ow_set.
------------------------------------------------------------------------*/
void BBNode::clean_up(BBNodeContainer& bbset)
{
	BBNodeIterator iter = bbset.begin();	
	for(; iter != bbset.end(); iter++) {
		delete(*iter);
		(*iter) = NULL;
	}
	bbset.clear();
}

/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
bool BBNode::check_duplicate(Pattern * pattern)
{
	PatternIterator pat_iter = pattern_list.begin();	
	for(; pat_iter != pattern_list.end(); pat_iter++) {
		if(pattern_compare((*pat_iter), pattern) == true)
			return true;
	}

	return false;
}

Pattern * BBNode::get_duplicate(Pattern * pattern)
{
	PatternIterator pat_iter = pattern_list.begin();	
	for(; pat_iter != pattern_list.end(); pat_iter++) {
		if(pattern_compare((*pat_iter), pattern) == true)
			return (*pat_iter);
	}

	return NULL;
}


/*------------------------------------------------------------------------
 * Precondition: Integer solution exists for master lp.
 * Store integer solution. Pattern variables values are stored in the
 * pattern objects.
------------------------------------------------------------------------*/
void BBNode::store_solution(glp_prob * master_lp)
{
	PatternIterator pat_iter = pattern_list.begin();	
	for(; pat_iter != pattern_list.end(); pat_iter++) {

		int col_index = (*pat_iter)->get_master_col_num();
		assert(col_index != -1);

		double int_sol = glp_get_col_prim(master_lp, col_index);
		(*pat_iter)->set_int_sol(int_sol);
	}
	lpx_write_cpxlp(master_lp, "best.lp");
}



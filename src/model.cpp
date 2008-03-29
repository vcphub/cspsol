/* File: model.cpp
 * Description: Functions related to master problem (i.e. cutting stock model)
 */
#include<iostream>
#include<string>
#include<sstream>
#include<cassert>
#include<cmath>
#include<vector>
#include "model.h"

const double SLACK_PENALTY = 10000.0;
std::vector<int> SlackIndexList;
const double EPSILON = 0.00001;

using namespace std;

/*------------------------------------------------------------------------
Each order demand must be satisfied.
------------------------------------------------------------------------*/
void add_demand_constraints(glp_prob * lp, OrderWidthContainer& ow_set)
{
	/* Add after existing rows. */
	int row_index = glp_get_num_rows(lp) + 1;

	/* Add demand row for each order. */
	glp_add_rows(lp, ow_set.size());

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {

		double rhs = (*ow_iter)->get_demand();
		glp_set_row_bnds(lp, row_index, GLP_LO, rhs, 0.0);

		ostringstream sout;
		sout << "Width" << (*ow_iter)->get_id();
		glp_set_row_name(lp, row_index, sout.str().c_str()); 

		(*ow_iter)->set_master_row_num(row_index);

		row_index++;
	}	

	cout << "Total rows, cols = "<<(glp_get_num_rows(lp))<<", "
		<<(glp_get_num_cols(lp))<<endl;
}

/*------------------------------------------------------------------------
 * Precondition: Function add_demand_constraints must be called.
 * Description: Initially no pattern variables are present in the master problem
 * therefore slack variables are neeed to satisfy demand constraints.
------------------------------------------------------------------------*/
void add_slack_variables(glp_prob * lp, OrderWidthContainer& ow_set)
{
	int col_index = glp_get_num_cols(lp) + 1;
	/* Add slack variable to each demand rows. */
	glp_add_cols(lp, ow_set.size());

	OrderWidthIterator ow_iter = ow_set.begin();
	for(; ow_iter != ow_set.end(); ow_iter++) {
		ostringstream sout;
		int ind[2];
		double val[2];

		sout << "Slack" << (*ow_iter)->get_master_row_num();
		glp_set_col_name(lp, col_index, sout.str().c_str());
		glp_set_col_bnds(lp, col_index, GLP_LO, 0.0, 0.0);
		glp_set_obj_coef(lp, col_index, SLACK_PENALTY);
		
		ind[1] = (*ow_iter)->get_master_row_num();
		val[1] = 1.0;
		glp_set_mat_col(lp, col_index, 1, ind, val);

		SlackIndexList.push_back(col_index);
		col_index++;
	}
	
	cout << "Total rows, cols = "<<(glp_get_num_rows(lp))<<", "
		<<(glp_get_num_cols(lp))<<endl;
}

/*------------------------------------------------------------------------
Use dual values in the master problem and store them in order objects.
------------------------------------------------------------------------*/
void store_dual_values(glp_prob * lp, OrderWidthContainer& ow_set)
{	
	int row_index;
	double value;

	OrderWidthIterator ow_iter = ow_set.begin();
	for(; ow_iter != ow_set.end(); ow_iter++) {

		row_index = (*ow_iter)->get_master_row_num();
		value = glp_get_row_dual(lp, row_index);
		(*ow_iter)->set_dual_value(value);
	}
}

/*------------------------------------------------------------------------
Try to add best pattern to the model.
------------------------------------------------------------------------*/
bool add_pattern(glp_prob * master_lp, Pattern * pattern)
{
	int col_index = 0;
	
	if(Pattern::check_duplicate(pattern) == false) {

		col_index = glp_get_num_cols(master_lp) + 1;
		glp_add_cols(master_lp, 1);
		glp_set_obj_coef(master_lp, col_index, 1.0);
		glp_set_col_bnds(master_lp, col_index, GLP_LO, 0.0, 0.0);
		glp_set_col_kind(master_lp, col_index, GLP_IV);
		glp_set_mat_col(master_lp, col_index, pattern->nzcnt, pattern->ind, pattern->val);
		
		pattern->set_master_col_num(col_index);

		return true;
	} else {
		//cout << "Duplicate patten, not added to the master problem." << endl;
		return false;
	}
}

/* Check status of slack variables. 
Return value:
true = if atleast one slack variable has positive value.
false = if all slack variables are close to zero.
*/
bool nonzero_slack_vars(glp_prob * lp)
{
	int col_ind;
	double x;

	for(size_t i = 0; i < SlackIndexList.size(); i++) {
		col_ind = SlackIndexList[i];
		x = glp_get_col_prim(lp, col_ind);
		if(x >= EPSILON)
			return true;
	}

	return false;
}

/* Utility function. */
void print_pattern_var(glp_prob * master_lp)
{
	int col_index = 0;
	for(size_t i = 0 ; i < PatternList.size(); i++) {
		col_index = PatternList[i]->get_master_col_num();
		fout << glp_get_col_prim(master_lp, col_index) <<" ";
	}

}


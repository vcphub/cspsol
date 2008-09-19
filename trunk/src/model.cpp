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

const double EPSILON = 0.00001;

using namespace std;

/*------------------------------------------------------------------------
Each order demand must be satisfied.
------------------------------------------------------------------------*/
void add_demand_constraints(glp_prob * master_lp, OrderWidthContainer& ow_set)
{
	/* Add after existing rows. */
	int row_index = glp_get_num_rows(master_lp) + 1;

	/* Add demand row for each order. */
	glp_add_rows(master_lp, ow_set.size());

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {

		double rhs = (*ow_iter)->get_demand();
		glp_set_row_bnds(master_lp, row_index, GLP_LO, rhs, 0.0);

		ostringstream sout;
		sout << "Width" << (*ow_iter)->get_id();
		glp_set_row_name(master_lp, row_index, sout.str().c_str()); 

		(*ow_iter)->set_master_row_num(row_index);

		row_index++;
	}	

	cout << "Add order demand constraints. ";
	cout << "Total rows, cols = "<<(glp_get_num_rows(master_lp))<<", "
		<<(glp_get_num_cols(master_lp))<<endl;
}

/*------------------------------------------------------------------------
 * Create trivial pattern for each OrderWidth object.
 * Add these patterns to the master problem.
------------------------------------------------------------------------*/
void add_init_patterns(glp_prob * master_lp, OrderWidthContainer& ow_set)
{

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {

		/* Only one non-zero element for each column/pattern. */
		int nzcnt = 1; 
		int * ind = new int[nzcnt+1];
		double * val = new double[nzcnt+1];
		ind[1] = (*ow_iter)->get_master_row_num();
		val[1] = 1.0;

		/* Create new pattern object and store arrays ind and val into it. */
		Pattern * pattern = new Pattern();
		pattern->ind = ind;
		pattern->val = val;
		pattern->nzcnt  = nzcnt;

		add_pattern(master_lp, pattern);
	}
	cout << "Added initial patterns. ";
	cout << "Total rows, cols = "<<(glp_get_num_rows(master_lp))<<", "
		<<(glp_get_num_cols(master_lp))<<endl;
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
+ Description: Try to add new best pattern to the model and global 
container PatternList. 
+ Arguments: glp_prob object, Pattern object, guaranteed to be new.
+ Return value:
true = New pattern added successfully.
------------------------------------------------------------------------*/
bool add_pattern(glp_prob * master_lp, Pattern * pattern)
{
	int col_index = 0;
	
	assert(pattern != NULL);

	/* Add to global container AllPatternList. */
	AllPatternList.push_back(pattern);

	col_index = glp_get_num_cols(master_lp) + 1;
	glp_add_cols(master_lp, 1);
	glp_set_obj_coef(master_lp, col_index, 1.0);
	glp_set_col_bnds(master_lp, col_index, GLP_LO, 0.0, 0.0);
	glp_set_col_kind(master_lp, col_index, GLP_IV);
	glp_set_mat_col(master_lp, col_index, pattern->nzcnt, pattern->ind, pattern->val);
		
	pattern->set_master_col_num(col_index);

	return true;
}

/* Utility function. */
/*
void print_pattern_var(glp_prob * master_lp)
{
	int col_index = 0;
	for(size_t i = 0 ; i < PatternList.size(); i++) {
		col_index = PatternList[i]->get_master_col_num();
		fout << glp_get_col_prim(master_lp, col_index) <<" ";
	}
}
*/

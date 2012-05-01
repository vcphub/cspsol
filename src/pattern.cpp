/* File: pattern.cpp
 * Description: 
 */

#include<vector>
#include<string>
#include<cassert>
#include<iostream>
#include<iomanip>
#include<sstream>
#include<cmath>

#include "glpk.h"
#include "pattern.h"
#include "extern.h"
#include "knapsack.h"
#include "cmdline.h"

using namespace std;

/* Global container to store all pattern objects. */
double max_pattern_width;	 
PatternContainer AllPatternList;

/*------------------------------------------------------------------------
 * Constructor
------------------------------------------------------------------------*/
Pattern::Pattern(void)
{
	this->ind = NULL;
	this->val = NULL;
	this->nzcnt = 0;
	this->var_status = FRACTIONAL;
	this->fixed_status = false;
	this->int_sol = 0.0;
        this->unused_width = max_pattern_width;
}

/*------------------------------------------------------------------------
 * Pattern constructor. Arrays ind and val and useful in defining the 
 * pattern.
------------------------------------------------------------------------*/
Pattern::Pattern(int * ind, double * val)
{
	this->ind = ind;
	this->val = val;
	this->nzcnt = 0;
	this->var_status = FRACTIONAL;
	this->fixed_status = false;
	this->int_sol = 0.0;
}

/*------------------------------------------------------------------------
 * Description:  Create sub-problem (which is knapsack problem)
 * Decision variables here are number of units of an OrderWidth objects 
 * present in the the best pattern. The decision variables are integer.
 * In other words, solution to subproblem defines the best pattern.
 *
 * Add max width constraint (sum of all orders selected in the pattern 
 * should be less than max_pattern_width.)
 * Add column (for each OrderWidth in ow_set) to sub-problem. 
 *
 * Note: The dual values stored from master problem are used as 
 * objective function coefficients.
------------------------------------------------------------------------*/
void Pattern::create_subprob(glp_prob * lp, OrderWidthContainer& ow_set,
				bool alternate_sol)
{
	/* Add pattern width constraint. */
	assert(lp != NULL);
	glp_set_prob_name(lp, "SubProb");
	glp_set_obj_dir(lp, GLP_MAX);

	glp_add_rows(lp, 1);
	glp_set_row_name(lp, 1, "PatternWidthConstraint");

	/* Workaround: Force branching. This will give us alternate 
	 * opt. int. solution. Assumption: All demands widths are
	 * integer. 
	 * */
	if(alternate_sol == true)
		glp_set_row_bnds(lp, 1, GLP_UP, 0.0, max_pattern_width + 0.5);
	else
		glp_set_row_bnds(lp, 1, GLP_UP, 0.0, max_pattern_width);

	/* Add integer variables to above constraints. */
	int col_ind = glp_get_num_cols(lp) + 1;
	glp_add_cols(lp, ow_set.size());

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {

		int ind[2];
		double val[2];
		double obj_func_coeff;

		glp_set_col_kind(lp, col_ind, GLP_IV);
		glp_set_col_bnds(lp, col_ind, GLP_LO, 0.0, 0.0);

		obj_func_coeff = (*ow_iter)->get_dual_value();
		glp_set_obj_coef(lp, col_ind, obj_func_coeff);

		ind[1] = 1;
		val[1] = (*ow_iter)->get_width();	/* width */
		/* Add column to the row with row_index = 1 */
		glp_set_mat_col(lp, col_ind, 1, ind, val);
		(*ow_iter)->set_subprob_col_num(col_ind);

		col_ind++;	
	}

	/* cout << "Subproblem: Total rows, cols = "<<(glp_get_num_rows(lp))<<", "
		<<(glp_get_num_cols(lp))<<endl; */

}

/*------------------------------------------------------------------------
Description: If possible, generate new pattern.
Return value: 
	Pointer to best pattern object OR
	NULL
------------------------------------------------------------------------*/
Pattern * Pattern::get_new_pattern(BBNode * node, OrderWidthContainer& ow_set, 
				int iter_count) 
{
	Pattern * new_pat = NULL;

	if(option->subintopt == false) {
		// Solve subproblem using dynamic programming.
		PatternContainer * solset;
		solset = get_dp_solution(ow_set, max_pattern_width);

		for(int i = 0; i < (int)solset->size(); i++)
		{
			new_pat = (*solset)[i];
			if(node->check_duplicate(new_pat) == true) {
				delete new_pat; /* delete dupe */
				new_pat = NULL;
			} else {
				// new_pat->print_pattern();
				// Best pattern found.
				break;
			}
		}
		delete solset;

	} else {

		// Solve subproblem MIP using glp_intopt
		new_pat = Pattern::generate_pattern(ow_set, iter_count, false);
                /* Pointer new_pat could be NULL. */
		if(new_pat && node->check_duplicate(new_pat) == true) {
			/* Check if alternate optimal integer solution exists. */
			if(option->workaround == true) {
				fout << "Got duplicate pattern. Looking for alternate." << endl;
				new_pat = Pattern::generate_pattern(ow_set, iter_count, true);
			}
		        if(new_pat && node->check_duplicate(new_pat) == true)
			        new_pat = NULL;
		}
	}

	return new_pat;
}


/*------------------------------------------------------------------------
Description: Create and solve subproblem (integer knapsack problem). 
Use optimal solution to sub-problem to generate best pattern. 
Return value: Pointer to best pattern object OR NULL.
------------------------------------------------------------------------*/

Pattern * Pattern::generate_pattern(OrderWidthContainer& ow_set, int iter_count, 
				bool tol_flag)
{
	glp_prob * subp_lp;	/* Subproblem IP */
	glp_iocp parm;
	ostringstream sout;

	subp_lp = glp_create_prob();

	/* New control parameter object. */
	glp_init_iocp(&parm);

	if(tol_flag == true) {
		parm.tol_obj = -parm.tol_obj; // i.e. -1e-7
		create_subprob(subp_lp, ow_set, true);	
	} else {
		/* Use default parameters. */
		create_subprob(subp_lp, ow_set, false);	
	}

	/* Now subprob has order width vars and max. width constraint */
	//sout << "subprob.lp." << iter_count;
	//lpx_write_cpxlp(subp_lp, sout.str().c_str());

	/* Solve the subproblem as integer program */
	int status = glp_simplex(subp_lp, NULL);
	assert(status == 0);
	//assert(glp_get_status(subp_lp) == GLP_OPT);

	status = glp_intopt(subp_lp, &parm);
	assert(status == 0);
	assert(glp_mip_status(subp_lp) == GLP_OPT);

	//fout << "Subproblem obj func value = "<< glp_mip_obj_val(subp_lp) << endl;

	/* Store solution into Pattern object. */	
	/* Count solution variables with non-zero values. */
	int ccnt = glp_get_num_cols(subp_lp);
	int nzcnt = 0;
	for(int col_ind = 1; col_ind <= ccnt; col_ind++) {
		double value = glp_mip_col_val(subp_lp, col_ind);
		if(value != 0.0) {
			nzcnt++;
		}
	}

	if(nzcnt == 0) {
		//lpx_write_cpxlp(subp_lp, "debug.subprob.lp");
		glp_delete_prob(subp_lp);
		return NULL;
	}

	/* Using solution of the subproblem, populate arrays ind and val. */
	int * ind = new int[nzcnt+1];
	double * val = new double[nzcnt+1];
	assert(ind != NULL); assert(val != NULL);

	OrderWidthIterator ow_iter = ow_set.begin();	
	for(int i = 1; ow_iter != ow_set.end(); ow_iter++) {

		int subprob_col_ind = (*ow_iter)->get_subprob_col_num();			
		double value = glp_mip_col_val(subp_lp, subprob_col_ind);
		if(value != 0.0) {
			ind[i] = (*ow_iter)->get_master_row_num();
			val[i] = value;
			i++;
		}
	}
	/* Create new pattern object and store arrays ind and val into it. */
	Pattern * pattern = new Pattern();
	pattern->ind = ind;
	pattern->val = val;
	pattern->nzcnt  = nzcnt;
	
	/* Clean up subproblem. */
	glp_delete_prob(subp_lp);

	return pattern;
}

/*------------------------------------------------------------------------
 * Description: Compare two pattern objects.
 * Return value : true if duplicate.
 * Return value = false, not duplicate
------------------------------------------------------------------------*/
bool pattern_compare(Pattern * lhs, Pattern * rhs)
{
	int i = 0;

	if(lhs->nzcnt != rhs->nzcnt)
		return false;

	for(i = 1; i <= rhs->nzcnt; i++) 
	{
		if((lhs->ind[i] != rhs->ind[i]) ||
			(lhs->val[i] != rhs->val[i]))
			break;
	}
	if(i == rhs->nzcnt + 1)
		return true;
	else
		return false;
}

/*------------------------------------------------------------------------
Description: Check if exactly same pattern already exists in PatterList.
Return value:
true = Duplicte pattern exists.
false = New pattern.
bool Pattern::check_duplicate(Pattern * pattern)
{
	PatternIterator pat_iter = PatternList.begin();	
	for(; pat_iter != PatternList.end(); pat_iter++) {
		if(pattern_compare((*pat_iter), pattern) == true)
			return true;
	}
	return false;
}
------------------------------------------------------------------------*/

void Pattern::print_pattern()
{
	fout << "# Pattern (nzcnt = "<<(nzcnt)<<"). {";
	for(int i = 1; i <= this->nzcnt; i++)
		fout << "("<<this->ind[i]<<" "<<(this->val[i])<<"), ";
	fout<<"}"<<endl;
}

/* 
 * FFD heuristics for generating initial patterns :
 * Given order width object and demand for the order widht, 
 * assign/fit as much demand as possible to the pattern. 
 * Calculate remaining demand.
 * */
void Pattern::assign_order_width(OrderWidth * ow, int& demand)
{
	int count = this->unused_width/ow->get_width();
  if(count > demand) 
  	count = demand;

  if(count == 0) 
  	return;

  int nzcnt = this->nzcnt;

  /* Copy arrays data from the pattern */
	int * ind = new int[nzcnt+1+1];
	double * val = new double[nzcnt+1+1];
  if(this->ind && this->val) {
  	for(int i = 1; i <= nzcnt; i++) {
    	ind[i] = this->ind[i];
      val[i] = this->val[i];
    }
  }

	ind[nzcnt+1] = ow->get_master_row_num();
	val[nzcnt+1] = count;

  delete [] (this->ind); this->ind = NULL;
  delete [] (this->val); this->val = NULL;

  this->ind = ind;
  this->val = val;
  this->nzcnt++;
  this->unused_width = this->unused_width - count * ow->get_width();

  demand = demand - count; 
}

/*------------------------------------------------------------------------
Clean up objects in PatterList.
------------------------------------------------------------------------*/
void Pattern::clean_up()
{
	PatternIterator pat_iter = AllPatternList.begin();	
	for(; pat_iter != AllPatternList.end(); pat_iter++) {
		delete(*pat_iter);
		(*pat_iter) = NULL;
	}
	AllPatternList.clear();
}

/* Clean up */
Pattern::~Pattern(void)
{
	delete [] ind; ind = NULL;
	delete [] val; val = NULL;
}

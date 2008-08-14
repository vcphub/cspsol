#ifndef PATTERN_H
#define PATTERN_H

#include<vector>
#include<iostream>
#include "order_width.h"
#include "glpk.h"

enum PatternVarStatus {INTEGER, FRACTIONAL};
class Pattern;	/* Forward class declaration */

typedef std::vector<Pattern*> PatternContainer;
typedef std::vector<Pattern*>::iterator PatternIterator;

/* Each pattern is made of 1 or more different OrderWidths. 
 * A paper roll of width max_pattern_width is cut into a pattern. */
class Pattern
{
	int master_col_num;
	/* Integer solution value of pattern variable. */
	double int_sol;		

public:
	PatternVarStatus var_status;	
	bool fixed_status;	

	/* ind[i] is row index of ith OrderWidth in this pattern. */		
	int * ind;		
	/* Number of ith OrderWidth in this pattern. */
	double * val;
	/* Non-zero count, length of arrays ind and val is nzcnt+1. */
	int nzcnt;		
	
	int get_master_col_num() { return master_col_num; }
	void set_master_col_num(int n) { master_col_num = n; }

	/* Integer solution */
	double get_int_sol() { return int_sol; }
	void set_int_sol(double x) { int_sol = x; }

	static Pattern * generate_pattern(OrderWidthContainer& ow_set, int icnt, 
					bool tol_flag);
	static void create_subprob(glp_prob * lp, OrderWidthContainer& ow_set,
					bool alternate_sol_flag);
	static bool check_duplicate(Pattern * pattern);
	void print_pattern();

	static void print_solution(std::ostream& fout, glp_prob* master_lp, 
			OrderWidthContainer& ow_set);
	static void store_solution(glp_prob * master_lp);
	static void clean_up();

	Pattern(void);
	Pattern(int * ind, double * val);
	~Pattern(void);
};

#endif

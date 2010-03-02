
#ifndef KNAPSACK
#define KNAPSACK

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
#include<cassert>
#include<cmath>
#include "pattern.h"

/*-----------------------------------------------------------------*/
/* Each object is a integer solution to subproblem. */
class KnapSol {

	public:
		/* Number of ith order width in this pattern. */	
		int * val;
		int size;
		KnapSol(int size); 
		KnapSol(KnapSol * rhs); 
		~KnapSol();

		void print(); 
		void add_solution(KnapSol * sol);
		int get_nzcnt();
};

/*-----------------------------------------------------------------*/
/* Solutions with same obj. func. value. */
class KnapSolSet {

	public:
		double int_sol;
		std::vector<KnapSol*> solutions;
		KnapSolSet();
		~KnapSolSet();

		int get_size();
		void add_solution(KnapSol * sol);
		KnapSol * get_solution(int i);
		bool solution_exists(KnapSol * new_sp);
		void print();
};

typedef std::vector<KnapSolSet*> KSSContainer;
typedef std::vector<KnapSolSet*>::iterator KSSIterator;

PatternContainer * get_dp_solution(OrderWidthContainer& owc, 
				double dbl_max_wid);

int get_rem_width(int max_width, int width);
KnapSolSet * opt_sol_set(OrderWidthContainer& owc, int max_width);

#endif

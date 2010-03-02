
#include<cassert>
#include<cmath>
#include<string> 
#include "knapsack.h"
#include "pattern.h"
#include "extern.h"

using namespace std;

/* Global definitions. */
/* Container for storing solution sets for each integer value 
 * of max. order width (0, max_order_width). */
KSSContainer AllKnapSolSets;

/*--------------------------------------------------------------------
 * Solve subproblem by dynamic programming algorithm.
 * Return value:
 * 	Container of one or more best patterns.
--------------------------------------------------------------------*/
PatternContainer * get_dp_solution(OrderWidthContainer& owc, 
				double dbl_max_wid)
{
	/* Must be INTEGER. */
	int max_width = (int)dbl_max_wid;

	/* Start with max. width = 0. */
	assert(AllKnapSolSets.empty());
	AllKnapSolSets.push_back(new KnapSolSet());

	/* Get optimal solutions values of max. order width
	 * 1 to max_width. */
	for(int w = 1; w <= max_width; w++) {
		KnapSolSet * ss = opt_sol_set(owc, w);
		AllKnapSolSets.push_back(ss);
	}

	/* Use final solution AllKnapSolSets[max_width]. */
	KnapSolSet * opt_ss = AllKnapSolSets[max_width];
	PatternContainer * opt_pat_set = new PatternContainer();

	/* For each KnapSol object, create Pattern object. */
	for(int si = 0; si < opt_ss->get_size(); si++) {

		KnapSol * sol = opt_ss->get_solution(si);
		int nzcnt = sol->get_nzcnt();

		Pattern * pattern = new Pattern();
		pattern->ind = new int[nzcnt+1];
		pattern->val = new double[nzcnt+1];
		assert(pattern->ind != NULL); 
		assert(pattern->val != NULL);
		pattern->nzcnt  = nzcnt;

		for(int li = 1, i = 0; i < int(owc.size()); i++) {
			if(sol->val[i] != 0) {
				pattern->ind[li] = owc[i]->get_master_row_num();
				pattern->val[li] = sol->val[i];	
				li++;
			}
		}

		opt_pat_set->push_back(pattern);
	}

	/* Clean up */
	for(int i = 0; i < (int)AllKnapSolSets.size(); i++)
		delete(AllKnapSolSets[i]);

	AllKnapSolSets.clear();

	return opt_pat_set;
}

/*--------------------------------------------------------------------
 * Core DP logic. Calculate one or more optimal integer
 * solutions for give value max_width. 
--------------------------------------------------------------------*/
KnapSolSet * opt_sol_set(OrderWidthContainer& owc, int max_width)
{
	double best_value = -10000.0; /* MAX */
	vector<int> best_ind;

	/* For each item in owc. */
	for(int i = 0; i < (int)owc.size(); i++) {

		/* If order width cannot fit. */
		if(owc[i]->get_width() > max_width) continue; 
			
		/* Important: Order width must be INTEGER. */
		int rem_width = get_rem_width(max_width, (int)owc[i]->get_width());

		double obj_val = 0.0;
		obj_val = owc[i]->get_dual_value() + 
				AllKnapSolSets[rem_width]->int_sol;

		if(fabs(obj_val - best_value) <= 1e-9) {
			/* Solution with same obj. func. val. */
			best_ind.push_back(i);
		} else if(obj_val > best_value) {
			/* Better solution. */
			best_ind.clear(); /* Remove existing indices. */
			best_ind.push_back(i);
			best_value = obj_val;
		} 
	}

	KnapSolSet * new_ss = new KnapSolSet();
	if(best_ind.empty() == true)
		new_ss->int_sol = 0.0;
	else
		new_ss->int_sol = best_value;

	for(int i = 0; i < (int)best_ind.size(); i++) {

		int bi = best_ind[i];
		int rem_width = get_rem_width(max_width, owc[bi]->get_width());

		/* For all solutions of substructure solution set. */
		KnapSolSet * ss = AllKnapSolSets[rem_width];

		for(int p = 0; p < ss->get_size(); p++) {

			KnapSol * sp = ss->solutions[p];
			KnapSol * new_sp = new KnapSol(sp);

			new_sp->val[bi] += 1;
			new_ss->add_solution(new_sp);
		}

		if(ss->get_size() == 0) {

			KnapSol * new_sp = new KnapSol(owc.size());
			new_sp->val[bi] += 1;
			new_ss->add_solution(new_sp);
		}

		/* Control the number of solutions. */
		int max_sol_cnt = max_width/powf(10.0, 4.0-option->opt_level);
		if((int)new_ss->solutions.size() > max_sol_cnt)
			break; 
	}

	/* Return the best solution. */
	//cout << "best int solution = " << new_ss->int_sol << endl;
	return new_ss;
}

/*--------------------------------------------------------------------
 * Constructor. Size is always equal to total number order widths.
--------------------------------------------------------------------*/
KnapSol::KnapSol(int size) 
{
	this->size = size;
	val = new int[size];
	assert(val != NULL);
	for(int i = 0; i < size; i++)
		val[i] = 0;
}

/*--------------------------------------------------------------------
 * Somewhat like a copy constructor.
--------------------------------------------------------------------*/
KnapSol::KnapSol(KnapSol * rhs)
{
	this->size = rhs->size;
	val = new int[size];

	assert(val != NULL);
	assert(rhs->val != NULL);

	for(int i = 0; i < size; i++)
		this->val[i] = rhs->val[i];
}

/* Destructor. */
KnapSol::~KnapSol()
{
	delete [] val; val = NULL;
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
void KnapSol::print() 
{ 
	for(int i = 0; i < size; i++) {
		if(val[i] != 0)
			cout<<"ow"<<i<<":"<< val[i] <<", ";
	}
	cout << endl;
}

/*--------------------------------------------------------------------
 * KnapSolSet: Constructor and Destructor.
--------------------------------------------------------------------*/
KnapSolSet::KnapSolSet() 
{
	int_sol = 0.0;
}

KnapSolSet::~KnapSolSet() 
{
	for(int i = 0; i < (int)solutions.size(); i++)
		delete(solutions[i]);

	solutions.clear();
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
int KnapSolSet::get_size() 
{ 
	return (int)solutions.size(); 
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
void KnapSolSet::add_solution(KnapSol * sol)
{
	if(this->solution_exists(sol) == false)
		solutions.push_back(sol);
        else {
                // KnapSol object not needed, so delete it.
                delete(sol); 
        }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
int KnapSol::get_nzcnt()
{
	int nzcnt = 0;
	for(int i = 0; i < size; i++)
		if(val[i] != 0)
			nzcnt++;
	return nzcnt;
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
KnapSol * KnapSolSet::get_solution(int i)
{
	if(i >= (int)solutions.size()) return NULL;
	return solutions[i];	
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
bool KnapSolSet::solution_exists(KnapSol * new_sp)
{
	int pi = 0;
	for(pi = 0; pi < (int)solutions.size(); pi++)
	{
		KnapSol * sp = solutions[pi];
		int i = 0;
		for(i = 0; i < (int)sp->size; i++) {
			if(sp->val[i] != new_sp->val[i])
				break;
		}
		if(i == (int)sp->size)
			break;	// Duplicate pattern found.
	}
	if(pi == (int)solutions.size())
		return false; // No duplicates found.

	return true; // Duplicate pattern found.
}


/*--------------------------------------------------------------------
--------------------------------------------------------------------*/
void KnapSolSet::print() 
{
	cout<<"# Solution set. obj. value = "<< int_sol << endl;
	for(int i = 0; i < (int)solutions.size(); i++) 
	{
		cout<<"S"<<i<<" ";
		solutions[i]->print();
		cout<<endl;
	}
}

/*--------------------------------------------------------------------
 * After adding a order item to the knapsack, calc. remaining width.
--------------------------------------------------------------------*/
int get_rem_width(int max_width, int width)
{
	int rem_width = max_width - width;
	if(rem_width < 0) rem_width = 0;

	return rem_width;
}


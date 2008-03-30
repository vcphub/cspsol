#ifndef MODEL_H
#define MODEL_H

#include "order_width.h"
#include "extern.h"
#include "glpk.h"

void add_demand_constraints(glp_prob * master_lp, OrderWidthContainer& ow_set);
void add_slack_variables(glp_prob * master_lp, OrderWidthContainer& ow_set);
void add_init_patterns(glp_prob * master_lp, OrderWidthContainer& ow_set);

void store_dual_values(glp_prob * lp, OrderWidthContainer& ow_set);
bool add_pattern(glp_prob * master_lp, Pattern * pattern);
bool integer_sol_found(glp_prob * lp);

void print_pattern_var(glp_prob * master_lp);
bool nonzero_slack_vars(glp_prob * lp);

#endif

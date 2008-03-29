#ifndef ORDER_WIDTH_H
#define ORDER_WIDTH_H

#include<iostream>
#include<string>
#include<vector>

class OrderWidth;	/* Forward declaration. */
typedef std::vector<OrderWidth*> OrderWidthContainer;
typedef std::vector<OrderWidth*>::iterator OrderWidthIterator;

class OrderWidth
{
	/* In meters. Each pattern is made of 1 or more different OrderWidths. */
	int width;				
	int demand;			/* Quantity ordered */	
	int id;
	static int count;	/* Keep track of total number of objects. */

	/* Master Model */
	/* Demand constraint in master problem */		
	int master_row_num;	
	double dual_value;	/* Row dual value */

	/* Subproblem */
	int subprob_col_num;	

public:
	OrderWidth(int width, int demand);

	/* OrderWidth object container related member functions. */
	static void read_order_data(OrderWidthContainer& ow_set, std::string filename);
	static void print_order_list(OrderWidthContainer& ow_set);

	int get_master_row_num() { return master_row_num; }
	void set_master_row_num(int n) { master_row_num = n; }

	int get_subprob_col_num() { return subprob_col_num; }
	void set_subprob_col_num(int n) { subprob_col_num = n; }

	double get_dual_value() { return dual_value; }
	void set_dual_value(double dual_value) { this->dual_value = dual_value; }

	int get_id() { return id; }
	int get_demand() { return demand; }
	int get_width() { return width; }

	/* Given master row index find OrderWidth object. */
	static OrderWidth* find_orderwidth(OrderWidthContainer& ow_set, int row_index);

	static void clean_up(OrderWidthContainer& ow_set);
	~OrderWidth(void);
};


#endif

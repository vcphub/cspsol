#ifndef ORDER_WIDTH_H
#define ORDER_WIDTH_H

#include<iostream>
#include<string>
#include<cassert>
#include<vector>

class OrderWidth;	/* Forward declaration. */
typedef std::vector<OrderWidth*> OrderWidthContainer;
typedef std::vector<OrderWidth*>::iterator OrderWidthIterator;

/* 
 * Order from customer. number of items = demand, item width = width.
 * */ 
class OrderWidth
{
	/* In meters. Each pattern is made of 1 or more different OrderWidths. */
	double width;           /* Width in units of length. */
	int demand;		/* Quantity ordered. */	
	int id;                 /* Unique identifier for each object.*/
	static int count;	/* Keep track of total number of objects. */

	/* Master problem/model */
        /* Row index of associated demand constraint. */
	int master_row_num;	
	double dual_value;	/* Row dual value */

	/* Subproblem */
	int subprob_col_num;	

public:
	OrderWidth(double width, int demand);

	/* OrderWidth object container related member functions. */
	static void read_item_data(OrderWidthContainer& ow_set, std::string filename);
	static void read_order_data(OrderWidthContainer& ow_set, std::string filename);
	static void print_order_list(OrderWidthContainer& ow_set);

	int get_master_row_num() { 
		assert(master_row_num != -1); 
		return master_row_num; 
	}
	void set_master_row_num(int n) { 
		assert(master_row_num == -1); 
		master_row_num = n; 
	}

	int get_subprob_col_num() { return subprob_col_num; }
	void set_subprob_col_num(int n) { subprob_col_num = n; }

	double get_dual_value() { return dual_value; }
	void set_dual_value(double dual_value) { this->dual_value = dual_value; }

	int get_id() { return id; }
	int get_demand() { return demand; }
	double get_width() { return width; }

	/* Given master row index find OrderWidth object. */
	static OrderWidth* find_orderwidth(OrderWidthContainer& ow_set, int row_index);

	static void clean_up(OrderWidthContainer& ow_set);
	~OrderWidth(void);
};

/* Function object. */
class CmpOrderWidth
{
        public:
                CmpOrderWidth() {}

                bool operator()(OrderWidth * lhs, OrderWidth * rhs) {
                        if(lhs->get_width() > rhs->get_width())
                                return true;
                        else 
                                return false;
                }
};

#endif

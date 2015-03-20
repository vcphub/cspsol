/* File: order_width.cpp 
 * Description: Member functions of class OrderWidth are defined.
 * A OrderWidth object stores all information about an order placed
 * by customer to paper mill.
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cassert>
#include<cstdlib>

#include "order_width.h"
#include "extern.h"

using namespace std;

// Initialize static property.
int OrderWidth::count = 0;

/*------------------------------------------------------------------------
 * Constructor
------------------------------------------------------------------------*/
OrderWidth::OrderWidth(double w, int d) :
	width(w), demand(d)
{
	if(demand <= 0) {
		cout<<"incorrect input: demand = "<<demand<<endl;
		exit(-1);
	}
	if(width <= 0 || width > max_pattern_width) {
		cout << "incorrect input: width = " << width;
		cout << " max pattern width = " << max_pattern_width << endl;
		exit(-1);
	}

	this->solution = 0;	 // solution not found yet.
	this->id = OrderWidth::count + 1;
	OrderWidth::count++;

	/* Initialize indices. */
	this->master_row_num = -1;
	this->subprob_col_num = -1;
}

/*------------------------------------------------------------------------
 * Cleanup orderwidth
------------------------------------------------------------------------*/
OrderWidth::~OrderWidth(void)
{
}

/*------------------------------------------------------------------------
# Static function.
------------------------------------------------------------------------*/
void OrderWidth::read_input(OrderWidthContainer& ow_set, std::string filename)
{
	if(option->bpp == true)
		OrderWidth::read_item_data(ow_set, option->data_file);
	else
		OrderWidth::read_order_data(ow_set, option->data_file);
}

/*------------------------------------------------------------------------
# Static function.
# Description: Read item data from file and populate container object. 
Although input data is for Bin Packing Problem (BPP). It is converted
into CSP using following equivalence:
 items         = orders
 item weight   = order width
 bin capacity  = max pattern width

# Input File Format:
First line = Number of items.
Second line = bin capacity. 
Remaining lines = Weights of all items.
------------------------------------------------------------------------*/
void OrderWidth::read_item_data(OrderWidthContainer& ow_set, std::string filename)
{
	ifstream fin;
	double width;
 	int demand = 1, count = 0;

	if(filename == "stdin") {
		cin >> count;
		cin >> max_pattern_width;

		for(int i = 0; i < count; i++)
		{
			cin >> width; 
			OrderWidth * order = new OrderWidth(width, demand);
			ow_set.push_back(order);
		}	
	} else {
		fin.open(filename.c_str());
		assert(fin.is_open());

		fin >> count;
		fin >> max_pattern_width;

		for(int i = 0; i < count; i++)
		{
			fin >> width; 
			OrderWidth * order = new OrderWidth(width, demand);
			ow_set.push_back(order);
		}	
	}
	if(!option->silent) {
		cout << "Reading item data from file " << filename << endl;
		cout << "Total items read from file = "<< ow_set.size() <<endl;
		cout << "Maximum pattern width = " << max_pattern_width <<endl;
	}
	fin.close();
}


/*------------------------------------------------------------------------
# Static function.
# Description: Read order data from file and populate container object. 
# Assumed Format:
First line of the input file must specify maximum length of roll. 
order_width demand_quantity
------------------------------------------------------------------------*/
void OrderWidth::read_order_data(OrderWidthContainer& ow_set, std::string filename)
{
	ifstream fin;
	double width;
 	int demand;

	if(filename == "stdin") {
		cin >> max_pattern_width;
		cin >> width >> demand;
		while(cin.eof() == 0)
		{
      // Create new object and add it to order width set.
			OrderWidth * order = new OrderWidth(width, demand);
			ow_set.push_back(order);
			cin >> width >> demand;	 /* read next line */
		}	

	} else {
		fin.open(filename.c_str());
		assert(fin.is_open());
		cout << "Reading order data from file " << filename << endl;
		fin >> max_pattern_width;
		fin >> width >> demand;		
		while(fin.eof() == 0)
		{
      // Create new object and add it to order width set.
			OrderWidth * order = new OrderWidth(width, demand);
			ow_set.push_back(order);
			fin >> width >> demand;	 /* read next line */
		}	
		cout << "Total orders read from file = " << ow_set.size() <<endl;
		cout << "Maximum pattern width = " << max_pattern_width <<endl;
		fin.close();
	}
}

/*------------------------------------------------------------------------
 * Print each order object in the container.
------------------------------------------------------------------------*/
void OrderWidth::print_order_list(OrderWidthContainer& ow_set)
{
	if(!option->silent) {
		OrderWidthIterator ow_iter = ow_set.begin();	
		for(; ow_iter != ow_set.end(); ow_iter++) {
			cout << (*ow_iter)->width <<" "<< (*ow_iter)->demand << endl;
		}
	}
}

/*------------------------------------------------------------------------
Clean up objects in ow_set.
------------------------------------------------------------------------*/
void OrderWidth::clean_up(OrderWidthContainer& ow_set)
{
	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {
		delete(*ow_iter);
		(*ow_iter) = NULL;
	}
	ow_set.clear();
}

/*------------------------------------------------------------------------
Given master row index find OrderWidth object. 
------------------------------------------------------------------------*/
OrderWidth* OrderWidth::find_orderwidth(OrderWidthContainer& ow_set, int row_index)
{
	OrderWidthIterator ow_iter = ow_set.begin();	
	for(; ow_iter != ow_set.end(); ow_iter++) {
		if((*ow_iter)->get_master_row_num() == row_index)
			return (*ow_iter);
	}
	assert(0);
	return NULL;
}



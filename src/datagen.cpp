/* Since real life data is not available, we need to generate some fake
 * data.
 */

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cassert>

using namespace std;
int max_pattern_width;

void generate_order_data(char * filename)
{
	ofstream fp;
	int width, demand;

	fp.open(filename);
	assert(fp != NULL);

	fp << max_pattern_width << endl;
	for(int i = 1; i <= max_pattern_width/20.0; i++) {
		/* Consider each range in the interval of 10. */
		width = i * 20;		
		/* Some random demand/order in the range 1 to 10. */
		demand = rand() % 10 + 1;	

		fp << width <<" "<< demand << endl;
	}
		
	fp.close();
}


int main(int argc, char * argv[])
{
	assert(argc == 3);
	max_pattern_width = atoi(argv[1]);	

	/* Load order data. */
	generate_order_data(argv[2]);

	return 0;
}

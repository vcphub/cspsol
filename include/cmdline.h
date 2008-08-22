#ifndef CMDLINE_H
#define CMDLINE_H

#include<fstream>
#include<vector>

enum SearchStrategy {DFS, BFS};
enum ReportFormat {TEXT, XML};

class CmdOption
{
	public:
		SearchStrategy search; 

		/* Input data file name */
		char * data_file; 

		/* Subproblem glp_intopt. */
		bool subintopt; 

		/* Workaround for multiple solutions. */
		bool workaround; 

		/* No progress output to standard terminal. */
		bool silent; 
		std::ofstream tout; /* Terminal outout redirection. */
		std::streambuf * cout_buf;

		/* Solution report formats. */
		std::vector<ReportFormat> rformats;
		std::vector<char*> rfilenames; 

		/* Run testcases. */
		bool test; 
		char * tc_file; 

		/* Set default values in constructor. */
		CmdOption();
		void redirect_cout();
		void restore_cout();

};

/* Global functions. */
CmdOption * process_arguments(int argc, char * argv[]);
void print_usage();

#endif

#ifndef CMDLINE_H
#define CMDLINE_H

#include<fstream>
#include<vector>

enum SearchStrategy {DFS, BFS};
enum ReportFormat {TEXT, HTML, XML};

#define VERSION_NUMBER "1.02"

/*
 * Command line options.
 * */
class CmdOption
{
	public:
                /* BB tree search. */
		SearchStrategy search; 

		/* Input data file name */
		char * data_file; 

		/* Log (copy of terminal output) file name */
		char * log_file; 

                /* User specified run time limit in SECONDS */
                int tm_lim;

		/* Subproblem glp_intopt. */
		bool subintopt; 

		/* Workaround for multiple solutions. */
		bool workaround; 

		/* Optimization level. Control runtime and memory. */
		double opt_level;

		/* Column generation at root node only. */
		bool cg_root_only;
                /* Solve bin packing problem (bpp) */
		bool bpp;

		/* No progress output to standard terminal. */
		bool silent; 
		std::ofstream tout; /* Terminal output redirection. */
		std::streambuf * cout_buf;

		/* Solution report formats. */
		std::vector<ReportFormat> rformats;
		std::vector<char*> rfilenames; 

		/* Run testcases. */
		bool test; 
		char * tc_file; 

		/* Set default values in constructor. */
		CmdOption();
                void process_arguments(int argc, char * argv[]);
                void print_usage();
                void print_version();
		void silent_cout();
		void redirect_cout();
		void restore_cout();

};

#endif

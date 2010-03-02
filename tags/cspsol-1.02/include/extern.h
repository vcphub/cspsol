#ifndef EXTERN_H
#define EXTERN_H

#include<vector>
#include<string>
#include<stack>
#include<queue>
#include<fstream>
#include "order_width.h"
#include "pattern.h"
#include "bb_node.h"
#include "cmdline.h"

extern OrderWidthContainer OrderWidthList;
extern PatternContainer AllPatternList;
extern BBNode * BestNode;

extern double max_pattern_width;
extern std::ofstream fout;
extern CmdOption * option;

//extern std::stack<BBNode*> BBStack;
extern std::queue<BBNode*> BBStack;

extern const double EPSILON;
extern const double Infinity;

#endif

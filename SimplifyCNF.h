
///////////////////
// SimplifyCNF.h //
///////////////////

#ifndef SimplifyCNF_h
#define SimplifyCNF_h

#include <vector>

// Normalize all clauses.
vector<vector<int>> simplify_cnf_1(const vector<vector<int>> & cnf);

// Remove duplicate literals from each clause.
vector<vector<int>> simplify_cnf_1(const vector<vector<int>> & cnf);

#endif // SimplifyCNF_h

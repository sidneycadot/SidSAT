
////////////////////
// SimplifyCNF.cc //
////////////////////

#include "SimplifyCNF.h"

vector<vector<int>> simplify_cnf_1(const vector<vector<int>> & cnf)
{
    vector<vector<int>> new_cnf;

    for (const vector<int> & clause : cnf)
    {
        set<int> variables;

        for (const int & literal : clause)
        {
            variables.insert(abs(literal));
        }

        if (variables.size() == clause.size())
        {
            new_cnf.push_back(clause);
        }
    }

    return new_cnf;
}

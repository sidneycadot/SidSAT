
//////////////
// SidSAT.h //
//////////////

#include <iostream>
#include <vector>
#include <algorithm>

#include "ReadDimacsCNF.h"

using namespace std;

void print_cnf(const vector<vector<long>> & cnf)
{
    for (const vector<long> & clause : cnf)
    {
        cout << "clause";
        for (const long & literal : clause)
        {
            cout << " " << literal;
        }
        cout << endl;
    }
}

vector<vector<long>> cnf_assign(const vector<vector<long>> & cnf, const long assignment)
{
    vector<vector<long>> new_cnf;

    for (const vector<long> & clause : cnf)
    {
        if (find(clause.begin(), clause.end(), assignment) != clause.end())
        {
            // This clause is satisfied by the assignment.
            // We will not include it in the new_cnf.
            continue;
        }

        // Make a new clause where remaining instances of the the literal (inverted w.r.t. assignment) are removed.
        vector<long> new_clause;

        for (const long & literal : clause)
        {
            if (literal != -assignment)
            {
                new_clause.push_back(literal);
            }
        }

        new_cnf.push_back(new_clause);
    }

    return new_cnf;
}

bool SimpleDPLL(const vector<vector<long>> & cnf, vector<long> & assignments)
{
    if (cnf.empty())
    {
        return true;
    }

    if (cnf.front().empty())
    {
        // An empty clause is not satisfiable.
        return false;
    }

    const long assignment = cnf.front().front(); // variable number of first variable of first clause.

    assignments.push_back(+assignment);

    if (SimpleDPLL(cnf_assign(cnf, +assignment), assignments))
    {
        return true;
    }

    assignments.back() = -assignment;

    if (SimpleDPLL(cnf_assign(cnf, -assignment), assignments))
    {
        return true;
    }

    assignments.pop_back();

    return false;
}

int main()
{
    vector<vector<long>> cnf = ReadDimacsCNF(cin);

    std::vector<long> assignments;

    bool sat = SimpleDPLL(cnf, assignments);

    if (sat)
    {
        cout << "SAT " << endl;
        for (const long & assignment : assignments)
        {
            cout << assignment << endl;
        }
    }
    else
    {
        cout << "UNSAT" << endl;
    }

    return 0;
}


//////////////
// SidSAT.h //
//////////////

#include <iostream>
#include <vector>
#include <algorithm>

#include "ReadDimacsCNF.h"

// PRECONDITIONING
//
// A variable can only occur once in a clause, either as a positive or as a negative literal.
//
// THESE RULES DO NOT CHANGE THE SOLUTION SET:
//
//   - If a clause A is a subset of a clause B, clause B can be discarded, since A -> B.
//   - If v=a leads to UNSAT, we can conclude than v == -a.
//   - In particular, if a clause has a single literal, the value of that variable is determined.
//
// UNFORCED ASSIGNMENT (satisfiability not affected, but the solution set may become smaller).
//
//     If a variable occurs only as a positive (or negative) value, it can be given that value and discarded.
//
//     More generally, if we can find an assignment to a SUBSET of the variables that satisfies all clauses where the variables appear,
//     we can assume that assignment (and, thus, get rid of those clauses).
//
// INVARIANT
//
// A non-trivial clause has at minimum 2 literals.

using namespace std;

void print_cnf(const vector<vector<int>> & cnf)
{
    for (const vector<int> & clause : cnf)
    {
        cout << "clause";
        for (const int & literal : clause)
        {
            cout << " " << literal;
        }
        cout << endl;
    }
}

vector<vector<int>> cnf_assign(const vector<vector<int>> & cnf, const int assignment)
{
    vector<vector<int>> new_cnf;

    for (const vector<int> & clause : cnf)
    {
        if (find(clause.begin(), clause.end(), assignment) != clause.end())
        {
            // This clause is satisfied by the assignment.
            // We will not include it in the new_cnf.
            continue;
        }

        // Make a new clause where remaining instances of the the literal (inverted w.r.t. assignment) are removed.
        vector<int> new_clause;

        for (const int & literal : clause)
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

bool SimpleDPLL(const vector<vector<int>> & cnf, vector<int> & assignments)
{
    if (cnf.empty())
    {
        return true;
    }

    if (cnf.front().empty())
    {
        // An empty clause is not satisfiable.

        if (false)
        {
            cout << "conflict: { ";
            for (unsigned i = 0; i < assignments.size(); ++i)
            {
                if (i != 0)
                {
                    cout << ", ";
                }
                cout << assignments[i];
            }
            cout << " }" << endl;
        }

        return false;
    }

    const int assignment = cnf.front().front(); // variable number of first variable of first clause.

    // Try the assignment (this will make a CNF that no longer has the variable; the first clause is guaranteed to be satisfied).

    assignments.push_back(+assignment);

    if (SimpleDPLL(cnf_assign(cnf, +assignment), assignments))
    {
        return true;
    }

    // Try the inverse assignment (this will make a CNF that no longer has the variable).
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
    vector<vector<int>> cnf = ReadDimacsCNF(cin);

    std::vector<int> assignments;

    bool sat = SimpleDPLL(cnf, assignments);

    if (sat)
    {
        cout << "SAT: { ";

        for (unsigned i = 0; i < assignments.size(); ++i)
        {
            if (i != 0)
            {
                cout << ", ";
            }

            cout << assignments[i];
        }
        cout << " }" << endl;
    }
    else
    {
        cout << "UNSAT" << endl;
    }

    return 0;
}

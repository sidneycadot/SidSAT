
//////////////
// SidSAT.h //
//////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "ReadDimacsCNF.h"

// PRECONDITIONING
//
//   - A variable can only occur once in a clause, either as a positive or as a negative literal.
//     (If a veriable occurs both positively and negatively in a clause, the clause is guaranteed to be satisfied.)
//
// THESE RULES DO NOT CHANGE THE SOLUTION SET:
//
//   - If a clause A is a subset of a clause B, clause B can be discarded, since A -> B (subsumption)
//   - If we do a number of assignments (v1 = b1, v2 = b2, v3 = b3) and end up with an unsat system, we can conclude that (v1 != b1 OR v2 != b2 OR v3 != b3) -- a conflict clause.
//   - In particular, with a single variable, if v = a leads to UNSAT, we can conclude than v == -a.
//   - Single-clause implication: if a clause has a single literal, the value of that variable is determined.
//
// UNFORCED ASSIGNMENT (satisfiability not affected, but the solution set may become smaller).
//
//   - If we can find assignments to a subset of the variables that satisfy all clauses where those variables appear,
//     we can assume those assignments (and, thus, get rid of those clauses).
//   - As a specific case, if a single variable occurs only as a positive (or negative) literal,
//     it can be given the corresponding value and be discarded along with its clauses.
//
// INVARIANT
//
//   - A non-trivial clause has at minimum 2 literals.

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

// This is correct but very, very slow -- we don't even do unit propagation.
// Consider it a reference implementation for verifying the correctness of better algorithms.

bool SimpleDPLL(const vector<vector<int>> & cnf, vector<int> & assignments)
{
    if (cnf.empty())
    {
        // An AND of zero clauses is true, so the problem is solved now.
        return true;
    }

    if (cnf.front().empty())
    {
        // An empty clause (OR of zero literals) is false, and thus not satisfiable.
        // This means that the entire CNF is not satisfiable.

        if (false)
        {
            cout << "    conflict: { ";
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

    // We cannot immediately conclude SAT/UNSAT. We will have to branch.

    const int assignment = cnf.front().front(); // variable number of first variable of first clause.

    // Try the positive assignment.
    // This will make a CNF that no longer has the variable in any clause;
    //   furthermore, the first clause is guaranteed to be satisfied, and will vanish.

    assignments.push_back(+assignment);

    if (SimpleDPLL(cnf_assign(cnf, +assignment), assignments))
    {
        return true;
    }

    // The positive assignment didn't lead to a SAT solution.
    // Try the inverse assignment next.
    // The this will make a CNF that no longer has the variable in any clause.

    assignments.back() = -assignment;

    if (SimpleDPLL(cnf_assign(cnf, -assignment), assignments))
    {
        return true;
    }

    assignments.pop_back();

    return false;
}

void solve(const char * filename)
{
    ifstream f(filename);

    vector<vector<int>> cnf = ReadDimacsCNF(f);

    vector<int> assignments;

    bool sat = SimpleDPLL(cnf, assignments);

    if (sat)
    {
        cout << "Instance '" << filename << "' is SATISFIABLE." << endl;

        cout << "    solution: { ";
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
        cout << "Instance '" << filename << "' is UNSATISFIABLE." << endl;
    }
}

int main(int argc, char ** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        solve(argv[i]);
    }
    return 0;
}

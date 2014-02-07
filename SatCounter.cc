
#include <iostream>
#include <vector>

using namespace std;

struct clause
{
    clause() : p(0), n(0) {}
    clause(const unsigned & pi, const unsigned & ni) : p(pi), n(ni) {}

    unsigned p; // positive literals
    unsigned n; // negative literals
};

ostream & operator << (ostream & os, const clause & c)
{
    os << "(" << c.p << ", " << c.n << ")";
}

vector<clause> mk_clauses(unsigned nv)
{
    vector<clause> clauses;

    if (nv == 0)
    {
        clauses.push_back(clause(0,0));
    }
    else
    {
        vector<clause> parents = mk_clauses(nv - 1);
        for (const clause & pc : parents)
        {
            clauses.push_back(clause(pc.p * 2 + 0, pc.n * 2 + 0));
            clauses.push_back(clause(pc.p * 2 + 1, pc.n * 2 + 0));
            clauses.push_back(clause(pc.p * 2 + 0, pc.n * 2 + 1));
        }
    }

    return clauses;
}

void enumerate_cnf(const vector<clause> & cnf)
{
}

int main()
{
    int nv = 4;
    int nc = 3;

    vector<clause> clauses = mk_clauses(nv);

    enumerate_cnf(clauses, nc);
}

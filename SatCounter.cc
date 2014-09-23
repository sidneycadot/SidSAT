
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
    return os;
}

inline bool is_subclause_of(const clause & x, const clause & y)
{
    // We return "true" iff clause x is a subclause of clause y.
    // This is to say: clause x does not have literals that do not occur in clause y.

    return ((x.p & ~y.p) == 0) && ((x.n & ~y.n) == 0);
}

unsigned weight(unsigned x)
{
    unsigned r = 0;
    while (x)
    {
        r += (x & 1);
        x >>= 1;
    }
    return r;
}

unsigned weight(const clause & x)
{
    // We return "true" iff clause x is a subclause of clause y.
    // This is to say: clause x does not have literals that do not occur in clause y.

    return weight(x.p) + weight(x.n);
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

vector<clause> filter_clauses(const vector<clause> clauses)
{
    vector<clause> new_clauses;

    for (const clause & c : clauses)
    {
        if (weight(c) >= 2)
        {
            new_clauses.push_back(c);
        }
    }

    return new_clauses;
}

struct Counts
{
    Counts() :
        all(0),
        after_subsumption(0)
    {
    }

    unsigned all;
    unsigned after_subsumption;
};

void enumerate_cnf(const vector<clause> & clauses, unsigned ci, unsigned first_index, vector<clause> & cnf, Counts & counts)
{
    const unsigned nc = cnf.size();

    if (ci != nc)
    {
        for (unsigned index = 0; index < first_index; ++index)
        {
            cnf[ci] = c;
            enumerate_cnf(clauses, ci + 1, cnf, counts);
        }
        return;
    }

    // We have a full CNF specification.
    ++counts.all;

    for (unsigned i = 0; i < nc; ++i)
    {
        for (unsigned j = i + 1; j < nc; ++j)
        {
            if (is_subclause_of(cnf[i], cnf[j]) || is_subclause_of(cnf[i], cnf[j]))
            {
                return;
            }
        }
    }

    ++counts.after_subsumption;
}

int main()
{
    int nv = 4;
    int nc = 3;

    vector<clause> clauses = mk_clauses(nv);

    clauses = filter_clauses(clauses);

    cout << "generated " << clauses.size() << " possible clauses." << endl;

    vector<clause> cnf(nc);
    Counts counts;
    enumerate_cnf(clauses, 0, cnf, counts);

    cout << "counts.all .................... : " << counts.all << endl;
    cout << "counts.after_subsumption ...... : " << counts.after_subsumption << endl;

    return 0;
}


//////////////////////
// ReadDimacsCNF.cc //
//////////////////////

#include <stdexcept>

#include "ReadDimacsCNF.h"

using namespace std;

static bool startswith(const string & line, const string & prefix)
{
    return line.substr(0, prefix.size()) == prefix;
}

static vector<string> tokenize(const string & line)
{
    const char * whitespace = " \t";

    vector<string> tokens;

    string::size_type tokenEndIndex = 0;

    for (;;)
    {
        string::size_type tokenStartIndex = line.find_first_not_of(whitespace, tokenEndIndex);

        if (tokenStartIndex == string::npos)
        {
            return tokens;
        }

        tokenEndIndex = line.find_first_of(whitespace, tokenStartIndex);

        if (tokenEndIndex == string::npos)
        {
            tokens.push_back(line.substr(tokenStartIndex));
            return tokens;
        }

        tokens.push_back(line.substr(tokenStartIndex, tokenEndIndex - tokenStartIndex));
    }
}

static bool parse_int(const string & s, int * value)
{
    try
    {
        const int v = stoi(s);

        if (to_string(v) != s)
        {
            throw logic_error(nullptr);
        }

        if (value != nullptr)
        {
            *value = v;
        }
    }
    catch (logic_error)
    {
        return false;
    }

    return true;
}

const vector<vector<int>> ReadDimacsCNF(istream & in)
{
    // We read the input line by line.

    string line;

    // Read preamble. It consists

    for (;;)
    {
        getline(in, line);

        if (startswith(line, "c"))
        {
            continue;
        }

        if (startswith(line, "p"))
        {
            break;
        }

        // We have an error in the preable.
        throw runtime_error("Error while reading preamble.");
    }

    vector<string> tokens = tokenize(line);

    int nv;
    int nc;

    bool ok = (tokens.size() == 4 && tokens[0] == "p" && tokens[1] == "cnf" && parse_int(tokens[2], &nv) && parse_int(tokens[3], &nc) && nv >= 0 && nc >= 0);

    if (!ok)
    {
        throw runtime_error("Unable to interpret 'p' line from preamble.");
    }

    vector<vector<int>> cnf;

    vector<int> clause;

    while (getline(in, line))
    {
        // The "%" sign as an end-of-clauses indicator is not specified by the DIMACS format specification.
        if (line == "%")
        {
            break;
        }

        // cerr << "CLAUSE LINE <" << line << ">" << endl;

        vector<string> tokens = tokenize(line);

        for (const string & token : tokens)
        {
            int value;
            if (!parse_int(token, & value))
            {
                throw runtime_error("Non-integer token found in clauses section.");
            }

            if (value == 0)
            {
                // This terminates the current clause.
                cnf.push_back(clause);
                clause.clear();
            }
            else
            {
                if (abs(value) > nv)
                {
                    throw runtime_error("Invalid literal found.");
                }

                clause.push_back(value);
            }
        }
    } // Walk the clause lines.

    if (!clause.empty())
    {
        // cerr << "WARNING: adding unterminated last clause." << endl;
        cnf.push_back(clause);
    }

    if (cnf.size() != static_cast<unsigned>(nc))
    {
        throw runtime_error("Invalid number of clauses.");
    }

    return cnf;
}

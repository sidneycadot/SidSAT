
//////////////////////
// ReadDimacsCNF.cc //
//////////////////////

#include <stdexcept>

#include <string>
#include "ReadDimacsCNF.h"

using namespace std;

static bool startswith(const string & line, const string & prefix)
{
    return line.substr(0, prefix.size()) == prefix;
}

static vector<string> tokenize(const string & line)
{
    const char * whitespace = " \t"; // accept space and tab characters as delimiters

    vector<string> tokens;

    string::size_type whitespaceStartIndex = 0;

    for (;;)
    {
        string::size_type tokenStartIndex = line.find_first_not_of(whitespace, whitespaceStartIndex);

        if (tokenStartIndex == string::npos)
        {
            // No next token is available -- we're done.
            break;
        }

        whitespaceStartIndex = line.find_first_of(whitespace, tokenStartIndex);

        if (whitespaceStartIndex == string::npos)
        {
            // No whitespace found; the current token is the last token.
            tokens.push_back(line.substr(tokenStartIndex));
            break;
        }

        // Push the current token
        tokens.push_back(line.substr(tokenStartIndex, whitespaceStartIndex - tokenStartIndex));
    }

    return tokens;
}

static bool parse_integer(const string & s, int * value)
{
    try
    {
        const int v = stoi(s); // interpret string as decimal integer.

        if (to_string(v) != s)
        {
            return false; // failure: not the canonical decimal representation of the integer.
        }

        if (value != nullptr)
        {
            *value = v;
        }

        return true; // succes
    }
    catch (...)
    {
        // any exception in the above indicates a conversion failure.
        return false;
    }
}

const vector<vector<int>> ReadDimacsCNF(istream & in)
{
    // We read the input line by line.

    string line;

    // Read the preamble.
    // It consists of zero or more comments (c), and ends with a problem specification (p).

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

    // The 'line' variable currently holds the "p" line.

    // Tokenize the 'p' line.

    vector<string> tokens = tokenize(line);

    // Verify validity of the tokenized 'p' line, and validate the variable and clause count.

    int nv; // number of variables.
    int nc; // number of clauses.

    const bool ok = (tokens.size() == 4 && tokens[0] == "p" && tokens[1] == "cnf" && parse_integer(tokens[2], &nv) && parse_integer(tokens[3], &nc) && nv >= 0 && nc >= 0);

    if (!ok)
    {
        throw runtime_error("Unable to interpret 'p' line from preamble.");
    }

    // Proceed to read the clauses.

    vector<vector<int>> cnf;

    vector<int> clause;

    while (getline(in, line))
    {
        // The "%" sign as an end-of-clauses indicator is not specified by the DIMACS format specification,
        // but it is used by many sample CNF files.

        if (line == "%")
        {
            break;
        }

        vector<string> tokens = tokenize(line);

        for (const string & token : tokens)
        {
            int literal;

            if (!parse_integer(token, &literal))
            {
                throw runtime_error("Non-integer token found in clauses section.");
            }

            if (literal == 0)
            {
                // This terminates the current clause.
                cnf.push_back(clause);
                clause.clear();
            }
            else
            {
                if (abs(literal) > nv)
                {
                    throw runtime_error("Invalid literal found.");
                }

                clause.push_back(literal);
            }
        } // Walk the tokens of the clause line.
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

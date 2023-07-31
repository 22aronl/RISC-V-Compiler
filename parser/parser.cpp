#include <iostream>
#include <cctype>

#include "group/expression.cpp"

class Tokenizer
{
    bool is_eof;
    char cur_ch;

    bool isalpha(char c)
    {
        return std::isalpha(c);
    }

    bool isalphanum(char c)
    {
        return std::isalnum(c);
    }

    bool isspace(char c)
    {
        return std::isspace(c) || c == '\t' || c == '\n' || c == '\r';
    }

    bool isoperator(char c)
    {
        return c == '/' || c == '*' || c == '%' || c == '+' || c == '-' || c == '&' ||
               c == '|' || c == '(' || c == ')' || c == '!' || c == '<' || c == '>';
    }

    bool isdec(char c)
    {
        return c == ':' || c == ',' || c == '(' || c == ')';
    }

    bool isterminating(char c)
    {
        return c == ';';
    }

    bool isnum(char c)
    {
        return false;
    }

    /**
     * haha what an L
     */
    bool issingle(char c)
    {
        return isoperator(c) || isdec(c) || isterminating(c);
    }

public:
    Tokenizer()
    {
        is_eof = false;
        if (!std::cin.get(cur_ch))
            is_eof = true;

        while (isspace(cur_ch))
        {
            if (!std::cin.get(cur_ch))
            {
                is_eof = true;
                break; // TODO: UPDATE
            }
        }
    }

    std::string getNextString()
    {
        std::string string;

        if (isalpha(cur_ch))
        {
            while (isalnum(cur_ch))
            {
                string += cur_ch;
                if (!std::cin.get(cur_ch))
                {
                    is_eof = true;
                    break;
                }
            }
        }
        else if (issingle(cur_ch))
        {
            string += cur_ch;

            if (!std::cin.get(cur_ch))
            {
                is_eof = true;
            }
        }
        else if (isnum(cur_ch))
        {
            // TODO: add method
        }
        else
        {
            // TODO: Add handling for invalid char
        }

        while (isspace(cur_ch)) // TODO: double removing whitespaces
        {
            if (!std::cin.get(cur_ch))
            {
                is_eof = true;
                break; // TODO: UPDATE
            }
        }

        return string;
    }

    bool getEOF()
    {
        return is_eof;
    }
};

class Parser
{

    Tokenizer tk;

    Expression* parseExpression() {
        int c = 0;
        return (Expression*) c;
    } 

};

int main()
{

    Tokenizer tokenizer;

    while (!tokenizer.getEOF()) // for (int i = 0; i < 10; i++) //
    {
        std::cout << tokenizer.getNextString() << " " << tokenizer.getEOF() << '\n';
    }

    return 0;
}

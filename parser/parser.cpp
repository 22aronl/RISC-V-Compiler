#include <iostream>
#include <cctype>

#include "group/expression.h"

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
            // TODO: Add support for removing comments
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
    std::string currentToken;

    void eat(std::string token)
    {
        if (currentToken == token)
        {
            currentToken = tk.getNextString();
        }
        else
        {
            // TODO: Throw an Error
        }
    }

    // checks if the currentToken is a number
    bool isNumber()
    {
        for (char c : currentToken)
        {
            if (!std::isdigit(c))
            {
                return false;
            }
        }
        return true;
    }

    Number *parseNumber(bool isNegative)
    {
        int value = std::stoi(currentToken);
        eat(currentToken);
        return new Number(isNegative ? -value : value);
    }

    Expression *parseValue()
    {
        if (currentToken == "(") // paranthese
        {
            eat("(");
            Expression *value = parseExpression();
            eat(")");
            return value;
        }
        else if (currentToken == "-")
        { // negative number //SEEMS useless
            eat("-");
            Expression *number = parseNumber(true);
            return number;
        }
        else if (isNumber())
        {
            return parseNumber(false);
        }
        else
        {
            std::string name = currentToken;
            eat(currentToken);
            if (currentToken == "(")
            {
                eat("(");
                std::vector<Expression *> args;
                while (currentToken != ")")
                {
                    args.push_back(parseExpression());
                    if (currentToken == ",")
                        eat(",");
                    else
                        //TODO: Throw an Error
                        break;
                }
                eat(")");
                return new Function_Call(name, args);
            }
            else if(currentToken == "[") {
                eat("[");
                Expression *index = parseExpression();
                eat("]");
                return new Array_Access(name, index);
            }
            else if(currentToken == ".") {
                eat(".");
                std::string member = currentToken;
                eat(currentToken);
                return new Struct_Access(name, member);
            }
            else
            {
                return new Variable(name);
            }
        }
    }

    Expression *parseNegative()
    {
        if(currentToken == "-") {
            eat("-");
            Expression *value = parseValue();
            return new Negation(value);
        }
        else {
            return parseValue();
        }
    }

    Expression *parseGeometric()
    {
        Expression *left = parseNegative();
        while (currentToken == "*" || currentToken == "/" || currentToken == "%")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseNegative();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    Expression *parseArithmetic()
    {
        Expression *left = parseGeometric();
        while (currentToken == "+" || currentToken == "-")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseGeometric();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    //>>, <<, >>>
    Expression *parseBitwiseShift()
    {
        Expression *left = parseArithmetic();
        while (currentToken == ">>" || currentToken == "<<" || currentToken == ">>>")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseArithmetic();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    //<, >, <=, >=
    Expression *parseInequality()
    {
        Expression *left = parseBitwiseShift();
        while (currentToken == "<" || currentToken == ">" || currentToken == "<=" || currentToken == ">=")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseBitwiseShift();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    //!=, ==
    Expression *parseEquality()
    {
        Expression *left = parseInequality();
        while (currentToken == "!=" || currentToken == "==")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseInequality();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    Expression *parseBitwiseAnd()
    {
        Expression *left = parseEquality();
        while (currentToken == "&")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseEquality();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    Expression *parseBitwiseOr()
    {
        Expression *left = parseBitwiseAnd();
        while (currentToken == "|")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseBitwiseAnd();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    Expression *parseLogicalAnd()
    {
        Expression *left = parseBitwiseOr();
        while (currentToken == "&&")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseBitwiseOr();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }

    Expression *parseLogicalOr()
    {
        Expression *left = parseLogicalAnd();
        while (currentToken == "||")
        {
            std::string op = currentToken;
            eat(currentToken);
            Expression *right = parseLogicalAnd();
            left = new Binary_Operation(left, right, op);
        }
        return left;
    }


public:
    Expression *parseExpression()
    {
        return parseLogicalOr();
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

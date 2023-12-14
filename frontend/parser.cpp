#include <iostream>
#include <cctype>

#include "group/expression.h"
#include "group/statement.h"
#include "group/global.h"

#include "../backend/compiler.h" //TODO: Remove this

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
               c == '|' || c == '(' || c == ')' || c == '!' || c == '<' || c == '>' ||
               c == '[' || c == ']' || c == '=' || c == '{' || c == '}' || c == '.';
    }

    bool isdec(char c)
    {
        return c == ':' || c == ',' || c == '(' || c == ')';
    }

    bool isterminating(char c)
    {
        return c == ';';
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

    bool isnum(char c)
    {
        return std::isdigit(c);
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

            if(cur_ch == '=' || cur_ch == '>' || cur_ch == '<') {
                string += cur_ch;
                if (!std::cin.get(cur_ch))
                {
                    is_eof = true;
                }
            }
        }
        else if (isnum(cur_ch)) // TODO: maybe issues with negative numbers
        {
            while (isnum(cur_ch))
            {
                string += cur_ch;
                if (!std::cin.get(cur_ch))
                {
                    is_eof = true;
                    break;
                }
            }
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
            std::cout << "Expected " << token << " but got " << currentToken << std::endl;
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

    Struct_Access *parseStructAccess()
    {
        std::string name = currentToken;
        eat(currentToken);
        if (currentToken == ".")
        {
            return new Struct_Access(new Variable(name), parseStructAccess());
        }
        else if(currentToken == "[") {
            eat("[");
            Expression *index = parseExpression();
            eat("]");
            if(currentToken == ".")
                return new Struct_Access(new Array_Access(name, index), parseStructAccess());
            else
                return new Struct_Access(new Array_Access(name, index));
        }
        else
        {
            return new Struct_Access(new Variable(name));
        }
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
                        // TODO: Throw an Error
                        break;
                }
                eat(")");
                return new Function_Call(name, args);
            }
            else if (currentToken == "[")
            {
                eat("[");
                Expression *index = parseExpression();
                eat("]");
                return new Array_Access(name, index);
            }
            else if (currentToken == ".")
            {
                eat(".");
                return new Struct_Access(new Variable(name), parseStructAccess());
            }
            else
            {
                return new Variable(name);
            }
        }
    }

    Expression *parseNegative()
    {
        if (currentToken == "-")
        {
            eat("-");
            Expression *value = parseValue();
            return new Negation(value);
        }
        else
        {
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
    Parser()
    {
        currentToken = tk.getNextString();
    }

    Expression *parseExpression()
    {
        return parseLogicalOr();
    }

    Statement *parseStatement()
    {
        if (currentToken == "for")
        {
            eat("for");
            eat("(");
            Statement *init = parseStatement();
            Expression *condition = parseExpression();
            eat(";");
            Statement *increment = parseStatement();
            eat(")");
            eat("{");
            Statement* body = parseMultipleStatements();
            eat("}");
            return new For_Loop(init, condition, increment, body);
        }
        else if (currentToken == "if")
        {
            eat("if");
            eat("(");
            Expression *condition = parseExpression();
            eat(")");
            eat("{");
            Statement* body = parseMultipleStatements();
            eat("}");
            if (currentToken == "else")
            {
                eat("else");
                eat("{");
                Statement* else_body = parseMultipleStatements();
                eat("}");
                return new If_Statement(condition, body, else_body);
            }
            else
            {
                return new If_Statement(condition, body);
            }
        }
        else if (currentToken == "new")
        {
            eat("new");
            std::string type = currentToken;
            eat(currentToken);
            std::string name = currentToken;
            eat(currentToken);
            if (currentToken == "[")
            {
                eat("[");
                Expression *size = parseExpression();
                eat("]");
                eat(";");
                return new Array_Declaration(type, name, size);
            }
            else
            {
                eat(";");
                return new Var_Declaration(type, name);
            }
        }
        else if (currentToken == "return")
        {
            eat("return");
            Expression *value = parseExpression();
            eat(";");
            return new Return_Statement(value);
        }
        else
        {
            std::string name = currentToken;
            eat(currentToken);

            Expression *leftOp;

            if (currentToken == ".")
            {
                eat(".");
                leftOp = new Struct_Access(new Variable(name), parseStructAccess());
            }
            else if (currentToken == "[")
            {
                eat("[");
                leftOp = new Array_Access(name, parseExpression());
                eat("]");
            }
            else
            {
                leftOp = new Variable(name);
            }

            std::string op = currentToken;
            eat(currentToken);

            Expression *rightOp = parseExpression();
            eat(";");

            return new Var_Assignment(leftOp, rightOp, op);
        }
    }

    Statement* parseMultipleStatements()
    {
        Statement* right;
        if(currentToken != "}") {
            right = parseStatement();
        }

        while (currentToken != "}")
        {
            Statement* left = parseStatement();
            right = new Sequence(right, left);
        }
        return right;
    }

    Global *parseGlobal()
    {
        if (currentToken == "volume")
        { // struct declaration
            eat("volume");
            std::string name = currentToken;
            eat(currentToken);
            eat("{");
            std::vector<Statement *> vars;
            while(currentToken != "}")
            {
                eat("new");
                std::string type = currentToken;
                eat(currentToken);
                std::string name = currentToken;
                eat(currentToken);
                if (currentToken == "[")
                {
                    eat("[");
                    Expression *size = parseExpression();
                    eat("]");
                    eat(";");
                    vars.push_back(new Array_Declaration(type, name, size));
                }
                else
                {
                    eat(";");
                    vars.push_back(new Var_Declaration(type, name));
                }
            }

            eat("}");
            eat(";");

            return new Struct_Declaration(name, vars);
        }
        else if (currentToken == "route")
        { // function declaration
            eat("route");
            std::string name = currentToken;
            eat(currentToken);
            eat("(");
            std::vector<Var_Declaration *> args;
            while (currentToken != ")")
            {
                std::string type = currentToken;
                eat(currentToken);
                std::string name = currentToken;
                eat(currentToken);
                args.push_back(new Var_Declaration(type, name));
                if (currentToken == ",")
                {
                    eat(",");
                }
                else
                {
                    break;
                }
            }
            eat(")");
            eat(":");
            std::string type = currentToken;
            eat(currentToken);

            eat("{");
            Statement* body = parseMultipleStatements();
            eat("}");

            return new Function_Declaration(type, name, args, body);
        }
        else if (currentToken == "new")
        { // variable declaration
            eat("new");
            std::string type = currentToken;
            eat(currentToken);
            std::string name = currentToken;
            eat(currentToken);

            if (currentToken == "[")
            {
                eat("[");
                Expression *size = parseExpression();
                eat("]");
                eat(";");
                return new Global_Array_Declaration(type, name, size);
            }
            else
            {
                eat("=");
                Expression *value = parseExpression();
                eat(";");
                return new Global_Var_Declaration(type, name, value);
            }
        }
        else if (currentToken == "structnew")
        {
            eat("structnew");
            std::string type = currentToken;
            eat(currentToken);
            std::string name = currentToken;
            eat(currentToken);
            eat(";");
            return new Struct_Var_Declaration(type, name);
        }

        std::cout << "UH OH" << currentToken << "\n";
        return nullptr; // TODO: Add Error Feedback
    }

    std::vector<Global *> parseFile()
    {
        std::vector<Global *> globals;
        while (!tk.getEOF())
        {
            globals.push_back(parseGlobal());
        }
        return globals;
    }
};

int main()
{
    Parser parser;

    Global* method = parser.parseGlobal();
    Environment* env = new Environment(nullptr);

    Function_Declaration* func = (Function_Declaration*)method;
    func->body->gen_icg(env, env->get_new_label(), env->get_new_label());
    env->print_icg();
    generate_basic_blocks(env);
    // parser.parseFile();
    return 0;
}

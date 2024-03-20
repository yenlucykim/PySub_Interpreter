#include "expevaluator.h"

std::vector<std::string> Expevaluator::infixToPostfix(std::vector<std::string>& infixString) {
    //bool isOperator(const std::string & token);
    //bool isUnaryOperator(const std::string & token);
    std::vector<std::string> postfixString;
    std::stack < std::string > stackVect;

    for (const std::string& token:infixString) {
        if (isdigit(token[0]) || isalpha(token[0])) {
            postfixString.push_back(token);
        }
        else if (token == "(") {
            stackVect.push(token); 
        }
        else if (token == ")") {
            while (!stackVect.empty() && stackVect.top() != "(") {
                postfixString.push_back(stackVect.top());
                stackVect.pop();
            }
            stackVect.pop();
        }
    
            //else {
            //    // Expression is not valid
            //    //std::cerr << "Invalid expression" << std::endl;
            //    //return {}; // Return an empty vector
            //    throw std::invalid_argument("Invalid expression");
            //}
        else {
            while (!stackVect.empty() && stackVect.top() != "(" && precOperator(stackVect.top()) >= precOperator(token))
            {
                postfixString.push_back(stackVect.top());
                stackVect.pop();
            }
            stackVect.push(token);
        }
    }

    while (!stackVect.empty()) {
        postfixString.push_back(stackVect.top());
        stackVect.pop();
    }

    return postfixString;
}
//----------------------------------------------------------------------------

int Expevaluator::evaluatePostfix(const std::vector<std::string>& postfixTokens) {

    std::stack<int> operandStack;

    //bool isOperator(const std::string & token);
    //bool isUnaryOperator(const std::string & token);
    for (const std::string& token : postfixTokens) {
        if (!isOperator(token) && !isUnaryOperator(token)) {
            if (isIdentifier(token)) {
                if (symbolTable.find(token) != symbolTable.end()) {
                    std::string value = retrieveVariableValue(token);
                    operandStack.push(std::stoi(value));
                    continue; 
                    //operandStack.push(std::stoi(symbolTable[token]));
                }
                else {
                    // Identifier not found in Symbol Table
                    std::cerr << "Error: Uninitialized identifier '" << token << "' used in the expression" << std::endl;
                    continue; 
                }
            }
            // If the token is an operand, push its value onto the stack
            int operand = std::stoi(token);
            operandStack.push(operand);
        }
    
        else if (isOperator(token)) {
            // If the token is a binary operator
            if (operandStack.size() < 2) {
                std::cerr << "Error: Not enough operands for operator '" << token << "'" << std::endl;
                return NULL; // Error: Insufficient operands
            }
            int operand2 = operandStack.top();
            operandStack.pop();
            int operand1 = operandStack.top();
            operandStack.pop();
            int result;

            if (token == "+") {
                result = operand1 + operand2;
            }
            else if (token == "-") {
                result = operand1 - operand2;
            }
            else if (token == "*") {
                result = operand1 * operand2;
            }
            else if (token == "/") {
                if (operand2 == 0) {
                    std::cerr << "Error: Division by zero" << std::endl;
                    return -1; // Error: Division by zero
                }
                result = operand1 / operand2;
            }
            else if (token == "%") {
                //division by 0
                if (operand2 == 0) {
                    std::cerr << "Error: Modulo by zero" << std::endl;
                    return -1;
                }
                result = operand1 % operand2;
            }
            else if (token == "and") {
                result = (operand1 & operand2);
            }
            else if (token == "or") {
                result = (operand1 | operand2);
            }
            else if (token == "<") {
                if (operand1 < operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            else if (token == "<=") {
                if (operand1 <= operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            else if (token == ">") {
                if (operand1 > operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            else if (token == ">=") {
                if (operand1 >= operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            else if (token == "!=") {
                if (operand1 != operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            else if (token == "==") {
                if (operand1 == operand2)
                {
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
            operandStack.push(result);
        }
        else if (isUnaryOperator(token)) {
            // If the token is a unary operator
            if (operandStack.empty()) {
                std::cerr << "Error: Not enough operands for unary operator" << std::endl;
                return -1; // Error: Insufficient operands
            }
            int operand = operandStack.top();
            operandStack.pop();
            int result = (operand == 0) ? 1 : 0;
            operandStack.push(result);
        }
    }
    return operandStack.top(); // The result of the expression
}

bool Expevaluator::isOperator(const std::string& token) {
    return (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "and" || token == "or" || token == "<" || token == "<=" || token == ">" || token == ">=" || token == "!=" || token == "==");
}


bool Expevaluator::isIdentifier(const std::string& token) {
    // Check if the token is a valid identifier
 //identifiers are var/ combination of vars 
    if (token.empty() || !isalpha(token[0]) && token[0] != '_') {
        // Identifier must start with a letter or underscore
        return false;
    }
    for (char c : token.substr(1)) {
        if(!isalnum(c) && c != '_') {
            // Subsequent characters can be letters, digits, or underscores
            return false;
        }
    }

    return true;
}
bool Expevaluator::isUnaryOperator(const std::string& token)
{
    return token == "not";
}
//----------------------------------------------------------------------------
Expevaluator::Expevaluator() {
    symbolTable.clear();
}

//----------------------------------------------------------------------------
int Expevaluator::precOperator(const std::string& s) {
    if (s == "or")
        return 0;
    else if (s == "and")
        return 1;
    else if (s == "not")
        return 2;
    else if (s == "<" || s == "<=" || s == ">" || s == ">=" || s == "!=" || s == "==")
        return 3;
    else if (s == "+" || s == "-")
        return 4;
    else if (s == "*" || s == "/" || s == "%")
        return 5;
    else
        return -1;
};
//----------------------------------------------------------------------------

void Expevaluator::storeVariable(const std::string& variable, const std::string& value) {
    // Check if the variable name is valid
    if (!isIdentifier(variable)) {
        std::cerr << "Error: Invalid variable name '" << variable << "'" << std::endl;
        // Handle the error or return if needed
        std::cout << "not able to store variable\n"; 
        return;
    }
    symbolTable[variable] = value;
}

std::string Expevaluator::retrieveVariableValue(const std::string& variable) {
    //find variable
    if (symbolTable.find(variable) != symbolTable.end()) {
        return symbolTable[variable];
    }
    else {
        // Variable not found
        return "This variable is not found.";
        //throw std::invalid_argument("Variable '" + variable + "' not found\n"); 
    }
}

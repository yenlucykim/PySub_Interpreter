#include <iostream>
#include <iomanip>
#include <cctype>
#include "lexanalyzer.h"
#include "interface.h"

std::string LexicalAnalyzer::enumToString(categoryType c) {
    if (c == categoryType::KEYWORD) return "KEYWORD";
    else if (c == categoryType::IDENTIFIER) return "IDENTIFIER";
    else if (c == categoryType::STRING_LITERAL) return "STRING_LITERAL";
    else if (c == categoryType::NUMERIC_LITERAL) return "NUMERIC_LITERAL";
    else if (c == categoryType::ASSIGNMENT_OP) return "ASSIGNMENT_OP";
    else if (c == categoryType::ARITH_OP) return "ARITH_OP";
    else if (c == categoryType::LOGICAL_OP) return "LOGICAL_OP";
    else if (c == categoryType::RELATIONAL_OP) return "RELATIONAL_OP";
    else if (c == categoryType::LEFT_PAREN) return "LEFT_PAREN";
    else if (c == categoryType::RIGHT_PAREN) return "RIGHT_PAREN";
    else if (c == categoryType::COLON) return "COLON";
    else if (c == categoryType::COMMA) return "COMMA";
    else if (c == categoryType::COMMENT) return "COMMENT";
    else if (c == categoryType::INDENT) return "INDENT";
    else return "UNKNOWN";
}
//----------------------------------------------------------------------------

bool LexicalAnalyzer::isUnderscore(char c) {
    return c == '_';
}

bool LexicalAnalyzer::isIdentifier(const std::string& string) {
// Check if the token is a valid identifier
//identifiers are var/ combination of vars 
    if (string.empty() || !isalpha(string[0]) && string[0] != '_') {
        // Identifier must start with a letter or underscore
        return false;
    }
    for (char c : string.substr(1)) {
        if (!isalnum(c) && c != '_') {
            // Subsequent characters can be letters, digits, or underscores
            return false;
        }
    }
}

bool LexicalAnalyzer::isLogicalOperator(const std::string& word) {
    std::vector<std::string> logical_op = { "and", "or", "not" };
    for (auto& op : logical_op) {
        if (word == op) { return true; }
    }
    return false;
}

bool LexicalAnalyzer::isKeyword(const std::string& word) {
    std::vector<std::string> keywords = { "print", "if", "elif", "else", "while", "input", "int" };
    for (auto& keyword : keywords) {
        if (word == keyword) { return true; }
    }
    return false;
}

bool LexicalAnalyzer::isRelationalOperator(char c1, char c2) {
    std::vector<std::string> relational_op = { "<", ">", "<=", ">=", "==", "!=" };
    for (auto& op : relational_op) {
        if (c1 == op[0] && c2 == op[1]) { return true; }
    }
    return false;
}

bool LexicalAnalyzer::isArithmeticOperator(char c) {
    std::vector<char> arithmetic_op = { '+', '-', '*', '/', '%' };
    for (auto op : arithmetic_op) {
        if (c == op) { return true; }
    }
    return false;
}
//----------------------------------------------------------------------------

LexicalAnalyzer::tokenLineType LexicalAnalyzer::readTokens(const std::string& programLine) {
    tokenLineType tokenLine;
    categoryType tokenCategory = categoryType::UNKNOWN;

    for (int i = 0; i < programLine.size(); ++i) {
        std::string tokenValue{};
        char c = programLine[i];

        //if c is a digit
        if (isdigit(c)) {
            tokenValue += c;
            while (isdigit(programLine[i + 1])) {
                i++;
                tokenValue += programLine[i];
            }
            tokenCategory = categoryType::NUMERIC_LITERAL;
        }
        //if c is a letter 
        else if (isalpha(c)||isUnderscore(c)) {
            tokenValue += c;

            while (i + 1 < programLine.size() && isIdentifier(tokenValue + programLine[i + 1])) {
                tokenValue += programLine[++i]; 
            }                

            //determine category type
            if (isKeyword(tokenValue)) {
                tokenCategory = categoryType::KEYWORD;
            }
            else if (isLogicalOperator(tokenValue)) {
                tokenCategory = categoryType::LOGICAL_OP;
            }
            else {
                tokenCategory = categoryType::IDENTIFIER;
            }
        }
        //if c is a string literal 
        else if (c == '"' || c == '\'') {
            tokenValue += c;
            while (c != programLine[i + 1]) {
                ++i;
                tokenValue += programLine[i];
            }
            ++i;
            tokenValue += programLine[i];
            tokenCategory = categoryType::STRING_LITERAL;
        }
        //if c is a parenthesis, colon, comma
        else if (c == '(') {
            tokenValue = c;
            tokenCategory = categoryType::LEFT_PAREN;
        }
        else if (c == ')') {
            tokenValue = c;
            tokenCategory = categoryType::RIGHT_PAREN;
        }
        else if (c == ':') {
            tokenValue = c;
            tokenCategory = categoryType::COLON;
        }
        else if (c == ',') {
            tokenValue = c;
            tokenCategory = categoryType::COMMA;
        }
        //if c is a comment 
        else if (c == '#') {
            tokenValue = programLine.substr(i, programLine.size() - i);     //-i+1? 
            tokenCategory = categoryType::COMMENT;
            i = programLine.size();
        }

        //if c is a space, check indentation 
        else if (isspace(c)) {
            //if space is not at the beginning of line
            if (i != 0) { continue; }
            //otherwise, collect all spaces
            tokenValue += c;
            while (isspace(programLine[i + 1])) {
                ++i;
                tokenValue += programLine[i];
            }
            tokenCategory = categoryType::INDENT;
        }

        //if c is an operator
        else if (isArithmeticOperator(c)) {
            tokenValue = c;
            tokenCategory = categoryType::ARITH_OP;
        }
        else if (isRelationalOperator(c, programLine[i + 1])) {
            tokenValue += c;            //work? 
            tokenValue += programLine[i + 1];
            i++;
            tokenCategory = categoryType::RELATIONAL_OP;
        }
        else if (c == '=') {
            tokenValue = c;
            tokenCategory = categoryType::ASSIGNMENT_OP;
        }
        else {
            tokenValue = c;
            tokenCategory = categoryType::UNKNOWN;
        }

        //make pairs of value and category 
        pairType tokenPair = make_pair(tokenValue, tokenCategory);
        tokenLine.push_back(tokenPair);
    }
    return tokenLine;
}
//----------------------------------------------------------------------------

void LexicalAnalyzer::showTokens() {
    std::cout << "\t\t***** TOKEN INFORMATION *****" << std::endl;
    int tokenLineNumber{ 0 };
    for (auto& tokenLine : tokenInfo) {
        std::cout << "Line #" << tokenLineNumber << ":\n";
        printTokenLine(tokenLine);
        ++tokenLineNumber;
    }
}
//-------------------------------------------------------------

void LexicalAnalyzer::pushBack(std::vector<std::string>& lines) {
    for (std::string& line : lines) {
        tokenInfo.push_back(readTokens
        (line));
    }
}
//-------------------------------------------------------------

void LexicalAnalyzer::printTokenLine(const tokenLineType& tokenLine) {
    int tokenNumber{ 0 };
    for (auto& tokenPair : tokenLine) {
        std::cout << "Token[" << tokenNumber << "]:\t" << std::setw(25) <<std::left << tokenPair.first << "- "
            <<enumToString(tokenPair.second) << std::endl;
        std::cout << "------------------------------------------------------------\n";
        ++tokenNumber;
    }
}
//-------------------------------------------------------------

LexicalAnalyzer::LexicalAnalyzer() {
    tokenInfo.clear();
}

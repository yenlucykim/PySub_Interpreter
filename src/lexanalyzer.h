#pragma once
#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <utility>
#include <vector>
#include <string>

class LexicalAnalyzer {
public:
    friend class Expevaluator;

    enum class categoryType
    {
        KEYWORD,
        IDENTIFIER,
        STRING_LITERAL,
        NUMERIC_LITERAL,
        ASSIGNMENT_OP,
        ARITH_OP,
        LOGICAL_OP,
        RELATIONAL_OP,
        LEFT_PAREN,
        RIGHT_PAREN,
        COLON,
        COMMA,
        COMMENT,
        INDENT,
        UNKNOWN
    };

public:
    typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
    typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
    typedef std::pair<std::string, categoryType> pairType;
    tokenType tokenInfo;
    std::string enumToString(categoryType c);
    bool isUnderscore(char c);
    bool isIdentifier(const std::string& string);
    bool isLogicalOperator(const std::string& word);
    bool isKeyword(const std::string& word);
    bool isRelationalOperator(char c1, char c2);
    bool isArithmeticOperator(char c);

public:
    LexicalAnalyzer();
    tokenLineType readTokens(const std::string& programLine);
    void printTokenLine(const tokenLineType& tokenLine);
    void showTokens();
    const tokenType& getTokenLine() const { return tokenInfo; }
    void pushBack(std::vector<std::string>& lines);
};

#endif 

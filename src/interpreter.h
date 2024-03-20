#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <stack>
#include "expevaluator.h"
#include "interface.h"

class Interpreter {
public:
    void interpretLine(const LexicalAnalyzer::tokenLineType& tokenLine/*, const std::string& command)*/);
    Expevaluator expEvaluation;
    Interface interfaceOb;
    void showVariables(Expevaluator& expEvaluation); 

private:
    bool containsComment(const LexicalAnalyzer::tokenLineType& tokens) const;
    bool containsPrint(const LexicalAnalyzer::tokenLineType& tokens) const;
    bool containsAssignment(const LexicalAnalyzer::tokenLineType& tokens) const;
    bool isIfStatement(const LexicalAnalyzer::tokenLineType& tokenLine) const;
    bool isWhileStatement(const LexicalAnalyzer::tokenLineType& tokenLine) const;
    void parsePrint(const LexicalAnalyzer::tokenLineType& tokenLine);
    void parseAssignment(const LexicalAnalyzer::tokenLineType& tokenLine);
    void parseInput(const LexicalAnalyzer::tokenLineType& tokenLine);
    void parseIfStatement(const LexicalAnalyzer::tokenLineType& tokenLine, std::vector<std::string>& lines);
    void parseWhileStatement(const LexicalAnalyzer::tokenLineType& tokenLine);

    bool evaluateCondition(std::string& condition);
    std::string extractSubstring(const std::string& str, size_t startIndex);
    void processIndentedLines(const std::vector<std::string>& lines, size_t lineIndex);
    std::string removeLeadingTab(const std::string& s);
    int trueCondition(const std::vector<std::string>& lines);
    void replaceNewLine(std::string& quotes); 
};

#endif
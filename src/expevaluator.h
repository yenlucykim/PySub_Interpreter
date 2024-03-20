#pragma once
#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include "lexanalyzer.h"
#include <map>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

class Expevaluator {
private:
	typedef std::map<std::string, std::string> symbolTableType;

public:
	symbolTableType symbolTable;

	std::vector<std::string> infixToPostfix(std::vector<std::string>& infixString);
	int evaluatePostfix(const std::vector<std::string>& postfixTokens);
	void storeVariable(const std::string& variable, const std::string& value);
	std::string retrieveVariableValue(const std::string& variable);
	Expevaluator();
	int precOperator(const std::string& s);
	bool isUnaryOperator(const std::string& token);
	bool isOperator(const std::string& token);
	bool isIdentifier(const std::string& token);
};

#endif
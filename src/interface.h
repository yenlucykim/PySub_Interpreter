#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include "expevaluator.h"

class Interface
{
private:
	typedef std::vector<std::string> programType;
	bool helpUtility;
	void displayPrompt();
	void processCommand(std::string& command);
	void commandDescription(std::string& command);
	void readFile(std::string& filename);
	std::string extractFilename(std::string& command);
	bool isExpression(const std::string& command);

public:
	Interface();
	void startInterface();
	int evaluateExpression(Expevaluator& expEvaluation, std::string& command);
	LexicalAnalyzer lexAnalysis;
	std::string userCommand{};
	programType programCode;

};

#endif
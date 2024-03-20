#include "interface.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>				

LexicalAnalyzer lexAnalysis;
Interpreter pysubi;

Interface::Interface()
{
	helpUtility = false;
}
//----------------------------------------------

void Interface::startInterface()
{
	std::cout << "PySUB Interpreter 1.0 on Windows (September 2023)\n"
		<< "Type 'help' for more information or 'quit' to exit.\n"
		<< "Type 'help(command)' with the specific command you want to execute inside the parentheses.\n"
		<< "Enter program lines or read file (<filename>.py) at command line interface.\n\n";

	while (true)
	{
		displayPrompt();
		std::getline(std::cin, Interface::userCommand);
		if (helpUtility)
		{
			commandDescription(Interface::userCommand);
		}
		else
		{
			if (Interface::userCommand.substr(0, 5) == "help(")
			{
				commandDescription(Interface::userCommand);
			}
			else
			{
				processCommand(Interface::userCommand);
			}
		}
	}
}
//----------------------------------------------

void Interface::displayPrompt()
{
	if (helpUtility)
	{
		std::cout << "help>";
	}
	else
	{
		std::cout << ">>> ";
	}
}
//-----------------------------------------------

void Interface::processCommand(std::string& command)
{
	std::string filename{};
	if (command == "quit" || command == "quit()")
	{
		exit(0);
	}
	else if (command == "help" || command == "help()")
	{
		helpUtility = true;
		std::cout << "Welcome to the help utility!\n"
			<< "* To exit and return to the interpreter, type 'exit'.\n"
			<< "* To get a list of all commands, type 'commands'.\n"
			<< "* To get a description of any command, type the command at the help> prompt.\n\n";
	}
	else if (command.substr(0, 4) == "read")
	{
		filename = extractFilename(command);
		if (filename != "") {
			readFile(filename);
		}
	}
	else if (command == "show" || command == "show()")
	{
		if (programCode.empty())
		{
			std::cout << "ERROR: This file is empty.\n";
		}
		int lineNumber{ 1 };
		for (std::string& line : programCode) {
			std::cout << "[" << lineNumber << "] " << line << "\n";
			++lineNumber;
		}
		std::cout << std::endl;
	}
	else if (command == "show(tokens)") {
		lexAnalysis.showTokens();
	}
	else if (command == "clear" || command == "clear()")
	{
		programCode.clear();
		lexAnalysis = LexicalAnalyzer();
		std::cout << "Cleared all lines.\n";
	}
	else if (command == "run" || command == "run()")
	{
		for (LexicalAnalyzer::tokenLineType& tokenLine : lexAnalysis.tokenInfo)
		{
			pysubi.interpretLine(tokenLine);
		}
		return; 
	}
	else if (command == "show(variables)")
	{
		pysubi.showVariables(pysubi.expEvaluation);
	}
	else if (isExpression(command))
		//expressions (if only int, paren, and operators), 
	{
		int result = evaluateExpression(pysubi.expEvaluation, command);
		std::cout << "Result: " << result << std::endl << std::endl;
	}
	else {
		LexicalAnalyzer::tokenLineType tokenLine = lexAnalysis.readTokens(command); 
		pysubi.interpretLine(tokenLine);

		if (pysubi.expEvaluation.isIdentifier(command)) {			
			std::string retrievedValue = pysubi.expEvaluation.retrieveVariableValue(command); 
			std::cout << retrievedValue << std::endl; 
			return; 
		}
	}
}
//-----------------------------------------------

bool Interface::isExpression(const std::string& command) {
	for (char c : command) {
		if (!(std::isdigit(c) || std::isspace(c) || lexAnalysis.isArithmeticOperator(c) ||
			c == '(' || c == ')')) {
			return false;
		}
	}
	/*char c = command.front();
	if (lexAnalysis.isLogicalOperator(command) || lexAnalysis.isRelationalOperator(c, c + 1)) {
		return true;
	}*/
	return true;
}
//------------------------------------------------

void Interface::commandDescription(std::string& command)
{
	if (command == "exit" || command == "exit()" || command == "help(exit)")
	{
		std::cout << "This command exits the help utility and returns to the interpreter.\n";
		helpUtility = false;
		std::cout << "Exiting help utility...\n\n";
	}
	else if (command == "commands" || command == "commands()" || command == "help(commands)")
	{
		std::cout << "Below is a list of commands. Enter any command at the prompt to get help.\n\n";
		std::cout << "quit	help	read	show	clear\n\n"
			<< "NOTE: All commands can also be written as functions, with '()'\n\n";
	}
	else if (command == "quit" || command == "quit()" || command == "help(quit)")
	{
		std::cout << "This command exits the command line interpreter.\n"
			<< "To exit the help utility and return to the interpreter, enter 'exit'\n";
	}
	else if (command == "help" || command == "help()" || command == "help(help)")
	{
		std::cout << "This command enters the help utility.\n";
	}
	else if (command == "read" || command == "read()" || command == "help(read)")
	{
		std::cout << "This command reads and stores lines of code from a PySub program file provided.\n";
	}
	else if (command == "show" || command == "show()" || command == "help(show)")
	{
		std::cout << "This command shows the lines of the program that are stored in program data structure.\n";
	}
	else if (command == "clear" || command == "clear()" || command == "help(clear)")
	{
		std::cout << "This command clears any lines that are stored in the program data structure.\n";
	}
	else
	{
		std::cout << "ERROR: This command does not exist in the help utility. Please type again.\n";
	}
	std::cout << std::endl; 
}
//-------------------------------------------------

void Interface::readFile(std::string& filename)
{
	programCode.clear();						//clear existing program lines 

	std::ifstream pyFile(filename);				//open file 
	if (!pyFile)
	{
		std::cout << "ERROR: Could not open file.\n\n";
		return;
	}
	std::string line;
	while (std::getline(pyFile, line))
	{
		programCode.push_back(line);
	}
	lexAnalysis.pushBack(programCode);
}
//-----------------------------------------------------

std::string Interface::extractFilename(std::string& command)
{
	size_t firstParen = command.find('(');
	size_t lastParen = command.find(')');

	if (firstParen != std::string::npos && lastParen != std::string::npos && lastParen > firstParen)
	{
		return command.substr(firstParen + 1, lastParen - firstParen - 1);
	}
	else
	{
		std::cout << "ERROR: Invalid format. Include filename in the proper format: read(<filename>.py)\n\n";
		return "";
	}
} 
//-----------------------------------------------------

int Interface::evaluateExpression(Expevaluator& expEvaluation, std::string& command) {

	LexicalAnalyzer::tokenLineType infixExpression = lexAnalysis.readTokens(command);
	std::vector<std::string> infixString;

	for (auto& pair : infixExpression) {
		infixString.push_back(pair.first);
	}

	std::vector<std::string> postfixExpression;
	
	try {
		postfixExpression = expEvaluation.infixToPostfix(infixString);
		
		//print each token in postfixExpression
	/*	for (auto& x : postfixExpression) {
			std::cout << x << '\t';
		}
		std::cout << '\n';*/

		int result = expEvaluation.evaluatePostfix(postfixExpression);
		return result;
	}
	catch (const std::invalid_argument& arg) {
		std::cerr << "Error : " << arg.what() << std::endl;
		return 0; 
	}
}
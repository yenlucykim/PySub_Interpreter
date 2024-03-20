#include <sstream>
#include "interpreter.h"
#include "interface.h"

void Interpreter::interpretLine(const LexicalAnalyzer::tokenLineType& tokenLine/*, const std::string& command*/) {
    if (containsComment(tokenLine)) {
        // Ignore comments
        return;
    }
    else if (containsPrint(tokenLine)) {
        parsePrint(tokenLine);
    }
    else if (containsAssignment(tokenLine)) {
        parseAssignment(tokenLine);
    }
    else if (isIfStatement(tokenLine)) {
        std::vector <std::string> lines;
        //if (command== "run" || command == "run()") {
        //    lines = interfaceOb.programCode;
        //}
        parseIfStatement(tokenLine, lines);
    }
    else if (isWhileStatement(tokenLine)) {
        parseWhileStatement(tokenLine);
        return;
    }
    else {
        //std::cout << "Error: Unrecognized statement type.\n";
        return;
    }
}
//----------------------------------------------------------------------------

bool Interpreter::isIfStatement(const LexicalAnalyzer::tokenLineType& tokenLine) const {
    if (tokenLine.size() >= 4 && tokenLine[0].first == "if") {
        // Check for the basic structure of "if" statement
        // Additional checks can be added for <expression> and indentation consistency
        return true;
    }
    //std::cerr << "Error: Incorrect syntax for 'if' statement.\n";
    return false;
}

bool Interpreter::containsComment(const LexicalAnalyzer::tokenLineType& tokenLine) const {
    for (const auto& token : tokenLine) {
        if (token.first[0] == '#') {
            return true;
        }
        else {
            /*if (tokens.empty() || tokens[0].first == "#") {
                return true;
            }*/
            return false;
        }
    }
}

bool Interpreter::isWhileStatement(const LexicalAnalyzer::tokenLineType& tokenLine) const {
    if (tokenLine.size() >= 4 && tokenLine[0].first == "while") {
        // Check if the basic structure of "while" statement is present
        // Additional checks can be added for <expression> and indentation consistency
        return true;
    }
    //std::cout << "Error: Incorrect syntax for 'while' statement.\n";
    return false;
}

bool Interpreter::containsPrint(const LexicalAnalyzer::tokenLineType& tokenLine) const {
    // Check if the tokens contain "print"

    if (tokenLine[0].first == "print") {
        return true;
    }
    return false;
}

bool Interpreter::containsAssignment(const LexicalAnalyzer::tokenLineType& tokenLine) const {
    // Check if the tokens contain an equal sign '='
    if (tokenLine.size() < 3 /*|| tokenLine[1].first != "=" || tokenLine[2].first!="="*/) {
        return false;
    }
    /*  if (tokenLine[1].first != "=" || tokenLine[2].first != "=") {
          return false;
      }*/
      // The first token should be an identifier
    if (tokenLine[0].second != LexicalAnalyzer::categoryType::IDENTIFIER) {
        if (tokenLine[0].second == LexicalAnalyzer::categoryType::KEYWORD) {
            return false;
        }
        std::string notIdentifier = tokenLine[0].first;
        //print out the tokenLine
        std::cout << "    ";
        for (const auto& pair : tokenLine) {
            std::cout << pair.first;
        }
        std::cout << std::endl << "    ";
        //print out the error 
        for (size_t i = 0; i < notIdentifier.size(); ++i) {
            // Mark the invalid character with '^'

            if (!(isalpha(notIdentifier[0])) && notIdentifier[0] != '_') {
                std::cout << "^";
            }
            else if (!isalnum(notIdentifier[i]) && notIdentifier[i] != '_') {
                std::cout << "^";
            }
            else {
                std::cout << " "; // Print a space for valid characters
            }
        }

        std::cout << "\nSyntaxError: Invalid variable name. Variable name must start with a letter or an underscore.\n";
        return false;
    }

    // The second token should be an assignment operator

    return true;
}
//----------------------------------------------------------------------------

void Interpreter::parseAssignment(const LexicalAnalyzer::tokenLineType& tokenLine) {
    // Check if it's an input statement
    if (tokenLine.size() >= 5 && tokenLine[2].first == "int" && tokenLine[3].first == "(" && tokenLine[4].first == "input") {
        parseInput(tokenLine);
        return;
    }

    // Check if it's a regular assignment
    if (tokenLine.size() >= 3 && tokenLine[1].first == "=") {
        // Extract identifier and expression
        std::string identifier = tokenLine[0].first;
        std::string expression;

        for (size_t i = 2; i < tokenLine.size(); ++i) {
            if (tokenLine[i].second == LexicalAnalyzer::categoryType::IDENTIFIER) {
                // Check if it's a variable and fetch its value from the symbol table
                std::string variableName = tokenLine[i].first;
                if (expEvaluation.symbolTable.find(variableName) != expEvaluation.symbolTable.end()) {
                    expression += expEvaluation.symbolTable[variableName];
                }
                else {
                    // Handle variable not found error
                    std::cerr << "Error: Variable '" << variableName << "' not found." << std::endl;
                    return;
                }
            }
            else {
                expression += tokenLine[i].first;
            }

            // Add a space if not the last token
            if (i < tokenLine.size() - 1) {
                expression += " ";
            }
        }

        //pass on to expevaluator for evaluation 
        int result = interfaceOb.evaluateExpression(expEvaluation, expression);
        expEvaluation.storeVariable(identifier, std::to_string(result));
    }
    else {
        // Invalid assignment statement
        std::cerr << "Error: Invalid assignment statement: " << std::endl;
    }
}

void Interpreter::parsePrint(const LexicalAnalyzer::tokenLineType& tokenLine) {
    // Check if the print statement has the correct format
    if (tokenLine.size() < 4 || tokenLine[1].second != LexicalAnalyzer::categoryType::LEFT_PAREN || tokenLine[tokenLine.size() - 1].second != LexicalAnalyzer::categoryType::RIGHT_PAREN) {
        std::cerr << "SyntaxError: Missing parentheses in the call to 'print'. Did you mean print(...)?" << std::endl << std::endl;
        return;
    }

    std::string quotedExpression;
    std::string expression;
    bool collectingExpression = true;
    std::deque<std::string> expressionDeque;

    for (size_t i = 2; i < tokenLine.size() - 1 && collectingExpression; ++i) {
        const std::string& tokenValue = tokenLine[i].first;

        // Check if the token is a string literal
        if (tokenValue.front() == '\"' || tokenValue.front() == '\'') {
            // Extract the content inside the quotes
            std::string quotes = tokenValue.substr(1, tokenValue.size() - 2);
            quotedExpression += quotes;
            replaceNewLine(quotedExpression);
            expressionDeque.push_back(quotedExpression);
            quotedExpression = "";
        }
        else if (tokenLine[i].second == LexicalAnalyzer::categoryType::RIGHT_PAREN) {
            collectingExpression = false;
        }
        else {
            if (tokenLine[i].second == LexicalAnalyzer::categoryType::COMMA) {
                continue;
            }
            expression += tokenValue;
            int result = interfaceOb.evaluateExpression(expEvaluation, expression);
            expressionDeque.push_back(std::to_string(result));
        }
    }

    //print out expressions
    for (const auto& expression : expressionDeque) {
        std::cout << expression << " ";
    }
    std::cout << std::endl;
}

void Interpreter::parseInput(const LexicalAnalyzer::tokenLineType& tokenLine) {
    // Extract identifier and string
    if (tokenLine[3].second != LexicalAnalyzer::categoryType::LEFT_PAREN || tokenLine[tokenLine.size() - 1].second != LexicalAnalyzer::categoryType::RIGHT_PAREN) {
        std::cerr << "Error: Missing parentheses in the call to 'input'." << std::endl;
        return;
    }
    std::string identifier = tokenLine[0].first;
    std::string promptString = tokenLine[6].first.substr(1, tokenLine[6].first.size() - 2);
    replaceNewLine(promptString);
    std::cout << promptString << " ";
    std::string input;
    std::getline(std::cin, input);

    //store input in symboltable
    expEvaluation.symbolTable[identifier] = input;
}

void Interpreter::parseIfStatement(const LexicalAnalyzer::tokenLineType& tokenLine, std::vector<std::string>& lines) {
    // Extract the expression from the "if" statement

    //test
    /*if (interfaceOb.userCommand == "run" || interfaceOb.userCommand == "run()") {
        bool conditionMet = trueCondition(lines);
        if (conditionMet) {
            int lineIndex = trueCondition(lines);
            processIndentedLines(lines, lineIndex);
            std::cout << std::endl;
            return;
        }
        else {
            std::cout << std::endl;
            return;
        }
    }*/

    std::string ifExpression;
    auto it = tokenLine.begin();

    while (it != tokenLine.end()) {
        ifExpression += it->first;
        ++it;
    }

    lines.push_back(ifExpression);
   
    bool additionalLines = true;
    std::string additionalLine;
    while (additionalLines) {
        std::cout << "...";
        std::getline(std::cin, additionalLine);

        if (additionalLine == "") {
            additionalLines = false;
            break;
        }
        lines.push_back(additionalLine);
    }
    int lineIndex = trueCondition(lines);
    processIndentedLines(lines, lineIndex);
    std::cout << std::endl;
    return;
}

void Interpreter::parseWhileStatement(const LexicalAnalyzer::tokenLineType& tokenLine) {
    // Extract the expression from the "while" statement
    std::vector<std::string> lines;
    std::string whileExpression;
    auto it = tokenLine.begin() + 1;

    while (it != tokenLine.end() - 1) {
        whileExpression += it->first;
        ++it;
    }

    lines.push_back(whileExpression);

    bool additionalLines = true;
    std::string additionalLine;
    while (additionalLines) {
        std::cout << "...";
        std::getline(std::cin, additionalLine);

        if (additionalLine == "") {
            additionalLines = false;
            break;
        }
        lines.push_back(additionalLine);
    }

    int startingLineIndex = 0;
    int endingLineIndex = startingLineIndex;

    // Find the index of the next non-indented line after the while loop
    for (int i = startingLineIndex + 1; i < lines.size(); ++i) {
        if (lines[i].front() != '\t') {
            endingLineIndex = i;
            break;
        }
    }

    // Evaluate the condition of the while loop
    bool whileCondition = evaluateCondition(whileExpression);

    // Interpret the lines inside the while loop if the condition is true
    while (whileCondition) {
        for (int i = 1; i < lines.size(); ++i) {
            std::string line = removeLeadingTab(lines[i]);
            LexicalAnalyzer::tokenLineType tokenLine = interfaceOb.lexAnalysis.readTokens(line);
            interpretLine(tokenLine);
        }
        // Re-evaluate the condition for the next iteration
        whileCondition = evaluateCondition(whileExpression);
    }
}
//----------------------------------------------------------------------------

bool Interpreter::evaluateCondition(std::string& condition) {
    bool conditionEvaluated = interfaceOb.evaluateExpression(expEvaluation, condition);

    if (conditionEvaluated) {
        return true;
    }
    else {
        return false;
    }
}

void Interpreter::showVariables(Expevaluator& expEvaluation)
{
    // Display variables and their values
    if (!expEvaluation.symbolTable.size() == 0) {
        std::cout << "Stored Variables:\n";
        for (const auto& var : expEvaluation.symbolTable)
        {
            std::cout << "    "<< var.first << " = " << var.second << std::endl;
        }
    }
    else {
        std::cout << "No Stored variables.\n";
    }

}

std::string Interpreter::extractSubstring(const std::string& str, size_t startIndex) {
    // Find the position of the delimiter after the startIndex
    size_t delimiterIndex = str.find(':', startIndex);

    // Extract substring from startIndex until the delimiter
    if (delimiterIndex != std::string::npos) {
        return str.substr(startIndex, delimiterIndex - startIndex);
    }
    else {
        // If delimiter is not found, extract until the end of the string
        return str.substr(startIndex);
    }
}

std::string Interpreter::removeLeadingTab(const std::string& s) {
    size_t pos = s.find_first_not_of('\t');
    return (pos == std::string::npos) ? "" : s.substr(pos);
}

void Interpreter::processIndentedLines(const std::vector<std::string>& lines, size_t lineIndex) {
    for (size_t i = lineIndex + 1; i < lines.size(); ++i) {
        if (lines[i].front() == '\t') {
            std::string line = removeLeadingTab(lines[i]);
            LexicalAnalyzer::tokenLineType tokenLine = interfaceOb.lexAnalysis.readTokens(line);
            interpretLine(tokenLine);
        }
        else {
            break; // Exit the loop if the line doesn't start with a tab
        }
    }
}

int Interpreter::trueCondition(const std::vector<std::string>& lines) {
    bool conditionMet = false;
    int lineIndex = 0;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].find("if") == 0) {
            std::string condition = extractSubstring(lines[i], 2); // Extract the condition

            conditionMet = evaluateCondition(condition);
            if (conditionMet) {
                lineIndex = i;
                return lineIndex;
            }
        }
        else if (lines[i].find("elif ") == 0) {
            if (!conditionMet) {
                std::string condition = extractSubstring(lines[i], 5); // Extract the condition

                conditionMet = evaluateCondition(condition);
                if (conditionMet) {
                    lineIndex = i;
                    return lineIndex;
                }
            }
        }
        else if (lines[i].find("else") == 0) {
            if (!conditionMet) {
                lineIndex = i;
                return lineIndex;
            }
        }
    }
}

void Interpreter::replaceNewLine(std::string& quotes) {
    //replace \n with a new line in quotedExpression 
    size_t pos = 0;
    while ((pos = quotes.find("\\n", pos)) != std::string::npos) {
        quotes.replace(pos, 2, "\n"); // Replace "\\n" with "\n"
        pos += 1; // Move past the replaced newline character
    }
}
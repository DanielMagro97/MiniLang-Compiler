#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "visitors/ASTtoXMLVisitor.h"
#include "visitors/semanticanalysis/SemanticAnalysis.h"
#include "visitors/interpreterexecution/InterpreterExecution.h"

using namespace std;

int Lexer::inputProgramIndex;
int Lexer::lineNumber;

int MiniLangI();

int main() {

    string input;
    do {
        cout << "Would you like to compile the code inside 'input', or Launch MiniLagI?\n"
                "Press 'i' and enter for the input file or 'm' and enter for MiniLangI" << endl;
        getline(cin, input);
        if (input[0] == 'm') {
            MiniLangI();
        } else if (input[0] != 'i') {
            cout << "Please only enter 'i' to compile the code inside 'input' or"
                    "'m' to launch the REPL MiniLangI" << endl;
        }
    } while (input[0] != 'm' && input[0] != 'i');

    // initialising the relative path and name of the input file
    string filename = "../MiniLangPrograms/input";

    // initialising the vector of chars which stores the input program
    Lexer::readProgram(filename);
    // initialise the array of states using the initialiseDFSAStates function
    States::initialiseDFSAStates();
    Lexer::inputProgramIndex = 0;
    Lexer::lineNumber = 1;

    Parser *parser = new Parser();
    vector<ASTNode*>* abstractSyntaxTree = parser->parse();

    ASTtoXMLVisitor asTtoXMLVisitor;
    asTtoXMLVisitor.visitAST(abstractSyntaxTree);
    std::cout << std::flush;

    SemanticAnalysis *semanticAnalysis = new SemanticAnalysis();
    semanticAnalysis->visitAST(abstractSyntaxTree);

    InterpreterExecution *interpreterExecution = new InterpreterExecution();
    interpreterExecution->visitAST(abstractSyntaxTree);

    delete parser;
    delete semanticAnalysis;
    delete interpreterExecution;

    return 0;
}


int MiniLangI() {
    Parser *parser = new Parser();
    SemanticAnalysis *semanticAnalysis = new SemanticAnalysis();
    InterpreterExecution *interpreterExecution = new InterpreterExecution();

    cout << "Please enter '#st', '#load', '#exit', or a MiniLang statement" << endl;
    string input;
    while (true) {
        cout << "MLi> ";
        getline(cin, input);

        if (input[0] == '#') {
            if (input == "#st") {
                for (int i = 0; i < semanticAnalysis->getSymbolTables().size(); i++) {
                    auto iterator = semanticAnalysis->getSymbolTables().at(i)->variables.begin();
                    while (iterator != semanticAnalysis->getSymbolTables().at(i)->variables.end()) {
                        cout << iterator->first << "\t" << iterator->second.toString() << endl;
                        iterator++;
                    }
                }
            } else if (input == "#load") {
                cout << "Please enter the file name you wish to load. The file must be "
                        "located inside the MiniLangPrograms folder." << endl;
                // initialising the relative path and name of the input file
                string filename;
                getline(cin, filename);


                Lexer::clearProgramCode();
                // initialising the vector of chars which stores the input program
                Lexer::readProgram("../MiniLangPrograms/" + filename);
                // initialise the array of states using the initialiseDFSAStates function
                States::initialiseDFSAStates();
                Lexer::inputProgramIndex = 0;
                Lexer::lineNumber = 1;

                parser->clearAST();
                vector<ASTNode*>* abstractSyntaxTree = parser->parse();
                semanticAnalysis->visitAST(abstractSyntaxTree);
                interpreterExecution->visitAST(abstractSyntaxTree);
            } else if (input == "#exit") {
                cout << "Exiting MiniLangI" << endl;
                break;
            }
        } else {
            Lexer::setProgramCode(input);
            Lexer::inputProgramIndex = 0;
            Lexer::lineNumber = 1;

            parser->clearAST();
            vector<ASTNode*>* abstractSyntaxTree = parser->parse();
            semanticAnalysis->visitAST(abstractSyntaxTree);
            interpreterExecution->visitAST(abstractSyntaxTree);
        }
    }

    delete parser;
    delete semanticAnalysis;
    delete interpreterExecution;

    exit(0);
}

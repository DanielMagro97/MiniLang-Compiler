#include <fstream>
#include <sstream>
#include <stack>

#include "Lexer.h"

using namespace std;

vector<char> Lexer::program;
State States::states[NO_OF_STATES];

void Lexer::readProgram(string filename) {

    ifstream is;
    is.open(filename);

    if (!is.good()) {
        ostringstream oss;
        oss << "Error opening file \"" << filename << "\"";
        throw ifstream::failure(oss.str());
    }

    char currentChar;

    // get the next character from the input file until the EOF is reached
    while (is.get(currentChar)) {
        // add the next character to the vector of chars storing the input
        Lexer::program.push_back(currentChar);
    }
    is.close();

    // add an EOF to the end of the vector of chars
    Lexer::program.push_back(EOF);
}

Token Lexer::getNextTok() {
    // stores the state which the lexer is currently in
    // initially set to the Start State
    int currentState = 0;
    // string which stores the lexeme read from the input file
    string lexeme;
    // stack of states which were visited while traversing the DFSA
    stack<int> visitedStates;
    // add the start state to the stack of visited states
    visitedStates.push(0);

    // do the following - until the next character of the input file does not result in an invalid transition
    // or until the entire input program has been read
    do {
        // retrieve the character to be parsed from the vector of chars storing the input
        char currentChar = Lexer::program.at(inputProgramIndex);
        // increment the index which indicates up to which character has been read from the vector of chars
        Lexer::inputProgramIndex++;

        // if the character being read is a whitespace or a tab, ignore it
        // unless a string literal is being read, in which case it is important to retain spaces
        if ( (currentChar == ' ' || currentChar == '\t') && currentState != 13) {
            continue;
        }
        // if the character being read is a new line, increment the line counter
        // and continue to the next iteration
        // unless a string literal is being read,
        // or a single line comment (//), in which case it is important to retain new lines
        if (currentChar == '\n' && !(currentState == 13 || currentState == 8)) {
            Lexer::lineNumber++;
            continue;
        }
        // if the current character is a \n and an inline comment was being read, it is still important to
        // increment the line number
        if (currentChar == '\n' && currentState == 8) {
            Lexer::lineNumber++;
        }


        // add the current character to the end of the lexeme
        lexeme += currentChar;
        // find what the next state should be by utilising the nextState method
        currentState = Lexer::nextState(currentState, currentChar);

        // if the DFSA is currently in state 15, and the lexeme is a keyword, then return a keyword token
        if (currentState == 15 && isKeyword(lexeme)) {
            Token token;
            token.lexeme = lexeme;
            if (lexeme == "and") {
                token.tokenIdentifier = TokenIdentifier::TOK_MultiplicativeOp;
            } else if (lexeme == "or") {
                token.tokenIdentifier = TokenIdentifier::TOK_AdditiveOp;
            } else if (lexeme == "true" || lexeme == "false") {
                token.tokenIdentifier = TokenIdentifier::TOK_BooleanLiteral;
            } else {
                token.tokenIdentifier = TokenIdentifier::TOK_Keyword;
            }
            return token;
        }

        // if the next reachable state is final, then it is not required to remember what states were followed
        // to reach this final state, as the rollback only needs to be up to this state at most
        if (States::states[currentState].finalState) {
            while (!visitedStates.empty()) {
                visitedStates.pop();
            }
        }
        // add the next state to the stack
        visitedStates.push(currentState);
    } while ( currentState != -1 && Lexer::inputProgramIndex < program.size() );

    // do while current state isn't final and until S00 is reached
    while (!States::states[currentState].finalState && !visitedStates.empty()) {
        // remove the non final state from the top of the stack
        visitedStates.pop();
        // delete the last character of the lexeme
        lexeme.pop_back();
        // move the index of the input program a character back
        Lexer::inputProgramIndex--;
        // set the current state to the state at the top of the stack of the visited states
        currentState = visitedStates.top();
    }

    // if the current state is a final state
    if (States::states[currentState].finalState) {
        // return the Token Identifier specified by the current state
        // and the current lexeme
        Token token;
        token.tokenIdentifier = States::states[currentState].tokenIdentifier;
        token.lexeme = lexeme;
        return token;
    } else {
        cout << "Lexical Error in line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

Token Lexer::getNextToken() {
    Token nextToken = getNextTok();
    // fetch the next token and only return a token if it is not an inline comment
    while (nextToken.tokenIdentifier == TOK_Comment) {
        nextToken = getNextTok();
    }
    return nextToken;
}

Token Lexer::peekNextToken() {
    int currentIndex = Lexer::inputProgramIndex;
    int currentLine = Lexer::lineNumber;

    Token peekNext = getNextToken();

    Lexer::inputProgramIndex = currentIndex;
    Lexer::lineNumber = currentLine;

    return peekNext;
}

int Lexer::nextState(int currentState, char currentChar){
    return transitionFunction[currentState][colAlphabet(currentChar)];
}

int Lexer::colAlphabet(char currentChar) {
    if (currentChar == '<' || currentChar == '>') return 0;
    else if (currentChar == '=') return 1;
    else if (currentChar == '!') return 2;
    else if (currentChar == '+' || currentChar == '-') return 3;
    else if (currentChar == '*') return 4;
    else if (currentChar == '/') return 5;
    else if (currentChar == '\n') return 7;
    else if (currentChar == '\"') return 8;
    else if (currentChar == '_' || isalpha(currentChar)) return 9;
    else if (isdigit(currentChar)) return 10;
    else if (currentChar == '.') return 11;
    else if (currentChar == '{' || currentChar == '}' || currentChar == '(' || currentChar == ')' ||
            currentChar == ',' || currentChar == ';' || currentChar == ':') return 12;
    else if (currentChar == EOF) return 13;
    else if ((currentChar > 32 && currentChar < 126) ||
             currentChar == '\t') return 6;
}

bool Lexer::isKeyword(string lexeme) {
    return (keywords.find(lexeme) != keywords.end());
}

#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "States.h"

using namespace std;

static set<string> keywords = {"real", "int", "bool", "string", "true", "false", "and", "or", "not",
                               "set", "var", "print", "return", "if", "else", "while", "def"};


class Lexer {

public:
    // integer which stores up to which character was read from the input file
    static int inputProgramIndex;
    // integer which stores what line is currently being read (how many '\n's have been encountered
    static int lineNumber;

    // method which turn the input program into a vector (array) of chars
    static void readProgram(string filename);
    // method which call getNextTok until the next token is not an inline comment
    static Token getNextToken();
    // method which returns the next token from the input program, however does not advance the
    // last character read (so running peekNextToken more than once will always return the same token, until
    // getNextToken is run which advances the program)
    static Token peekNextToken();

    // this method is only made use of by MiniLangI. This method clears the vector of chars
    static void clearProgramCode() {
        program.clear();
    }
    // this method is only made use of by MiniLangI. This method sets the vector of chars to whatever line of code
    // the user inputs in the command line
    static void setProgramCode(string programInput) {
        clearProgramCode();
        for (auto const &programChar : programInput) {
            program.push_back(programChar);
        }
        program.push_back(EOF);
    }

private:
    // vector of chars which will hold the stream of characters that make up the input program/file
    static vector<char> program;

    // method which continues reading the input program from where it last left off,
    // and returns the next Token from the input MiniLang program
    static Token getNextTok();
    // method which returns the next state given the current state and the current character
    // using the transition table
    static int nextState(int currentState, char currentChar);
    // method which returns what column of the transition table the character argument is referring to
    static int colAlphabet(char currentChar);
    // method which checks whether a lexeme is a keyword
    static bool isKeyword(string lexeme);
};
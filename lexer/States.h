#pragma once

#include "Tokens.h"

#define NO_OF_STATES 21 // 0 - 20

#define ALPHABET_SIZE 14
//<|>, =, !, +|-, *, /, [\x20-\x7E], \n, ", _|[A-Z]|[a-z], [0-9], ., {|}|(|)|,|;|:, EOF

struct State {
    int stateID;
    // 0 = rejecting state, 1 = accepting state
    bool finalState;
    // token generated
    TokenIdentifier tokenIdentifier;
};

// the state transition function, stored as a 2D Matrix of current states on the Y, and character type on the X
static constexpr int transitionFunction[NO_OF_STATES][ALPHABET_SIZE] = {
        // inputs                                                   state:
        {1, 3, 2, 5, 6, 7, -1, 0, 13, 15, 16, -1, 19, 20},          /*S00*/
        {-1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    /*S01*/
        {-1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    /*S02*/
        {-1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    /*S03*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S04*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S05*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S06*/
        {-1, -1, -1, -1, 10, 8, -1, -1, -1, -1, -1, -1, -1, -1},    /*S07*/
        {8, 8, 8, 8, 8, 8, 8, 9, 8, 8, 8, 8, 8, -1},                /*S08*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S09*/
        {10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 10, 10, -1},   /*S10*/
        {10, 10, 10, 10, 11, 12, 10, 10, 10, 10, 10, 10, 10, -1},   /*S11*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S12*/
        {13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 13, 13, -1},   /*S13*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S14*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, 15, 15, -1, -1, -1},   /*S15*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, 17, -1, -1},   /*S16*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 18, -1, -1, -1},   /*S17*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 18, -1, -1, -1},   /*S18*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   /*S19*/
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}    /*S20*/
};

class States {
public:
    // array of states which make up the DFSA
    static State states[NO_OF_STATES];


    static void initialiseDFSAStates(){
        std::set<int> finalStates = {1, 3, 4, 5, 6, 7, 9, 12, 14, 15, 16, 18, 19, 20};

        for (int i = 0; i < NO_OF_STATES; i++) {
            states[i].stateID = i;
            // if state i is included in the set of final states, then set it as a final state
            states[i].finalState = (finalStates.find(i) != finalStates.end());
        }

        // setting what TokenIdentifier every state in the DFSA returns
        states[0].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[1].tokenIdentifier = TokenIdentifier::TOK_RelationalOp;
        states[2].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[3].tokenIdentifier = TokenIdentifier::TOK_Equals;
        states[4].tokenIdentifier = TokenIdentifier::TOK_RelationalOp;
        states[5].tokenIdentifier = TokenIdentifier::TOK_AdditiveOp;
        states[6].tokenIdentifier = TokenIdentifier::TOK_MultiplicativeOp;
        states[7].tokenIdentifier = TokenIdentifier::TOK_MultiplicativeOp;
        states[8].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[9].tokenIdentifier = TokenIdentifier::TOK_Comment;
        states[10].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[11].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[12].tokenIdentifier = TokenIdentifier::TOK_Comment;
        states[13].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[14].tokenIdentifier = TokenIdentifier::TOK_StringLiteral;
        // state 15 might return TOK_KEYWORD, if it is detected that the lexeme is a keyword
        states[15].tokenIdentifier = TokenIdentifier::TOK_Identifier;
        states[16].tokenIdentifier = TokenIdentifier::TOK_IntegerLiteral;
        states[17].tokenIdentifier = TokenIdentifier::TOK_ERROR;
        states[18].tokenIdentifier = TokenIdentifier::TOK_RealLiteral;
        states[19].tokenIdentifier = TokenIdentifier::TOK_Punctuation;
        states[20].tokenIdentifier = TokenIdentifier::TOK_EOF;
    }
};
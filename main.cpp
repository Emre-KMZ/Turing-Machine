/*
this is a turing machine simulator
*/

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// data
typedef char TapeSymbol;
typedef int State;

enum Move {
    LEFT = -1,
    RIGHT = 1,
    STAY = 0
};

struct Instruction {
    State state; // current state
    TapeSymbol write_t;
    TapeSymbol write_f;
    Move move_t;
    Move move_f;
    int next_state_t;
    int next_state_f;
};

class Machine {
    vector<TapeSymbol> tape;
    int head;
    int state;
    unordered_map<State, Instruction> instructions;
};
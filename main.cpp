/*
this is a turing machine simulator
*/

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef char TapeSymbol;
typedef int State;

const TapeSymbol REST = '!';

enum Move {
    LEFT = -1,
    RIGHT = 1,
    STAY = 0
};

struct tuple_hash {
    template <class T1, class T2>
    size_t operator() (const tuple<T1, T2>& t) const {
        auto h1 = hash<T1>{}(get<0>(t));
        auto h2 = hash<T2>{}(get<1>(t));
        return h1 ^ h2;
    }
};

struct Instruction {
    State state; // current state
    TapeSymbol read; 
    TapeSymbol write; // write symbol if true
    Move move;
    State next_state;
};

class Machine {
private:
    vector<TapeSymbol> tape;
    int head;
    State state;
    unordered_map<tuple<State, TapeSymbol>, Instruction, tuple_hash> instructions;
public:

    Machine(string tape, State state) : head(0), state(state) {
        this->tape.resize(tape.size());
        for (int i = 0; i < tape.size(); i++) {
            this->tape[i] = tape[i];
        }
    }

    Machine() : head(0), state(0) {
        tape.resize(1);
        fill(tape.begin(), tape.end(), '#');
    }

    Machine(State state) : head(0), state(state) {
        tape.resize(1);
        fill(tape.begin(), tape.end(), '#');
    }

    Machine(vector<TapeSymbol> tape, State state) : tape(tape), head(0), state(state) {}

    Machine(vector<TapeSymbol> tape) : tape(tape), head(0), state(0) {}

    void add_instruction(Instruction instruction) {
        instructions[tuple(instruction.state, instruction.read)] = instruction;
    }

    bool execute_instruction() {
        if (instructions.find(tuple(state, tape[head])) == instructions.end() && instructions.find(tuple(state, REST)) == instructions.end()) { 
            cerr << "No instruction found for state " << state << " and symbol " << tape[head] << endl;
            return false;
        }
        Instruction instruction = instructions.find(tuple(state, tape[head])) != instructions.end() ? instructions[tuple(state, tape[head])] : instructions[tuple(state, REST)];
        tape[head] = instruction.write;
        head += instruction.move;
        state = instruction.next_state;

        if (state == -1){
            // cerr << "Machine halted" << endl;
            return false;
        }

        if (head < 0) {
            // not infinite to the left
            cerr << "Tape is infinite to the left" << endl;
            print_tape();
            return false;

        } else if (head >= tape.size()) {
            // infinite to the right
            tape.resize(tape.size() * 2);
            fill(tape.begin() + tape.size() / 2, tape.end(), '#');

        }
        return true;
    }

    void print_tape() {
        int r_lim = tape.size()-1;
        while (tape[r_lim] == '#') {
            r_lim--;
        }

        for (int i = 0; i <= r_lim; i++) {
            cout << tape[i] << " ";
        }
        cout << endl;
    }

    void take_instruction_set(vector<Instruction> instruction_set) {
        for (Instruction instruction : instruction_set) {
            add_instruction(instruction);
        }
    }

};

int main() {
    // incrementer
    vector<Instruction> instruction_set_incrementer;
    instruction_set_incrementer.push_back({0, '0', '0', RIGHT, 0});
    instruction_set_incrementer.push_back({0, '1', '1', RIGHT, 0});
    instruction_set_incrementer.push_back({0, '#', '#', LEFT, 1});
    instruction_set_incrementer.push_back({1, '0', '0', LEFT, 1});
    instruction_set_incrementer.push_back({1, '0', '1', STAY, -1});
    instruction_set_incrementer.push_back({1, '#', '1', STAY, -1});

    Machine machine("0000", 0);
    machine.take_instruction_set(instruction_set_incrementer);
    while (machine.execute_instruction()) {}
    cout << "for 0000: "; 
    machine.print_tape();

    Machine machine2("0001", 0);
    machine2.take_instruction_set(instruction_set_incrementer);
    while (machine2.execute_instruction()) {}
    cout << "for 0001: "; 
    machine2.print_tape();

    Machine machine3("0010", 0);
    machine3.take_instruction_set(instruction_set_incrementer);
    while (machine3.execute_instruction()) {}
    cout << "for 0010: "; 
    machine3.print_tape();



}
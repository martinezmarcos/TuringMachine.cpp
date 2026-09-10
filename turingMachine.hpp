#ifndef TURING_MACHINE_HPP
#define TURING_MACHINE_HPP

#include <string>
#include <unordered_map>
#include <deque>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

struct Action {
    std::string nextState;
    char writeSymbol;
    int moveDirection;
};

class TuringMachine {
private:
    std::deque<char> tape;
    int headPosition;
    std::string currentState;
    std::unordered_map<std::string, Action> transitions;

    std::string makeKey(const std::string& state, char symbol);

public:
    TuringMachine(std::string initialState, std::string initialTape);
    
    bool loadProgram(const std::string& filepath);
    void addTransition(std::string state, char readSym, std::string nextState, char writeSym, int move);
    bool step();
    void run(int delayMs = 200, int maxSteps = 1000);
    void printState();
};

#endif
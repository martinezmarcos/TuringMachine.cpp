#ifndef TURING_MACHINE_HPP
#define TURING_MACHINE_HPP

#include <string>
#include <unordered_map>
#include <deque>
#include <iostream>
#include <thread>
#include <chrono>

struct Action {
    std::string nextState;
    char writeSymbol;
    int moveDirection; // -1 (izquierda), 1 (derecha), 0 (quieto)
};

class TuringMachine {
private:
    std::deque<char> tape;     
    int headPosition;           // cabezal
    std::string currentState;   // estado actual
    std::unordered_map<std::string, Action> transitions; 


    std::string makeKey(const std::string& state, char symbol);

public:
  
    TuringMachine(std::string initialState, std::string initialTape);

  
    void addTransition(std::string state, char readSym, std::string nextState, char writeSym, int move);

    // un solo ciclo de reloj
    bool step();
    // 
    void run(int delayMs = 200, int maxSteps = 1000);
    void printState();
};

#endif
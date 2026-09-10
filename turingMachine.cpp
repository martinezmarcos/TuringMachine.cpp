#include "TuringMachine.hpp"

TuringMachine::TuringMachine(std::string initialState, std::string initialTape) {
    currentState = initialState;
    headPosition = 0;
    if (initialTape.empty()) {
        tape.push_back('_');
    } else {
        for (char c : initialTape) {
            tape.push_back(c);
        }
    }
}

std::string TuringMachine::makeKey(const std::string& state, char symbol) {
    return state + "|" + symbol; // "q0|1 ejemplo"
}

void TuringMachine::addTransition(std::string state, char readSym, std::string nextState, char writeSym, int move) {
    transitions[makeKey(state, readSym)] = {nextState, writeSym, move};
}

bool TuringMachine::step() {
    // memoria infinita simulada
    if (headPosition >= (int)tape.size()) {
        tape.push_back('_'); 
    } else if (headPosition < 0) {
        tape.push_front('_'); 
        headPosition = 0; // index
    }

    char currentSymbol = tape[headPosition];
    std::string key = makeKey(currentState, currentSymbol);

    if (transitions.find(key) != transitions.end()) {
        Action action = transitions[key];
        tape[headPosition] = action.writeSymbol;
        currentState = action.nextState;
        headPosition += action.moveDirection;
        
        return true; 
    } else {
        currentState = "HALT"; 
        return false; 
    }
}

void TuringMachine::printState() {
    std::cout << "Estado: [" << currentState << "]\nCinta:  ";
    for (int i = 0; i < (int)tape.size(); ++i) {
        std::cout << tape[i] << " ";
    }
    std::cout << "\n        ";   
    for (int i = 0; i < headPosition; ++i) {
        std::cout << "  "; 
    }
    std::cout << "^\n\n";
}

void TuringMachine::run(int delayMs, int maxSteps) {
    int steps = 0;
    printState();
    
    while (currentState != "HALT" && steps < maxSteps) {
        if (!step()) break; 
        printState();
        steps++;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    
    std::cout << "== MAQUINA DETENIDA == (Pasos: " << steps << ")\n";
}
#include "turingMachine.hpp" 
#include <cstdlib>

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
    return state + "|" + symbol;
}

void TuringMachine::addTransition(std::string state, char readSym, std::string nextState, char writeSym, int move) {
    transitions[makeKey(state, readSym)] = {nextState, writeSym, move};
}

bool TuringMachine::step() {
    if (headPosition >= (int)tape.size()) {
        tape.push_back('_'); 
    } else if (headPosition < 0) {
        tape.push_front('_'); 
        headPosition = 0; 
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
    system("cls"); 
    
    std::cout << "--- MAQUINA DE TURING ---\n\n";
    std::cout << "Estado actual: [" << currentState << "]\n";
    std::cout << "Cinta:  ";
    
    for (int i = 0; i < (int)tape.size(); ++i) {
        std::cout << tape[i] << " ";
    }
    std::cout << "\n        "; 
    
    for (int i = 0; i < headPosition; ++i) {
        std::cout << "  "; 
    }
    std::cout << "^\n\n";
}
bool TuringMachine::loadProgram(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filepath << "\n";
        return false;
    }

    std::string line;
    int ruleCount = 0;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string state, nextState;
        char readSym, writeSym;
        int move;

        if (iss >> state >> readSym >> nextState >> writeSym >> move) {
            addTransition(state, readSym, nextState, writeSym, move);
            ruleCount++;
        }
    }
    
    std::cout << "Programa cargado exitosamente (" << ruleCount << " reglas).\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    return true;
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
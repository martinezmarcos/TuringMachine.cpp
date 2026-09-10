#include "turingMachine.hpp"

int main() {
    // 23 en decimal
    std::string estadoInicial = "buscar_fin";
    std::string cintaInicial = "10111"; 
    TuringMachine tm(estadoInicial, cintaInicial);
    if (!tm.loadProgram("programas/incrementador.tm")) {
        return 1; 
    }
    // 250ms delay
    tm.run(250);

    return 0;
}
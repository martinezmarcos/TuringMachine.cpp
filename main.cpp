// inicilizar la cinta con 1011 (11 en decimal), si le suman 1 
// la cinta final se queda en 1100 (12 en decimal). 



#include "TuringMachine.hpp"

int main() {
    TuringMachine tm("buscar_fin", "1011");
    tm.addTransition("buscar_fin", '0', "buscar_fin", '0', 1); 
    tm.addTransition("buscar_fin", '1', "buscar_fin", '1', 1);
    tm.addTransition("buscar_fin", '_', "sumar", '_', -1);
    tm.addTransition("sumar", '1', "sumar", '0', -1);   
    tm.addTransition("sumar", '0', "HALT", '1', 0);
    // caso especial
    tm.addTransition("sumar", '_', "HALT", '1', 0);
    // 400 ms delay para ver como trabaja
    std::cout << "Iniciando Maquina de Turing - Incrementador Binario\n";
    std::cout << "===================================================\n";
    tm.run(400); 
    return 0;
}
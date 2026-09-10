# Turing Machine — Binary Incrementer

Implementación de una **Máquina de Turing en C++** capaz de incrementar en uno un número binario.

Dada una cinta inicial como:

```text
1011
```

la máquina produce:

```text
1100
```

Es decir:

```text
1011₂ + 1 = 1100₂
11₁₀ + 1 = 12₁₀
```

El proyecto simula la cinta, el cabezal de lectura/escritura, los estados y las transiciones de una Máquina de Turing.

## Funcionamiento

La máquina utiliza tres estados:

| Estado       | Descripción                                                            |
| ------------ | ---------------------------------------------------------------------- |
| `buscar_fin` | Recorre la cinta hacia la derecha hasta encontrar el final del número. |
| `sumar`      | Realiza el incremento binario desde el bit menos significativo.        |
| `HALT`       | Estado final que detiene la ejecución.                                 |

El algoritmo se divide en dos etapas.

### 1. Buscar el final

La máquina comienza en `buscar_fin` y avanza hacia la derecha mientras encuentra `0` o `1`.

```text
1011_
^
```

```text
1011_
    ^
```

Al encontrar `_`, reconoce el final del número y cambia al estado `sumar`, desplazándose una posición hacia la izquierda.

### 2. Incrementar el número

La suma de `1` en binario se realiza desde el bit menos significativo:

* Si encuentra `1`, escribe `0` y continúa hacia la izquierda.
* Si encuentra `0`, escribe `1` y finaliza.
* Si llega a `_`, significa que todos los bits eran `1`; escribe un nuevo `1` y finaliza.

Para `1011`:

```text
1011
   ↓
1010
  ↓
1000
 ↓
1100
```

Resultado:

```text
1100
```

## Transiciones

Las transiciones utilizadas son:

| Estado       | Lee | Escribe | Movimiento | Siguiente estado |
| ------------ | --- | ------- | ---------- | ---------------- |
| `buscar_fin` | `0` | `0`     | Derecha    | `buscar_fin`     |
| `buscar_fin` | `1` | `1`     | Derecha    | `buscar_fin`     |
| `buscar_fin` | `_` | `_`     | Izquierda  | `sumar`          |
| `sumar`      | `1` | `0`     | Izquierda  | `sumar`          |
| `sumar`      | `0` | `1`     | Quieto     | `HALT`           |
| `sumar`      | `_` | `1`     | Quieto     | `HALT`           |

La transición especial:

```text
(sumar, _) → (HALT, 1, quieto)
```

permite resolver casos como:

```text
1111 + 1 = 10000
```

## Arquitectura

El proyecto está compuesto por tres archivos:

```text
.
├── main.cpp
├── TuringMachine.hpp
└── TuringMachine.cpp
```

### `main.cpp`

Define la máquina concreta y sus transiciones.

Ejemplo:

```cpp
TuringMachine tm("buscar_fin", "1011");
```

También inicia la ejecución de la simulación:

```cpp
tm.run(400);
```

El parámetro `400` establece un retraso de 400 ms entre cada paso para poder visualizar el funcionamiento de la máquina.

### `TuringMachine.hpp`

Define la interfaz de la clase `TuringMachine` y la estructura `Action`.

La clase mantiene:

* La cinta.
* La posición del cabezal.
* El estado actual.
* El conjunto de transiciones.

### `TuringMachine.cpp`

Contiene la implementación de la máquina:

* Inicialización de la cinta.
* Gestión del cabezal.
* Búsqueda de transiciones.
* Escritura de símbolos.
* Cambio de estados.
* Movimiento sobre la cinta.
* Ejecución paso a paso.
* Visualización del estado actual.

## Representación de la cinta

La cinta se implementa mediante:

```cpp
std::deque<char>
```

El símbolo `_` representa una celda vacía de la cinta.

El uso de `std::deque` permite extender la cinta dinámicamente tanto hacia la derecha como hacia la izquierda, simulando una cinta potencialmente infinita.

## Compilación

El proyecto requiere un compilador compatible con C++.

Con `g++`:

```bash
g++ main.cpp TuringMachine.cpp -o turing_machine
```

En Linux:

```bash
./turing_machine
```

En Windows:

```powershell
.\turing_machine.exe
```

## Ejecución

Al iniciar el programa se muestra el estado de la máquina y el contenido de la cinta después de cada transición.

Ejemplo conceptual:

```text
Iniciando Maquina de Turing - Incrementador Binario
===================================================

Estado: [buscar_fin]
Cinta:  1 0 1 1 _
        ^

...

Estado: [sumar]
Cinta:  1 1 0 0 _
          ^

== MAQUINA DETENIDA == (Pasos: ...)
```

## Conceptos utilizados

* Máquinas de Turing
* Autómatas y máquinas de estados
* Simulación de una cinta de memoria
* Transiciones de estados
* Programación orientada a objetos en C++
* `std::deque`
* `std::unordered_map`
* Suma binaria y propagación de acarreo
* Ejecución paso a paso de algoritmos

## Licencia

Este proyecto puede utilizarse con fines educativos y de aprendizaje.

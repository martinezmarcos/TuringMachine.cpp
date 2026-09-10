# Turing Machine Simulator

Implementación de una **Máquina de Turing en C++** capaz de cargar y ejecutar programas definidos mediante archivos de texto `.tm`.

El simulador permite representar una máquina mediante:

* Una cinta de símbolos.
* Un cabezal de lectura y escritura.
* Estados.
* Transiciones.
* Programas externos definidos en archivos `.tm`.

Actualmente incluye dos programas:

* `incrementador.tm`: incrementa un número binario en uno.
* `inversor.tm`: invierte todos los bits de una cadena binaria.

---

## Estructura del proyecto

```text
.
├── main.cpp
├── turingMachine.hpp
├── turingMachine.cpp
└── programas
    ├── incrementador.tm
    └── inversor.tm
```

### `main.cpp`

Inicializa la Máquina de Turing, define la cinta inicial y carga el programa que será ejecutado.

Actualmente utiliza:

```cpp
std::string estadoInicial = "buscar_fin";
std::string cintaInicial = "10111";

TuringMachine tm(estadoInicial, cintaInicial);
```

La cinta inicial representa:

```text
10111₂ = 23₁₀
```

Luego se carga el programa:

```cpp
tm.loadProgram("programas/incrementador.tm");
```

Finalmente se ejecuta la máquina con un retraso de 250 ms entre cada paso:

```cpp
tm.run(250);
```

---

# Máquina de Turing

Una Máquina de Turing se puede representar mediante una cinta, un cabezal y un conjunto de reglas de transición.

En este proyecto, cada transición tiene la siguiente estructura:

```text
[EstadoActual] [Lee] [NuevoEstado] [Escribe] [Movimiento]
```

Por ejemplo:

```text
buscar_fin 1 buscar_fin 1 1
```

significa:

```text
Estado actual:  buscar_fin
Símbolo leído:  1
Nuevo estado:   buscar_fin
Símbolo escrito: 1
Movimiento:     derecha
```

Los valores posibles para el movimiento son:

```text
-1  izquierda
 0  quieto
 1  derecha
```

El símbolo `_` representa una celda vacía de la cinta.

---

# Arquitectura

## `TuringMachine`

La clase principal contiene el estado interno de la máquina:

```cpp
std::deque<char> tape;
int headPosition;
std::string currentState;
std::unordered_map<std::string, Action> transitions;
```

### Cinta

La cinta se implementa mediante:

```cpp
std::deque<char>
```

Esto permite extender dinámicamente la cinta hacia ambos lados cuando el cabezal alcanza sus límites.

### Cabezal

`headPosition` indica la posición actual del cabezal dentro de la cinta.

### Estado

`currentState` almacena el estado en el que se encuentra la máquina.

### Transiciones

Las reglas se almacenan en un `std::unordered_map`.

Cada transición se identifica mediante una clave construida a partir del estado y el símbolo leído:

```text
estado|símbolo
```

Por ejemplo:

```text
sumar|1
```

---

# Archivos de programa `.tm`

Una de las características principales del proyecto es que las reglas de transición no están escritas directamente en C++.

Se cargan desde archivos externos.

Esto permite modificar el comportamiento de la máquina sin recompilar la lógica de `TuringMachine`.

El formato de cada regla es:

```text
EstadoActual SímboloLeído NuevoEstado SímboloEscrito Movimiento
```

Las líneas que comienzan con `#` son comentarios y se ignoran.

---

# Incrementador Binario

El programa:

```text
programas/incrementador.tm
```

implementa un incrementador binario.

Su objetivo es realizar:

```text
n + 1
```

Por ejemplo:

```text
10111₂ + 1 = 11000₂
```

En decimal:

```text
23 + 1 = 24
```

## Programa

```text
# Fase 1: Ir al extremo derecho
buscar_fin 0 buscar_fin 0 1
buscar_fin 1 buscar_fin 1 1
buscar_fin _ sumar _ -1

# Fase 2: Sumar
sumar 1 sumar 0 -1
sumar 0 HALT 1 0
sumar _ HALT 1 0
```

## Funcionamiento

El algoritmo tiene dos fases.

### Fase 1 — Buscar el extremo derecho

Mientras encuentra `0` o `1`, la máquina avanza hacia la derecha:

```text
10111_
^
```

hasta llegar a:

```text
10111_
     ^
```

Cuando encuentra `_`, cambia al estado `sumar` y se mueve una posición hacia la izquierda.

### Fase 2 — Incrementar

La máquina comienza desde el bit menos significativo.

Si encuentra un `1`:

```text
1 + 1 = 10
```

escribe `0` y continúa hacia la izquierda:

```text
sumar 1 sumar 0 -1
```

Si encuentra un `0`:

```text
0 + 1 = 1
```

escribe `1` y termina:

```text
sumar 0 HALT 1 0
```

Existe además un caso especial para cuando todos los bits son `1`.

Si la máquina llega a `_` mientras está en `sumar`, escribe un `1`:

```text
sumar _ HALT 1 0
```

Esto permite resolver casos como:

```text
1111₂ + 1 = 10000₂
```

---

# Ejemplo de ejecución

La cinta inicial configurada en `main.cpp` es:

```text
10111
```

La máquina busca el final:

```text
10111_
     ^
```

Luego comienza a procesar los bits desde la derecha.

Como el último bit es `1`, lo convierte en `0` y continúa hacia la izquierda.

Finalmente:

```text
10111
   ↓
10110
  ↓
10100
 ↓
10000
↓
11000
```

Resultado:

```text
10111₂ + 1 = 11000₂
```

o:

```text
23₁₀ + 1 = 24₁₀
```

---

# Inversor de Bits

El programa:

```text
programas/inversor.tm
```

implementa un inversor de bits.

Su función es reemplazar:

```text
0 → 1
1 → 0
```

hasta alcanzar el final de la cadena.

El programa es:

```text
invertir 1 invertir 0 1
invertir 0 invertir 1 1
invertir _ HALT _ 0
```

Por ejemplo:

```text
Entrada:
10110

Salida:
01001
```

La máquina permanece en el estado `invertir` mientras encuentra bits y avanza hacia la derecha.

Cuando encuentra `_`, cambia a `HALT` y termina la ejecución.

---

# Carga de programas

La función:

```cpp
bool TuringMachine::loadProgram(const std::string& filepath)
```

abre el archivo `.tm` y procesa sus reglas línea por línea.

Para cada regla:

1. Lee el estado actual.
2. Lee el símbolo.
3. Lee el nuevo estado.
4. Lee el símbolo que debe escribirse.
5. Lee la dirección del movimiento.
6. Registra la transición en la máquina.

Por ejemplo:

```text
sumar 1 sumar 0 -1
```

se transforma internamente en una transición equivalente a:

```text
Estado: sumar
Lee: 1
Escribe: 0
Movimiento: izquierda
Siguiente estado: sumar
```

Al cargar el programa, el simulador muestra la cantidad de reglas encontradas:

```text
Programa cargado exitosamente (6 reglas).
```

---

# Ejecución paso a paso

La función:

```cpp
bool TuringMachine::step()
```

ejecuta una única transición.

En cada paso:

1. Comprueba la posición del cabezal.
2. Lee el símbolo actual.
3. Busca la transición correspondiente.
4. Escribe el nuevo símbolo.
5. Actualiza el estado.
6. Mueve el cabezal.

La función:

```cpp
void TuringMachine::run(int delayMs, int maxSteps)
```

repite estos pasos hasta que:

* La máquina llega al estado `HALT`.
* No existe una transición válida.
* Se alcanza el límite máximo de pasos.

El retraso permite observar visualmente cada transición.

En `main.cpp` se utilizan:

```cpp
tm.run(250);
```

por lo que existe una pausa de 250 ms entre cada paso.

---

# Visualización

El simulador muestra el estado actual y el contenido de la cinta:

```text
--- MAQUINA DE TURING ---

Estado actual: [sumar]
Cinta:  1 0 1 0 0 _

            ^
```

El carácter `^` indica la posición actual del cabezal.

La pantalla se limpia después de cada transición mediante:

```cpp
system("cls");
```

Por este motivo, la visualización actual está orientada a Windows.

---

# Compilación

El proyecto requiere un compilador de C++ compatible con `g++`.

Desde la carpeta raíz del proyecto, ejecutar:

```powershell
g++ main.cpp turingMachine.cpp -o turing.exe
```

Esto genera:

```text
turing.exe
```

---

# Ejecución

Una vez compilado, ejecutar:

```powershell
.\turing.exe
```

El programa cargará:

```text
programas/incrementador.tm
```

utilizando la cinta:

```text
10111
```

y comenzará la simulación.

Es importante ejecutar el programa desde la **carpeta raíz del proyecto**, ya que `main.cpp` utiliza la ruta relativa:

```text
programas/incrementador.tm
```

---

# Requisitos

* Windows
* C++17 o superior
* `g++` / MinGW
* Terminal compatible con PowerShell o CMD

No se utilizan librerías externas.

Las principales librerías estándar utilizadas son:

```cpp
<string>
<unordered_map>
<deque>
<iostream>
<thread>
<chrono>
<fstream>
<sstream>
<cstdlib>
```

---

# Conceptos

El proyecto implementa los siguientes conceptos:

* Máquinas de Turing
* Máquinas de estados
* Reglas de transición
* Simulación de cinta
* Lectura y escritura de símbolos
* Movimiento de cabezal
* Estados de ejecución
* Carga de programas desde archivos
* `std::deque`
* `std::unordered_map`
* Programación orientada a objetos en C++
* Simulación paso a paso

## Resultado

El proyecto permite definir diferentes Máquinas de Turing mediante archivos `.tm` y ejecutarlas utilizando un mismo motor de simulación en C++.

La separación entre el **simulador** y los **programas de transición** permite que la lógica de cada máquina se defina de forma independiente al código principal.

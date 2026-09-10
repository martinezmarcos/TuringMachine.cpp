# PROGRAMA: Incrementador Binario
# Formato: [EstadoActual] [Lee] [NuevoEstado] [Escribe] [Movimiento(-1,0,1)]

# Fase 1: Ir al extremo derecho
buscar_fin 0 buscar_fin 0 1
buscar_fin 1 buscar_fin 1 1
buscar_fin _ sumar _ -1

# Fase 2: Sumar 
sumar 1 sumar 0 -1
sumar 0 HALT 1 0
sumar _ HALT 1 0
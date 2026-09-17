Proyecto

Simulación de Ejecución de Procesos en un Sistema Operativo


Objetivo General: Desarrollar un programa en lenguaje C que simule la ejecución de procesos a nivel de sistema operativo, utilizando un archivo de entrada para configurar el sistema y los procesos, aplicando conceptos de manejo de colas, planificación y simulación de recursos.  

 

Objetivos Específicos: 
1. Implementar un simulador que lea y procese datos de configuración de un archivo de texto.  
2. Modelar la ejecución de procesos considerando parámetros como procesadores, hilos, quantum e iteraciones.  
3. Mostrar en pantalla el estado actual de la simulación, incluyendo la cola de procesos y el avance de la ejecución.  
4. Garantizar que el programa termine correctamente cuando no haya más procesos pendientes.  
5. Documentar el código y el funcionamiento del simulador de manera clara y profesional.  

 

Descripción del Proyecto:
Los estudiantes deberán construir un programa en lenguaje C que simule la ejecución de procesos en un sistema operativo. El programa puede ejecutarse en cualquier versión de Unix (e.g., Linux) o sobre Microsoft Windows.  

El simulador leerá un archivo de texto de entrada con la siguiente estructura:  


- Primera parte (configuración del sistema): 
  Procesadores [N]  
  Hilos [K]  

- Parte posterior (detalle de procesos):
  Cada línea describirá un proceso con el formato:  
  [ProcessId] | [ParentProcessId] | [ProgramCounter] | [Registros] | [Tamaño] | [Hilos] | [Quantum] | [Iteracion]  

  Donde:  
  - ProcessId: Número único del proceso.  
  - ParentProcessId: Número del proceso padre (puede ser 0 si es el proceso inicial).  
  - ProgramCounter: Nombre o identificador del proceso (e.g., cadena de texto).  
  - Registros: Cantidad de registros que utilizará el proceso.  
  - Tamaño: Cantidad en Bytes del proceso.  
  - Hilos: Cantidad de hilos asociados al proceso.  
  - Quantum: Cantidad de pulsos del procesador que utilizará para su ejecución en cada turno.  
  - Iteracion: Cantidad de veces que se ejecutará el proceso (determina el número de ciclos en la cola).  

Una vez cargada la información, el programa iniciará la simulación de la ejecución de los procesos de acuerdo con la configuración del sistema (número de procesadores y hilos).

 

Durante la simulación:  

- Se debe mostrar en pantalla el estado actual de la ejecución, incluyendo la cola de procesos pendientes, el proceso en ejecución, el quantum restante y cualquier otra métrica relevante (e.g., tiempo de CPU simulado).  
- Después de agotar el quantum asignado a un proceso, este regresará a la cola de ejecución.  
- La cantidad de ejecuciones de cada proceso estará definida por el parámetro "Iteracion".  
- El programa terminará su ejecución cuando no haya más procesos en la cola de ejecución.  

El simulador debe manejar la planificación de procesos de manera simple (e.g., round-robin o FIFO), considerando el número de procesadores y hilos para simular paralelismo si aplica.  

 

Requisitos Técnicos:  

Lenguaje de Programación: C estándar (compatible con compiladores como GCC o MSVC) o C++.
- Sistema Operativo: Cualquier versión de Unix (e.g., Linux, macOS) o Microsoft Windows. El programa debe ser portable y no depender de librerías específicas del SO.  
 

Entrada/Salida:
  - Leer el archivo de texto (e.g., "config.txt") usando funciones estándar de C (fopen, fscanf, etc.).  
  - Mostrar la simulación en consola (stdout), con actualizaciones periódicas (e.g., usando sleep o delay para simular tiempo).  

Estructuras de Datos: Usa estructuras (struct) para representar procesos y colas (e.g., listas enlazadas o arreglos dinámicos para la cola de procesos).  
- Simulación: Implementa un loop principal que simule ciclos de CPU, asignando quantum y manejando iteraciones.  
- Seguridad y Robustez: Maneja errores como archivos no encontrados o formatos inválidos, usando chequeos y mensajes de error.  
Pasos Recomendados: 

1. Leer y parsear el archivo de entrada para cargar la configuración y los procesos.  
2. Inicializar la cola de procesos y estructuras de datos.  
3. Simular la ejecución en un loop, mostrando el estado en cada ciclo.  
4. Actualizar el estado de los procesos (e.g., reducir quantum, mover a cola si quantum = 0).  
5. Terminar cuando la cola esté vacía.  

Entregables:

- Código fuente completo en C (archivos .c y .h si aplica).  
- Programa ejecutable (compilado para el SO elegido, e.g., .exe para Windows o binario para Linux).  
- Documentación (PDF o Word, 5-10 páginas): Incluye descripción del diseño, explicación del código, diagrama de flujo, y un ejemplo de archivo de entrada con salida simulada.  
- Archivo de prueba ("config.txt") con datos de ejemplo.  
- Copiar todos los artefactos en un directorio compartido en Google Drive antes de la fecha de entrega.  

Ejemplo de Archivo de Entrada ("config.txt"):

Procesadores 2  
Hilos 4  

1 | 0 | Proceso1 | 5 | 1024 | 2 | 10 | 3  
2 | 1 | Proceso2 | 4 | 512 | 1 | 8 | 2  
3 | 0 | Proceso3 | 6 | 2048 | 3 | 12 | 4  

 

Criterios de Evaluación:


- Correctitud del Código (50%): Funcionalidad del simulador, manejo de configuración y procesos.  

- Funcionamiento del aplicativo (50%)

 

Recursos Recomendados:  

- "The C Programming Language" de Kernighan y Ritchie para sintaxis básica.  
- Tutoriales en línea sobre listas enlazadas en C.  
- Compiladores: GCC para Unix/Linux, Visual Studio para Windows.  

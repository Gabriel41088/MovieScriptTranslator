# Java to JavaScript Translator

## Descripción

Java to JavaScript Translator es una aplicación de escritorio desarrollada en C++ y Qt que traduce un subconjunto del lenguaje Java a JavaScript. Incluye análisis léxico, sintáctico y semántico antes de realizar la traducción.

## Objetivos

- Identificar los tokens del código fuente.
- Detectar determinados errores sintácticos.
- Detectar algunos errores semánticos, como el uso de variables no declaradas.
- Convertir instrucciones compatibles de Java a JavaScript.
- Ofrecer una interfaz gráfica para introducir, analizar, traducir, cargar y guardar código.

## Tecnologías utilizadas

- C++17
- Qt 6 Widgets
- CMake
- Ninja
- MSYS2 UCRT64

## Fases del traductor

### 1. Análisis léxico

El analizador léxico lee el código fuente y lo divide en tokens, como identificadores, palabras reservadas, números, cadenas, operadores y símbolos.

También detecta caracteres no reconocidos y cadenas o comentarios sin cerrar.

Archivo principal: `src/lexer.cpp`

### 2. Análisis sintáctico

El analizador sintáctico revisa determinados aspectos de la estructura del código, como el equilibrio de paréntesis, llaves y corchetes, y algunos errores en las expresiones.

Archivo principal: `src/parser.cpp`

### 3. Análisis semántico

El analizador semántico realiza comprobaciones básicas relacionadas con las variables, incluyendo ciertos casos de uso de variables no declaradas.

Archivo principal: `src/semantic.cpp`

### 4. Traducción

Si las comprobaciones anteriores pasan, el traductor convierte las instrucciones compatibles al código JavaScript correspondiente.

Archivos principales: `src/translator.cpp` e `include/translator.h`

## Interfaz gráfica

La aplicación incluye:

- Editor de código Java.
- Área de salida para JavaScript.
- Botón para analizar y traducir.
- Botón para limpiar los editores.
- Opción para cargar archivos Java.
- Opción para guardar el resultado como archivo JavaScript.
- Mensajes de estado y notificaciones de errores.

## Estructura del proyecto

```text
MovieScriptTranslator/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── translator.h
│   ├── lexer.h
│   ├── parser.h
│   └── semantic.h
├── src/
│   ├── main.cpp
│   ├── translator.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   └── semantic.cpp
└── build/
```

## Compilación

Desde MSYS2 UCRT64, ejecutar:

```bash
cd ~/MovieScriptTranslator
cmake -S . -B build -G Ninja
cmake --build build
```

Para ejecutar la aplicación:

```bash
./build/MovieScript
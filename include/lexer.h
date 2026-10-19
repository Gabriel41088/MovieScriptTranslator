
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TipoToken
{
    Identificador,
    Numero,
    Cadena,
    PalabraReservada,
    Operador,
    Simbolo,
    Fin,
    Error
};

struct Token
{
    TipoToken tipo;
    std::string texto;
    int linea;
    int columna;
};

struct ResultadoLexico
{
    bool correcto;
    std::vector<Token> tokens;
    std::string mensaje;
};

ResultadoLexico analizarLexico(const std::string& codigo);

#endif

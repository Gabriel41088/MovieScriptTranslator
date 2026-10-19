#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <vector>

struct ResultadoSintactico
{
    bool correcto;
    std::string mensaje;
};

ResultadoSintactico analizarSintaxis(
    const std::vector<Token>& tokens
);

#endif
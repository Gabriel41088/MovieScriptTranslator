
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>

struct ResultadoTraduccion
{
    bool correcto;
    std::string codigo;
    std::string mensaje;
};

ResultadoTraduccion traducirMovieScript(
    const std::string& entrada
);

#endif

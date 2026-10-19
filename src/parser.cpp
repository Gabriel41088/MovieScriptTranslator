#include "parser.h"

#include <initializer_list>
#include <string>

namespace
{
    bool esSimbolo(const Token& token, const std::string& simbolo)
    {
        return token.tipo == TipoToken::Simbolo &&
               token.texto == simbolo;
    }

    bool esOperador(const Token& token, const std::string& operador)
    {
        return token.tipo == TipoToken::Operador &&
               token.texto == operador;
    }

    bool esPalabra(const Token& token, const std::string& palabra)
    {
        return token.texto == palabra;
    }

    ResultadoSintactico errorSintactico(
        const Token& token,
        const std::string& detalle)
    {
        return {
            false,
            "Error sintactico en linea " +
            std::to_string(token.linea) +
            ", columna " +
            std::to_string(token.columna) +
            ": " + detalle
        };
    }
}

ResultadoSintactico analizarSintaxis(const std::vector<Token>& tokens)
{
    if (tokens.empty())
    {
        return {false, "Error sintactico: no hay tokens para analizar."};
    }

    int parentesis = 0;
    int llaves = 0;
    int corchetes = 0;

    bool esperaOperando = false;
    bool ultimoFueOperador = false;

    for (std::size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];

        if (token.tipo == TipoToken::Fin)
            break;

        if (esSimbolo(token, "("))
            parentesis++;
        else if (esSimbolo(token, ")"))
        {
            parentesis--;

            if (parentesis < 0)
                return errorSintactico(token, "parentesis de cierre sin apertura.");
        }
        else if (esSimbolo(token, "{"))
            llaves++;
        else if (esSimbolo(token, "}"))
        {
            llaves--;

            if (llaves < 0)
                return errorSintactico(token, "llave de cierre sin apertura.");
        }
        else if (esSimbolo(token, "["))
            corchetes++;
        else if (esSimbolo(token, "]"))
        {
            corchetes--;

            if (corchetes < 0)
                return errorSintactico(token, "corchete de cierre sin apertura.");
        }

        if (token.tipo == TipoToken::Operador)
        {
            if (token.texto == "++" || token.texto == "--")
            {
                ultimoFueOperador = false;
                esperaOperando = false;
                continue;
            }

            if (token.texto == "!")
            {
                ultimoFueOperador = true;
                esperaOperando = true;
                continue;
            }

            if (ultimoFueOperador)
            {
                return errorSintactico(
                    token,
                    "hay dos operadores consecutivos o falta un operando."
                );
            }

            ultimoFueOperador = true;
            esperaOperando = true;
        }
        else if (token.tipo == TipoToken::Identificador ||
                 token.tipo == TipoToken::Numero ||
                 token.tipo == TipoToken::Cadena ||
                 token.tipo == TipoToken::PalabraReservada)
        {
            ultimoFueOperador = false;
            esperaOperando = false;
        }
        else if (esSimbolo(token, ";") ||
                 esSimbolo(token, ",") ||
                 esSimbolo(token, ")"))
        {
            if (esperaOperando && ultimoFueOperador)
            {
                return errorSintactico(
                    token,
                    "la expresion termina antes de completar el operador."
                );
            }
        }
    }

    const Token& ultimo = tokens.back();

    if (parentesis != 0)
        return errorSintactico(ultimo, "parentesis sin cerrar.");

    if (llaves != 0)
        return errorSintactico(ultimo, "llaves sin cerrar.");

    if (corchetes != 0)
        return errorSintactico(ultimo, "corchetes sin cerrar.");

    if (ultimoFueOperador && esperaOperando)
        return errorSintactico(ultimo, "la expresion termina con un operador.");

    return {true, "Analisis sintactico completado."};
}
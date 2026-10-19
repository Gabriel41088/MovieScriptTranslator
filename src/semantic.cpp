
#include "semantic.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace
{
    bool esTipo(const std::string& texto)
    {
        static const std::unordered_set<std::string> tipos = {
            "int", "double", "float", "long", "short",
            "byte", "char", "boolean", "String"
        };

        return tipos.count(texto) > 0;
    }

    bool esReservada(const std::string& texto)
    {
        static const std::unordered_set<std::string> reservadas = {
            "public", "private", "protected", "static", "void",
            "class", "if", "else", "while", "for", "return",
            "true", "false", "null", "new", "this", "super",
            "String", "System", "out", "println", "print",
            "main", "args"
        };

        return reservadas.count(texto) > 0;
    }
}

ResultadoSemantico analizarSemantica(
    const std::vector<Token>& tokens)
{
    std::unordered_map<std::string, std::string> variables;

    // Primera pasada: registrar declaraciones sencillas.
    for (std::size_t i = 0; i + 1 < tokens.size(); ++i)
    {
        if (tokens[i].tipo == TipoToken::Fin)
            break;

        if (tokens[i].tipo == TipoToken::PalabraReservada &&
            esTipo(tokens[i].texto) &&
            tokens[i + 1].tipo == TipoToken::Identificador)
        {
            variables[tokens[i + 1].texto] = tokens[i].texto;
        }
    }

    // Segunda pasada: buscar identificadores sin declarar.
    for (std::size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];

        if (token.tipo == TipoToken::Fin)
            break;

        if (token.tipo != TipoToken::Identificador)
            continue;

        // No revisar nombres que forman parte de declaraciones.
        if (i > 0 &&
            tokens[i - 1].tipo == TipoToken::PalabraReservada &&
            esTipo(tokens[i - 1].texto))
        {
            continue;
        }

        // Ignorar nombres de clases y métodos.
        if (i > 0 &&
            tokens[i - 1].texto == "class")
        {
            continue;
        }

        if (i + 1 < tokens.size() &&
            tokens[i + 1].texto == "(")
        {
            continue;
        }

        // Ignorar miembros después de un punto: System.out.println.
        if (i > 0 && tokens[i - 1].texto == ".")
            continue;

        if (esReservada(token.texto))
            continue;

        if (variables.find(token.texto) == variables.end())
        {
            return {
                false,
                "Error semantico en linea " +
                std::to_string(token.linea) +
                ", columna " +
                std::to_string(token.columna) +
                ": la variable '" + token.texto +
                "' no ha sido declarada."
            };
        }
    }

    return {
        true,
        "Analisis semantico completado."
    };
}

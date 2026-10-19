
#include "lexer.h"

#include <cctype>
#include <unordered_set>

namespace
{
    const std::unordered_set<std::string> palabrasReservadas = {
        "public", "private", "protected", "static", "void",
        "class", "int", "double", "float", "long", "short",
        "byte", "char", "boolean", "String", "if", "else",
        "while", "for", "return", "true", "false", "null",
        "new"
    };

    bool esInicioIdentificador(char c)
    {
        unsigned char u = static_cast<unsigned char>(c);
        return std::isalpha(u) || c == '_' || c == '$';
    }

    bool esParteIdentificador(char c)
    {
        unsigned char u = static_cast<unsigned char>(c);
        return std::isalnum(u) || c == '_' || c == '$';
    }

    void agregarToken(
        ResultadoLexico& resultado,
        TipoToken tipo,
        const std::string& texto,
        int linea,
        int columna)
    {
        resultado.tokens.push_back({tipo, texto, linea, columna});
    }
}

ResultadoLexico analizarLexico(const std::string& codigo)
{
    ResultadoLexico resultado;
    resultado.correcto = true;

    int i = 0;
    int linea = 1;
    int columna = 1;

    auto avanzar = [&]()
    {
        if (codigo[i] == '\n')
        {
            linea++;
            columna = 1;
        }
        else
        {
            columna++;
        }

        i++;
    };

    while (i < static_cast<int>(codigo.size()))
    {
        char c = codigo[i];

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            avanzar();
            continue;
        }

        // Comentarios de una linea.
        if (c == '/' && i + 1 < static_cast<int>(codigo.size())
            && codigo[i + 1] == '/')
        {
            while (i < static_cast<int>(codigo.size())
                   && codigo[i] != '\n')
            {
                avanzar();
            }
            continue;
        }

        // Comentarios de varias lineas.
        if (c == '/' && i + 1 < static_cast<int>(codigo.size())
            && codigo[i + 1] == '*')
        {
            int inicioLinea = linea;
            int inicioColumna = columna;

            avanzar();
            avanzar();

            bool cerrado = false;

            while (i < static_cast<int>(codigo.size()))
            {
                if (codigo[i] == '*'
                    && i + 1 < static_cast<int>(codigo.size())
                    && codigo[i + 1] == '/')
                {
                    avanzar();
                    avanzar();
                    cerrado = true;
                    break;
                }

                avanzar();
            }

            if (!cerrado)
            {
                resultado.correcto = false;
                resultado.mensaje =
                    "Error lexico en linea " +
                    std::to_string(inicioLinea) +
                    ", columna " + std::to_string(inicioColumna) +
                    ": comentario sin cerrar.";
                return resultado;
            }

            continue;
        }

        int inicioLinea = linea;
        int inicioColumna = columna;

        // Identificadores y palabras reservadas.
        if (esInicioIdentificador(c))
        {
            std::string texto;

            while (i < static_cast<int>(codigo.size())
                   && esParteIdentificador(codigo[i]))
            {
                texto += codigo[i];
                avanzar();
            }

            TipoToken tipo = palabrasReservadas.count(texto)
                ? TipoToken::PalabraReservada
                : TipoToken::Identificador;

            agregarToken(
                resultado, tipo, texto,
                inicioLinea, inicioColumna
            );

            continue;
        }

        // Numeros enteros y decimales sencillos.
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            std::string texto;
            bool tienePunto = false;

            while (i < static_cast<int>(codigo.size()))
            {
                char actual = codigo[i];

                if (std::isdigit(static_cast<unsigned char>(actual)))
                {
                    texto += actual;
                    avanzar();
                }
                else if (actual == '.' && !tienePunto
                         && i + 1 < static_cast<int>(codigo.size())
                         && std::isdigit(
                             static_cast<unsigned char>(codigo[i + 1])))
                {
                    tienePunto = true;
                    texto += actual;
                    avanzar();
                }
                else
                {
                    break;
                }
            }

            agregarToken(
                resultado, TipoToken::Numero, texto,
                inicioLinea, inicioColumna
            );

            continue;
        }

        // Cadenas de texto.
        if (c == '"')
        {
            std::string texto;
            texto += c;
            avanzar();

            bool cerrada = false;

            while (i < static_cast<int>(codigo.size()))
            {
                char actual = codigo[i];

                if (actual == '\n')
                    break;

                texto += actual;
                avanzar();

                if (actual == '\\' && i < static_cast<int>(codigo.size()))
                {
                    texto += codigo[i];
                    avanzar();
                    continue;
                }

                if (actual == '"')
                {
                    cerrada = true;
                    break;
                }
            }

            if (!cerrada)
            {
                resultado.correcto = false;
                resultado.mensaje =
                    "Error lexico en linea " +
                    std::to_string(inicioLinea) +
                    ", columna " + std::to_string(inicioColumna) +
                    ": cadena de texto sin cerrar.";
                return resultado;
            }

            agregarToken(
                resultado, TipoToken::Cadena, texto,
                inicioLinea, inicioColumna
            );

            continue;
        }

        // Operadores de dos caracteres.
        if (i + 1 < static_cast<int>(codigo.size()))
        {
            std::string doble = codigo.substr(i, 2);

            if (doble == "==" || doble == "!=" ||
                doble == ">=" || doble == "<=" ||
                doble == "&&" || doble == "||" ||
                doble == "++" || doble == "--" ||
                doble == "+=" || doble == "-=" ||
                doble == "*=" || doble == "/=")
            {
                agregarToken(
                    resultado, TipoToken::Operador, doble,
                    inicioLinea, inicioColumna
                );

                avanzar();
                avanzar();
                continue;
            }
        }

        // Operadores de un caracter.
        if (std::string("+-*/%=!><").find(c) != std::string::npos)
        {
            agregarToken(
                resultado, TipoToken::Operador, std::string(1, c),
                inicioLinea, inicioColumna
            );

            avanzar();
            continue;
        }

        // Simbolos permitidos en el subconjunto.
        if (std::string("(){}[];,.").find(c) != std::string::npos)
        {
            agregarToken(
                resultado, TipoToken::Simbolo, std::string(1, c),
                inicioLinea, inicioColumna
            );

            avanzar();
            continue;
        }

        resultado.correcto = false;
        resultado.mensaje =
            "Error lexico en linea " + std::to_string(linea) +
            ", columna " + std::to_string(columna) +
            ": caracter no reconocido '" + std::string(1, c) + "'.";

        return resultado;
    }

    resultado.tokens.push_back({
        TipoToken::Fin, "", linea, columna
    });

    resultado.mensaje = "Analisis lexico completado.";
    return resultado;
}

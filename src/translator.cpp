

#include "translator.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include <sstream>
#include <string>
#include <regex>
#include <vector>

namespace
{
    std::string quitarEspacios(const std::string& texto)
    {
        const size_t inicio = texto.find_first_not_of(" \t\r\n");

        if (inicio == std::string::npos)
            return "";

        const size_t fin = texto.find_last_not_of(" \t\r\n");
        return texto.substr(inicio, fin - inicio + 1);
    }

    std::string reemplazar(
        const std::string& texto,
        const std::string& patron,
        const std::string& destino)
    {
        return std::regex_replace(
            texto,
            std::regex(patron),
            destino
        );
    }

    std::string traducirLinea(std::string linea)
    {
        linea = quitarEspacios(linea);

        if (linea.empty())
            return "";

        if (linea.rfind("//", 0) == 0)
            return linea;

        // Convertir la impresion por consola.
        linea = reemplazar(
            linea,
            R"(System\s*\.\s*out\s*\.\s*println\s*\()",
            "console.log("
        );

        linea = reemplazar(
            linea,
            R"(System\s*\.\s*out\s*\.\s*print\s*\()",
            "process.stdout.write("
        );

        // Convertir declaraciones simples de variables.
        const std::vector<std::string> tipos = {
            "int", "double", "float", "String",
            "boolean", "char", "long", "short", "byte"
        };

        for (const auto& tipo : tipos)
        {
            linea = reemplazar(
                linea,
                "\\b" + tipo + "\\s+",
                "let "
            );
        }

        // Eliminar el punto y coma final.
        if (!linea.empty() && linea.back() == ';')
            linea.pop_back();

        return linea;
    }
}

ResultadoTraduccion traducirMovieScript(const std::string& entrada)
{


    if (quitarEspacios(entrada).empty())
    {
        return {
            false,
            "",
            "Error: no se ha introducido codigo Java."
        };
    }

ResultadoLexico lexico = analizarLexico(entrada);

if (!lexico.correcto)
{
    return {
        false,
        "",
        lexico.mensaje
    };
}

ResultadoSintactico sintaxis = analizarSintaxis(lexico.tokens);

if (!sintaxis.correcto)
{
    return {
        false,
        "",
        sintaxis.mensaje
    };
}



ResultadoSemantico semantica = analizarSemantica(lexico.tokens);

if (!semantica.correcto)
{
    return {
        false,
        "",
        semantica.mensaje
    };
}



    std::istringstream lector(entrada);
    std::ostringstream salida;
    std::string linea;
    int numeroLinea = 0;

    std::vector<std::string> pilaBloques;

    salida << "// Codigo traducido de Java a JavaScript\n\n";

    while (std::getline(lector, linea))
    {
        numeroLinea++;
        linea = quitarEspacios(linea);

        if (linea.empty())
            continue;

        // Ignorar la declaracion de clase.
        if (std::regex_match(
                linea,
                std::regex(R"((public\s+)?class\s+\w+\s*\{)")))
        {
            pilaBloques.push_back("class");
            continue;
        }

        // Ignorar la cabecera del metodo main.
        if (std::regex_match(
                linea,
                std::regex(
                    R"(public\s+static\s+void\s+main\s*\(\s*String\s*(\[\s*\]\s*\w+|\w+\s*\[\s*\])\s*\)\s*\{)"
                )))
        {
            pilaBloques.push_back("main");
            continue;
        }

        // Procesar llaves de cierre.
        if (linea == "}")
        {
            if (pilaBloques.empty())
            {
                return {
                    false,
                    "",
                    "Linea " + std::to_string(numeroLinea) +
                    ": llave de cierre sin bloque abierto."
                };
            }

            const std::string bloque = pilaBloques.back();
            pilaBloques.pop_back();

            if (bloque != "class" && bloque != "main")
                salida << "}\n";

            continue;
        }

        // Traducir condicionales.
        if (std::regex_match(
                linea,
                std::regex(R"(if\s*\(.*\)\s*\{)")))
        {
            salida << linea << "\n";
            pilaBloques.push_back("if");
            continue;
        }

        if (std::regex_match(
                linea,
                std::regex(R"(else\s*\{)")))
        {
            salida << "else {\n";
            pilaBloques.push_back("else");
            continue;
        }

        // Traducir ciclos sencillos.
        if (std::regex_match(
                linea,
                std::regex(R"(while\s*\(.*\)\s*\{)")))
        {
            salida << linea << "\n";
            pilaBloques.push_back("while");
            continue;
        }

        if (std::regex_match(
                linea,
                std::regex(R"(for\s*\(.*\)\s*\{)")))
        {
            salida << linea << "\n";
            pilaBloques.push_back("for");
            continue;
        }

        salida << traducirLinea(linea) << "\n";
    }

    if (!pilaBloques.empty())
    {
        return {
            false,
            "",
            "Hay bloques sin cerrar. Revisa las llaves del codigo Java."
        };
    }

    return {
        true,
        salida.str(),
        "Traduccion completada."
    };
}

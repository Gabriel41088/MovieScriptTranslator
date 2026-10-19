
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QSplitter>
#include <QFont>
#include <QString>

#include "translator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow ventana;
    ventana.setWindowTitle("Java to JavaScript Translator");
    ventana.resize(1100, 720);

    QWidget *central = new QWidget(&ventana);
    QVBoxLayout *principal = new QVBoxLayout(central);
    principal->setContentsMargins(18, 18, 18, 18);
    principal->setSpacing(12);

    QLabel *titulo = new QLabel("JAVA TO JAVASCRIPT");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "color: #8B5CF6;"
        "padding: 12px;"
    );

    QLabel *descripcion = new QLabel(
        "Traductor con analisis lexico, sintactico y semantico"
    );
    descripcion->setAlignment(Qt::AlignCenter);
    descripcion->setStyleSheet(
        "font-size: 14px;"
        "color: #9CA3AF;"
        "padding-bottom: 8px;"
    );

    QLabel *etiquetaEntrada = new QLabel("CODIGO JAVA");
    QLabel *etiquetaSalida = new QLabel("JAVASCRIPT GENERADO");

    etiquetaEntrada->setStyleSheet(
        "font-weight: bold; padding: 5px;"
    );
    etiquetaSalida->setStyleSheet(
        "font-weight: bold; padding: 5px;"
    );

    QPlainTextEdit *editorEntrada = new QPlainTextEdit();
    QPlainTextEdit *editorSalida = new QPlainTextEdit();

    editorEntrada->setPlaceholderText(
        "Escribe aqui tu codigo Java..."
    );
    editorSalida->setPlaceholderText(
        "Aqui aparecera el codigo JavaScript..."
    );

    editorEntrada->setLineWrapMode(QPlainTextEdit::NoWrap);
    editorSalida->setLineWrapMode(QPlainTextEdit::NoWrap);
    editorSalida->setReadOnly(true);

    QFont fuenteCodigo("Consolas", 11);
    editorEntrada->setFont(fuenteCodigo);
    editorSalida->setFont(fuenteCodigo);

    editorEntrada->setStyleSheet(
        "QPlainTextEdit {"
        "background-color: #111827;"
        "color: #E5E7EB;"
        "border: 1px solid #374151;"
        "border-radius: 6px;"
        "padding: 8px;"
        "}"
    );

    editorSalida->setStyleSheet(
        "QPlainTextEdit {"
        "background-color: #0B1220;"
        "color: #A7F3D0;"
        "border: 1px solid #374151;"
        "border-radius: 6px;"
        "padding: 8px;"
        "}"
    );

    editorEntrada->setPlainText(
        "public class Main {\n"
        "    public static void main(String[] args) {\n"
        "        int edad = 20;\n"
        "        String nombre = \"Gabriel\";\n"
        "\n"
        "        System.out.println(\"Hola, \" + nombre);\n"
        "\n"
        "        if (edad >= 18) {\n"
        "            System.out.println(\"Es mayor de edad\");\n"
        "        }\n"
        "    }\n"
        "}\n"
    );

    QSplitter *divisor = new QSplitter(Qt::Horizontal);

    QWidget *panelEntrada = new QWidget();
    QVBoxLayout *layoutEntrada = new QVBoxLayout(panelEntrada);
    layoutEntrada->setContentsMargins(0, 0, 6, 0);
    layoutEntrada->addWidget(etiquetaEntrada);
    layoutEntrada->addWidget(editorEntrada);

    QWidget *panelSalida = new QWidget();
    QVBoxLayout *layoutSalida = new QVBoxLayout(panelSalida);
    layoutSalida->setContentsMargins(6, 0, 0, 0);
    layoutSalida->addWidget(etiquetaSalida);
    layoutSalida->addWidget(editorSalida);

    divisor->addWidget(panelEntrada);
    divisor->addWidget(panelSalida);
    divisor->setStretchFactor(0, 1);
    divisor->setStretchFactor(1, 1);

    QPushButton *botonTraducir =
        new QPushButton("ANALIZAR Y TRADUCIR");

    QPushButton *botonLimpiar =
        new QPushButton("LIMPIAR");

    QPushButton *botonCargar =
        new QPushButton("CARGAR JAVA");

    QPushButton *botonGuardar =
        new QPushButton("GUARDAR JAVASCRIPT");

    botonTraducir->setStyleSheet(
        "QPushButton {"
        "background-color: #7C3AED;"
        "color: white;"
        "font-weight: bold;"
        "padding: 11px;"
        "border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "background-color: #6D28D9;"
        "}"
    );

    const QString estiloBoton =
        "QPushButton {"
        "padding: 10px;"
        "border: 1px solid #4B5563;"
        "border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "background-color: #374151;"
        "}";

    botonLimpiar->setStyleSheet(estiloBoton);
    botonCargar->setStyleSheet(estiloBoton);
    botonGuardar->setStyleSheet(estiloBoton);

    QHBoxLayout *botones = new QHBoxLayout();
    botones->setSpacing(10);
    botones->addWidget(botonTraducir);
    botones->addWidget(botonLimpiar);
    botones->addWidget(botonCargar);
    botones->addWidget(botonGuardar);

    QLabel *estado = new QLabel("Listo para analizar y traducir.");
    estado->setStyleSheet(
        "color: #9CA3AF;"
        "padding: 5px;"
    );
    estado->setWordWrap(true);

    principal->addWidget(titulo);
    principal->addWidget(descripcion);
    principal->addWidget(divisor, 1);
    principal->addLayout(botones);
    principal->addWidget(estado);

    // Analisis y traduccion
    QObject::connect(
        botonTraducir,
        &QPushButton::clicked,
        [&]()
        {
            const QString entrada = editorEntrada->toPlainText();

            if (entrada.trimmed().isEmpty())
            {
                editorSalida->clear();
                estado->setText("Error: no se ha introducido codigo Java.");

                QMessageBox::warning(
                    &ventana,
                    "Codigo vacio",
                    "Escribe o carga codigo Java antes de traducir."
                );
                return;
            }

            editorSalida->clear();
            estado->setText(
                "Ejecutando analisis lexico, sintactico y semantico..."
            );
            QApplication::processEvents();

            ResultadoTraduccion resultado =
                traducirMovieScript(entrada.toStdString());

            if (!resultado.correcto)
            {
                editorSalida->clear();
                estado->setText("Analisis detenido: se encontro un error.");

                QMessageBox::warning(
                    &ventana,
                    "Error de analisis",
                    QString::fromStdString(resultado.mensaje)
                );
                return;
            }

            editorSalida->setPlainText(
                QString::fromStdString(resultado.codigo)
            );

            estado->setText(
                "Analisis lexico, sintactico y semantico completado. "
                "Traduccion correcta."
            );
        }
    );

    // Limpiar editores
    QObject::connect(
        botonLimpiar,
        &QPushButton::clicked,
        [&]()
        {
            editorEntrada->clear();
            editorSalida->clear();
            estado->setText("Editores limpiados.");
        }
    );

    // Cargar archivo Java
    QObject::connect(
        botonCargar,
        &QPushButton::clicked,
        [&]()
        {
            QString ruta = QFileDialog::getOpenFileName(
                &ventana,
                "Abrir archivo Java",
                QString(),
                "Archivos Java (*.java);;Archivos de texto (*.txt);;Todos los archivos (*)"
            );

            if (ruta.isEmpty())
                return;

            QFile archivo(ruta);

            if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QMessageBox::critical(
                    &ventana,
                    "Error",
                    "No se pudo abrir el archivo seleccionado."
                );
                return;
            }

            QTextStream flujo(&archivo);
            editorEntrada->setPlainText(flujo.readAll());
            archivo.close();

            editorSalida->clear();
            estado->setText("Archivo Java cargado correctamente.");
        }
    );

    // Guardar resultado JavaScript
    QObject::connect(
        botonGuardar,
        &QPushButton::clicked,
        [&]()
        {
            if (editorSalida->toPlainText().trimmed().isEmpty())
            {
                QMessageBox::warning(
                    &ventana,
                    "Sin resultado",
                    "Primero debes traducir el codigo Java."
                );
                return;
            }

            QString ruta = QFileDialog::getSaveFileName(
                &ventana,
                "Guardar archivo JavaScript",
                "traducido.js",
                "Archivos JavaScript (*.js)"
            );

            if (ruta.isEmpty())
                return;

            if (!ruta.endsWith(".js", Qt::CaseInsensitive))
                ruta += ".js";

            QFile archivo(ruta);

            if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::critical(
                    &ventana,
                    "Error",
                    "No se pudo guardar el archivo JavaScript."
                );
                return;
            }

            QTextStream flujo(&archivo);
            flujo << editorSalida->toPlainText();
            archivo.close();

            estado->setText("Archivo JavaScript guardado correctamente.");

            QMessageBox::information(
                &ventana,
                "Archivo guardado",
                "El archivo JavaScript se guardo correctamente."
            );
        }
    );

    ventana.setCentralWidget(central);
    ventana.show();

    return app.exec();
}

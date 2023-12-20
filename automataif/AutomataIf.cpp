#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;

enum Tipos
{
    PalabraReservada,
    ParentesisAbierto,
    ParentesisCerrado,
    LlaveAbierta,
    LlaveCerrada,
    Variable,
    Numero,
    OperadorRelacional,
    OperadorLogico,
    OperadorIgualdad,
    Espacio,
    SaltoDeLinea,
    Desconocido
};

class Token
{
private:
    string contenido;
    Tipos clasificacion;
    string identificador;
    int subidentificador;
    int estado;

public:
    Token()
    {
        contenido = "";
        clasificacion = Desconocido;
        identificador = "";
        subidentificador = 0;
        estado = 0;
    }
    void setContenido(string contenido)
    {
        this->contenido = contenido;
    }
    void setClasificacion(Tipos clasificacion)
    {
        this->clasificacion = clasificacion;
    }
    void setIdentificador(string identificador)
    {
        this->identificador = identificador;
    }
    void setSubidentificador(int subidentificador)
    {
        this->subidentificador = subidentificador;
    }
    void setEstado(int estado)
    {
        this->estado = estado;
    }
    string getContenido()
    {
        return this->contenido;
    }
    Tipos getClasificacion()
    {
        return this->clasificacion;
    }
    string getIdentificador()
    {
        return this->identificador;
    }
    int getSubidentificador()
    {
        return this->subidentificador;
    }
    int getEstado()
    {
        return this->estado;
    }
};

string TipoToString(Tipos tipo)
{
    switch (tipo)
    {
    case PalabraReservada:
        return "Palabra Reservada";
    case ParentesisAbierto:
        return "Parentesis Abierto";
    case ParentesisCerrado:
        return "Parentesis Cerrado";
    case LlaveAbierta:
        return "Llave Abierta";
    case LlaveCerrada:
        return "Llave Cerrada";
    case Variable:
        return "Variable";
    case Numero:
        return "Numero";
    case OperadorRelacional:
        return "Operador Relacional";
    case OperadorLogico:
        return "Operador Logico";
    case OperadorIgualdad:
        return "Operador Igualdad";
    case Espacio:
        return "Espacio";
    case SaltoDeLinea:
        return "Salto de Linea";
    case Desconocido:
        return "Desconocido";
    }
    return "";
}

bool esOperadorRelacional(char c)
{
    return (c == '<' || c == '>' || c == '=');
}

bool esOperadorLogico(char c)
{
    return (c == '&' || c == '|');
}

bool esOperadorIgualdad(char c)
{
    return (c == '=' || c == '!' || c == '<' || c == '>');
}

int main()
{
    ifstream archivo_entrada("codigo.txt");
    ofstream archivo_salida("automata_codigo.txt", ios::trunc);

    char c;
    string buffer = "";
    int estado = 0;
    const int F = -1;

    Token token;
    unordered_map<Tipos, int> contadorSubidentificadores;

    while (!archivo_entrada.eof())
    {
        estado = 0;
        buffer = "";

        while (estado >= 0 && !archivo_entrada.eof())
        {
            c = archivo_entrada.peek();
            int estado_anterior = estado; // Guardamos el estado anterior antes de cambiarlo

            switch (estado)
            {
            case 0: // Inicial
                if (isalpha(c))
                    estado = 1; // Palabra Reservada o Variable
                else if (isdigit(c))
                    estado = 2; // Número
                else if (esOperadorRelacional(c))
                    estado = 3; // Operador Relacional
                else if (esOperadorLogico(c))
                    estado = 4; // Operador Lógico
                else if (esOperadorIgualdad(c))
                    estado = 5; // Operador Igualdad
                else if (c == '(')
                    estado = 6; // Parentesis Abierto
                else if (c == ')')
                    estado = 7; // Parentesis Cerrado
                else if (c == '{')
                    estado = 8; // Llave Abierta
                else if (c == '}')
                    estado = 9; // Llave Cerrada
                else if (c == ' ')
                    estado = 10; // Espacio
                else if (c == '\n' || c == '\r')
                    estado = 11; // Salto de Linea
                else if (!isspace(c))
                    estado = 12; // Desconocido
                break;
            case 1: // Palabra Reservada o Variable
                if (!isalnum(c) && c != '_')
                    estado = F;
                break;
            case 2: // Número
                if (!isdigit(c))
                    estado = F;
                break;
            case 3: // Operador Relacional
                estado = F;
                break;
            case 4: // Operador Lógico
                estado = F;
                break;
            case 5: // Operador Igualdad
                estado = F;
                break;
            case 6: // Parentesis Abierto
                estado = F;
                break;
            case 7: // Parentesis Cerrado
                estado = F;
                break;
            case 8: // Llave Abierta
                estado = F;
                break;
            case 9: // Llave Cerrada
                estado = F;
                break;
            case 10: // Espacio
                if (c != ' ')
                    estado = F;
                break;
            case 11: // Salto de Linea
                if (c != '\n' && c != '\r')
                    estado = F;
                break;
            case 12: // Desconocido
                if (isspace(c))
                    estado = F;
                break;
            }

            if (estado >= 0 && estado != F)
            {
                buffer += c;
                archivo_entrada.get();
            }

            if (estado == F)
            {
                token.setContenido(buffer);
                if (isalpha(buffer[0]))
                {
                    if (buffer == "if" || buffer == "else")
                    {
                        token.setClasificacion(PalabraReservada);
                        token.setIdentificador("PR");
                    }
                    else
                    {
                        token.setClasificacion(Variable);
                        token.setIdentificador("VAR");
                    }
                }
                else if (isdigit(buffer[0]))
                {
                    token.setClasificacion(Numero);
                    token.setIdentificador("NUM");
                }
                else if (esOperadorRelacional(buffer[0]))
                {
                    token.setClasificacion(OperadorRelacional);
                    token.setIdentificador("OR");
                }
                else if (esOperadorLogico(buffer[0]))
                {
                    token.setClasificacion(OperadorLogico);
                    token.setIdentificador("OL");
                }
                else if (esOperadorIgualdad(buffer[0]))
                {
                    token.setClasificacion(OperadorIgualdad);
                    token.setIdentificador("OI");
                }
                else if (buffer == "(")
                {
                    token.setClasificacion(ParentesisAbierto);
                    token.setIdentificador("PA");
                }
                else if (buffer == ")")
                {
                    token.setClasificacion(ParentesisCerrado);
                    token.setIdentificador("PC");
                }
                else if (buffer == "{")
                {
                    token.setClasificacion(LlaveAbierta);
                    token.setIdentificador("LA");
                }
                else if (buffer == "}")
                {
                    token.setClasificacion(LlaveCerrada);
                    token.setIdentificador("LC");
                }
                else if (buffer == " ")
                {
                    token.setClasificacion(Espacio);
                    token.setIdentificador("ESP");
                }
                else if (buffer == "\n" || buffer == "\r")
                {
                    token.setClasificacion(SaltoDeLinea);
                    token.setIdentificador("SDL");
                }
                else
                {
                    token.setClasificacion(Desconocido);
                    token.setIdentificador("DESC");
                }

                // Obtener el contador de subidentificadores para el tipo actual
                int &contadorSubidentificador = contadorSubidentificadores[token.getClasificacion()];
                // Incrementar el contador y asignarlo al token
                token.setSubidentificador(++contadorSubidentificador);
                // Asignar el estado anterior al token
                token.setEstado(estado_anterior);

                archivo_salida << "ESTADO: q" << token.getEstado() << ", " << token.getContenido() << " = " << TipoToString(token.getClasificacion()) << ", ID: " << token.getIdentificador() << ", SUBIDENTIFICADOR: " << token.getSubidentificador() << endl;

                buffer = "";
                token.setContenido("");
                token.setClasificacion(Desconocido);
                token.setIdentificador("");
                token.setSubidentificador(0);
                token.setEstado(0);
            }
        }
    }

    archivo_entrada.close();
    archivo_salida.close();

    return 0;
}


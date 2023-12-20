#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <unordered_set>

using namespace std;

enum Tipos
{
    PalabraReservada,
    OperadordeAgrupacion,
    LlaveAbierta,
    LlaveCerrada,
    Variable,
    Numero,
    OperadorRelacional,
    OperadorLogico,
    OperadordeDesplazamiento,
    OperadorAritmetico,
    PuntoyComa,
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
    case OperadordeAgrupacion:
        return "Operador de Agrupacion";
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
    case OperadordeDesplazamiento:
    	return "Operador de Desplazamiento";
    case OperadorAritmetico:
        return "Operador Aritmetico";
    case PuntoyComa:
        return "Punto y Coma";
    case Espacio:
        return "Espacio";
    case SaltoDeLinea:
        return "Salto de Linea";
    case Desconocido:
        return "Desconocido";
    }
    return "";
}

bool esOperadordeAgrupacion(char c)
{
    return (c == '(' || c == ')' );
}

bool esInicioOperadorRelacional(char c)
{
    return (c == '<' || c == '>' || c == '=' || c == '!');
}

bool esOperadorRelacional(const string& op)
{
    static const unordered_set<string> operadoresRelacionales = {"!=", "==", "<=", ">="};
    return operadoresRelacionales.count(op) > 0;
}

bool esOperadorLogico(const string& op)
{
    static const unordered_set<string> operadoresLogicos = {"||", "&&"};
    return operadoresLogicos.count(op) > 0;
}

bool esOperadordeDesplazamiento(const string& op)
{
    static const unordered_set<string> operadoresdeDesplazamiento = {"<<", ">>"};
    return operadoresdeDesplazamiento.count(op) > 0;
}

bool esOperadorAritmetico(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool esPuntoyComa(char c)
{
    return (c == ';');
}

bool esComentarioInicio(char c)
{
    return (c == '/');
}

bool esComentarioFin(char c)
{
    return (c == '\n');
}

int main()
{
    ifstream archivo_entrada("codigo.txt");
    ofstream archivo_salida("automata_codigo.txt", ios::trunc);

    char c;
    string buffer = "";
    int estado = 0;
    
    int estado_anterior = 0; // Declarar estado_anterior antes del bucle
    
    const int F = -1;
    
    bool enComentarioBloque = false;

    Token token;
    unordered_map<Tipos, int> contadorSubidentificadores;

    while (!archivo_entrada.eof())
    {
        estado = 0;
        buffer = "";

        while (estado >= 0 && !archivo_entrada.eof())
        {
            c = archivo_entrada.peek();
            
            if (enComentarioBloque)
            {
                // Omitir contenido del comentario de bloque
                while (!archivo_entrada.eof() && !esComentarioFin(c))
                {
                    archivo_entrada.get();
                    c = archivo_entrada.peek();
                }

                // Detectar el final del comentario de bloque
                if (c == '*')
                {
                    archivo_entrada.get(); // Consumir '*'
                    c = archivo_entrada.get(); // Consumir '/'
                    enComentarioBloque = false;
                }
            }
            else if (esComentarioInicio(c))
            {
                archivo_entrada.get(); // Consumir '/'
                c = archivo_entrada.peek();

                if (c == '/')
                {
                    // Omitir comentario de línea
                    while (!archivo_entrada.eof() && c != '\n')
                    {
                        archivo_entrada.get();
                        c = archivo_entrada.peek();
                    }
                }
                else if (c == '*')
                {
                    // Iniciar comentario de bloque
                    enComentarioBloque = true;
                    archivo_entrada.get(); // Consumir '*'
                }
                else
                {
                    // No es un comentario, seguir procesando
                    enComentarioBloque = false;
                    buffer += '/';
                    estado_anterior = estado; // Restaurar el estado anterior
                }
            }
            else
            {
            	
            int estado_anterior = estado; // Guardamos el estado anterior antes de cambiarlo

            switch (estado)
            {
            case 0: // Inicial
                if (isalpha(c))
                    estado = 1; // Palabra Reservada o Variable
                else if (isdigit(c))
                    estado = 2; // Número
                else if (esOperadorRelacional(string(1, c)))
    				estado = 3; // Operador Relacional
                else if (esOperadorLogico(string(1, c)))
                    estado = 4; // Operador Lógico
                else if (esOperadordeDesplazamiento(string(1, c)))
                    estado = 5; // Operador de Desplazamiento
                else if (esOperadorAritmetico(c))
                    estado = 6; // Operador Aritmetico
                else if (esOperadordeAgrupacion(c))
                    estado = 7; // Operador de Agrupacion
                else if (c == '{')
                    estado = 8; // Llave Abierta
                else if (c == '}')
                    estado = 9; // Llave Cerrada
                else if (esPuntoyComa(c))
                	estado = 10;
                else if (c == ' ')
                    estado = 11; // Espacio
                else if (c == '\n' || c == '\r')
                    estado = 12; // Salto de Linea
                else if (!isspace(c))
                    estado = 13; // Desconocido
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
                if (!esInicioOperadorRelacional(c) && buffer.length() <= 1)
			        estado = F;
			    else if (buffer.length() == 2 && !esOperadorRelacional(buffer))
			        estado = F;
			    else if (buffer.length() > 2)
			        estado = F;
			    break;
            case 4: // Operador Lógico
            	if (!esOperadorLogico(buffer) && buffer.length() <= 1)
            		estado = F;
                else if (buffer.length() == 2 && !esOperadorLogico(buffer))
			        estado = F;
			    else if (buffer.length() > 2)
			        estado = F;
			    break;
            case 5: // Operador de Desplazamiento
                if (buffer.length() == 2 && !esOperadordeDesplazamiento(buffer))
			        estado = F;
			    else if (buffer.length() > 2)
			        estado = F;
			    break;
            case 6: // Operador Aritmetico
                estado = F;
                break;
            case 7: // Parentesis Abierto
                estado = F;
                break;
            case 8: // Llave Abierta
                estado = F;
                break;
            case 9: // Llave Cerrada
                estado = F;
                break;
            case 10: // Punto y Coma
                estado = F;
                break;
            case 11: // Espacio
                if (c != ' ')
                    estado = F;
                break;
            case 12: // Salto de Linea
                if (c != '\n' && c != '\r')
                    estado = F;
                break;
			case 13: // Desconocido
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
                    if (buffer == "if" || buffer == "else" || buffer == "cout" || buffer == "cin")
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
                else if ((buffer == "<" || buffer == ">" || buffer == "=" || buffer == "==" || buffer == "!=" || buffer == ">=" || buffer == "<=") && (buffer.length() == 1 || buffer.length() == 2))
                {
                token.setClasificacion(OperadorRelacional);
                token.setIdentificador("OR");
                }
                else if ((buffer == "||" || buffer == "&&") && buffer.length() == 2)
                {
                token.setClasificacion(OperadorLogico);
                token.setIdentificador("OL");
                }
                else if ((buffer == "<<" || buffer == ">>") && buffer.length() == 2)
                {
                    token.setClasificacion(OperadordeDesplazamiento);
                    token.setIdentificador("OD");
                }
                else if (esOperadorAritmetico(buffer[0]))
                {
                    token.setClasificacion(OperadorAritmetico);
                    token.setIdentificador("OA");
                }
                else if (esOperadordeAgrupacion(buffer[0]))
                {
                    token.setClasificacion(OperadordeAgrupacion);
                    token.setIdentificador("OdA");
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
                else if (esPuntoyComa(buffer[0]))
                {
                    token.setClasificacion(PuntoyComa);
                    token.setIdentificador("PC");
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
				
				archivo_salida 
			    << "ESTADO: q" 
			    << token.getEstado() 
			    << ", " << token.getContenido() 
			    << " = " 
			    << TipoToString(token.getClasificacion()) 
			    << ", ID: " << token.getIdentificador() 
			    << ", SUBIDENTIFICADOR: " 
			    << token.getSubidentificador() 
			    << endl;
				}
			}
        }
    }

    archivo_entrada.close();
    archivo_salida.close();

    return 0;
}
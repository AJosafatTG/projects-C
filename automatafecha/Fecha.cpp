	#include <iostream>
	#include <fstream>
	#include <cctype>
	
	using namespace std;
	
	enum Tipos
	{
	FechaValida, FechaInvalida
	};
	
	class Token
	{
	private:
	    string contenido;
	    Tipos clasificacion;
	
	public:
	    Token()
	    {
	        contenido = "";
	        clasificacion = FechaValida;
	    }
	    void setContenido(string contenido)
	    {
	        this->contenido = contenido;
	    }
	    void setClasificacion(Tipos clasificacion)
	    {
	        this->clasificacion = clasificacion;
	    }
	    string getContenido()
	    {
	        return this->contenido;
	    }
	    Tipos getClasificacion()
	    {
	        return this->clasificacion;
	    }
	};
	
	string TipoToString(Tipos tipo) {
	switch(tipo) {
	    case FechaValida: return "FechaValida";
	    case FechaInvalida: return "FechaInvalida";
	}
	return "";
	}
	
	int main() {
	
	ifstream archivo_entrada("fecha.txt"); 
	ofstream archivo_salida("automata.txt",ios::trunc);
	
	char c;
	string buffer = "";
	int estado = 0;
	const int F = -1;
	
	Token token;
	
	while (!archivo_entrada.eof())
	{
	    estado = 0;
	    buffer = "";
	
	    while (estado >= 0 && !archivo_entrada.eof())
	    {
	        c = archivo_entrada.peek();
	        switch (estado)
	        {
	            case 0:
	                if (isspace(c))
	                {
	                    estado = 0;
	                }
	                else if (c == '0')
	                    estado = 1;
	                else if (c == '1'||c == '2')
	                    estado = 2;
	                else if (c == '3')
	                    estado = 3;
	                else{
	                	estado = 21;
					}
	                break;
	            case 1:
	                if (c != '0' && isdigit(c)){
	                    estado = 4;								
	                }else{
	                	estado = 21;
					}
	                break;
	            case 2:
	                if (isdigit(c)){
	                    estado = 4;								
	                }else{
	                	estado = 21;
					}
	                break;
	            case 3:
	                if (c == '0'||c == '1'){
	                	estado = 4;
					}  
	                else{
	                	estado = 21;
					}	
	                break;
	            case 4:
	                if (c == '/'){
	                	estado = 5;	
					}
	                break;
	            case 5:
	            	if (c == '0'){
	            		estado = 6;
					}else if (c == '1'){
						estado = 7;
					}else{
						estado = 21;
					}
	            	break;
	            case 6:
	            	if (c != 0 && isdigit(c)){
	            		estado = 8;
					}else{
						estado = 21;
					}
					break;
				case 7:
					if (c == '0' || c == '1' || c == '2'){
						estado = 8;
					}else{
						estado = 21;
					}
					break;
				case 8:
					if (c == '/'){
						estado = 9;
					}
					break;
				case 9:
					if (c == '0'){
						estado = 10;
					}else if (c == '1'){
						estado = 13;
					}else if(c == '2'){
						estado = 16;
					}else{
						estado = 21;
					}
					break;
				case 10:
					if(isdigit(c)){
						estado = 11;
					}else{
						estado = 21;
					}
					break;
				case 11:
					if(isdigit(c)){
						estado = 12;
					}else{
						estado = 21;
					}
					break;
				case 12:
					if(c != '0' && isdigit(c)){
						estado = 20;
					}else{
						estado = 21;
					}
					break;
				case 13:
					if(isdigit(c)){
						estado = 14;
					}else{
						estado = 21;
					}
					break;
				case 14:
					if(isdigit(c)){
						estado = 15;
					}else{
						estado = 21;
					}
					break;
				case 15:
					if(isdigit(c)){
						estado = 20;
					}else{
						estado = 21;
					}
					break;
				case 16:
					if(c == '0'){
						estado = 17;
					}else{
						estado = 21;
					}
					break;
				case 17:
					if(c == '0' || c == '1'){
						estado = 18;
					}else if(c == '2'){
						estado = 19;
					}else{
						estado = 21;
					}
					break;
				case 18:
					if(isdigit(c)){
						estado = 20;
					}else{
						estado = 21;
					}
					break;
				case 19:
					if(c == '0' || c == '1' || c == '2' || c == '3'){
						estado = 20;
					}else{
						estado = 21;
					}
					break;
				case 20:
					token.setClasificacion(FechaValida);
					estado = F;
					break;
				case 21:
				if (isspace(c) || archivo_entrada.eof())
				{
	    			token.setContenido(buffer);
	    			token.setClasificacion(FechaInvalida);
	    			estado = F;
				}
				else
				{			
	    		buffer += c;
	    		archivo_entrada.get();
	}
	break;
	        }
	
	        if (estado >= 0 && estado != 21)
	        {
	            if (estado > 0)
	            {
	                buffer += c;
	            }
	            archivo_entrada.get();
	        }
	
	        if(estado == F){
	        	token.setContenido(buffer);
	        	archivo_salida << token.getContenido() << " = " << TipoToString(token.getClasificacion()) << endl;
	
	        	buffer = "";
	        	token.setContenido("");
	        	token.setClasificacion(FechaValida);
			}
	        
	    }
	
	}
	
	archivo_entrada.close(); 
	archivo_salida.close();
	
	return 0;
	}

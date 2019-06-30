#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>

	//-------------------------//
	void gotoxy( int column, int line )
	{
		COORD coord;
		coord.X = column;
		coord.Y = line;
		SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
	}

	int wherex()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD  result;
		if ( !GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi ) )
			return -1;
		return result.X;
	}

	int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD  result;
		if ( !GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi ) )
			return -1;
		return result.Y;
	}

#endif
#ifdef _UNIX_
	#include <ncurses.h>
	#include <stdlib.h>

	//-------------------------//
	// No hay funciones gotoxy en linux, por lo menos al uso
	void gotoxy(int x,int y)
	{
		printf("%c[%d;%df",0x1B,y,x);
	}

	int wherex()
	{
		int y, x;
		 getyx(stdscr, y, x);
		 return x;
	}
	int wherey()
	{
		int y, x;
		 getyx(stdscr, y, x);
		 return y;
	}

	//-------------------------//
	// Dado que la libreria ncurses contiene algo similar a la kbhit, pero no a la getch(), aqui esta descrita la funcion

	#include <termios.h>
	//#include <stdio.h> // la ncurses ya incluye la stdio.h

	static struct termios old, newt; // creamos dos nuevos tipos de consola, antigua y nueva

	// Inicializamos la configuración de la consola
	void initTermios(int echo)
	{
	  tcgetattr(0, &old); // guardamos en old la configuración antigua de la consola, en este caso la predeterminada por el OS
	  newt = old; // copiamos en newt los predeterminados de old
	  newt.c_lflag &= ~ICANON; // seteamos el buffer de datos de entrada a 0 para que no guarde ningún caracter y no espere a in intro
	  if (echo) {	// si echo es 0 no se imprimirá, si es uno, se mostrá lo que se escriba en pantalla
		  newt.c_lflag |= ECHO; // ECHO activo
	  } else {
		  newt.c_lflag &= ~ECHO; // ECHO DESactivado
	  }
	  tcsetattr(0, TCSANOW, &newt); // seteamos los valores de la terminal a los nuevos
	}

	// Reseteamos los valores de la terminal a los predefinidos por el OS
	void resetTermios(void)
	{
	  tcsetattr(0, TCSANOW, &old);
	}

	// Lee un caracter en ch
	// 'echo' define si se mostrará el caracter por pantalla o no
	char getch_(int echo)
	{
	  char ch;	// declaramos el caracter
	  initTermios(echo); // declaramos la nueva terminal con buffer de entrada 0
	  ch = getchar(); // leemos el buffer de datos
	  resetTermios(); // reseteamos la terminal, con su buffer original
	  return ch; // devuelve el caracter
	}

	// Lee un caracter, seteando la opcion de echo a 0, eliminando la opcion de imprimir caracteres de manera nativa
	char getch(void)
	{
	  return getch_(0);
	}

	// Lee un caracter, seteando la opcion de echo a 1, permitiendo la impresion de manera nativa
	char getche(void)
	{
	  return getch_(1);
	}

#endif

using namespace std;

void _Sleep( double x = 1 )
{
	for( unsigned int i = 0; i < x*1000000000; i++); // espera un segundo
}


int main ( int argn, char *argv[] )
{
	string nombre_archivo = "hola.txt"; // esta linea es solo para hacer pruebas
	cout << "Nombre del archivo: ";
	getline( cin , nombre_archivo );
	#ifdef _WIN32
		system( "cls" );
	#endif
	#ifdef _UNIX_
		system( "clear" );
	#endif
	ifstream tryopen_file;
	tryopen_file.open( nombre_archivo.c_str() );
	ofstream file;
	string str_salida;
	string str_aux;
	if( tryopen_file.is_open() )
	{
		while( !tryopen_file.eof() )
		{
			getline( tryopen_file, str_aux );
			str_salida += str_aux;
			str_salida += '\n';
			cout << " ~ | " << str_aux << endl;
		}
		str_salida.resize( str_salida.size() - 1 );
		str_salida += '\n';
		cout << " ~ | ";
		tryopen_file.close();
		file.open( nombre_archivo.c_str(), ios::ate );
	}
	else
	{
		tryopen_file.close();
	}

	/*	-- Special Keys --
	@{
	*/

		const int BACKSPACE = 8;
		const char _BACKSPACE = BACKSPACE; // para borrar
		const int EXIT = 27;
		const int SAVE = 19;
		const int LINE_JUMP = 13;
		const int COMPILE = 67; // ( usaré f9 de proposito general )

	/*
	@}
	*/

	char keypress;
	unsigned int linepos = 0;
	long unsigned int str_pos = str_salida.size();

	while( true )
	{
		keypress = getch();
		//printf("%d\n", keypress);

		switch (keypress)
		{
			case LINE_JUMP:
				str_salida += '\n';
							   //01234
				cout << '\n' << " ~ | ";
				linepos = 0;
			break;
			case BACKSPACE:
				if( str_salida.size() > 0 && linepos > 0 ){
					str_salida.resize( str_salida.size() - 1 );
					cout << _BACKSPACE;
					cout << " ";
					cout << _BACKSPACE;
					linepos--;
				}
				else if( str_salida.size() > 0 && linepos == 0)
				{
					#ifdef _WIN32
						system( "cls" );
					#endif
					#ifdef _UNIX_
						system( "clear" );
					#endif
					str_salida.resize( str_salida.size() - 1 );
					linepos = 0;
					unsigned int i, j = i = 0;
					j = str_salida.size();
					cout << " ~ | ";
					for( i=0; i < j; i++){
						if( str_salida[i]=='\n' )
							cout << str_salida[i] << " ~ | ";
						else
							cout << str_salida[i];
					}
					i=j;
					while(i>=0)
					{
						i--;
						if( str_salida[i]=='\n')
							break;
						linepos++;
					}
				}
			break;
			case SAVE:
				if( !file.is_open() )
					file.open( nombre_archivo.c_str() );
				file << str_salida;
				file.close();
					   //12345678901234567890
				cout << "  -  Saving...  -  ";
				for( unsigned int i = 0; i < 19 ; i++){
					_Sleep(0.003);
					cout << _BACKSPACE;
					cout << " ";
					cout << _BACKSPACE;
				}
			break;
			case EXIT:
				if( !file.is_open() )
					file.open( nombre_archivo.c_str() );
				file << str_salida;
				file.close();
				return 0;
			break;
			case COMPILE:
				//syste( g++ -o [nombre del archivo] [combre del archivo].[cc|cpp]) ;
			break;
			default:
				linepos++;
				cout << keypress;
				str_pos = str_salida.size();
				str_salida.insert( str_salida.begin() + str_pos , keypress );
			break;
		}
	}
	return 0;
}
//derecha = àM
//izquierda = àK
//arriba = àH
//abajo = àP

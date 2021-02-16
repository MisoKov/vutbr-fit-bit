/**
 *	@file	tail2.cc		
 *	@author	Michal Koval, FIT
 *	@date	12.04.2019
 *	@brief	Zdrojovy kod programu tail v C++
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad a)
 *			reachable leaks.........	
 */		

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cmath>
/**
 * @def DEFAULT_PRINTED_LINES
 * @brief Implicitny pocet tlacenych riadkov
 */
#define DEFAULT_PRINTED_LINES 10

using namespace std;
/**
 * @brief      Spracuje argumenty
 *
 * @param[in]  argc  Pocet argumentov
 * @param[in]      argv  Pole argumentob
 * @param[in,out]      n     { parameter_description }
 *
 * @return     Meno subora na citanie
 */
string process_args(int argc, char* argv[], long *n)
{
	string filename;
	if (argc < 1 || argc > 4){
		cerr << "Zadany nespravny pocet argumentov: " << argc <<endl;
		return filename;
	// prepinac
	} else if (argc == 3 || argc == 4){
		string temp;
		temp.assign(argv[1]);
		if (temp == "-n"){
			// zikanie cisla z 2. parametru
			temp.assign(argv[2]);
			try{
				*n = stol(temp, nullptr, 10);
			}
			catch (...){
				cerr << "Argument pre prepinac -n nie je cislo: " << argv[2];
				return filename;
			}
			long n_len = floor(log10(abs(*n))) + 1;
			if (argv[2][0] == '+' || argv[2][0] == '-'){
				n_len++;
			}
			temp.assign(argv[2]);
			long arg_len = temp.length();
			// osetrenie nespravneho cisla pre -n
			if (n_len != arg_len){
				cerr << "Argument pre prepinac -n nie je cislo: " << argv[2] << endl;
				*n = -1;
				return filename;
			} else if (*n < 1){
				cerr << "Argument pre prepinac -n musi byt > 0, je: " << argv[2] << endl;
				*n = -1;
				return filename;
			}
			if (argc == 4){
				filename = argv[3];
			}
		} else {
			cerr << "Zadany nespravny prepinac: " << argv[1] << endl;
			*n = -1; 
			return filename;
		}
	// bez prepinaca
	} else if (argc == 2){
		filename = argv[1];
	}
	return filename;
}
/**
 * @brief      Cita riadky textu zo streamu
 *
 * @param[in]      stream  Stream, ktoreho citame
 *
 * @return     Fronta s nacitami riadkami
 */
queue<string> read_lines(istream& stream)
{
	// citanie a ukladanie riadkov
	queue<string> lines;
	string temp;
	while (getline(stream, temp)){
		lines.push(temp);
	}
	return lines;
}
/**
 * @brief      Cita riadky texty zo suboru
 *
 * @param[in]  filename  Meno suboru
 *
 * @return     Fronta s nacitanymi riadkami
 */
queue<string>  read_file(string filename)
{
	queue<string> lines;
	// otvorenie suboru
	ifstream file(filename);
	if (! file){
		cerr << "Subor " << filename <<" sa nepodarilo otvorit" << endl;
		return lines;
	}
	lines = read_lines(file);
	// uzavretie suboru
	file.close();
	return lines;
}

/**
 * @brief      Vytlaci poslednych n riadkov
 *
 * @param[in]  lines  Fronta riadkov text
 * @param[in]  n      Pocet riadkov na tlacenie
 */
void print_result(queue<string> lines, long n)
{ 
	long len = lines.size();
	// ak je pocet riadkov mensi ako n vypisu sa vsetky riadky
	if (len < n){
		for (int i = 0; i < len; i++){
			cout << lines.front() << endl;
			lines.pop();
		}
	// vypis n poslednych riadkov
	} else {
		for (int i = 0; i < len - n; i++){
			lines.pop();
		}
		for (int i = 0; i < n; i++){
			cout << lines.front() << endl;
			lines.pop();
		}
	}
}

int main (int argc, char *argv[])
{
	std::ios::sync_with_stdio(false); // sposobuje reachable leaky
	//osetrenie argumentov
	long n = DEFAULT_PRINTED_LINES;
	string filename = process_args(argc, argv, &n);
	if (n == -1){
		return 1;
	}
	// rozdelenie textu do riadkov
	queue<string> lines;
	if (!filename.empty()){
		// nezadana cesta - vypis zo stdin
		lines = read_file(filename);
	} else {
		// zadana cesta - text zo suboru
		lines = read_lines(cin);
	}
	// kontrola chyb
	if (lines.empty()){
		return 1;
	}
	// vypis poslednych riadkov
	print_result(lines, n);

	return 0;
}
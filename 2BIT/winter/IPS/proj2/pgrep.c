/**
 * Project: IPS2 - Paralelní GREP
 *
 * @brief Zdrojovy kod implementacie paralelneho GREPu.
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>
#include <regex>

using namespace std;

// protoypy funkcii
char *read_line(int *res);
void f(int ID);
int process_args(void);

// globalne premenne
vector<mutex *> thread_start; /* pole zamku promenne velikosti */
vector<mutex *> thread_finish;
mutex score_m;
vector <thread *> threads; /* pole threadu promenne velikosti */

int g_argc;
char **g_argv;
char *line;
int vstup;
int final_score;

char *read_line(int *res){
	string line;
	char *str;
	if (getline(cin, line)){
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res=1;
		return str;
	} else {
		*res=0;
		return NULL;
	}

}

void f(int ID){
	int re_sc = atoi(g_argv[3+(ID*2)]);
	regex re(g_argv[2+(ID*2)]);
	(*(thread_start[ID])).lock();
	while (vstup){
		if(regex_match(line, re)){
			score_m.lock();
			final_score += re_sc;
			score_m.unlock();
		}
		(*(thread_finish[ID])).unlock();
		(*(thread_start[ID])).lock();
	}
}

int process_args(void){
	// kontrola argumentov
	if (g_argc % 2 != 0){
		fprintf(stderr, "ERROR: Zadany parny pocet argumentov\n");
		fprintf(stderr, "Format: pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...\n" );
		return 1;
	} else if (g_argc < 4){
		fprintf(stderr, "ERROR: Zadany maly pocet argumentov\n");
		fprintf(stderr, "Format: pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...\n" );
		return 1;
	}
	return 0;
}

int init_m(int n){
	/* vytvorime zamky */
	thread_start.resize(n); /* nastavime si velikost pole zamky */
	thread_finish.resize(n);
	mutex *m;
	for(int i = 0; i < n; i++){	
		m = new mutex();
		if (m == NULL){
			fprintf(stderr, "ERROR: Mutex sa nepodarilo vytvorit\n");
			return 1;
		}
		thread_start[i]= m;
		m = new mutex();
		if (m == NULL){
			fprintf(stderr, "ERROR: Mutex sa nepodarilo vytvorit\n");
			return 1;
		}
		thread_finish[i]= m;
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
		(*(thread_start[i])).lock();
		(*(thread_finish[i])).lock();
	}
	return 0;
}

int init_thread(int n){
	threads.resize(n); /* nastavime si velikost pole threads */
	thread *new_t;
	for(int i = 0; i < n; i++){
		new_t = new thread (f, i);
		if (new_t == NULL){
			fprintf(stderr, "ERROR: Thread sa nepodarilo vytvorit\n");
			return 1;
		}
		threads[i]=new_t;
	}
	return 0;
}

void free_m(int n){
	for(int i = 0; i < n; i++){
		delete thread_start[i];
		delete thread_finish[i];
	}
}

int main (int argc, char *argv[]){
	// kontrola argumentov
	g_argc = argc;
	g_argv = argv;
	if (process_args()){
		return 1;
	}
	// inizializacia mutexov
	int num_regex=(argc-2)/2;
	if (init_m(num_regex)){
		return 1;
	}
	// inicializacia threadov
	if (init_thread(num_regex)){
		// uvolnenie zamkov
		return 1;
	}
	// citanie riadku zo vstupu a matchovanie s regexmi
	line=read_line(&vstup);
	int req_sc = atoi(argv[1]);
	while (vstup) {
		final_score = 0;
		// thready cakaju na nacitanie riadku od mainu
		for (int i = 0; i < num_regex; i++){
			(*(thread_start[i])).unlock();
		}
		// main caka na spracovanie riadku threadmi
		for (int i = 0; i < num_regex; i++){
			(*(thread_finish[i])).lock();
		}
		if (final_score >= req_sc){ 
			printf("%s\n",line);
		}
		free(line); /* uvolnim pamet */
		line=read_line(&vstup);
	}
	// unlock po ukonceni while pre ukoncenie threadov
	for(int i = 0; i < num_regex; i++){
		(*(thread_start[i])).unlock();
	}
	// join threadov
	for(int i = 0; i < num_regex; i++){
		(*(threads[i])).join();
		delete threads[i];
	}
	// dealokacia mutexov
	free_m(num_regex);
	return 0;
}

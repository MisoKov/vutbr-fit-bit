/**
 *	@file	proj2.h
 *	@author	Michal Koval, xkoval17, FIT VUT
 *	@date	18.04.2019
 *	@brief	Header file s prototypmi funkcii pre proj2.c
 *	@note	Compiler: gcc 7.3
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

/**
 * Pozadovany pocet argumentov
 */
#define NUM_OF_ARGS 6
/**
 * Minimalna doma v ms na navrat k molu
 */
#define MIN_WAIT_TIME 20

/**
 * Meno zdielaneho objektu
 */
#define shmNAME "/xkoval17-ios-proj2-shm"
/**
 * Meno semaforu
 */
#define semNAME "/xkoval17-ios-proj2-sem"
/**
 * Meno semaforu
 */
#define semMUTEXNAME "/xkoval17-ios-proj2-mutex"

/**
 * @brief      Makro funckia pre namapovanie zdielanej premennej
 *
 * @param      pointer  Pointer na premennu
 *
 * @return     Pointer na zdielanu premennu alebo v pripade chyby MAP_FAILED
 */
#define MMAP(pointer) (pointer = mmap(NULL, sizeof(*pointer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))
/**
 * @brief      Makro funkcia na odmapovanie zdielanej premennej
 *
 * @param      pointer  Pointer na premennu
 *
 * @return     0, v pripade chyby -1
 */
#define UNMAP(pointer) (munmap(pointer, sizeof(pointer)))
/**
 * @brief      Makro funkcia na vyratanie random cisla od nula po max
 *
 * @param      max   Maximalna hodnota cisla
 *
 * @return     Nahodny interger od nula po max
 */
#define DELAY(max) (max == 0 ? 0 : (rand() % max))
/**
 * @brief      Makro funkcia na zapisa do mutex suboru
 *
 * @param      a       Poradove cislo akcie
 * @param      person  Typ procesu, hacker/serf
 * @param      i       Identifikacne cislo procesu
 * @param      action  Akcia
 * @param      nh      Pocet hackerov na mole
 * @param      ns      Pocet serfov na mole
 */
#define FILE_PRINT(a,person,i,action,nh,ns) do{ \
	sem_wait(s_file_access); \
	*a_counter += 1; \
	if (nh < 0 || ns < 0){ \
		fprintf(out_file, "%d\t: %s %d\t: %s\n", *a_counter, person, i, action); \
	} else{ \
		fprintf(out_file, "%d\t: %s %d\t: %s\t: %d\t: %d\n", *a_counter, person, i, action, nh, ns); \
	} \
	fflush(out_file); \
	sem_post(s_file_access); \
	} while (0)

/**
 * Datovy typ pre strukturu bariey
 */
typedef struct {
    sem_t *sem;
    sem_t *mutex;
    int *shm;
    int size;
} barrier_t;

/**
 * Deklaracia pointrov pre zdielane premenne semaforov
 */
sem_t *s_file_access,
	  *s_molo,
	  *s_mutex,
	  *s_hacs,
	  *s_serfs,
	  *s_allOut,
	  *s_boarding,
	  *s_boarded = NULL;

/**
 * Deklaracia zdielanych premennych
 */
int *molo_hacs,
	*molo_serfs,
	*molo_people,
	*a_counter,
	*on_board = NULL;

/**
 * Deklaracia premennej pre vystupny subor
 */
FILE *out_file;

/**
 * Datovy typ struktury pre spracovanie vstupnych argumentov
 */
typedef struct args {
	long p;
	long h;
	long s;
	long r;
	long w;
	long c;
}Args;


/**
 * @brief      Spracuje a skontroluje argumenty
 *
 * @param[in]  argc  pocet argumentov
 * @param[in]  argv  Pole vstupnych argumnetov
 *
 * @return     Vrati pointer na strukturu celociselnych argumentov alebo NULL v pripade chyby
 */
Args* process_arguments(int argc, char *argv[]);

/**
 * @brief      Kontroluje ci su jedtlive argumenty v spravnom rozmedzi cisel
 *
 * @param[in]  args  Pole celociselnych argumetov
 *
 * @return     0 v priprade ze argumenty su spravne, 1 v pripade najdenej chyby
 */
int check_args(long *args);

/**
 * @brief      Uvolni vsetky data a resources pouzite v programe, vola podfunkcie na uvolnenie
 *
 * @param[in]      args  Pointer na strukturu vstupnych argumentov
 *
 * @return     0 v pripade uspesneho uvolnenia, 1 ak nastala niekde chyba
 */
int cleanup_all(Args *args);

/**
 * @brief      Funkcia volajuca jednotlive inicializacne funkcie
 *
 * @return     0 v pripade ze vsetky inizializacie dopadli uspesne, v opacnom pripade 1
 */
int init_all(void);

/**
 * @brief      Proces pre serfov
 *
 * @param[in]  p     Počet generovanych osob
 * @param[in]  s     Max doba v ms, po které je generován nový proces serfs
 * @param[in]  r     Max doba v ms plavby
 * @param[in]  w     Max doba v ms, po které se osoba vrací zpět na molo
 * @param[in]  c     Kapacita mola
 */
void pr_serfs(long p, long s, long r, long w, long c, Args *args);

/**
 * @brief      Proces pre hackerov
 *
 * @param[in]  p     Počet generovanych osob
 * @param[in]  h     Max doba v ms, po které je generován nový proces hackers
 * @param[in]  r     Max doba v ms plavby
 * @param[in]  w     Max doba v ms, po které se osoba vrací zpět na molo
 * @param[in]  c     Kapacita mola
 */
void pr_hackers(long p, long h, long r, long w, long c, Args *args);

/**
 * @brief      Inicializacia zdielanych premennych
 *
 * @return     0 v pripade uspesnej inizializacie vsetkych zdielanych premennych, v opacnom pripade 1
 */
int init_shared(void);

/**
 * @brief      Inicializacia semaforov
 *
 * @return     0 v pripade uspesnej inizializacie vsetkych semaforov, v opacnom pripade 1
 */
int init_semaphores(void);

/**
 * @brief      Uvolnenie semaforov
 *
 * @return     0 v pripade uspesneho uvolnenia vsetkych semaphorov, v pripade chyby 1
 */
int destroyer_of_semaphores(void);

/**
 * @brief      Odmapovanie zdielanych premennych 
 *
 * @return     0 v pripade uspesneho odmapovania vsetkych zdielanych premennych, v pripade chyby 1
 */
int unmap_all(void);

/**
 * @brief      Funkcia pre serfa na vytvorenie posadky
 *
 * @param[in]  i     Identifikator serfa
 *
 * @return     1 v pripade ze process je kapitan, inac 0
 */
int serfs_create_group(int i);

/**
 * @brief      Funkcia pre hackera na vytvorenie posadky
 *
 * @param[in]  i     Identifikator hackera
 *
 * @return     1 v pripade ze process je kapitan, inac 0
 */
int hackers_create_group(int i);

/**
 * @brief      Rekurzivne pokusanie sa o vstup na molo
 *
 * @param[in]  person  Identifikacne cislo, hacker == 0, serf == 1
 * @param[in]  i       Identifikator procesu
 * @param[in]  c       Kapacita mola
 * @param[in]  w       Minimalna doba vratenia sa k molu
 */
void try_to_enter_molo(int person, int i, int c, int w);

/**
 * @brief      Spojenie procesov bariery
 *
 * @param[in,out]      barrier  Pointer na barieru
 */
void bar_join(barrier_t *barrier);

/**
 * @brief      Otvorenie bariery
 *
 * @param[in]      barrier  Pointer na barrieru
 * @param[in]  size     Velkost bariery
 */
void bar_open(barrier_t *barrier, int size);

/**
 * @brief      Zatvorenie bariery
 *
 * @param[in,out]      barrier  The barrier
 */
void bar_close(barrier_t *barrier);

/**
 * @brief      Inicializacia barriery
 *
 * @param[in]      barrier  Pointer na barrieru
 */
void bar_init(barrier_t *barrier);

/**
 * @brief      Odmapovanie zdielannych premmenych bariery
 */
void bar_unlink();
/**
 *	@file	proj2.c	
 *	@author	Michal Koval, xkoval17, FIT VUT
 *	@date	18.04.2019
 *	@brief	IOS projekt 2: implementacia synchronizačního problému River Crossing
 *	@see    http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf
 *	@note	Compiler: gcc 7.3
 */

#include "proj2.h"

void bar_join(barrier_t *barrier)
{
    sem_wait(barrier->mutex);
    barrier->shm[0]++;
    if (*barrier->shm == barrier->size) {
        for (int i = 0; i < barrier->size-1; i++) {
            sem_post(barrier->sem);
        }
        barrier->shm[0] = 0;
        sem_post(barrier->mutex);
    } else {
        sem_post(barrier->mutex);
        sem_wait(barrier->sem);
    }
}

void bar_open(barrier_t *barrier, int size)
{
    if ((barrier->sem = sem_open(semNAME, O_RDWR)) == SEM_FAILED){
    	fprintf(stderr, "Error: bar_open: Zlyhanie otvorenia semaforu barriery\n");
    }
    if ((barrier->mutex = sem_open(semMUTEXNAME, O_RDWR)) == SEM_FAILED){
    	fprintf(stderr, "Error: bar_open: Zlyhanie otvorenia semaforu barriery\n");
    }
    barrier->size = size;

    int shmID;
    if ((shmID = shm_open(shmNAME, O_RDWR, S_IRUSR | S_IWUSR)) == -1){
    	fprintf(stderr, "Error: bar_open: Zlyhanie otvorenia zdielaneho objektu\n");
    }
    if ((barrier->shm = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0)) == MAP_FAILED){
    	fprintf(stderr, "Error: bar_open: Zlyhanie nampovania zdielanej premennej\n");
    }
    close(shmID);
}

void bar_close(barrier_t *barrier)
{
    sem_close(barrier->sem);
    sem_close(barrier->mutex);
    munmap(barrier->shm, sizeof(int));
}

void bar_init(barrier_t *barrier)
{
    int shmID;
    if ((shmID = shm_open(shmNAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR)) == -1){
    	fprintf(stderr, "Error: bar_init: Zlyhanie otvorenia zdielanej premennej\n");
    }
    ftruncate(shmID, sizeof(int));
    if ((barrier->shm = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0)) == MAP_FAILED){
       	fprintf(stderr, "Error: bar_init: Zlyhanie nampovania zdielanej premennej\n");
    }
    close(shmID);
    barrier->shm[0] = 0;
    munmap(barrier->shm, sizeof(int));

    if ((barrier->mutex = sem_open(semMUTEXNAME, O_CREAT, 0666, 1)) == SEM_FAILED){
    	fprintf(stderr, "Error: bar_init: Zlyhanie otvorenia semaforu barriery\n");
    }
    if ((barrier->sem = sem_open(semNAME, O_CREAT, 0666, 0)) == SEM_FAILED){
    	fprintf(stderr, "Error: bar_init: Zlyhanie otvorenia semaforu barriery\n");
    }
    sem_close(barrier->mutex);
    sem_close(barrier->sem);
}

void bar_unlink()
{
    shm_unlink(shmNAME);
    sem_unlink(semMUTEXNAME);
    sem_unlink(semNAME);
}

int init_all(void)
{
	// otvorenie suboru
	out_file = fopen("proj2.out", "w");
	if (out_file == NULL){
		fprintf(stderr, "Error: init: Nepodarilo sa otvorit output subor\n");
		return 1;
	}
	// inicializacia zdielanych premennych
	if (init_shared() == 1){
		return 1;
	}
	// inicializacia semaforov
	if (init_semaphores() == 1){
		return 1;
	}
	// vytvorime a inicializujeme barieru
    barrier_t barrier;
    bar_init(&barrier);
	*a_counter = 0;
	*on_board = 0;
	*molo_hacs = 0;
	*molo_serfs = 0;
	*molo_people = 0;
	return 0;
}
int init_shared(void)
{
	if (MMAP(molo_hacs) == MAP_FAILED){
		fprintf(stderr, "Error: init_shared: Chyba pri mapovani premennej 'molo_hacs'\n");
		return 1;
	}
	if (MMAP(molo_serfs) == MAP_FAILED){
		fprintf(stderr, "Error: init_shared: Chyba pri mapovani premennej 'molo_serfs'\n");
		return 1;
	}
	if (MMAP(molo_people) == MAP_FAILED){
		fprintf(stderr, "Error: init_shared: Chyba pri mapovani premennej 'molo_people'\n");
		return 1;
	}
	if (MMAP(a_counter) == MAP_FAILED){
		fprintf(stderr, "Error: init_shared: Chyba pri mapovani premennej 'a_counter'\n");
		return 1;
	}
	if (MMAP(on_board) == MAP_FAILED){
		fprintf(stderr, "Error: init_shared: Chyba pri mapovani premennej 'on_board'\n");
		return 1;
	}			
	return 0;
}
int init_semaphores(void)
{
	// mapovanie
	if (MMAP(s_file_access) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_file_access'\n");
		return 1;
	}
	if (MMAP(s_molo) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_molo'\n");
		return 1;
	}
	if (MMAP(s_hacs) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_hacs'\n");
		return 1;
	}
	if (MMAP(s_serfs) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_serfs'\n");
		return 1;
	}
	if (MMAP(s_mutex) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_mutex'\n");
		return 1;
	}
	if (MMAP(s_allOut) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_allOut'\n");
		return 1;
	}
	if (MMAP(s_boarding) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_boarding'\n");
		return 1;
	}
	if (MMAP(s_boarded) == MAP_FAILED){
		fprintf(stderr, "Error: init_semaphores: Chyba pri mapovani semaforu 's_boarded'\n");
		return 1;
	}
	// inicializacia
	if (sem_init(s_file_access,1,1) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_file_access'\n");
		return 1;
	}
	if (sem_init(s_molo,1,1) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_molo'\n");
		return 1;
	}
	if (sem_init(s_hacs,1,0) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_hacs'\n");
		return 1;
	}
	if (sem_init(s_serfs,1,0) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_serfs'\n");
		return 1;
	}
	if (sem_init(s_mutex,1,1) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_mutex'\n");
		return 1;
	}
	if (sem_init(s_boarding,1,1) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_boarding'\n");
		return 1;
	}
	if (sem_init(s_allOut,1,0) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_allOut'\n");
		return 1;
	}
	if (sem_init(s_boarded,1,0) == -1){
		fprintf(stderr, "Error: init_semaphores: Chyba pri inicializovani semaforu 's_boarded'\n");
		return 1;
	}
	return 0;
}
int cleanup_all(Args *args)
{
	int err = 0;
	// zatvorenie suboru
	if (fclose(out_file) == EOF){
		fprintf(stderr, "Error: cleanup_all: Nepodarilo sa zavriet output subor\n");
		err = 1;
	}
	// zatvorenie semaforu
	if (destroyer_of_semaphores() == 1){
		err = 1;
	}
	// unmap
	if (unmap_all() == 1){
		err = 1;
	}
	// dealokovanie struktury argumentov
	free(args);

	bar_unlink();

	return err;
}
int destroyer_of_semaphores(void)
{
	int err = 0;
	if (sem_destroy(s_file_access) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_file_accsess'\n");
		err = 1;
	}
	if (sem_destroy(s_molo) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_molo'\n");
		err = 1;
	}
	if (sem_destroy(s_serfs) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_serfs'\n");
		err = 1;
	}
	if (sem_destroy(s_hacs) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_hacs'\n");
		err = 1;
	}
	if (sem_destroy(s_mutex) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_mutex'\n");
		err = 1;
	}
	if (sem_destroy(s_allOut) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_allOut'\n");
		err = 1;
	}
	if (sem_destroy(s_boarding) == -1){
		fprintf(stderr, "Error: destroyer_of_semaphores: Nepodarilo za uvolnit semafor 's_boarding'\n");
		err = 1;
	}
	return err;
}
int unmap_all(void)
{
	int err = 0;
	if (UNMAP(molo_hacs) == -1){
		fprintf(stderr, "Error: unmap_all: Nepodarilo za odmapovat 'molo_hacs'\n");
		err = 1;
	}
	if (UNMAP(molo_serfs) == -1){
		fprintf(stderr, "Error: unmap_all: Nepodarilo za odmapovat 'molo_serfs'\n");
		err = 1;
	}
	if (UNMAP(molo_people) == -1){
		fprintf(stderr, "Error: unmap_all: Nepodarilo za odmapovat 'molo_people'\n");
		err = 1;
	}
	if (UNMAP(a_counter) == -1){
		fprintf(stderr, "Error: unmap_all: Nepodarilo za odmapovat 'serfs'\n");
		err = 1;
	}
	return err;
}
int check_args(long *args)
{
	// P >= 2 && (P % 2) == 0
	if(args[0] < 2 || (args[0]%2 == 1)){
		fprintf(stderr, "Argument P: %ld, musi splnat P >= 2 && (P %% 2) == 0\n", args[0]);
		return 1;
	// H >= 0 && H <= 2000
	} else if (args[1] < 0 || args[1] > 2000){
		fprintf(stderr, "Argument H: %ld, musi splnat H >= 0 && H <= 2000\n", args[1]);
		return 1;
	// S >= 0 && S <= 2000
	} else if (args[2] < 0 || args[2] > 2000){
		fprintf(stderr, "Argument S: %ld, musi splnat S >= 0 && S <= 2000\n", args[2]);
		return 1;
	// R >= 0 && R <= 2000
	} else if (args[3] < 0 || args[3] > 2000){
		fprintf(stderr, "Argument R: %ld, musi splnat R >= 0 && R <= 2000\n", args[3]);
		return 1;
	// W >= 20 && W <= 2000
	} else if (args[4] < 20 || args[4] > 2000){
		fprintf(stderr, "Argument W: %ld, musi splnat W >= 20 && W <= 2000\n", args[4]);
		return 1;
	// C >= 5
	} else if (args[5] < 5){
		fprintf(stderr, "Argument C: %ld, musi splnat C >= 5\n", args[5]);
		return 1;

	}
	return 0;
}
Args* process_arguments(int argc, char *argv[])
{
	// kontrola poctu vstupov
	if (argc != 7){
		fprintf(stderr, "Error: process_arguments: Zadany nespravny pocet argumentov: %d, ma ich byt 6\n", argc-1);
		return NULL;
	}
	// alokacia pola pre predanie pracovanych argumentov
	long* arg_arr = malloc (sizeof(long)*NUM_OF_ARGS);
	if (arg_arr == NULL){
		fprintf(stderr, "Error: process_arguments: Chyba alokacie pamate\n");
		return NULL;
	}
	// prevod argumentov na int a vlozenie do pola
	// argumenty v poli v poradi P H S R W C, ako by mali byt aj na vstupe
	char *str_end;
	for (int i = 1; i < argc; i++){
		arg_arr[i-1] = strtol(argv[i], &str_end, 10);
		if (*str_end != '\0'){
			fprintf(stderr, "Error: process_arguments: Argument '%s' zadany v nespravnom formate, ma to byt cele cislo\n", argv[i]);
			free(arg_arr);
			return NULL;
		}
	}
	// kontrola argumentov
	if (check_args(arg_arr) == 1){
		free(arg_arr);
		return NULL;
	}
	// struktura pre odovzdanie argumentov
	Args *args = malloc(sizeof(Args));
	args->p = arg_arr[0];
	args->h = arg_arr[1];
	args->s = arg_arr[2];
	args->r = arg_arr[3];
	args->w = arg_arr[4];
	args->c = arg_arr[5];

	free(arg_arr);
	return args;
}
void try_to_enter_molo(int person, int i, int c, int w)
{	
	sem_wait(s_molo);
	if (*molo_people < c){
		// vstupenie na molo a cakanie
		*molo_people += 1;
		if (person == 0){
			*molo_hacs += 1;
			FILE_PRINT(a_counter, "HACK", i, "waits", *molo_hacs, *molo_serfs);
		} else {
			*molo_serfs += 1;
			FILE_PRINT(a_counter, "SERF", i, "waits", *molo_hacs, *molo_serfs);
		}
		sem_post(s_molo);
	} else {
		if (person == 0){
			FILE_PRINT(a_counter, "HACK", i, "leaves queue", *molo_hacs, *molo_serfs);
		} else {
			FILE_PRINT(a_counter, "SERF", i, "leaves queue", *molo_hacs, *molo_serfs);
		}
		sem_post(s_molo);
		usleep(DELAY(w-MIN_WAIT_TIME+1)+MIN_WAIT_TIME);
		// opatovny pokus o vstup na molo - rekurzia
		try_to_enter_molo(person, i, c, w);
	}
}
int hackers_create_group(int i)
{
	int ret_captain = 0;
	// vytvorenie skupiny
	sem_wait(s_mutex);
	if (*molo_hacs >= 4){
		// 4 hackery
		sem_wait(s_molo);
		*molo_hacs -= 4;
		*molo_people -= 4;
		ret_captain = 1;
		FILE_PRINT(a_counter, "HACK", i, "boards", *molo_hacs, *molo_serfs);
		sem_post(s_molo);
		for (int i = 0; i < 4; i++) sem_post(s_hacs);
	} else if (*molo_serfs >= 2 && *molo_hacs >= 2){
		// 2 hackery 2 serfy
		sem_wait(s_molo);
		*molo_hacs -= 2;
		*molo_serfs -= 2;
		*molo_people -= 4;
		ret_captain = 1;
		FILE_PRINT(a_counter, "HACK", i, "boards", *molo_hacs, *molo_serfs);
		sem_post(s_molo);
		for (int i = 0; i < 2; i++) sem_post(s_hacs);
		for (int i = 0; i < 2; i++) sem_post(s_serfs);
	} else {
		sem_post(s_mutex);
	}
	return ret_captain;
}
int serfs_create_group(int i)
{
	int ret_captain = 0;
	// vytvorenie skupiny
	sem_wait(s_mutex);
	if (*molo_serfs >= 4){
		// 4 hackery
		sem_wait(s_molo);
		*molo_serfs -= 4;
		*molo_people -= 4;
		ret_captain = 1;
		FILE_PRINT(a_counter, "SERF", i, "boards", *molo_hacs, *molo_serfs);
		sem_post(s_molo);
		for (int i = 0; i < 4; i++) sem_post(s_hacs);
	} else if (*molo_serfs >= 2 && *molo_hacs >= 2){
		// 2 hackery 2 serfy
		sem_wait(s_molo);
		*molo_hacs -= 2;
		*molo_serfs -= 2;
		*molo_people -= 4;
		ret_captain = 1;
		FILE_PRINT(a_counter, "SERF", i, "boards", *molo_hacs, *molo_serfs);
		sem_post(s_molo);
		for (int i = 0; i < 2; i++) sem_post(s_hacs);
		for (int i = 0; i < 2; i++) sem_post(s_serfs);
	} else {
		sem_post(s_mutex);
	}
	return ret_captain;
}
void pr_hackers(long p, long h, long r, long w, long c, Args *args)
{
	pid_t pid;
	pid_t *children_processes = malloc(sizeof(pid_t)*p);
	for (int i = 1; i <= p; i++){
		int isCaptain;
		usleep(DELAY(h));
		pid = fork();
		if (pid == 0){
			// child - proces hackera
			// zapis start
			FILE_PRINT(a_counter, "HACK", i, "starts", -1, -1);
			// pokus vstupit na molo
			try_to_enter_molo(0, i, c ,w); // 0 == HACK
			// vytvorenie skupiny
			isCaptain = hackers_create_group(i);
			// nalodenie
			sem_wait(s_hacs);
			barrier_t barrier;
		    bar_open(&barrier, 4);
		    sem_wait(s_boarding);
		    *on_board += 1;
		    if (*on_board == 4){
		    	sem_post(s_boarded);
		    }
		    sem_post(s_boarding);
		    if (isCaptain){
		    	sem_wait(s_boarded);
		    	printf("%d kapitan na lodke\n", i);
				usleep(DELAY(r)); // plavba
			} else {
				printf("%d cestujuci na lodke\n", i);
			}
		    bar_join(&barrier);
		    // vylodenie
		    if (!isCaptain) {
		    	sem_wait(s_molo);
		    	FILE_PRINT(a_counter, "HACK", i, "member exits", *molo_hacs, *molo_serfs);
		    	sem_post(s_molo);
		    	sem_wait(s_boarding);
		    	*on_board -= 1;
		    	if (*on_board == 1){
		    		sem_post(s_allOut);
		    	}
		    	sem_post(s_boarding);
		    } else {
		    	sem_wait(s_allOut);
		    	sem_wait(s_boarding);
		    	*on_board -= 1;
		  		sem_post(s_boarding);
				FILE_PRINT(a_counter, "HACK", i, "captain exits", *molo_hacs, *molo_serfs);
				sem_post(s_mutex);
			}
		    bar_close(&barrier);
			// ukoncenie processu
			exit(0);
		} else if (pid > 0){
			// parent
			children_processes[i-1] = pid;
		} else {
			fprintf(stderr, "Error: pr_hackers: fork - Chyba pri vytvarani procesu hacker\n");
			free(children_processes);
			cleanup_all(args);
			kill(0,SIGKILL);
		}
	}
	for (int i = 0; i < p; i++){
		if (waitpid(children_processes[i],NULL,0) == -1){
			fprintf(stderr, "Error: pr_hackers: Potomok s ID %d sa neukoncil\n", children_processes[i]);			
		}
	}
	free(children_processes);
	exit(0);
}
void pr_serfs(long p, long s, long r, long w, long c, Args *args)
{
	pid_t pid;
	pid_t *children_processes = malloc(sizeof(pid_t)*p);
	for (int i = 1; i <= p; i++){
		int isCaptain;
		usleep(DELAY(s));
		pid = fork();
		if (pid == 0){
			// child - proces hackera
			// zapis start
			FILE_PRINT(a_counter, "SERF", i, "starts", -1, -1);
			// pokus vstupit na molo
			try_to_enter_molo(1, i, c ,w); // 1 == SERF
			// vytvorenie skupiny
			isCaptain = serfs_create_group(i);
			// nalodenie
			sem_wait(s_serfs);
			barrier_t barrier;
		    bar_open(&barrier, 4);
		    sem_wait(s_boarding);
		    *on_board += 1;
		    if (*on_board == 4){
		    	sem_post(s_boarded);
		    }
		    sem_post(s_boarding);
		    if (isCaptain){
		    	sem_wait(s_boarded);
		    	printf("%d kapitan na lodke\n", i);
				usleep(DELAY(r)); // plavba
			} else {
				printf("%d cestujuci na lodke\n", i);
			}
		    bar_join(&barrier);
		    // vylodenie
		    if (!isCaptain) {
		    	sem_wait(s_molo);
		    	FILE_PRINT(a_counter, "SERF", i, "member exits", *molo_hacs, *molo_serfs);
		    	sem_post(s_molo);
		    	sem_wait(s_boarding);
		    	*on_board -= 1;
		    	if (*on_board == 1){
		    		sem_post(s_allOut);
		    	}
		    	sem_post(s_boarding);
		    } else {
		    	sem_wait(s_allOut);
		    	sem_wait(s_boarding);
		    	*on_board -= 1;
		  		sem_post(s_boarding);
				FILE_PRINT(a_counter, "SERF", i, "captain exits", *molo_hacs, *molo_serfs);
				sem_post(s_mutex);
			}
		    bar_close(&barrier);
			// ukoncenie processu
			exit(0);
		} else if (pid > 0){
			// parent
			children_processes[i-1] = pid;
		} else {
			fprintf(stderr, "Error: pr_hackers: fork - Chyba pri vytvarani procesu serfs\n");
			free(children_processes);
			cleanup_all(args);
			kill(0,SIGKILL);
		}
	}
	for (int i = 0; i < p; i++){
		if (waitpid(children_processes[i],NULL,0) == -1){
			fprintf(stderr, "Error: pr_hackers: Potomok s ID %d sa neukoncil\n", children_processes[i]);			
		}
	}
	free(children_processes);
	exit(0);
}
int main (int argc, char *argv[])
{
	bar_unlink(); // v pripade predoslej chyby
	// spracovanie argumentov
	Args *args = process_arguments(argc, argv);
	if (args == NULL){
		return 1;
	}
	// inicializacia
	if (init_all() == 1){
		cleanup_all(args);
		return 1;
	}
	// vytvorenie procesov
	pid_t ID_hackers;
	pid_t ID_serfs; 
	ID_hackers = fork();
	if (ID_hackers == 0){
		// child -process hacker
		pr_hackers(args->p, args->h, args->r, args->w, args->c, args);
	} else if (ID_hackers > 0) {
		// parent - main
		ID_serfs = fork();
		if (ID_serfs == 0){
			// child - process serf
			pr_serfs(args->p, args->s, args->r, args->w, args->c, args);
		} else if (ID_serfs < 0){
			// chyba fork
			fprintf(stderr, "Error: main: fork - Chyba pri vytvarani procesu pr_serfs\n");
			cleanup_all(args);
			return 1;
		}
	} else {
		// chyba fork
		fprintf(stderr, "Error: main: fork - Chyba pri vytvarani procesu pr_hackers\n");
		cleanup_all(args);
		return 1;
	}
	// cakanie na ukoncenie procesov
	waitpid(ID_hackers, NULL, 0);
	waitpid(ID_serfs, NULL, 0);
	// upratanie
	if (cleanup_all(args) == 1){
		return 1;
	}
	return 0;
}
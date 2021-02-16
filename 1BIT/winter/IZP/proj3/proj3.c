/*----------------------------------*/
/*              IZP                 */
/*            Projekt 2             */
/*          Michal Koval            */
/*    xkoval17@stud.fit.vutbr.cz    */
/*----------------------------------*/
/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>
#include <errno.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    // alokacia pamate pre pole obj_t
    if (cap >=1) {
        c->obj = malloc(sizeof(struct obj_t) * cap);
        if (c->obj == NULL) {
            c->capacity = 0;
            return;
        }
    }
    c->capacity = cap;
    return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    c->size = 0;
    c->capacity = 0;
    c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    //rozsirenie kapacity shluku ak je uz plna
    if (c->capacity <= c->size) {
        if (resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL) {
            return;
        }
    }
    //pridanie objektu
    c->obj[c->size] = obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // pridanie objektov
    for (int i = 0; i < c2->size; i++){
        append_cluster(c1, c2->obj[i]);
    }

    //zoradenie
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    //odstranenie zhluku na danom indexe
    clear_cluster(&carr[idx]);
    //posun clusterov v poli dolava namiesto zmazaneho clusteru
    for (int i = idx; i < narr - 1; i++) {
        carr[i] = carr [i+1];
    }
    return --narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float x = fabs(o2->x - o1->x);
    float y = fabs(o2->y - o1->y);
    return sqrt(x*x + y*y);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // nastavenie premennych
    float vzdialenost = obj_distance(&c1->obj[0], &c2->obj[0]);
    float temp_vzdialenost;

    // hladanie najmensej vzdialenosti medzi vsetkymi objektmy clusterov
    for (int x = 0; x < c1->size; x++){
        for (int y = 0; y< c2->size; y++) {
            if ((temp_vzdialenost = obj_distance(&c1->obj[x], &c2->obj[y])) < vzdialenost){
                vzdialenost = temp_vzdialenost;
            }
        }
    }
    return vzdialenost;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // ak mame len 2 clustery nie je co hladat
    if (narr == 2){
        *c1 = 0;
        *c2 = 1;
    }

    // hladanie najmensej vzdialenosti medzi vsetkymi clustermi, na zaklade najmensiej vzdialenosti objektov clusterov
    float vzdialenost_susedov = cluster_distance(&carr[0], &carr[1]);
    float temp_vzdialenost;
    for (int x = 0; x < narr; x++) {
        for (int y = x + 1; y < narr; y++){
            if (vzdialenost_susedov > (temp_vzdialenost = cluster_distance(&carr[x], &carr[y]))){
                vzdialenost_susedov = temp_vzdialenost;
                *c1 = x;
                *c2 = y;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

int str_to_obj (char *str, struct obj_t *obj);

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/

void dealokacia (struct cluster_t *clusters,int poc_clusterov);

int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    //otvorenie suboru
    FILE * objekty;
    if ((objekty = fopen(filename, "r")) == NULL) {
        fprintf(stderr,"ERROR - subor: '%s' sa nepodarilo otvorit\n", filename);
        *arr = NULL;
        return 0;
    }
    //nacitanie countu zo suboru
    int count;
    fscanf(objekty, "count=%d\n",&count);
    if (count < 1){
        fprintf(stderr, "ERROR - zadany nespravny count: %d\n", count);
        fclose(objekty);
        *arr = NULL;
        return 0;
    }

    //inicializacia pola clusterov
    struct cluster_t *temp_arr;
    temp_arr = malloc(sizeof(struct cluster_t) * count);
    if (temp_arr == NULL) {
        fprintf(stderr, "ERROR - alokacie pamate pre pole shlukov\n");
        fclose(objekty);
        *arr = NULL;
        return 0;
    }

    //inicializacia cluusterov v poli a nacitanie riadkov zo suboru do objektov clusterov
    char riadok [100];
    char c = 0;
    for (int akt_obj = 0; akt_obj < count; akt_obj++) {
        //inicializacia clusteru pre akt_obj
        init_cluster(&temp_arr[akt_obj], CLUSTER_CHUNK);

        //citanie riadku zo suboru
        memset(riadok, 0, sizeof(riadok));
        for (int akt_c = 0; (c = getc(objekty)) != '\n' && c != EOF; akt_c++){
            riadok[akt_c] = c;
        }
        riadok[strlen(riadok)] = '\0';

        //kontrola konca suboru
        if (c == EOF && count > akt_obj) {
            fprintf(stderr,"ERROR - count:%d je vacsi ako pocet objektov v subore:%d\n", count, akt_obj);
            fclose(objekty);
            *arr = NULL;
            dealokacia(temp_arr, ++akt_obj);
            return 0;
        }

        //ukladanie dat z riadku do docasneho objektu
        struct obj_t temp_obj;
        if (str_to_obj(riadok, &temp_obj) == 1) {
            fclose(objekty);
            *arr = NULL;
            dealokacia(temp_arr, ++akt_obj);
            return 0;
        }

        // kotrola rovnakych id
        for (int kontrol_obj = 0; kontrol_obj < akt_obj; kontrol_obj++) {
            if (temp_arr[kontrol_obj].obj->id == temp_obj.id){
                fprintf(stderr, "ERROR - objekty zo vstupu s rovnakym id\n");
                fclose(objekty);
                *arr = NULL;
                dealokacia(temp_arr, ++akt_obj);
                return 0;
            }
        }

        //append objektu do i-teho clusteru z pola
        append_cluster(&temp_arr[akt_obj],temp_obj);
    }
    fclose(objekty);
    *arr = temp_arr;
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int str_to_obj (char *str, struct obj_t *obj)
{
    // osetrenie - vsetko musi byt kladne
    if ((strchr(str, '-')) != NULL){
        fprintf(stderr, "ERROR - hodnoty objektu: %s musia byt kladne\n", str);
        return 1;
    }
    long id = 0;
    long x = 0;
    long y = 0;
    int porad_cisla = 1;
    for (unsigned int i = 0; i < strlen(str); i++){
        // pismena su cifry
        if (47 < str[i] &&  str[i] < 58) {
            // id
            if (porad_cisla == 1) {
                id *= 10;
                id += str[i] - '0';
            //x
            } else if (porad_cisla == 2) {
                x *= 10;
                x += str[i] - '0';
            //y
            } else if (porad_cisla == 3) {
                y *= 10;
                y += str[i] - '0';
            } else {
            // zadanych viac cisel
                fprintf(stderr, "ERROR - nespravne zadany riadok suboru s objektymi: %s\n", str);
                return 1;
            }
        // medzera = skok na dalsie cislo
        } else if (str[i] == ' '){
            porad_cisla++;
        // ziadne ine znaky nemaju byt v subore
        } else {
            fprintf(stderr, "ERROR - nespravne zadany riadok suboru s objektymi: %s\n", str);
            return 1;
        }
    }
    // kontrola pretecenia
    if (x > INT_MAX || y > INT_MAX || id > INT_MAX) {
        fprintf(stderr, "ERROR - pretecenie integeru, zadane privelke hodnoty v stupnom subore: id:%ld %ld %ld\n", id, x, y);
        return 1;
    // kontrola x ,y
    } else if (x > 1000 || y > 1000){
        fprintf(stderr, "ERROR - suradnice objektov musia byt v rozmedzi 0 <= cislo <= 1000\nZadane boli x:%ld y:%ld\n", x,y);
        return 1;
    }
    obj->id = id;
    obj->x = x;
    obj->y = y;
    return 0;
}

int arg_to_int (char *arg)
{
    // osetrenie - N musi byt kladne
    if ((strchr(arg, '-')) != NULL){
        fprintf(stderr, "ERROR - Argument N: %s musi byt kladny\n", arg);
        return 0;
    }
    // konvertovanie argumentu na int N
    char *endptr;
    unsigned long pomocne_n = strtoul(arg, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "ERROR - Nespravne zadany argument N: '%s', ma to byt kladne cele cislo\n", arg);
        return 0;
    // kotrola pretecenia
    } else if (errno == ERANGE) {
        fprintf(stderr,"ERROR - Pretecenie uint argumentu N, zadana privelka hodnota\n");
        return 0;
    // osetrenie - n musi byt vacsie ako 0
    } else if (pomocne_n == 0) {
        fprintf(stderr, "ERROR - Argument N: '%s', musi byt vacsi ako 0\n", arg);
    } else if (pomocne_n > UINT_MAX) {
        fprintf(stderr,"ERROR - Pretecenie uint argumentu N, zadana privelka hodnota\n");
        return 0;
    }
    unsigned int n = pomocne_n;
    return n;
}

void dealokacia (struct cluster_t *clusters,int poc_clusterov)
{
    // dealokacia objektov
    for (int i = 0; i < poc_clusterov; i++){
        free(clusters[i].obj);
    }
    // dealokacia pola clusterov
    free(clusters);
}
int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    int n_arg;

    // spracovanie argumentu n
    if (argc == 2) {
        n_arg = 1;
    } else if (argc == 3) {
        n_arg = arg_to_int(argv[2]);
        if (n_arg == 0){
            fprintf(stderr, "ERROR - argument N: %d musi byt vacsi ako 0\n", n_arg);
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR - zadany nespravny pocet arugemtnov\nargc: %d  ---  1 < argc < 4\n", argc);
        return 1;
    }
    // nacitanie vstupnych objektov do pola clusterov
    int count;
    if ((count = load_clusters(argv[1], &clusters)) == 0) {
        return 1;
    }

    // kontrola pozadovaneho poctu clusterov
    if (n_arg > count) {
        fprintf(stderr, "ERROR - neda sa vytvorit %d clusterov z %d objektov\n", n_arg, count);
        dealokacia (clusters,count);
        return 1;
    } else {
    // zmena poctu clusterov - merging
        int index1;
        int index2;
        while (n_arg < count){
            // najdenie dvoch najblizsich clusterov
            find_neighbours(clusters, count, &index1, &index2);
            // merge najdenych clusterov do clusteru s index1
            merge_clusters(&clusters[index1], &clusters[index2]);
            // odstranenie povodneho clusteru na index2
            count = remove_cluster(clusters, count, index2);
        }
        print_clusters(clusters, n_arg);
    }

    // dealokacia
    dealokacia (clusters,count);
    return 0;
}

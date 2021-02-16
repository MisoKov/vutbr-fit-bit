/**
 *	@file	htab.h	
 *  @author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Hlavickovy subor pre pracu s hashovacou tabulkou a program wordcount
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 * 	@licence Public domain
 */

#ifndef __HTABLE_H__ 
#define __HTABLE_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // size_t 
#include <stdbool.h> // bool 

/**
 * @def HTAB_SIZE
 * @brief Velkost hashovacej tabulky
 * 
 * Vyber velkosti tabulky - na zaklade 3 kriterii:
 * @see https://planetmath.org/goodhashtableprimes
 * 1. cislo by malo byt prvocislo pretoze to zamedzuje zhlukovanie zaznamov na niektorych indexoch
 * 2. jednotlive navrhovanie cisla su zhruba dvakrat vacsie ako predosle, pre vhodny vyber vzhladom
 * 	  na velkost tabulky, ale v tomto pripade nevieme aky bude velky vstupny text, ale na zaklade 
 * 	  poctu slov v jazyku (wiki) vieme ze nam netreba tabulku pre cesky jazyk vacsiu ako 250 000 a 
 * 	  pre anglicky 470 000, pricom treba brat do uvahy ze mnoho slov sa takmer vobec nepouziva a v 
 * 	  jednom zozname tabulky moze byt niekolko zaznamov
 * @see https://en.wikipedia.org/wiki/List_of_dictionaries_by_number_of_words
 * 3. cislo by malo byt vzdialene od mocnin 2 - z praxe sa ukazalo ze take cisla su vhodne kvoli dobrym vysledkom
 * 
 * Testy:
 * text - Koran (anglicky, zdroj: Guttenberg):
 * @see http://www.gutenberg.org/ebooks/2800
 * size: 6151 time: 0m0,388s
 * size: 12289 time: 0m0,360s
 * size: 24593 time: 0m0,356s
 * size: 49157 time: 0m0,357s
 * size: 98317 time: 0m0,358s
 * 
 * text - seq 1000000 2000000|shuf
 * size: 6151 time: 0m16,819s
 * size: 12289 time: 0m11,108s
 * size: 24593 time: 0m7,597s
 * size: 49157 time: 0m5,604s
 * size: 98317 time: 0m4,808s
 * 
 * Rozhodol som sa pre 12289 pretoze sice 2. test ukazal ze vacsie velkosti si vedia lepsie poradit s 
 * vacsimi rozmanitejsimi textmi, ale asi iba niektore azijske jazyky maju cez milion slov, takze 2. 
 * text nebol prave najvhodnejsia testovacia vzorka, a pri prvom teste sa ukazalo ze po 12289 nie je
 * niejaky vyrazny narast v rychlosti za cenu pamate.
 */
#define HTAB_SIZE 12289
/**
 * @def WORD_MAX_LEN
 * @brief Max dlzka slova
 */
#define WORD_MAX_LEN 127
/**
 * @brief Neuplny datovy typ pre hashovaciu tabulku
 */
typedef struct htab htab_t; 
/**
 * @brief Neuplna struktura pre zaznam hashovacej tabulky
 */
struct htab_item; 

/**
 * @brief Struktura pre iteraciu cez zaznamy hashovacej tabulky
 */
typedef struct htab_iterator { 
	struct htab_item *ptr; /** <Pointer na zaznam> */
	const htab_t *t; /** <Pointer na tabulku, v ktorej sa zaznam nachadza> */
	int idx; /** <Index pola tabulky, v ktorom sa zaznam nachadza> */
} htab_iterator_t; 
/**
 * @fn	int get_word(char *s, int max, FILE *f)
 * @param[out]	s Pointer na pole znakov kde sa ulozi nacitane slovo
 * @param[in]	max Maximalna dlzka slova
 * @param[in]	f Stream, z ktoreho citame slovo
 * @return	Dlzka nacitaneho slova
 * @brief	Vrati dlzku slova a ulozi slovo na pointer s, alebo vrati EOF v pripade konca riadku alebo 0 v pripade chyby
 */
int get_word(char *s, int max, FILE *f);    
/**
 * @fn	unsigned int htab_hash_function(const char *str)
 * @param[in]	str String, ktory chceme previest do hashcodu
 * @return     Hashcode
 * @brief	Vrati hashcode slova ulozenom na str
 */
unsigned int htab_hash_function(const char *str); 
/**
 * @fn	htab_t *htab_init(size_t n)
 * @param[in]	n Velkost tabulky
 * @return  Inicializovana hashovacia tabulka
 * @brief	Alokuje pamat a inicializuje hashovaci tabulku o velkosti n
 */
htab_t *htab_init(size_t n);
/**
 * @fn	htab_t *htab_init(size_t n)
 * @param[in]	n Velkost tabulky
 * @param[in,out]	from Zdrojova tabulka
 * @return  Inicializovana hashovacia tabulka doplnena o zaznamy z from
 * @brief	Alokuje pamat a inicializuje hashovaci tabulku o velkosti n a vlozi do novej tabulky zaznamy zo zdrojovej tabulky form
 */
htab_t *htab_move(size_t n, htab_t *from);
/**
 * @fn	size_t htab_size(const htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @return  Pocet zaznamov hashovacej tabulky
 * @brief	Vracia pocet zaznamov hashovacej tabulky
 */ 
size_t htab_size(const htab_t * t);
/**
 * @fn	size_t htab_bucket_count(const htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @return  Pocet jedinecnych prvkov (bucketov) hashovacej tabulky
 * @brief	Vracia pocet jedinecnych prvkov (bucketov) hashovacej tabulky
 */ 
size_t htab_bucket_count(const htab_t * t); 
/**
 * @fn	htab_iterator_t htab_lookup_add(htab_t * t, const char *key)
 * @param[in,out]	t Hashovacia tabulka
 * @param[in]	key Kluc zaznamu
 * @return  Iterator so zaznamom key
 * @brief	Najde zaznam odpovedajuci key a vrati iterator na neho ukazujuci, ak v tabulke nie je vytvori ho a vrati iterator nan
 */ 
htab_iterator_t htab_lookup_add(htab_t * t, const char *key); 
/**
 * @fn	htab_iterator_t htab_begin(const htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @return  Iterator prveho zaznamu tabulky
 * @brief	Najde prvy zaznam tabulky a vrati jeho iterator
 */ 
htab_iterator_t htab_begin(const htab_t * t); 
/**
 * @fn	htab_iterator_t htab_end(const htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @return  Iterator posledneho zaznamu tabulky
 * @brief	Najde posledny zaznam tabulky a vrati jeho iterator
 */ 
htab_iterator_t htab_end(const htab_t * t); 
/**
 * @fn	htab_iterator_t htab_iterator_next(htab_iterator_t it)
 * @param[in]	it Iterator
 * @return  Iterator nasledujuceho zaznamu v tabulke
 * @brief	Najde nasledujuci zaznam tabulky a vrati jeho iterator
 */ 
htab_iterator_t htab_iterator_next(htab_iterator_t it);
/**
 * @fn	static inline bool htab_iterator_valid(htab_iterator_t it)
 * @param[in]	it Iterator
 * @return  True ak je validny iterator, false ak je nevalidny
 * @brief	Vrati true ak je validny iterator, false ak je nevalidny
 */ 
static inline bool htab_iterator_valid(htab_iterator_t it) { return it.ptr!=NULL; }
/**
 * @fn	static inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2)
 * @param[in]	it1 Iterator 1
 * @param[in]	it2 Iterator 2
 * @return  True ak su iteratory zhodne, false ak nie su
 * @brief	Porovna iteratory a vrati true ak su zhodne, v opacnom pripade false
 */ 
static inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2) { 
	return it1.ptr==it2.ptr && it1.t == it2.t; 
}
/**
 * @fn	const char * htab_iterator_get_key(htab_iterator_t it)
 * @param[in]	it Iterator
 * @return  Kluc iteratoru
 * @brief	Vrati kluc iteratoru
 */ 
const char * htab_iterator_get_key(htab_iterator_t it); 
/**
 * @fn	int htab_iterator_get_value(htab_iterator_t it)
 * @param[in]	it Iterator
 * @return  Frekvencia kluca iteratoru
 * @brief	Vrati frekvenciu kluca iteratoru
 */ 
int htab_iterator_get_value(htab_iterator_t it); 
/**
 * @fn	int htab_iterator_set_value(htab_iterator_t it, int val)
 * @param[in]	it Iterator
 * @param[in]	val Hodnota na ktoru nastavime iterator
 * @return  Nova nastavena hodnota
 * @brief	Nastavi iteratot it na hodnotu val
 */ 
int htab_iterator_set_value(htab_iterator_t it, int val); 
/**
 * @fn	void htab_clear(htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @brief	Dealokacia vsetkych poloziek (bucketov) tabulky
 */ 
void htab_clear(htab_t * t); 
/**
 * @fn	void htab_free(htab_t * t)
 * @param[in]	t Hashovacia tabulka
 * @brief	Dealokacia tabulky
 */ 
void htab_free(htab_t * t);

#endif // __HTABLE_H__
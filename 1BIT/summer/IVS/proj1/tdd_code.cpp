//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     MICHAL KOVAL <xkoval17@stud.fit.vutbr.cz>
// $Date:       $2018-02-24
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author MICHAL KOVAL
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	// nastavenie pointra na prvy prvok na NULL
	m_pHead=NULL;
}

PriorityQueue::~PriorityQueue()
{
	// ziskanie pointra na prvy prvok
	Element_t *bunka = GetHead();
	// prechadzanie buniek za radom a ich dealokacia
	while (bunka != NULL)
	{
		// ulozenie pointra na mazanie
		Element_t *akt_bunka = bunka;
		// nastavenie pointra na dalsiu bunku
		bunka = bunka->pNext;
		// dealokacia aktualne bunky
		free(akt_bunka);
	}
}

void PriorityQueue::Insert(int value)
{
	// ziskanie pointra na prvu bunku
	Element_t *bunka = GetHead();
	// prazdna fronta
	if (bunka == NULL){
		m_pHead = ElementConstructor(value, NULL, NULL);
	// neprazdna fronta
	} else {
		Element_t *prev_bunka;
		Element_t *pom_bunka;
		// postupne prechadzanie buniek do konca fronty
		while (bunka != NULL)
		{	
			// zaradenie value pred aktualnu bunku
			if (value <= bunka->value){
				// aktualna bunka je prva
				if (bunka->pPrev == NULL){
					m_pHead = ElementConstructor(value, bunka, NULL);
					bunka->pPrev = m_pHead;
					return;
				// vlozenie value pred aktualnu bunku
				} else {
					pom_bunka = ElementConstructor(value, bunka, bunka->pPrev);
					bunka->pPrev->pNext = pom_bunka;
					bunka->pPrev = pom_bunka;
					return;
				}
			}
		       		
			// ulozenie aktualnej bunky a nastavenie nasledujucej bunky
			prev_bunka = bunka;
			bunka = bunka->pNext;
		}
		// vlozenie value na koniec fronty
		if (value > prev_bunka->value){
			bunka = ElementConstructor(value, NULL, prev_bunka);
			prev_bunka->pNext = bunka;
			return;
		}
	}
}

bool PriorityQueue::Remove(int value)
{
	Element_t *bunka = Find(value);
	if (bunka != NULL){
		// najdena bunka je prva vo fronte
		if (bunka->pPrev == NULL && bunka->pNext != NULL){
			bunka->pNext->pPrev = NULL;
			m_pHead = bunka->pNext;
		// najdena bunka je posledna vo fronte
		} else if (bunka->pNext == NULL && bunka->pPrev != NULL){
			bunka->pPrev->pNext = NULL;
		// najdena bunka je jedina vo fronte
		} else if (bunka->pNext == NULL && bunka->pPrev == NULL){
			m_pHead = NULL;
		// aktualna bunka je medzi zaciatkom a koncom fronty
		} else {
			bunka->pNext->pPrev = bunka->pPrev;
			bunka->pPrev->pNext = bunka->pNext;
		}
		// zmazanie aktualnej bunky
		free(bunka);
		return true;
	}
	// value sa nenachadza vo fronte alebo je fronta prazdna
	return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	//ziskanie hlavy fronty
	Element_t *bunka = GetHead();
	// osetrenie prazdnej fronty
	if (bunka == NULL){
		return NULL;
	}
	// prechadzanie posupne bunkami
	while (bunka != NULL){
		if (value == bunka->value){
			return bunka;
		}
		bunka = bunka->pNext;
	}
	// bunka s value nebola najdena
	return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
	// vratenie pointra na prvy prvok
	return m_pHead;
}

PriorityQueue::Element_t *PriorityQueue::ElementConstructor(int value, Element_t* pNext, Element_t* pPrev)
{
	// alokacie pamate pre novu bunku
	Element_t* bunka = (Element_t*) malloc(sizeof(Element_t));
	if (bunka == NULL){
		fprintf(stderr, "Chyba alokacie pamate\n");
		return NULL;
	}
	// inicialzacia bunky
	bunka->value = value;
	bunka->pNext = pNext;
	bunka->pPrev = pPrev;

	return bunka;
}

/*** Konec souboru tdd_code.cpp ***/


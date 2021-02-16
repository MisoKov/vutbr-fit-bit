/*----------------------------------*/
/*              IZP                 */
/*            Projekt 1             */
/*          Michal Koval            */
/*    xkoval17@stud.fit.vutbr.cz    */
/*----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int zapis_konca_riadku (char *riadok, unsigned int velkost_riadku) {
     // ZAPIS '\n' NA KONIEC RIADKU AK SA ZMESTI DO RIADKU
    if ((strlen(riadok)) < velkost_riadku) {
        riadok[strlen(riadok)] = '\n';
        return 0;
    } else {
        printf("ERROR - zadany pridlhy riadok textu");
        return 1;
    }
}

void nacitat_riadok (char *riadok, unsigned int velkost_riadku, int *znak, int *ukoncenie)
{
    int index = 0;
    // CITANIE ZNAK PO ZNAKU ZO STDINU
    while ((*znak = getchar()) != EOF && *znak != '\n') {
        // KONTROLA DLZKY RIADKU
        if ((strlen(riadok)) < velkost_riadku) {
            riadok[index] = *znak;
            index++;
        // PRIDLHY RIADOK
        } else {
            printf("ERROR - zadany pridlhy riadok textu na vstupe\n");
            *ukoncenie = 1;
            break;
        }
    }
    // '\n' NA KONIEC RIADKU
    zapis_konca_riadku(riadok, velkost_riadku);
}

void vypis_riadku (char *riadok, int velkost_riadku)
{
    printf("%s", riadok);

    // VYMAZANIE PRAVE NAPISANEHO STRINGU Z POLA - VYTVORENIE PRAZDNEHO POLA PRE DALSI STRING
    memset(riadok, 0, velkost_riadku);
}

int pocet_riadkov (FILE *subor)
{
    // PREJDENIE VSETKYMI RIADKAMI PO KONIEC SUBORU - RATANIE '\n'
    int line_counter = 0;
    char c;
    while ((c = getc(subor)) != EOF){
        if (c == '\n'){
            line_counter++;
        }
    }
    // REWIND NA ZAC SUBORU
    rewind(subor);

    return ++line_counter;
}

int kontrola_konca_prikazu (FILE * f_prikazy)
{
    // KONTROLA KONCA PRIKAZ - NAJDENIE '\n' alebo EOF - PRECHOD NA DALSI RIADOK PRIKAZOVEHO SUBORU
    int znak;
    if ((znak = getc(f_prikazy)) != '\n' && znak != EOF) {
        printf("ERROR kontrola prikazu - zadany chybny/nedefinovavy prikaz\n");
        return 1;
    } else {
        return 0;
    }
}

int prikaz_citanie_N (FILE *subor)
{
    // CITANIE CIFERNYCH ZNAKOV A ICH ZRATAVANIE KYM NENARAZIME NA INY ZNAK
    int c = getc(subor);
    int n = 0;
    // KONTROLA CIFRY
    if (c > '0' && c <= '9') {
      n = c - '0';
      while ((c = getc(subor)) >= '0' && c <= '9') {
          n = n*10 + (c - '0');
      }
      // VRATENIE 1. NECIFERNEHO ZNAKU DO STREAMU PRE NESKORSIU KONTROLU
      ungetc(c,subor);
      return n;
    // NEZADANIE X, X = 0
    } else if (c == '\n' || c == EOF) {
        ungetc(c, subor);
        return 0;
    // ANI PRVY ZNAK NIE JE CIFRA - CHYBNY PRIKAZ
    } else {
        return -1;
    }
}

void odstranenie_LF (char *riadok)
{
    if (riadok[strlen(riadok)-1] == '\n') {
        riadok[strlen(riadok)-1] = 0;
    }
}

int citanie_patternu (FILE *stream, char *pattern, int oddelovac, int velkost_patternu)
{
    // VYMAZANIE POLA KVOLI MOZNOSTI VYSKYTU ZNAKOV Z PREDOSLEHO PATTERNU
    memset(pattern, 0, velkost_patternu);
    char c;
    // KONTROLA PRAZDNEHO PATTERNU
    if ((c = getc(stream)) == oddelovac || c == EOF || c == '\n') {
        printf("ERROR - chybny prikaz = prazdny pattern\n");
	return 1;
    } else {
        // CITANIE ZNAKOV PATTERNU
        pattern[0] = c;
	while ((c = getc(stream)) != oddelovac && c != EOF && c != '\n') {
            pattern[strlen(pattern)] = c;
        }
        // KONTROLA ROVNAKEHO ODDELOVACA
        if (oddelovac == c) {
            return 0;
        } else {
            return 1;
        }
    }
}

char* najdenie_patternu (char* riadok, char* pattern)
{   char *ptr;
    if ((ptr = strstr(riadok, pattern)) != NULL) {
        // PATTERN JE V STRINGU
        return ptr;
    } else {
        // PATTERN NIE JE V STRINGU
        return NULL;
    }
}
int citanie_replacementu (FILE *stream, char *replacement_string) {
    int c;
    // NACITANIE ZNAKOV NA RIADKU PRIKU PO KONIEC RIADKU
    for (int i = 0;((c = getc(stream)) != '\n' && c != EOF); i++) {
        replacement_string[i] = c;
    }
    if (strlen(replacement_string) == 0) {
        return 1;
    } else if (strlen(replacement_string) == 1) {
	if (replacement_string[0] == '\n' || replacement_string[0] == EOF) {
	    return 1;
	} else {
	    return 0;
	}
    } else {
	return 0;
    }
}

int replace (char* ptr_na_pattern, char *riadok, char *pattern, unsigned int max_velkost_riadku, char *replacement_string)
{
    // KONTROLA CI SA UPRAVENY RIADOK ZMESTI DO POLA AKTUALNEHO RIADKU
    if ((strlen(replacement_string) - strlen(pattern) + strlen(riadok)) <= max_velkost_riadku) {

        // ULOZENIE STRINGU PRED PATTERNOM NA NESKOR
        char buffer_pred[max_velkost_riadku];
        memset(buffer_pred, 0, max_velkost_riadku);
        int poc_znakov_pred_patt = strlen(riadok) - strlen(ptr_na_pattern);
        for (int i = 0; i < poc_znakov_pred_patt; i++) {
            buffer_pred[i] = riadok[i];
        }

        // ULOZENIE STRINGU PO PATTERNE NA NESKOR
        char buffer_po[max_velkost_riadku];
        memset(buffer_po, 0, max_velkost_riadku);
        strcat(buffer_po, (ptr_na_pattern + strlen(pattern)));

        // MAZANIE RIADKU
        memset(riadok, 0, max_velkost_riadku);
        // PRIADAVANIE ZNAKOV PRED PATTERNOM
        for (int y = 0; y < poc_znakov_pred_patt; y++){
            riadok[y] = buffer_pred[y];
        }
        // PRIDAVANIE REPLACEMENTU
        strcat(riadok, &replacement_string[0]);
        // PRIDAVANIE ZNAKOV PO PATTERNE
        strcat(riadok, buffer_po);
        return 0;
    } else {
        printf("ERROR - zadany pridlhy riadok\n");
        return 1;
    }
}
int main (int argc, char* argv[])
{
    // OTVORENIE SUBORU S PRIKAZMI
    if (argc != 2) {
        printf("ERROR - zadany nespravny pocet argumentov");
        return 1;
    }
    FILE *f_prikazy;
    if ((f_prikazy = fopen(argv[1], "r")) == NULL){
        printf("ERROR - subor s prikazmy sa nepodarilo otvorit\n");
        return 1;
    }
    // DEKLARACIA PREMENNYCH
    char aktualny_riadok [1024] = {0};       // 1024 - min 1000 znakov + pekne okruhle cislo 1kb
    unsigned int velkost_riadku = sizeof(aktualny_riadok);
    char pomocny_string [velkost_riadku];
    memset(pomocny_string, 0, velkost_riadku);
    char pattern[velkost_riadku];
    char replacement_string[velkost_riadku];
    char* ptr_string;
    int poc_prikazov = pocet_riadkov(f_prikazy);
    int poc_iteracii;
    int aktualne_pismeno;
    int prikaz;
    int oddelovac;
    int koniec_vstup = -1;
    int koniec_pridlhy_riadok = 0;
    int prikaz_counter = 0;
    // KONTROLA VSTUPU
    nacitat_riadok(aktualny_riadok, velkost_riadku, &aktualne_pismeno, &koniec_pridlhy_riadok);
    if (koniec_pridlhy_riadok == 1) {
        printf("ERROR - zadany pridlhy riadok\n");
        return 1;
    }
    if (aktualne_pismeno == EOF) {
        printf("ERROR - nic nie je na vstupe");
        return 1;
    }
    // ---HLAVNY CYKLUS---
    // CITANIE PRIKAZOV A ICH VYKONAVAIE
    while ((prikaz = getc(f_prikazy)) != EOF) {
        poc_iteracii = 0;
        prikaz_counter++;
        switch (prikaz){
            case ('n'):
                poc_iteracii = prikaz_citanie_N(f_prikazy);
                // KONTROLA EOL/EOF NA KONCI PRIKAZU - PRECHOD NA DALSI PRIKAZ
                if (kontrola_konca_prikazu(f_prikazy) == 1) {
                    return 1;
                } else {
                    // 0 JE '\n' A TO JE 1 ITERACIA
                    if (poc_iteracii == 0) {
                        poc_iteracii = 1;
                    }
                    if (poc_iteracii > 0) {
                        // VYPIS N RIADKOV
                        for (int i = 0; i < poc_iteracii; i++){
                            vypis_riadku(aktualny_riadok, velkost_riadku);
                            nacitat_riadok(aktualny_riadok, velkost_riadku, &aktualne_pismeno, &koniec_pridlhy_riadok);
                            if (koniec_pridlhy_riadok == 1) {
                                printf("ERROR - zadany pridlhy riadok\n");
                                return 1;
                            }
                            // KONTROLA KONCA VSTUPNEHO TEXTU
                            if (aktualne_pismeno == EOF)  {
                                // EOL MAME NACITANE UZ NA N-1 PRIKAZE, TAKZE AK CHCEME TLACIT AJ POSLEDNY RIADOK MUSIME KONCIT POGRAM O ITERACIU NESKOR
                                if (i < poc_iteracii){
                                    koniec_vstup++;
                                }
                                if (koniec_vstup == 1) {
                                    printf("=======================================\n");
                                    printf("KONIEC VSTUPNEHO TEXTU\n");
                                    return 0;
                                }
                            }
                        }
                    // CHYBNY PRIKAZ
                    } else {
                        printf("ERROR - zadany chybny/nedefinovavy prikaz\n");
                        return 1;
                    }
                    break;
                }
            case ('q'):
                // KONTROLA EOL NA KONCI PRIKAZU
                if (kontrola_konca_prikazu(f_prikazy) == 1) {
                    return 1;
                } else {
                    return 0;
                }
            case ('i'):
                // TLAC CONTENTU ZNAK PO ZNAKU
                while ((prikaz = getc(f_prikazy)) != '\n' && prikaz != EOF)
                    putchar(prikaz);
                putchar('\n');
                break;
            case ('a'):
                // ODSTRANENIE LF NA KONCI AKTUALNEHO RIADKU
                odstranenie_LF(aktualny_riadok);
                // KONTROLA CONTENTU
                if ((prikaz = getc(f_prikazy)) != '\n' && prikaz != EOF) {
                    // APPEND CONTENTU ZNAK PO ZNAKU NA POMOCNY STRING
                    memset(pomocny_string, 0, velkost_riadku);
                    do {
                        if (strlen(pomocny_string) < velkost_riadku) {
                            pomocny_string[poc_iteracii] = prikaz;
                        } else {
                            printf("ERROR - zadany pridlhy riadok textu");
                            return 1;
                        }
                        poc_iteracii++;
                    } while ((prikaz = getc(f_prikazy)) != '\n' && prikaz != EOF);
                    if (strlen(pomocny_string) + strlen(aktualny_riadok) < velkost_riadku) {
                        strcat(aktualny_riadok, pomocny_string);
                    } else {
                        printf("ERROR - zadany pridlhy riadok textu");
                        return 1;
                    }
                } else {
                    printf("ERROR - A zadany chybny prikaz\n");
                    return 1;
                }
                // VRATENIE LF NA KONIEC RIADKU
                if (zapis_konca_riadku(aktualny_riadok, velkost_riadku) == 1) {
                    return 1;
                } else {
                    break;
                }
            case ('b'):
                memset(pomocny_string, 0, velkost_riadku);
                // CITANIE CONTENTU A UKLADANIE DO BUFFERU
                for (int i = 0;(prikaz = getc(f_prikazy)) != '\n'; i++) {
                    // KONTROLA POCTU ZNAKOV CONTENTU
                    if (strlen(pomocny_string) < velkost_riadku) {
                        pomocny_string[i] = prikaz;
                    } else {
                        printf("ERROR - zadany pridlhy riadok textu\n");
                        return 1;
                    }
                }
                // KONTROLA POCTU ZNAKOV AKT RIADKU SPOLU S CONTENTOM
                if ((strlen(pomocny_string) + strlen(aktualny_riadok)) < velkost_riadku) {
                    strcat(pomocny_string,aktualny_riadok);
                    strcpy(aktualny_riadok, pomocny_string);
                } else {
                    printf("ERROR - zadany pridlhy riadok textu\n");
                    return 1;
                }
                break;
            case ('s'):
                // URCENIE ODDELOVACIEHO ZNAKU
                oddelovac = getc(f_prikazy);
                if (oddelovac == '\n' || oddelovac == EOF) {
                    printf("ERROR - zadany nedefinovany/chybny prikaz\n");
                    return 1;
                } else {
                    // PRECITANIE PATTERNU Z PRIKAZU
                    if (citanie_patternu (f_prikazy, pattern, oddelovac, velkost_riadku) == 1) {
                        printf("ERROR - zadany chybny prikaz\n");
                        return 1;
                    } else {
                        // CITANIE REPLACEMENT STIRNGU
                        memset(replacement_string, 0, velkost_riadku);
                        if (citanie_replacementu(f_prikazy, replacement_string) == 1) {
			    printf("ERROR - zadany prazdny REPLACEMENT v prikazu s\n");
			    return 1;
		 	} else {
                            // NAJDENIE PATTERNU V AKTUALNOM RIADKU
                            if ((ptr_string = najdenie_patternu(aktualny_riadok, pattern)) == NULL) {
                                break;
                            } else {
                                // REPLACEMENT
                                if (replace (ptr_string, aktualny_riadok, pattern, velkost_riadku, replacement_string) == 1) {
                                    printf("ERROR - zadany pridlhy retazec\n");
                                    return 1;
                                } else {
                                    break;
                                }
                            }
                        }
                    }
		}
            case ('S'):
                // URCENIE ODDELOVACIEHO ZNAKU
                oddelovac = getc(f_prikazy);
                if (oddelovac == '\n' || oddelovac == EOF) {
                    printf("ERROR - zadany nedefinovany/chybny prikaz\n");
                    return 1;
                } else {
                    // PRECITANIE PATTERNU Z PRIKAZU
                    if (citanie_patternu (f_prikazy, pattern, oddelovac, velkost_riadku) == 1) {
                        printf("ERROR - zadany chybny prikaz\n");
                        return 1;
                    } else {
                        // CITANIE REPLACEMENT STIRNGU
                        memset(replacement_string, 0, velkost_riadku);
                        if (citanie_replacementu(f_prikazy, replacement_string) == 1) {
			    printf("ERROR - zadany prazdny REPLACEMENT v prikaze\n");
			    return 1;
			} else {
                            // NACHADZANIE VSETKYCH PATTERNOV V AKT RIADKU A ICH PREPIS ZA REP
                            while ((ptr_string = najdenie_patternu(aktualny_riadok, pattern)) != NULL) {
                                // REPLACEMENT
                                if (replace (ptr_string, aktualny_riadok, pattern, velkost_riadku, replacement_string) == 1) {
                                    printf("ERROR - zadany pridlhy retazec\n");
                                    return 1;
                                } else {
                                    continue;
                                }
                            }
                            break;
                        }
                    }
	        }
            case ('d'):
                // KONTROLA VSTUPU
                if (aktualne_pismeno == EOF) {
                    printf("======== KONIEC VSTUP ========DD");
                    return 1;
                }
                // MAZANIE AKT RIADKU
                memset(aktualny_riadok, 0, velkost_riadku);
                // NACITANIE DALSIEHO RIADKU
                nacitat_riadok(aktualny_riadok, velkost_riadku, &aktualne_pismeno, &koniec_pridlhy_riadok);
                if (koniec_pridlhy_riadok == 1) {
                    printf("ERROR - zadany pridlhy riadok\n");
                    return 1;
                }
                // KONTROLA EOL/EOF NA KONCI PRIKAZU A JEHO ZAHODENIE - PRECHOD NA DALSI RIADOK PRIKAZOV
                if (kontrola_konca_prikazu(f_prikazy) == 1) {
                    return 1;
                } else {
                    break;
                }
            case ('f'):
                // CITANIE PATTERNU
                citanie_patternu(f_prikazy, pattern, '\n', velkost_riadku);
                // KONTROLA PATTERNU
                if (strlen(pattern) == 0) {
                    printf("ERROR - prikazu f nebol zadany pattern\n");
                    return 1;
                } else {
                    // HLADANIE PATTERNU
                    while (najdenie_patternu(aktualny_riadok, pattern) == NULL){
                        // NACITANIE DALSIEHO RIADKU
                        memset(aktualny_riadok, 0, velkost_riadku);
                        nacitat_riadok(aktualny_riadok, velkost_riadku, &aktualne_pismeno, &koniec_pridlhy_riadok);
                        if (koniec_pridlhy_riadok == 1) {
                            printf("ERROR - zadany pridlhy riadok\n");
                            return 1;
                        }
                        // KONTROLA KONCA VSTUPNEHO TEXTU
                        if (aktualne_pismeno == EOF)  {
                            // EOL MAME NACITANE UZ NA N-1 PRIKAZE, TAKZE AK CHCEME TLACIT AJ POSLEDNY RIADOK MUSIME KONCIT POGRAM O ITERACIU NESKOR
                            koniec_vstup++;
                            if (koniec_vstup == 1) {
                                printf("ERROR - pattern nebol najdeny\n");
                                return 0;
                            }
                        }
                    }
                    break;
                }
            case ('c'):
                // CITANIE X
                poc_iteracii = prikaz_citanie_N(f_prikazy);
                if (poc_iteracii == 0 || poc_iteracii == -1) {
                    printf("ERROR - chybne zadany prikaz\n");
                    return 1;
                } else {
                    // KONTROLA MEDZERY (SPACE) ZNAKU
                    if (getc(f_prikazy) != ' ') {
                        printf("ERROR - chybne zadany prikaz\n");
                        return 1;
                    } else {
                        // CITANIE PATTERNU
                        if (citanie_patternu (f_prikazy, pattern, '\n', velkost_riadku) == 1) {
                            printf("ERROR - zadany chybny prikaz\n");
                            return 1;
                        } else {
                            // NAJDENIE PATTERNU
                            if (najdenie_patternu(aktualny_riadok, pattern) == NULL) {
                                break;
                            } else {
                                // OSETRENIE ZACYKLENIA
                                if (poc_iteracii == prikaz_counter) {
                                    printf("ERROR - zacyklenie skok stale na ten isty prikaz\nUKONCUJEM\n");
                                    return 1;
                                } else {
                                    // NASTAVENIE DALSIEHO PRIKAZU
                                    ungetc((poc_iteracii + '0'), f_prikazy);
                                    ungetc('g', f_prikazy);
                                    break;
                                }
                            }
                        }
                    }
                }
            case ('e'):
                // ZAPIS EOL NA KONIEC AKTUALNEHO RIADKU
                if (zapis_konca_riadku(aktualny_riadok, velkost_riadku) == 1) {
                    return 1;
                } else {
                    // KONTROLA EOL/EOF NA KONCI PRIKAZU A JEHO ZAHODENIE - PRECHOD NA DALSI RIADOK PRIKAZOV
                    if (kontrola_konca_prikazu(f_prikazy) == 1) {
                        return 1;
                    } else {
                        break;
                    }
                }
            case ('r'):
                odstranenie_LF(aktualny_riadok);
                // KONTROLA EOL/EOF NA KONCI PRIKAZU A JEHO ZAHODENIE - PRECHOD NA DALSI RIADOK PRIKAZOV
                if (kontrola_konca_prikazu(f_prikazy) == 1) {
                    return 1;
                } else {
                    break;
                }
            case ('g'):
              // KONTROLA X
              if ((poc_iteracii = prikaz_citanie_N(f_prikazy)) < 1 ) {
                  return 1;
              } else {
                  rewind(f_prikazy);
                  // SKOK NA 1. RIADOK
                  if (poc_iteracii == 1) {
                      prikaz_counter = poc_iteracii;
                      break;
                  // OSETRENIE ZACYKLENIA
                  } else if (poc_iteracii == prikaz_counter) {
                      printf("ERROR - zacyklenie v nekonecnom cykle\n");
                      return 1;
                  // OSETRENIE SKOKU MIMO ROZSAHU PRIKAZOV
                  } else if (poc_iteracii > poc_prikazov) {
                      printf("ERROR - vyskocienie prikazom 'g' nad pocet prikazov v prikazovom subore\n");
                      return 1;
                  // SKOK NA Xty RIADOK
                  } else {
                      for (int i = 1; i < poc_iteracii; i++){
                          while (getc(f_prikazy) != '\n') {
                              continue;
                          }
                      }
                      prikaz_counter = poc_iteracii;
                      break;
                  }
              }
            default:
                printf("ERROR - zadany nedefinovany prikaz\n");
                return 1;
        }
    }
    // VYPIS ZOSTAVAJUCEHO VSTUPU
    vypis_riadku(aktualny_riadok, velkost_riadku);
    while ((aktualne_pismeno = getchar()) != EOF){
        putchar(aktualne_pismeno);
    }
    putchar(aktualne_pismeno);
    return 0;
}
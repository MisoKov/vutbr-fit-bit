/*----------------------------------*/
/*              IZP                 */
/*            Projekt 2             */
/*          Michal Koval            */
/*    xkoval17@stud.fit.vutbr.cz    */
/*----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

double log_kontrola_cisla_x (double x)
{
    // vratenie specifickych vysledkov logaritmu
    if (x == 1.0){
        return +0.0;
    } else if (fabs(x) == 0.0){
        return -INFINITY;
    } else if (x < 0.0){
        return NAN;
    } else if (x == NAN){
        return NAN;
    } else if (isinf(x)){
        return INFINITY;
    } else if (x == -NAN) {
        return -NAN;
    } else {
        return 1;
    }
}

double pow_kontrola_argumentov(double x, double y, int *checker)
{
    *checker = 0;
    // vratenie specifickych vysledkov umocnenia
    if (fabs(y) == 0){
        return 1.0;
    } else if (x == +0.0 && !signbit(x)) {
        if (y < 0 && (floor(y) == y && (int)y % 2 == 1)) {
            return INFINITY;
        } else if (y > 0 && (floor(y) == y && (int)y % 2 == 1)) {
            return +0.0;
        }
    } else if (x == -0.0 && signbit(x)) {
        if (y < 0 && (floor(y) == y && (int)y % 2 == 1)) {
            return -INFINITY;
        } else if (y > 0 && (floor(y) == y && (int)y % 2 == 1)) {
            return -0.0;
        }
    } else if (isinf(x)) {
        if (y > 0){
            return INFINITY;
        } else if (y < 0) {
            return 0;
        } else if (isnan (y)) {
            return NAN;
        }
    } else if (x == -INFINITY) {
        if (y < 0){
            if (floor(y) == y && (int)y % 2 == 1.0){
                return -0;
            } else {
                return +0;
            }
        } else {
            if (floor(y) == y && (int)y % 2 == 1.0){
                return -INFINITY;
            } else {
                return +INFINITY;
            }
        }
    } else if (fabs(x) == 0.0){
        if (y == -INFINITY){
            return INFINITY;
        } else if (y <0 && ((floor(y) == y && (int)y % 2 == 0) || (floor(y) != y))) {
            return INFINITY;
        } else if (y > 0 && ((floor(y) == y && (int)y % 2 == 0) || (floor(y) != y))) {
            return +0.0;
        }
    } else if (x == 1) {
        if (y == -INFINITY){
            return NAN;
        } else {
            return 1;
        }
    } else if (x == -1 && (isinf(y) || y == -INFINITY)) {
        return 1;
    } else if (y == -INFINITY) {
        if (fabs(x) < 1){
            return INFINITY;
        } else if (isnan(x)) {
            return NAN;
        } else {
            return 0;
        }
    } else if (isinf(y)){
        if (fabs(x) < 1){
            return 0;
        } else if (isnan(x)) {
            return NAN;
        } else {
            return INFINITY;
        }
    } else if (x < 0 && floor(y) != y) {
        return NAN;
    } else if (isnan(x) || isnan(y)) {
        return NAN;
    } else if (x < 0){
        return NAN;
    }
    *checker = 1;
    return 1;
}

int arg_to_int (char *arg)
{
    // osetrenie - n musi byt kladne
    if ((strchr(arg, '-')) != NULL){
        fprintf(stderr, "ERROR - Argument n: %s musi byt kladny\n", arg);
        return 0;
    }
    // konvertovanie argumentu na int n
    char *endptr;
    unsigned long pomocne_n = strtoul(arg, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "ERROR - Nespravne zadany argument n: '%s', ma to byt kladne cele cislo\n", arg);
        return 0;
    // kotrola pretecenia
    } else if (errno == ERANGE) {
        fprintf(stderr,"ERROR - Pretecenie uint argumentu n, zadana privelka hodnota\n");
        return 0;
    // osetrenie - n musi byt vacsie ako 0
    } else if (pomocne_n == 0) {
        fprintf(stderr, "ERROR - Argument n: '%s', musi byt vacsi ako 0\n", arg);
    } else if (pomocne_n > UINT_MAX) {
        fprintf(stderr,"ERROR - Pretecenie uint argumentu n, zadana privelka hodnota\n");
        return 0;
    }
    unsigned int n = pomocne_n;
    return n;
}

double arg_to_double (char *arg, int *error)
{
    double cislo;
    // konvertovanie arugementu na double
    if (strcmp(arg, "inf") == 0){
        cislo = INFINITY;
    } else if (strcmp(arg, "-inf") == 0) {
        cislo = -INFINITY;
    } else if (strcmp(arg, "nan") == 0) {
        cislo = NAN;
    } else if (strcmp(arg,"-nan") == 0) {
        cislo = -NAN;
    } else {
        char *endptr;
        cislo = strtod(arg, &endptr);
        if (*endptr){
            fprintf(stderr, "ERROR - Nespravne zadany argument '%s', ma to byt IBA realne cislo\n", arg);
            *error = 1;
        }
    }
    return cislo;
}

double taylor_log(double x, unsigned int n)
{
    double sum;
    // osetrenie x
    if ((sum = log_kontrola_cisla_x(x)) != 1){
        return sum;
    }

    // taylorov polynom na vypocet ln
    sum = 0.0;
    double citatel = 1.0;
    // x = (0;1)
    if (x < 1){
        x = 1 - x;
        for (unsigned int i = 1; i <= n; i++){
            citatel *= x;
            sum -= citatel/i;
        }
    // x = (1;nekonecno)
    } else {
        for (unsigned int i = 1; i <= n; i++){
            citatel *= (x-1)/x;
            sum += citatel/i;
        }
    }

    return sum;
}

double cfrac_log(double x, unsigned int n)
{
    double cf;
    // osetrenie x
    if ((cf = log_kontrola_cisla_x(x)) != 1){
        return cf;
    }
    // cointinued fraction iteracne vypocty pre log
    double z = (x-1.0)/(x+1.0);
    cf = 0.0;
    if (n > 1) {
        for (;n > 1; n--){
            cf =(((n-1)*(n-1) * z*z)/(2*n -1.0 - cf));
        }
    }

    return (2.0 * z) / (1 - cf);
}

double taylor_pow(double x, double y, unsigned int n)
{
    double sum;
    // kontrola zakladu
    int checker;
    sum = pow_kontrola_argumentov(x, y, &checker);
    if (checker != 1){
        return sum;
    }

    //kontrola x<0
    if (x<0){
        return NAN;
    }

    // osetrenie nekonecneho vysledku logaritmu
    //vysledok bude tiez nekonecy
    double ln = taylor_log(x, n);
    if (isinf(ln)){
       return ln;
    }

    // taylorov polynom pre pow
    double citatel = 1.0;
    double menovatel = 1.0;
    double kontrolor = 0.0;
    sum = 1.0;
    for (unsigned int i = 1; i <n; i++){
        citatel *= ln*y;
        menovatel *= i;
        kontrolor = citatel/menovatel;
        if (isnan(kontrolor)){
            break;
        }
        sum += kontrolor;
    }
    return sum;

}

double taylorcf_pow(double x, double y, unsigned int n)
{
    double sum;
    // kontrola zakladu
    int checker;
    sum = pow_kontrola_argumentov(x, y, &checker);
    if (checker != 1){
        return sum;
    }
    // kontrola x<0
    if (x<0){
        return NAN;
    }
    // osetrenie nekonecneho vysledku logaritmu
    //vysledok bude tiez nekonecy
    double ln = cfrac_log(x, n);
    if (isinf(ln)){
       return ln;
    }

    // taylorov polynom pre pow
    double citatel = 1.0;
    double menovatel = 1.0;
    double kontrolor = 0.0;
    sum = 1.0;
    for (unsigned int i = 1; i <n; i++){
        citatel *= ln*y;
        menovatel *= i;
        kontrolor = citatel/menovatel;
        if (isnan(kontrolor)){
            break;
        }
        sum += kontrolor;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    int error = 0;
    // pow vetva
    if (argc == 5 && strcmp(argv[1], "--pow") == 0){
        // prekonvertovanie a osetrenie argumentov
        double x = arg_to_double(argv[2], &error);
        if (error == 1){
            return 1;
        }
        double y = arg_to_double(argv[3], &error);
        if (error == 1){
            return 1;
        }
        unsigned int n;
        if ((n = arg_to_int(argv[4])) == 0){
            return 1;
        }
        // tlac vysledkov
        printf("         pow(%g,%g) = %.12g\n", x, y, pow(x,y));
        printf("  taylor_pow(%g,%g) = %.12g\n", x, y, taylor_pow(x, y, n));
        printf("taylorcf_pow(%g,%g) = %.12g\n", x,y, taylorcf_pow(x, y, n));
    // log vetva
    } else if (argc == 4 && strcmp(argv[1], "--log") == 0){
        // prekonvertovanie o osetrenie argumentov
        double x = arg_to_double(argv[2], &error);
        if (error == 1){
            return 1;
        }
        unsigned int n;
        if ((n = arg_to_int(argv[3])) == 0){
            return 1;
        }
        //tlac vysledkov
        printf("       log(%g) = %.12g\n", x, log(x));
        printf(" cfrac_log(%g) = %.12g\n", x, cfrac_log(x, n));
        printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
    } else {
        // nespravne zadane argumenty
        fprintf(stderr, "ERROR - zle zadane argumenty programu\n");
        fprintf(stderr, "Argc: %d, ma ich byt 4 pre log a 5 pre pow\n", argc);
        fprintf(stderr, "Arg[1]: %s, ma to byt '--log' alebo '--pow'\n", argv[1]);
        return 1;
    }
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT "zavodnici.txt"
#define OUTPUT "output.txt"
#define BUFFER 100
#define DELIMITERS " :,."

typedef struct {
    int minuty;
    int sekundy;
    int setiny;
}CAS;

typedef struct {
    int startovniCislo;
    char prijmeni[30];
    char jmeno[30];
    CAS prvniKolo;
    CAS druheKolo;
}ZAVODNIK;

ZAVODNIK * input(ZAVODNIK * zavodnici, int * pocetZavodniku){
    FILE * fileRead;
    char buffer[BUFFER];
    int radky = 0;
    int sloupec;
    char* token;
    if((fileRead = fopen(INPUT, "r")) == NULL){
        printf("INPUT FILE OPENING ERROR.\n");
    } else{
        while (fgets(buffer, sizeof(buffer), fileRead) != NULL){
            if(radky != 0){
                sloupec=0;
                token = strtok(buffer, DELIMITERS);
                zavodnici = (ZAVODNIK*) realloc(zavodnici, radky * sizeof(ZAVODNIK));
                if(zavodnici != NULL){
                    while (token != NULL){
                        switch (sloupec) {
                            case 0:
                                zavodnici[radky-1].startovniCislo = atoi(token);
                                break;
                            case 1:
                                strcpy(zavodnici[radky-1].prijmeni, token);
                                break;
                            case 2:
                                strcpy(zavodnici[radky-1].jmeno, token);
                                break;
                            case 3:
                                zavodnici[radky-1].prvniKolo.minuty = atoi(token);
                                break;
                            case 4:
                                zavodnici[radky-1].prvniKolo.sekundy = atoi(token);
                                break;
                            case 5:
                                zavodnici[radky-1].prvniKolo.setiny = atoi(token);
                                break;
                            case 6:
                                zavodnici[radky-1].druheKolo.minuty = atoi(token);
                                break;
                            case 7:
                                zavodnici[radky-1].druheKolo.sekundy = atoi(token);
                                break;
                            case 8:
                                zavodnici[radky-1].druheKolo.setiny = atoi(token);
                                break;
                        }
                        sloupec++;
                        token = strtok(NULL, DELIMITERS);
                    }
                }
            }
            radky++;
        }
        if(fclose(fileRead) == EOF){
            printf("INPUT FILE CLOSING ERROR.\n");
        }
    }
    *pocetZavodniku = radky-1;
    return zavodnici;
}
void swap(ZAVODNIK *x, ZAVODNIK *y){
    ZAVODNIK pom;
    pom = *x;
    *x = *y;
    *y = pom;
}

int celkovyCas(ZAVODNIK * zavodnik){
    return (zavodnik->prvniKolo.minuty*6000+zavodnik->prvniKolo.sekundy*100+zavodnik->prvniKolo.setiny)+
            (zavodnik->druheKolo.minuty*6000+zavodnik->druheKolo.sekundy*100+zavodnik->druheKolo.setiny);
}

void bubbleSort(ZAVODNIK * zavodnik, int pocetZavodniku){
    for (int i = 0; i < pocetZavodniku - 1; ++i) {
        for (int j = 0; j < pocetZavodniku - i - 1; ++j) {
            if(celkovyCas(&zavodnik[j]) > celkovyCas(&zavodnik[j+1])){
                swap(&zavodnik[j], &zavodnik[j+1]);
            }
        }
    }
}

void output(ZAVODNIK * zavodnik, int pocetZavodniku){
    FILE *fileWrite;
    if((fileWrite= fopen(OUTPUT, "w")) == NULL){
        printf("OUTPUT FILE OPENING ERROR.\n");
    } else{
//        fprintf(fileWrite, "PORADI  START. CISLO  PRIJMENI              JMENO  1. KOLO  2. KOLO  CAS CELKEM\n");
        fprintf(fileWrite, "%6s %15s %20s %20s %8s %8s %8s\n", "PORADI", "START. CISLO", "PRIJMENI", "JMENO", "1. KOLO", "2. KOLO", "CELKEM");
        for (int i = 0; i < pocetZavodniku; ++i) {
            fprintf(fileWrite, "%6d %15d %20s %20s %02d:%2d.%02d %02d:%02d.%02d %02d:%02d.%02d\n",
                                i+1,
                                zavodnik[i].startovniCislo,
                                zavodnik[i].prijmeni,
                                zavodnik[i].jmeno,
                                zavodnik[i].prvniKolo.minuty,
                    zavodnik[i].prvniKolo.sekundy,
                    zavodnik[i].prvniKolo.setiny,
                    zavodnik[i].druheKolo.minuty,
                    zavodnik[i].druheKolo.sekundy,
                    zavodnik[i].druheKolo.setiny,
                    (celkovyCas(&zavodnik[i])/6000),
                    (celkovyCas(&zavodnik[i])%6000)/100,
                    ((celkovyCas(&zavodnik[i])%6000)%100)
            );
        }
        if(fclose(fileWrite) == EOF){
            printf("OUTPUT FILE CLOSING ERROR.\n");
        }
    }
}

int main() {
    ZAVODNIK * zavodnici = NULL;
    int pocetZavodniku = 0;
    zavodnici = input(zavodnici,& pocetZavodniku);
    printf("%d\n", pocetZavodniku);
    bubbleSort(zavodnici, pocetZavodniku);
    output(zavodnici, pocetZavodniku);
    free(zavodnici);
    return 0;
}

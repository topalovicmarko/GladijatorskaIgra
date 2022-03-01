/*Gladijatorska igra
Topalovic Marko MH 75/2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define broj_igraca 64

// Struktura koja modeluje takmicare po svojim karakteristikama
typedef struct Takmicari {
    int  ID,ogrlica,oklop,oruzje,zivotni_poeni,otpornost,snaga;
}t;

//Ciscenje bafera
void ocisti_ulazni_bafer() {
    int c;
    while (((c = getchar()) != '\n') && c != EOF);
}

//Rastuce sortiranje po oruzju ili oklopu
void sort_rastuce(t* t1, int odgovor) {
    int i,j;
    t z;
    if(odgovor==2) {

        for(i=0; i < broj_igraca; i++) {

            for(j=0; j < broj_igraca -1; j++) {


                if(t1[i].oruzje<t1[j].oruzje) {

                    z=t1[i];
                    t1[i]=t1[j];
                    t1[j]=z;
                }

            }
        }

        for(int i=0; i<64; i++)
            printf("%d:%d:%d:%d:%d:%d:%d\n", t1[i].ID, t1[i].ogrlica,t1[i].oklop, t1[i].oruzje, t1[i].zivotni_poeni, t1[i].snaga, t1[i].otpornost);
    } else if(odgovor==1) {
        for(i=0; i < broj_igraca; i++) {

            for(j=0; j < broj_igraca -1; j++) {


                if(t1[i].oklop<t1[j].oklop) {

                    z=t1[i];
                    t1[i]=t1[j];
                    t1[j]=z;
                }

            }
        }

        for(int i=0; i<64; i++)
            printf("%d:%d:%d:%d:%d:%d:%d\n", t1[i].ID, t1[i].ogrlica,t1[i].oklop, t1[i].oruzje, t1[i].zivotni_poeni, t1[i].snaga, t1[i].otpornost);
    }

}

//Opadajuće sortiranje po oruzju ili oklopu
void sort_opadajuce(t* t1, int odgovor) {
    int i,j;
    t z;
    if (odgovor==2) {
        for(i=0; i < broj_igraca; i++) {

            for(j=0; j < broj_igraca -1; j++) {


                if(t1[i].oruzje>t1[j].oruzje) {

                    z=t1[i];
                    t1[i]=t1[j];
                    t1[j]=z;
                }

            }
        }

        for(int i=0; i<64; i++)
            printf("%d:%d:%d:%d:%d:%d:%d\n", t1[i].ID, t1[i].ogrlica,t1[i].oklop, t1[i].oruzje, t1[i].zivotni_poeni, t1[i].snaga, t1[i].otpornost);

    } else if(odgovor==1) {
        for(i=0; i < broj_igraca; i++) {

            for(j=0; j < broj_igraca -1; j++) {


                if(t1[i].oklop>t1[j].oklop) {

                    z=t1[i];
                    t1[i]=t1[j];
                    t1[j]=z;
                }
            }
        }
    }

    for(int i=0; i<64; i++)
        printf("%d:%d:%d:%d:%d:%d:%d\n", t1[i].ID, t1[i].ogrlica,t1[i].oklop, t1[i].oruzje, t1[i].zivotni_poeni, t1[i].snaga, t1[i].otpornost);

}

int main() {

    FILE *dat = NULL;
    FILE *txt = NULL;
    FILE *mask = NULL;
    FILE *zipi = NULL;

    t takmicari[64];

    char naziv_datoteke[16];
    int napad[64];
    int odbrana[64];
    int i1,i2;//razlika;

    long br_bajta;
    unsigned int pomoc,maska;
    int odgovor,odgovor1,s=0;

    /* Maske za ekstrakciju
      ID_igraca     (31 - 26)    1111 1100 0000 0000 0000 0000 0000 0000    0xfc000000
      ogrlica       (25 - 24)    0000 0011 0000 0000 0000 0000 0000 0000    0x03000000
      oklop         (23 - 20)    0000 0000 1111 0000 0000 0000 0000 0000    0x00f00000
      oruzje        (19 - 16)    0000 0000 0000 1111 0000 0000 0000 0000    0x000f0000
      zivotni_poeni (15 - 10)    0000 0000 0000 0000 1111 1100 0000 0000    0x0000fc00
      snaga         (9  -  5)    0000 0000 0000 0000 0000 0011 1110 0000    0x000003e0
      otpornost     (4  -  0)    0000 0000 0000 0000 0000 0000 0001 1111    0x0000001f */
    unsigned int BitID_igraca = 0xfc000000, BitOgrlica = 0x03000000, BitOklop = 0x00f00000, BitOruzje = 0x000f0000, BitZivotni_poeni = 0x0000fc00,BitSnaga = 0x000003e0, BitOtpornost = 0x0000001f;


    printf("Unesite naziv datoteke koju zelite otvoriti\n");
    scanf("%15s", naziv_datoteke);
    ocisti_ulazni_bafer();

    // otvaramo datoteku za binarno citanje (rb)
    if((dat = fopen(naziv_datoteke,"rb")) == NULL ) {
        perror("Greska 32"); //Standardna greška : No such file or directory
        exit(1);
    }


    if (fseek(dat, 0, SEEK_END) == -1) {  // postavljanje pokazivaca datoteke na kraj datoteke
        perror(NULL);
        fclose(dat);
        exit(2);
    }

    if ((br_bajta = ftell(dat)) == -1) {  // brojanje bajta od pocetka datoteke do trenutnog pokazivaca datoteke
        perror(NULL);
        fclose(dat);
        exit(3);
    }
    rewind(dat);

    for (br_bajta = 0; br_bajta < 64; br_bajta++)  { // feof funkcija vraca != 0 ako je stigla do kraja datoteke
        fread(&pomoc, sizeof(int), 1, dat);  // citamo jedan short int iz datoteke i smestamo u promenljivu 'pomoc'
        if (feof(dat)) {
            fprintf(stderr, "Neocekivan kraj datoteke.\n");
            fclose(dat);
            exit(4);
        }
        takmicari[br_bajta].ID = (pomoc & BitID_igraca) >> 26;
        takmicari[br_bajta].ogrlica = (pomoc & BitOgrlica) >> 24 ;
        takmicari[br_bajta].oklop = (pomoc & BitOklop) >> 20 ;
        takmicari[br_bajta].oruzje = (pomoc & BitOruzje) >> 16 ;
        takmicari[br_bajta].zivotni_poeni = (pomoc & BitZivotni_poeni) >> 10 ;
        takmicari[br_bajta].snaga = (pomoc & BitSnaga) >> 5 ;
        takmicari[br_bajta].otpornost = (pomoc & BitOtpornost);

    }
    fclose(dat);  // zatvaranje datoteke

    /* for(int i=0; i<64; i++) {
         printf("%d %d %d %d %d %d %d\n", takmicari[i].ID, takmicari[i].ogrlica,takmicari[i].oklop, takmicari[i].oruzje, takmicari[i].zivotni_poeni, takmicari[i].snaga, takmicari[i].otpornost);

     }*/


    //Sortiranje po oklpou ili oruzju
    printf("Da li zelite da sortirate po (1) oklopu ili (2) oruzju?\n");
    scanf("%d", &odgovor);

    if(odgovor == 2) {

        printf("Da li zelite da ispisete u (1) rastucem ili (2) opadajucem?\n");
        scanf("%d", &odgovor1);

        if(odgovor1== 1) {

            sort_rastuce(takmicari,odgovor);

        } else if(odgovor1==2) {
            sort_opadajuce(takmicari,odgovor);
        }
    } else if(odgovor == 1) {
        printf("\nDa li zelite da ispisete u (1) rastucem ili (2) opadajucem?");
        scanf("%d", &odgovor1);
        if(odgovor1== 1) {

            sort_rastuce(takmicari,odgovor);

        } else if(odgovor1==2) {

            sort_opadajuce(takmicari,odgovor);

        }

    }

    //Nosioci ogrlica
    printf("\n\nNosioci ogrlica:\n");
    for(int i=0; i<64; i++) {
        if(takmicari[i].ogrlica != 0)
            printf("%d:%d:%d:%d:%d:%d:%d\n", takmicari[i].ID, takmicari[i].ogrlica,takmicari[i].oklop, takmicari[i].oruzje, takmicari[i].zivotni_poeni, takmicari[i].snaga, takmicari[i].otpornost);
    }

    //Upis top 5 po napadu u datoteku "napad.top"
    txt = fopen("napad.top","w");
    for (int i=0; i<64; i++) {

        if (takmicari[i].ogrlica == 1) {
            napad[i] = (takmicari[i].snaga+takmicari[i].oruzje) * 2;

        }

        else {
            napad[i]= takmicari[i].snaga+takmicari[i].oruzje;
        }

    }
    for( int i=0; i < 64; i++) {

        for(int j=0; j < 63; j++) {


            if(napad[i]>napad[j]) {
                t z;

                s=napad[i];
                napad[i]=napad[j];
                napad[j]=s;

                z=takmicari[i];
                takmicari[i]=takmicari[j];
                takmicari[j]=z;
            }
        }

    }
    for(int i=0; i<5; i++) {
        fprintf(txt," %d:%d:%d:%d:%d:%d:%d\n", takmicari[i].ID, takmicari[i].ogrlica,takmicari[i].oklop, takmicari[i].oruzje, takmicari[i].zivotni_poeni, takmicari[i].snaga, takmicari[i].otpornost);
    }
    fclose(txt); //Zatvaranje datoteke

    ////Upis top 5 po odbrani u datoteku "odbrana.top"
    mask = fopen("odbrana.top","w");
    for(int i=0; i<64; i++) {
        if (takmicari[i].ogrlica == 2) {

            maska=0x0111;
            takmicari[i].otpornost=(maska & takmicari[i].otpornost); //Bitni operator AND (&)

        }
        odbrana[i]=takmicari[i].otpornost+takmicari[i].oklop;
    }
    for( int i=0; i < 64; i++) {

        for(int j=0; j < 63; j++) {


            if(odbrana[i]>odbrana[j]) {
                t z;
                s=odbrana[i];
                odbrana[i]=odbrana[j];
                odbrana[j]=s;

                z=takmicari[i];
                takmicari[i]=takmicari[j];
                takmicari[j]=z;

            }
        }

    }
    for(int i=0; i<5; i++) {
        fprintf(mask," %d:%d:%d:%d:%d:%d:%d\n", takmicari[i].ID, takmicari[i].ogrlica,takmicari[i].oklop, takmicari[i].oruzje, takmicari[i].zivotni_poeni, takmicari[i].snaga, takmicari[i].otpornost);
    }
    fclose(mask); //Zatvaranje datoteke

//Upis top 5 po zivotnim poenima "zipi.top"
    zipi= fopen("zipi.top","w");
    for(int i=0; i<64; i++) {
        if (takmicari[i].ogrlica == 3) {
            napad[i2]=takmicari[i1].snaga+takmicari[i1].oruzje;

            takmicari[i].zivotni_poeni= ~takmicari[i].zivotni_poeni; //Bitni operator NOT (~)

        }
    }
    for( int i=0; i < 64; i++) {

        for(int j=0; j < 63; j++) {


            if(takmicari[i].zivotni_poeni>takmicari[j].zivotni_poeni) {
                t z;
                s=takmicari[i].zivotni_poeni;
                takmicari[i].zivotni_poeni=takmicari[j].zivotni_poeni;
                takmicari[j].zivotni_poeni=s;

                z=takmicari[i];
                takmicari[i]=takmicari[j];
                takmicari[j]=z;

            }
        }

    }
    for(int i=0; i<5; i++) {
        fprintf(zipi," %d:%d:%d:%d:%d:%d:%d\n", takmicari[i].ID, takmicari[i].ogrlica,takmicari[i].oklop, takmicari[i].oruzje, takmicari[i].zivotni_poeni, takmicari[i].snaga, takmicari[i].otpornost);
    }
    fclose(zipi); //Zatvaranje datoteke


//Duel, unos igraca
    printf("\nUnos ID-a igraca za duel.\n");
    printf("ID prvog igraca: \n");
    scanf("%d",&i1);
    printf("ID drugog igraca: \n");
    scanf("%d",&i2);
    takmicari[i1].ID=i1;
    takmicari[i2].ID=i2;

    napad[i1]=takmicari[i1].snaga+takmicari[i1].oruzje;
    odbrana[i1]=takmicari[i1].otpornost+takmicari[i1].oklop;


    napad[i2]=takmicari[i2].snaga+takmicari[i2].oruzje;
    odbrana[i2]=takmicari[i2].otpornost+takmicari[i2].oklop;


    printf("\nID %d, o: %d, n:%d, z:%d",takmicari[i1].ID,odbrana[i1],napad[i1],takmicari[i1].zivotni_poeni);
    printf("\nID %d, o: %d, n:%d, z:%d\n",takmicari[i2].ID,odbrana[i2],napad[i2],takmicari[i2].zivotni_poeni);

    if(i1==i2) {
        printf("\nUneti su isti igraci!\n");
        return 0;
    } else {

        /*do
         {


           if(napad[i1]>odbrana[i2])
           {
           razlika=napad[i1]-odbrana[i2];
           takmicari[i2].zivotni_poeni=takmicari[i2].zivotni_poeni-razlika;
           }
           else if(napad[i1]<odbrana[i2])
           {

           takmicari[i1].zivotni_poeni=takmicari[i1].zivotni_poeni-1;
           }

           else if(napad[i2]>odbrana[i1])
           {
           razlika=napad[i2]-odbrana[i1];
           takmicari[i1].zivotni_poeni=takmicari[i1].zivotni_poeni-razlika;
           }
           else if(napad[i2]<odbrana[i1])
           {

           takmicari[i2].zivotni_poeni=takmicari[i2].zivotni_poeni-1;
           }

           }while((takmicari[i1].zivotni_poeni||takmicari[i2].zivotni_poeni) == 0);

        */
        if((takmicari[i1].zivotni_poeni && takmicari[i2].zivotni_poeni) == 0) {
            printf("\nNereseno\n");
        } else if(takmicari[i2].zivotni_poeni==0) {
            printf("\nDrugi ima nula");
        } else if (takmicari[i1].zivotni_poeni==0) {
            printf("\nPrvi ima nula\n");
        }

    }

    return 0;


}

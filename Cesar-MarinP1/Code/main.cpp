#include<iostream>
#include<string.h>
#include<stdio.h>
#define CITY_LEN	25
#define SPORT_LEN	60
#define DISCIPLINE_LEN	100
#define NAME_LEN	60
#define NOC_LEN		4
#define GENDER_LEN	7
#define EVENT_LEN	60
#define MEDAL_LEN	7



struct medallist {
    char city[CITY_LEN]; // City in which the games were held
    int edition; // Year in which this edition of the games were held
    char sport[SPORT_LEN];
    char discipline[DISCIPLINE_LEN];
    char athlete[NAME_LEN]; // Name of the athlete
    char noc[NOC_LEN]; // 3-letter country code for the National Olympic Committee (NOC)
    char gender[GENDER_LEN]; // Men or Women
    char event[EVENT_LEN];
    char event_gender; // M or W
    char medal[MEDAL_LEN]; // Gold, Silver, or Bronze
};
int sizeofdatabase;
medallist *database;

void MedalSum() {
    int gbronze = 0;
    int gsilver = 0;
    int ggold = 0;
    int gtotal = 0;
    int bronze =0;
    int silver = 0;
    int gold = 0;
    int total = 0;
    printf("Medal Summary\nEdition\tBronze\tSilver\tGold\tTotal\n");
    for(int i = 1896; i <= 2008; i = i+4){
        for(int j = 0; j< sizeofdatabase; j++){
            if(database[j].edition == i){
                if(strncmp(database[j].medal,"Bronze",1) == 0) {
                    bronze = bronze+1;
                    gbronze = gbronze+1;
                }
                if(strncmp(database[j].medal,"Silver",1) == 0){
                    silver = silver+1;
                    gsilver = gsilver+1;
                }
                if(strncmp(database[j].medal,"Gold",1) == 0){
                    gold = gold+1;
                    ggold = ggold+1;
                }
            }
        }
        total = bronze+silver+gold;
        gtotal = gtotal+total;
        printf("%d\t%d\t%d\t%d\t%d\n", i, bronze, silver, gold, total);
        bronze = 0;
        silver = 0;
        gold = 0;
        total = 0;

    }
    printf("GT\t%d\t%d\t%d\t%d\n", gbronze, gsilver, ggold, gtotal);
}

void GenMedalSum() {

    int yeargrandtotal;
    int mbronze =0;
    int msilver = 0;
    int mgold = 0;

    int wbronze = 0;
    int wsilver = 0;
    int wgold = 0;

    int mbronzetotal = 0;
    int msilvertotal = 0;
    int mgoldtotal = 0;

    int wbronzetotal = 0;
    int wsilvertotal = 0;
    int wgoldtotal = 0;


    int ggtotal = 0;


    printf("Gender Medal Summary\n\tBronze\t\t\tSilver\t\t\tGold\n");
    printf("Edition\tMen\tWomen\tTotal\tMen\tWomen\tTotal\tMen\tWomen\tTotal\tGrand Total\n");
    for(int i = 1896; i <= 2008; i= i+4){
        for(int j = 0; j< sizeofdatabase; j++){
            if((database[j].edition == i) && (strncmp(database[j].gender, "Men", 1) == 0)){
                if(strncmp(database[j].medal,"Bronze",1) == 0) {
                    mbronze = mbronze+1;
                    mbronzetotal = mbronzetotal+1;
                }
                if(strncmp(database[j].medal,"Silver",1) == 0){
                    msilver = msilver+1;
                    msilvertotal = msilvertotal+1;
                }
                if(strncmp(database[j].medal,"Gold",1) == 0){
                    mgold = mgold+1;
                    mgoldtotal = mgoldtotal+1;
                }
            }
            else {
                if ((database[j].edition == i) && (strncmp(database[j].gender, "Women", 1) == 0)) {
                    if (strncmp(database[j].medal, "Bronze", 1) == 0) {
                        wbronze = wbronze + 1;
                        wbronzetotal = wbronzetotal + 1;
                    }
                    if (strncmp(database[j].medal, "Silver", 1) == 0) {
                        wsilver = wsilver + 1;
                        wsilvertotal = wsilvertotal + 1;
                    }
                    if (strncmp(database[j].medal, "Gold", 1) == 0) {
                        wgold = wgold + 1;
                        wgoldtotal = wgoldtotal + 1;
                    }
                }
            }

        }
        yeargrandtotal = mbronze+wbronze+msilver+wsilver+mgold+wgold;
        ggtotal = ggtotal + yeargrandtotal;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, mbronze, wbronze, (mbronze+wbronze), msilver, wsilver, (msilver+wsilver), mgold, wgold, (mgold+wgold), (mbronze+wbronze+msilver+wsilver+mgold+wgold) );
        mbronze = 0;
        wbronze = 0;
        msilver = 0;
        wsilver = 0;
        mgold = 0;
        wgold = 0;
        yeargrandtotal = 0;

    }
    printf("GT\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
           mbronzetotal, wbronzetotal, (mbronzetotal+wbronzetotal), msilvertotal, wsilvertotal, (msilvertotal+wsilvertotal), mgoldtotal, wgoldtotal, (mgoldtotal+wgoldtotal), ggtotal );
    printf("\n\tBronze\tSilver\tGold\tTotal\n");
    printf("Men\t%d\t%d\t%d\t%d\n", mbronzetotal, msilvertotal, mgoldtotal, (mbronzetotal+msilvertotal+mgoldtotal));
    printf("Women\t%d\t%d\t%d\t%d\n", wbronzetotal, wsilvertotal, wgoldtotal, (wbronzetotal+wsilvertotal+wgoldtotal));

    }

void TopAth(char *sport, char *discipline, char *gender, char *event) {
    for(int i = 1896; i<= 2008; i = i+4){
        printf("%d Top Athletes in %s %s %s %s\n", i, sport, discipline, gender, event);
        printf("Medal\tAthlete\t\tNOC\n\n");
        for(int j = 0; j< sizeofdatabase; j++){
            if( (database[j].edition == i) && (strncmp(database[j].sport, sport, 7) == 0) && (strncmp(database[j].discipline, discipline, 20) == 0) && (strncmp(database[j].gender, gender, 1) == 0) && (strncmp(database[j].event, event, 20) == 0) && (strncmp(database[j].medal, "Gold", 1) == 0) ){
                printf("%s\t%s\t%s\n", database[j].medal, database[j].athlete, database[j].noc);

            }
        }
        for(int j = 0; j< sizeofdatabase; j++){
            if( (database[j].edition == i) && (strncmp(database[j].sport, sport, 7) == 0) && (strncmp(database[j].discipline, discipline, 20) == 0) && (strncmp(database[j].gender, gender, 1) == 0) && (strncmp(database[j].event, event, 20) == 0) && (strncmp(database[j].medal, "Silver", 1) == 0) ){
                printf("%s\t%s\t%s\n", database[j].medal, database[j].athlete, database[j].noc);

            }
        }
        for(int j = 0; j< sizeofdatabase; j++){
            if( (database[j].edition == i) && (strncmp(database[j].sport, sport, 7) == 0) && (strncmp(database[j].discipline, discipline, 20) == 0) && (strncmp(database[j].gender, gender, 1) == 0) && (strncmp(database[j].event, event, 20) == 0) && (strncmp(database[j].medal, "Bronze", 1) == 0) ){
                printf("%s\t%s\t%s\n\n", database[j].medal, database[j].athlete, database[j].noc);

            }
        }

    }
    }

void Run(int l) {
    // scan input -- DO NOT TOUCH!
    for(int i= 0; i<l; i++){

        scanf("%[^\t]", database[i].city);
        scanf("%d\t", &database[i].edition);
        scanf("%[^\t]\t", database[i].sport);
        scanf("%[^\t]\t", database[i].discipline);
        scanf("%[^\t]\t", database[i].athlete);
        scanf("%[^\t]\t", database[i].noc);
        scanf("%[^\t]\t", database[i].gender);
        scanf("%[^\t]\t", database[i].event);
        scanf("%[^\t]\t", &database[i].event_gender);
        scanf("%[^\n]\n", database[i].medal);

    }
    /*
    printf("Running medal summary...\n");
    MedalSum();
    printf("\n Running Gender Medal Summary...\n");
    GenMedalSum();
    printf("\n Running Top Athlete...\n");
    char sport[] = "Aquatics";
    char discipline[] = "Swimming";
    char gender[] = "Men";
    char event[] = "100m freestyle";
    TopAth(sport, discipline, gender, event);
     */

    // read in queries, then call function based on queries.
    int q;
    scanf("%d", &q);
    printf("You will read %d queries\n",q);
    for(int i = 1; i<= q; i++){
        char firstw[7];
        scanf("%s", firstw); //FIRST SCAN
        printf("You read in first word: %s\n", firstw);

        if(strncmp(firstw, "medal", 2) == 0){
            char ignore[50];
            scanf("%[^\n]\n", ignore);
            // for now just run medal summary; later check if medal summary or medal standings
            MedalSum();
        }
        if(strncmp(firstw, "gender", 2) == 0){
            char ignore2[50];
            scanf("%[^\n]\n", ignore2);
            GenMedalSum();
        }
        if(strncmp(firstw, "top", 2) == 0){
            // read in input and run top athlete
            char sport[60];
            char discipline[100];
            char gender[7];
            char event[60];
            scanf(" athlete ");
            scanf("\"%[^\"]\"",sport);
            scanf(" \"%[^\"]\"",discipline);
            scanf("%s", gender);
            scanf(" \"%[^\"]\"\n",event);
            TopAth(sport, discipline, gender, event);
        }
        if(strncmp(firstw, "most", 2) == 0){
            char ignore3[50];
            scanf("%[^\n]\n", ignore3);
            // read in data and run most decorated n algorithm, no most decorated n algorithm yet
        }
    }
}


void starter() {
    int l;
    scanf("%d", &l);
    database = new medallist[l];
    sizeofdatabase = l;
    Run(l);
}

int main() {
        starter();
        return 0;
    }













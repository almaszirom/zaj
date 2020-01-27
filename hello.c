/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen

//Konstansok:

#define InputFileName "in.txt"

//30 napos a honap. 0 bazisu az indexeles.
#define NapokSzama 30

// Fajta nevek: A..F, az 6 féle
#define FajtakSzama 6

// Futarok: 1..9. osszesen 9 fele. 0bazisu az indexeles.
#define FutarokSzama 9
    
//beolvasas logolasa
#define LogRead 0

//statisztikak logolasa
#define LogStat 1


//Pizzafajta (A..F) beárazása.
int pizzaAr(char fajta){ 
    //Az "A" típusú pizza 1000, a "B" 1100, ..., az "F" típusú 1500 Ft-ba kerül.
    //Ez egy 100-al emelkedo sorozat, 1000-tol kezdve. 
    //A fajta meg 1esevel emelkedik 'A'-tol kezdve
    return (fajta - 'A')*100 + 1000;
}


void feladat191125()
{
    //input file
    FILE* f = fopen (InputFileName, "r");
	
	//Statisztikakhoz adatok deklaralasa, inicializalasa
    
    int napiPizzaDarab[NapokSzama];  //Melyik napon mennyi pizza lett kiszallitva
    int napiPizzaEladas[NapokSzama];  //Melyik napon mennyi erteku pizza lett eladva
    int napiPizzaMaszk[NapokSzama];  //Bitmask aszerint, hogy melyik pizzabol volt akarmennyi eladas az adott napon.
    for(int i=0; i<NapokSzama; i++){ 
        napiPizzaDarab[i] = 0;
        napiPizzaEladas[i] = 0;
        napiPizzaMaszk[i] = 0;
    }
    
    int eladottFajtaDarab[FajtakSzama]; //kulonbozo pizzak havi eladasa
    for(int i=0; i<FajtakSzama; i++) eladottFajtaDarab[i] = 0;
    
    int futarBevetel[FutarokSzama]; //futarok osszBevetele
    for(int i=0; i<FutarokSzama; i++) futarBevetel[i] = 0;

    //Beolvasas: minden sorra: /////////////////////////////////////////////////

    //aktualis adat
    int futar=0, nap=0, darab=0;
    char fajta=0;

    char str[1000];
    while(fscanf(f, "%d %d %[^\n]", &futar, &nap, str)==3){
        if(LogRead) printf("futar:%d  nap:%d  str:%s\n", futar, nap, str);
        
        //minden maradék párra a sor vegen:
        int scanRes;
        do{
            scanRes = sscanf(str, "%d %c %[^\n]", &darab, &fajta, str);
            if(LogRead) printf("  darab:%d  fajta:%c  str:%s\n", darab, fajta, str);
            
            //itt elkapjuk az osszes [futar, nap, darab, fajta] rekordot 
            //es mindenfele statisztikat csinalunk belole.
            
            if(nap>=1 && nap<=NapokSzama){ //range check
                napiPizzaDarab[nap-1] += darab; //0bazis
                if(fajta>='A' && fajta<'A'+FajtakSzama){
                    napiPizzaMaszk[nap-1] |= 1<<(fajta - 'A');
                    napiPizzaEladas[nap-1] += pizzaAr(fajta)*darab;
                }
            }
                
            if(fajta>='A' && fajta<'A'+FajtakSzama)
                eladottFajtaDarab[fajta-'A'] += darab;
                
            if(futar>=1 && futar<=9)
                futarBevetel[futar-1] += pizzaAr(fajta);
                
        }while(scanRes==3); //3 -> van még olvasnivaló
        
    }

    //Tovabbi statisztikak szamolasa////////////////////////////////////////////
    
    //Legrosszabb darabszam  (kell a 4.-hez)
    int legkevesebbNapiMennyiseg = napiPizzaDarab[0];  //napiPizzaDarab.minimumkereses
    for(int i=1; i<NapokSzama; i++)  
        if(legkevesebbNapiMennyiseg > napiPizzaDarab[i])
            legkevesebbNapiMennyiseg = napiPizzaDarab[i];

    int legjobbFajtaEladas = eladottFajtaDarab[0];     //eladottFajtaDarab.maximumkereses
    for(int i=1; i<FajtakSzama; i++) 
        if(legjobbFajtaEladas < eladottFajtaDarab[i])
            legjobbFajtaEladas = eladottFajtaDarab[i];
            
    int legnagyobbFutarBevetel = futarBevetel[0];
    for(int i=1; i<FutarokSzama; i++)
        if(legnagyobbFutarBevetel < futarBevetel[i])
            legnagyobbFutarBevetel = futarBevetel[i];
        
    //Statisztikak kiiratasa
    if(LogStat){
        printf("Eladott darab datum szerint, illetve az egyes pizzafajtak eladasanak ténye:\n");
        for(int i=0; i<NapokSzama; i++){
            char str[FajtakSzama+1];
            for(int j=0; j<FajtakSzama; j++)
                str[j] = napiPizzaMaszk[i] & (1<<j) ? 'A'+j : '.'; //Fajta betujele, ha volt eladas, kulonben meg egy pont.
            str[FajtakSzama] = '\0';
            
            printf("%2d : %4d darab %7d Ft  %s\n", i+1, napiPizzaDarab[i], napiPizzaEladas[i], str); //0based
        }
        printf("\n");
        
        printf("Eladott darab fajtak szerint:\n");
        for(int i=0; i<FajtakSzama; i++) 
            printf("%c : %4d darab\n", i+'A', eladottFajtaDarab[i]);
        printf("\n");

        printf("Futarok bevetele:\n");
        for(int i=0; i<FutarokSzama; i++) 
            printf("%d : %6d Ft\n", i+1, futarBevetel[i]);
        printf("\n");
    }
    

    //Feladatok kiszamolasa/////////////////////////////////////////////////////
    
    printf("4. Hanyadikán szállították ki a legkevesebb pizzát a hónapban?\n>>> ");
    for(int i=0; i<NapokSzama; i++)
        if(legkevesebbNapiMennyiseg == napiPizzaDarab[i])
            printf("%d ", i+1); //0based
    printf("\n\n");
    
    printf("5. Melyik típusú pizza volt a legkelendobb a hónap során?\n>>> ");
    for(int i=0; i<FajtakSzama; i++)
        if(legjobbFajtaEladas == eladottFajtaDarab[i])
            printf("%c ", i+'A');
    printf("\n\n");
    
    printf("6. Az \"A\" típusú pizza 1000, a \"B\" 1100, ..., az \"F\" típusú 1500 Ft-ba kerül.\n");
    printf("   Hányas számú futár szedte be a legtöbb pénzt a hónapban?\n>>> ");
    for(int i=0; i<FutarokSzama; i++)
        if(legnagyobbFutarBevetel == futarBevetel[i])
            printf("%d ", i+1); //0based
    printf("\n\n");
    
    printf("8. Írassa ki csokkeno sorrendben az elso 4 legeredmenyesebb nap beveteleit!\n>>> ");
    {
    	const int N = 4;		
		
		int temp[NapokSzama];
    	for(int i=0; i<NapokSzama; i++) temp[i] = napiPizzaEladas[i];
    	
    	//bubble sort, csokkeno
    	for(int i=0  ; i<NapokSzama-1; i++)
    	for(int j=i+1; j<NapokSzama  ; j++) //ciklus minden párra 
    	    if(temp[i]<temp[j]){ int a = temp[i];  temp[i] = temp[j];  temp[j] = a;	} //csere, ha rossz a sorrend
    	    
    	for(int i=0; i<NapokSzama && i<N; i++){
    		printf("%d ", temp[i]);
		}
		printf("\n\n");
	}

    printf("10. Kérje be egy nap sorszámát, majd írassa ki, hogy aznap mely típusú pizzából nem rendeltek egy darabot sem!\n");
    printf("??? Adja meg a vizsgált napot (1..30)! >");
	int vizsgaltNap = 0;
	scanf("%d", &vizsgaltNap);
    
    if(vizsgaltNap>=1 && vizsgaltNap<=NapokSzama){
        printf(">>> A vizsgalt nap: %d. Ezekbol a fajtakbol nem rendeltek egyet sem: ", vizsgaltNap);
        int maszk = napiPizzaMaszk[vizsgaltNap-1]; //0based
        
        for(int i=0; i<FajtakSzama; i++)
            if((maszk & (1<<i))==0) 
                printf("%c ", i+'A');
    }else{
        printf("A vizsgalt nap (%d) ervenytelen.", vizsgaltNap);
    }
    printf("\n\n");
}


char* szovegModosito(char* in){
	int len = strlen(in); //stings.h
	char* res = (char*)malloc(len*2+1); //2x akkora terulet lefoglalva. Annal nagyobb nem lehet.
	char* out = res;
	while(*in){ //minden egyes input karakterre:
		char c = *(in++);
		//karakter osztalyozasa
		char isNum = c>='0' && c<='9';
		char isAlpha = c>='a' && c<='z' 
		            || c>='A' && c<='Z';
		
		if(isNum){ //osszes számjegy elott van egy szóköz beszúrva
			*(out++) = ' '; 
			*(out++) = c; 
		}else if(isAlpha){ //a betuk elott nem kell szokez
			*(out++) = c; 
		}//minden mas kihagyva
	}
	return res;
}

void tombCsere(int* arr, int len){
	if(len%2 == 0){ //paros elemszamu tomboknel a 2. es az 1. fele a tombnek fel lesz cserelve
		for(int i=0; i<len/2; ++i){
			int j = i+len/2;
			int tmp = arr[i];	arr[i] = arr[j];    arr[j] = tmp;  //csere
		}
    }   
}

void feladat200112(){
  	printf("1. Készíts függvényt, ami visszaadja a paraméterként kapott szöveg olyan változatát, ahol a szövegben "
	       "eloforduló összes számjegy elott van egy szóköz beszúrva, a nem alfanumerikus (tehát nem betu, vagy "
		   "szám) karakterek pedig hiányoznak!\n");
  	char* a = szovegModosito("Hello 123 +-fvw%^&#()");
  	printf("%s\n\n", a);
  	free(a);
  	
  	printf("2. Készíts függvényt, ami a paraméterében kapott egész tömb elso felének elemeit megcseréli a második "
	  	   "felének elemeivel, ha a tömb páros szemu elembol áll! Páratlan számú elemeknél ne történjen csere! "
		   "Példa a tömbelemek sorszámaira csere esetén: 1,2,3,4->3,4,1,2\n");
	int ia[4] = {1, 2, 3, 4};
	tombCsere(ia, 3); //valtozatlan
	for(int i=0; i<4; i++) printf("%d", ia[i]); printf("\n"); 
	tombCsere(ia, 4); //csere
	for(int i=0; i<4; i++) printf("%d", ia[i]); printf("\n"); 
  	printf("\n\n");
}

int keresJegy(int* arr, int N, int jegy){
	for(int i=0; i<N; i++) 
	  	if(arr[i]==jegy) 
		    return 1;
	return 0;
}

int keresJegyNem(int* arr, int N, int jegy){
	for(int i=0; i<N; i++) 
	  	if(arr[i]!=jegy)   //nem-egyenlo!!!!!!!!
		    return 1;
	return 0;
}

int szamolJegy(int* arr, int N, int jegy){
	int res;
	for(int i=0; i<N; i++) 
	  	if(arr[i]==jegy) 
		    res ++;
	return res;
}

void jelesEllenoriz(int* arr, int N){
	printf(".) volt bukas? %s\n", keresJegy(arr, N, 1) ? "igen" : "nem");
	printf("a) mind jeles? %s\n", keresJegyNem(arr, N, 5) ? "nem" : "igen");
	printf("b) ennyi jeles volt: %d\n\n", szamolJegy(arr, N, 5));
}

void feladat200113(){ //51
//Egy tömb 10 hallgató jegyeit tartalmazza. Készíts programot, ami kiírja, volt-e bukás a csoportban! A leggyorsabb megoldásra törekedj!
//a Írasd ki, hogy jeles volt-e az osztály (minden jegy ötös), vagy nem!
//b Írasd ki, hogy hány jeles volt az osztályban!
	#define N 10
	int mindJeles  [N] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
	int nehanyJeles[N] = {5, 5, 2, 1, 5, 1, 1, 2, 3, 5 };
	jelesEllenoriz(mindJeles  , N);
	jelesEllenoriz(nehanyJeles, N);
}



int main(){
  //feladat191125();
  //feladat200112();
  feladat200113();
  return 0;	
}

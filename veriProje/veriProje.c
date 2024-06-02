#include <stdio.h>
#include <stdlib.h>
#define MAX 50

struct Node {         //grafi adjacency list haline getirebilmek icin node olusturulmasi
    char vertex;	  //node icinde weight(kenar agirligi), vertex ve sonraki dugumun adresi tutulur
    int weight;
    struct Node *next;
};

typedef struct {      //adjacency list'in her listesinin head elemaný 
    struct Node *head;
}List;

typedef struct {      //Olusturulan komsuluk listelerinin hepsini gezebilmemizi saglayan yapi
    List lists[MAX];  //bir liste listesi tutar
    int top;
}Graph;

typedef struct {      //DFS ile gezi esnasinda kullanilan stack
    char stack[MAX];  //vertexleri tutar
    int cevre;        //bu vertexlerin gezilmesiyle bir cycle olustugunda bu cycle'ýn olusturdugu cokgenin cevresini tutar
    int top;		  //stack'e ekleme cikarma yapabilmek icin tutulan deger
}Stack;

/*
@brief createNode fonksiyonu, komsuluk listesine eklenmesi icin yeni bir dugum olusturur ve komsuluk bilgilerini(vertex ve weight) bu dugume kaydeder.
@param vertex fonksiyonda oluþturulacak dugume eklenecek vertex bilgisi
@param weight fonksiyonda oluþturulacak dugume eklenecek kenar agirligi bilgisi
@return ustunde islem yapilan dugumu dondurur.
*/
struct Node *createNode(char vertex, int weight) {      
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));    //yeni dugum icin dinamik olarak yer acilir
    if (newNode==NULL) {
        printf("Yer acilamadi.\n");
        exit(1);
    }
    newNode->vertex = vertex;                  //yeni dugume bilgiler aktarilir.
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;                            //yeni dugum dondurulur
}
/*
@brief initializeList fonksiyonu, yeni karsilasilan bir dugum icin komsuluk listesi baslatilmasini saglar.
@param list, head node'u NULL'a esitlenecek liste
*/
void initializeList(List *list) {
    list->head=NULL;
}
/*
@brief push fonksiyonu, yeni olusturulan listeyi graph'a ekler
@param liste, graph'a eklenecek yeni olusturulmus liste
@param g, listeleri tutan stack yapisi
*/
void push(Graph *g, List *liste) {
    g->lists[g->top] = *liste;      //liste graph'a eklenir
    g->top++;
}
/*
@brief graphKontrol fonksiyonu, graph'ý tutan stack yapisinda gezerek tum listelerin head vertexlerini,
parametre olarak aldigi vertex'le karsilastirir. Daha onceden listesi olusturulmus bir vertex ise listesinin sonuna komsusunun bilgisini ekler.
Onceden eklenmemis bir vertex ise yeni bir liste olusturulup head elemanina parametre olarak alinan vertex eklenir. head->next elemanina da parametre 
olarak aldigi nextVertex eklenir.
@param vertex onceden graph'a eklenip eklenmedigi kontrol edilecek ve alinan sonuca gore islem gerceklestirilecek vertex bilgisi
@param nextVertex vertex'e komsulugu bulunan vertex
@param weight, vertex ve nextVertex dugumleri arasinda olusan kenarin agirlik bilgisi
@param g icinde gezilecek graph 
*/
void graphKontrol(char vertex, char nextVertex, int weight, Graph *g) {
    int i = 0, flag = 0;
    struct Node *newNode;

    while (i!=g->top && flag==0) {              //graph icindeki listelerde gezilerek head node'larda tutulan vertexlerle parametre olarak alinan vertexler karsilastirilir.
        if (g->lists[i].head->vertex==vertex) {
            flag = 1;                           //vertexin daha onceden olusturulmus listesi varsa flag 1 yapilir ve donguden cikilir.
        }
        i++;
    }
    if (flag==0) {                              //vertex'in daha onceden olusturulmus listesi yoksa durumu
        List liste;                             //yeni bir liste olusturulur.
        initializeList(&liste);
        newNode=createNode(vertex, 0);          //createNode fonksiyonu cagirilarak vertex icin bir node olusturulur.
        liste.head=newNode;                     //vertex, listenin head node'u olarak atanir
        newNode=createNode(nextVertex, weight); //createNode fonksiyonu cagirilarak vertex'in komsusu olan nextVertex icin bir node olusturulur. 
												//Vertex ile nextVertex arasindaki kenarin agirligi next vertex icin olusturulan node'un weight elemanina atanir.
        liste.head->next=newNode;               //head->next'e nextVertex'in icinde bulundugu node atanir.
        push(g, &liste);                        //Olusturulan liste graph'a eklenir.

    } else {                                    //vertex'in listede onceden bulunma durumu
        i--;                                    //i degiskeni vertex'in kacinci listede bulundugunu tutar.
        struct Node *p = g->lists[i].head;      
        while (p->next!=NULL) {					//vertex'in bulundugu liste icinde gezerek son node'a kadar ilerlenir. Boylece yeni komsunun eklenecegi konum bulunur.
            p=p->next;
        }
        p->next=createNode(nextVertex, weight); //nextVertex icin bir node olusturulur ve bu node vertex'in listesinin sonuna eklenir.
    }
}
/*
@brief readFromFile fonksiyonu, fileName adli dosyayi acar ve satir satir okuma yapar. 
Her satir icin graphKontrol fonksiyonunu cagirir ve okunan bilgiler bu fonksiyonda liste yapisina eklenir.
@param fileName icinde graph'ýn komsuluk bilgilerini iceren dosyanin adi
@param dizi dosyadan okunan bilgilerin gecici olarak tutuldugu dizi
@param g okunan komsuluk bilgilerinin eklenecegi graph yapisi
*/
void readFromFile(char *fileName, char *dizi, Graph *g) {
    FILE *file = fopen(fileName, "r");                       //dosya okuma modunda acilir
    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        exit(1);
    }
    while (fscanf(file, " %c %c %d ", &dizi[0], &dizi[1], &dizi[2]) == 3) {    //her satirdan 3 deger okunur ve dosya sonuna gelinene kadar devam eder
        printf("okunan degerler: %c, %c, %d\n", dizi[0], dizi[1], dizi[2]);
        graphKontrol(dizi[0], dizi[1], dizi[2], g);                            //ornek olarak okunacak A B 2 bilgisinde hem A icin hem B icin graphKontrol fonksiyonu cagirilir.
        graphKontrol(dizi[1], dizi[0], dizi[2], g);                            //boylece hem A'nin listesine hem de B'nin listesine ekleme yapilmis olur
    }
    fclose(file);                                           //tum satirlar okundugunda dosya kapanir
}
/*
@brief printGraph fonksiyonu, olusturulan adjacency list yapisini ekrana yazdirir
@param g icinde ekrana yazdirilacak listeleri tutan graph yapisi
*/
void printGraph(Graph *g) {
    int i;
    printf("\ngraph:\n");
    for (i = 0; i < g->top; i++) {                      //tum listeler gezilir
        printf("%d. = %c", i, g->lists[i].head->vertex);
        struct Node *p = g->lists[i].head;
        while (p->next != NULL) {                       //listelerin icerikleri ekrana yazdirilir
            p = p->next;
            printf(" -> %c(%d)", p->vertex, p->weight);
        }
        printf("\n");
    }
}
/*
@brief getIndex fonksiyonu, parametre olarak aldigi vertex'in graph icindeki listesinin sirasini bulur
@param g listeleri tutan, icinde arama yapilacak graph yapisi
@param vertex listesinin graph icindeki sirasi bulunacak dugum
@return vertex'in listesinin graph icindeki sirasini dondurur
*/
int getIndex(Graph *g, char vertex) {
	int i;
    for (i=0;i<g->top;i++) {                       //tum listeler gezilerek listelerin head vertexleri ile parametre olarak alinan vertex karsilastirilir.
        if (g->lists[i].head->vertex == vertex) {  //karsilastirilan listenin head vertex'i ile parametre olarak alinan vertex ayni ise vertex'in listesi bulunmus olur.
            return i;                              //i degiskeninde listenin sirasi tutulur ve bu degisken dondurulur.
        } 
    }
    return -1;                                     //eger vertex bulunamazsa -1 dondurulur
}
/*
@brief checkVertex fonksiyonu, usedVertex dizisini parametre olarak alir, bu dizide daha once DFS islemi baslatilmis dugumler tutulur.
checkVertex fonksiyonu ile parametre olarak alinan bir vertexin usedVertex dizisinde olup olmadigi kontrol edilir.
@param usedVertex daha once DFS islemi baslatilmis dugumlerin tutuldugu dizi
@param g listeleri tutan graph yapisi
@param vertex usedVertex dizisinde olup olmadigi kontrol edilecek dugum
@return vertex usedVertex dizisinde varsa 1 yoksa 0 dondurur
*/
int checkVertex(char *usedVertex, char vertex, Graph *g){
	int i;
	for(i=0;i<g->top;i++){           //usedVertex dizisi icinde gezilir.
		if(usedVertex[i]==vertex){   //vertex, usedVertex dizisi icinde bulunursa 1 dondurulur
			return 1;
		}
	}
	return 0;                        //vertex, usedVertex dizisi icinde bulunmazsa 0 dondurulur
}
/*
@brief addCycle fonksiyonu, DFS ile gezinme sirasinda bulunan cycle'larin daha once bulunup bulunmadigini kontrol eder ve daha once bulunmamis bir cycle ise cycles matrisine ekler
@param cycles DFS islemi ile bulunmus cycle'lari, bu cycle'larin cevre uzunluklarini ve dugum sayisini (kacgen oldugunu) tutan matris
@param s DFS isleminde kullanilmis, cycle'in bilgilerini tutan stack yapisi
@param g listeleri tutan graph yapisi
@param cycleCounter bulunmus cycle sayisi
*/
void addCycle(char **cycles, Stack *s, Graph *g, int *cycleCounter){
	int i, flag=0, j, k, isMatched=0;
	for(i=0;i<(*cycleCounter);i++){                   //matrisin tum satirlari gezilir
		if(cycles[i][0]==s->top){                     //kenar sayisi, parametre olarak alinan cycle'in kenar sayisina esit olan cycle'larla karsilastirma islemi yapilir
			for(j=1;j<=s->top;j++){                   
				for(k=0;k<s->top;k++){
					if(cycles[i][j]==s->stack[k]){
						flag++;                       //eger tum vertexler birbirinin ayniysa flag, cycle'in kenar sayisina esit olacak kadar artar.
					}
				}
			}
		}
		if(flag==s->top && s->cevre==cycles[i][j]){   //eger tum vertexler ve cevre uzunluklari ayniysa ayni cycle tekrar bulunmus demektir.
			isMatched=1;                              //ayni cycle'ý tekrar yazdirmamak icin isMatched 1'e esitlenir.
		}
		flag=0;
	}
	if(isMatched==0){                                     //daha onceden cycles matrisine eklenmemis bir cycle'sa
		(*cycleCounter)++;
		cycles[(*cycleCounter)-1][0]=s->top;              //s->top'ta tutulan vertex sayisi(kacgen oldugu), cycles matrisinin bu cycle'in eklenecegi satirinin ilk elemanina eklenir.
		for(j=1;j<=s->top;j++){
			cycles[(*cycleCounter)-1][j]=s->stack[j-1];   //matrise cycle'ýn vertex bilgileri eklenir.
		}
		cycles[(*cycleCounter)-1][j]=s->cevre;            //matrise cycle'ýn cevre uzunlugu eklenir.
														  //Ornek: 3 A B C 12 seklinde satirlar elde edilir.
	}
}
/*
@brief calculatePolygonCount fonksiyonu, hangi cokgen tipinden kacar tane bulundugunu hesaplar ve ekrana yazdirir.
@param cycles DFS islemi ile bulunmus cycle'lari, bu cycle'larin cevre uzunluklarini ve dugum sayisini (kacgen oldugunu) tutan matris
@param cycleCounter bulunmus cycle sayisi
*/
void calculatePolygonCount(char **cycles, int *cycleCounter){
	int i, j, edgeCount=3, totalCycleCounter=0, polygonCounter=0;     //cokgenler en az 3 kenarli oldugundan edgeCount 3'ten baslatilir.
	while(totalCycleCounter<*cycleCounter){               //bulunan tum cycle'lar gezilir
		for(i=0;i<*cycleCounter;i++){                     //tum cycle'lar gezilene kadar her cokgen cesidi icin matris icinde arama yapilir   
			if(cycles[i][0]==edgeCount){                  //edgeCount'a esit sayida kenari olan cycle bulundugunda
				polygonCounter++;                         //totalCycleCounter(islem yapilan cycle sayisi) ve polygonCounter(o dongudeki edgeCount'a esit kenar sayisina sahip cycle sayisi) bir arttirilir.
				totalCycleCounter++;
			}
		}
		if(polygonCounter>0){                             //bu dongu icindeki edgeCount sayisi kadar kenari olan cycle'lar bulunmussa sayisi ekrana yazdirilir
			printf("%d'gen sayisi: %d\n", edgeCount, polygonCounter);
		}
		polygonCounter=0;
		edgeCount++;                                       //bir sonraki dongu icin edgeCount 1 arttirilir
	}	
}
/*
@brief printCycle fonksiyonu, cycle'lari cokgen tiplerine gore gruplayip siralayarak ekrana yazdirir.
@param cycles DFS islemi ile bulunmus cycle'lari, bu cycle'larin cevre uzunluklarini ve dugum sayisini (kacgen oldugunu) tutan matris
@param cycleCounter bulunmus cycle sayisi
*/
void printCycle(char **cycles, int *cycleCounter){
	int i, j, edgeCount=3, totalCycleCounter=0, polygonCounter=1;          //cokgenler en az 3 kenarli oldugundan edgeCount 3'ten baslatilir.
	printf("\nSekil sayisi: %d\n",(*cycleCounter));                        //toplam sekil sayisi ekrana yazdirilir
	calculatePolygonCount(cycles, cycleCounter);                           //calculatePolygonCount fonksiyonu cagirilarak hangi cakgenden kacar tane oldugu ekrana yazdirilir
	while(totalCycleCounter<*cycleCounter){                                //tum cokgenler ekrana yazdirilana kadar surecek bir dongu baslatilir
		for(i=0;i<*cycleCounter;i++){
			if(cycles[i][0]==edgeCount){                                   //icinde bulunulan dongudeki edgeCount sayisi kadar kenari bulunan cycle'lar sirayla numaralandirilarak ekrana yazdirilir
				printf("%d. %d'gen: ",polygonCounter, cycles[i][0]);	   
				for(j=1;j<=cycles[i][0];j++){
					printf("%c ", cycles[i][j]);
				}
				printf("%c	", cycles[i][1]);
				printf("Cevre uzunlugu: %d\n", cycles[i][cycles[i][0]+1]); //cycle'in cevre uzunlugu da ekrana yazdirilir
				polygonCounter++;
				totalCycleCounter++;
			}
		}
		printf("\n");
		polygonCounter=1;
		edgeCount++;                               //sonraki dongude varsa bir fazla sayida kenari olan cycle'larin ekrana yazdirilabilmesi icin edgeCount 1 arttirilir
	}

}
/*
@brief DFS fonksiyonu, DFS algoritmasini kullanarak recursive sekilde grafý gezer. Cycle bulundugunda addCycle fonksiyonunu cagirarak bu cycle'in, daha once gezilmemisse, matrise ekle. 
@param s graph gezilirken ustunden gecilen dugumlerin tutuldugu stack. Cycle bulundugu zaman cevre bilgisini de tutar.
@param g komsuluk listelerini tutan yapi
@param vertex DFS isleminin baslangicinin yapilacagi vertex'i tutar
@param listNo parametre olarak alinan vertex'in listesinin sira numarasini tutar
@param visited gezilen dugumleri tutan dizi
@param parent parametre olarak alinan vertex'ten once gezilmis vertex'i tutar
@param start DFS'nin uygulanmaya basladigi vertex'i tutar
@param usedVertex graph'ta daha onceden DFS fonksiyonunun baslangici olarak kullanilmis dugumleri tutar
@param cycles DFS islemi ile bulunmus cycle'lari, bu cycle'larin cevre uzunluklarini ve dugum sayisini (kacgen oldugunu) tutan matris
@param cycleCounter bulunmus cycle sayisi
*/
void DFS(Stack *s, Graph *g, char vertex, int listNo, int visited[], int parent, char start, char *usedVertex, char **cycles, int *cycleCounter) {
    struct Node *node = g->lists[listNo].head->next;    //node, parametre olarak alinan vertex'in komsusunu gosterir
    struct Node *tmp;
	int i, j, vertexUsed=0, index;
	visited[listNo] = 1;                                //parametre olarak alinan dugum ziyaret edildi olarak isaretlenir ve stack'e eklenir
    s->stack[s->top] = vertex;
    s->top++;
    while (node!=NULL) {                                //parametre olarak alinan dugum'un tum komsulari gezilir
        i = getIndex(g, node->vertex);                  //komsu dugumun listeler icindeki konumu bulunur 
        if (i!=-1 && i!=parent && node->vertex==start) {        //gezilen dugum start dugumune yani baslangicta DFS cagirilirkenki dugume esitse ve parent degilse cycle tespit edilmis olur
            s->cevre+=node->weight;                                 //cycle icin cevre hesaplamasina baslanir. Oncelikle ustunde bulunulan dugumun weight'i cevreye eklenir
            for (j=0; j<s->top; j++) {                              //cycle'i olusturan tum vertex'ler gezilerek kenarlarin agirliklari bulunur
                if (j<s->top-1) {
                    index = getIndex(g, s->stack[j]);               //stack'teki vertexin listesinin indexi bulunur
                    tmp = g->lists[index].head->next;                
                    while (tmp!=NULL) {                             //vertex'in komsulari gezilir ve stack'te kendisinden sonra gelen vertex aranir
                        if (tmp->vertex == s->stack[j+1]) {
                            s->cevre+=tmp->weight;                  //kendisinden sonra gelen vertex bulundugunda weight kullanilarak kenar agirligi cevreye eklenir
                        }
                        tmp = tmp->next;
                    }
                }
            }
            addCycle(cycles, s, g, cycleCounter);         //addCycle fonsiyonuna cycle olusturan stack gonderilir ve gerekli kosullar saglaniyorsa (daha once eklenmis bir cycle degilse) bu cycle cycles matrisine eklenir  
            s->cevre=0;                                   //cycle eklendikten sonra diger cevrime hazirlanmak icin cevre 0'lanir
        }
        vertexUsed=checkVertex(usedVertex, node->vertex, g);        //checkVertex fonksiyonu ile DFS'ye baslanmadan once bulundugumuz node'un daha once DFS baslatmada kullanilmis bir node olup olmadigi kontrol edilir.
        															//eger kullanilmissa vertexUsed, 1'e esitlenir
        if (!visited[i] && vertexUsed==0) {                //uzerinde bulundugumuz node daha once DFS baslatmada kullanilmamis ve visited degilse bu vertex ile DFS fonksiyonu cagirilir
            DFS(s, g, node->vertex, i, visited, listNo, start, usedVertex, cycles, cycleCounter);
        }
        node=node->next;
    }

    visited[listNo]=0;                //dongu tamamlandiginda stack'ten bir eleman cikarilir ve uzerinde bulundugumuz dugum tekrar ziyaret edilebilir hale getirilir
    s->top--;                         
}

int main() {
    int i, cycleCounter=0, *visited;                          
    char fileName[MAX], dizi[3], *usedVertex, **cycles;
    /*
    cycleCounter: cycle sayisini tutar
    visited: DFS algoritmasinda kullanilir gezilmis dugumleri tutar
    fileName: okunacak dosyanin ismini tutar
    dizi:  dosyadan okunan satirlar gecici olarak bu dizide tutulur
    usedVertex: DFS cagirilirken baslangic dugumu olarak verilen vertexleri tutar. 
	Diger dongulerde bu dugumlerin tekrar uzerinden gecilmesini ve ayni cycle'larin tekrar okunmasini engeller.
    cycles: bulunan cycle'larin icerdigi vertex sayisini, vertex bilgilerini ve cevre uzunlugunu tutar
    */
    cycles = (char **)malloc(MAX * sizeof(char *));             //dinamik olarak yer acma
    for(i=0;i<MAX;i++) {
        cycles[i] = (char *)malloc(MAX * sizeof(char));
    }
    if (cycles == NULL) {
        printf("Yer acilamadi.\n");
        exit(1);
    }

    Graph *g = (Graph*)malloc(sizeof(Graph));
    if (g == NULL) {
        printf("Yer acilamadi.\n");
        exit(1);
    }
    g->top = 0;
    
	Stack *s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) {
        printf("Yer acilamadi.\n");
        exit(1);
    }
    s->top = 0;
	s->cevre=0;

    printf("dosya adi: ");                           //kullanicidan dosya ismi alinir
    scanf("%s", fileName);

    readFromFile(fileName, dizi, g);                 //readFromFile fonksiyonu cagirilarak dosya okunur ve komsuluk listesi olusturulur
    printGraph(g);

    visited=(int*)malloc(g->top);                    //dinamik olarak yer acma
	usedVertex= (char*)malloc(g->top);
	for(i=0;i<g->top;i++){                           //olusturulan dizilerin tum elemanlarini 0'lama
		visited[i]=0;
		usedVertex[i]=0;
	}
	
    for (i = 0; i <g->top; i++) {                           //her dugum icin , eger visited degilse, DFS fonksiyonu cagirilir ve cycle'lar belirlenir.
        if (!visited[i]) {
            DFS(s, g, g->lists[i].head->vertex, i, visited, -1, g->lists[i].head->vertex, usedVertex, cycles, &cycleCounter);       //-1 olarak gonderilen parametre 'parent'i belirtir.
																																	//DFS cagirilirken parent henuz olmadigi icin -1 olarak gonderilir.	 
			usedVertex[i]=g->lists[i].head->vertex;	       //her dongude baslangic olarak kullanilan vertex usedVertex dizisine eklenir ve sonraki dongulerde tekrar bu dugumlerden gecilmesi engellenir.
        												   //boylece ayni cycle'larin tekrar okunmasinin onune gecilir
		}
    }
    printCycle(cycles, &cycleCounter);                     //cokgen bilgileri ekrana yazdirilir

    for (i=0; i<MAX; i++) {                                //dinamik olarak acilan yerlerin serbest birakilmasi
        free(cycles[i]);
    }
    free(cycles);
    free(visited);
    free(usedVertex);
    free(g);
    free(s);
    
    return 0;
}

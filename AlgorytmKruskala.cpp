#include <iostream>
#include <fstream>     // dodanie biblioteki pozwalaj¹cej na obs³ugê plików

using namespace std;

int n, m;
int A[1000000][3], C[1000000],T[1000000];


//odczytywanie danych z pliku
void plik(){

    fstream plik;                                       //klasa strumienia pliku

    plik.open("in1.txt", std::ios::in);                 //otwarcie pliku (ios in, czyli tylko odczyt)

    if (plik.good()){                                   //sprawdzenie, czy plik zosta³ otwarty

        cout<<"Otwatro pomyslnie plik.\nOdczytwywanie danych."<<endl;

        for(int i = 0;!plik.eof() && (i <= m);i++){     //odczytywanie pliku dokóki nie nast¹pi koniec pliku "eof"
                                                        //ORAZ liczba wczytanych wierszy nie przekracza ilosci krawêdzi
            if(i == 0){                                 //wczytanie pierwszego wiersza
                plik>>n;
                plik>>m;
            } else{
                for(int j = 0; j < 3;j++){              //wczytywanie danych z 3 kolumn w petli
                    plik>>A[i-1][j];                    //wczytywanie kolejnych elementow z pliku - krawędzi i ich wag
                }                                       //A[i-1][j] -1 temu, bo ju¿ wczytaliœmy pierwszy wiersz przy i=0,
            }                                           //ale chcemy zachowaæ zapisywanie danych od pocz¹tku tablicy
        }

        plik.close();                                   //zamkniêcie pliku
        cout<<"Odczyt zakonczony."<<endl<<"Zamknieto plik."<<endl;

    }
    else{                                               //plik nie zosta³ otwarty, wiêc program zg³osi³ b³¹d
        cout<<"Blad! Nie mozna otworzyc pliku! Sprawdz czy istnieje!"<<endl<<endl;
    }
}

//sprawdzanie, czy element należy do drzewa
int szukaj(int i){
    if(C[i] == i){
        return i;
    } else{
        return (C[i] = szukaj(C[i]));
    }

}

bool sprawdz (int a, int b){

    int znalezioneA = szukaj(a);                        //szukanie sprawdzanych elementów w drzewie
    int znalezioneB = szukaj(b);

    if(znalezioneA == znalezioneB){
        return false;                                   //sprawdzane elementy należą do tego samego drzewa, więc nie dodajemy ich
    }

//tuataj łączymy ze sobą drzewa, które są połączone sprawdzanym w tej chwili wierzchołkiem, łączymy, bo funckia zwórci nam true.
    if(T[znalezioneA] <= T[znalezioneB]){
        T[znalezioneB] += T[znalezioneA];               //zwiększa się ilość elementów w naszym drzewie
        C[znalezioneA] = znalezioneB;                   //
    } else{
        T[znalezioneA] += T[znalezioneB];
        C[znalezioneB] = znalezioneA;
    }

    return true;                                        //funkcja zwracając true każe dodać koszt tej krawędzi grafu
}

void odczytaneDane(){

    for(int i = 0; i < m; i++){
            for(int j = 0; j < 3; j++){
                cout<<A[i][j];
                cout<<" ";
            }
            cout<<endl;
    }
}

//sortowanie b¹belkowe z użyciem bufora
void sortowanie(){
    int i, iloscElementow = m, bufor;

    while(iloscElementow){

            for(i = 0; i < iloscElementow - 1; i++){

                if (A[i][2] > A[i+1][2]){

                    bufor = A[i][0];
                    A[i][0] = A[i+1][0];
                    A[i+1][0] = bufor;

                    bufor = A[i][1];
                    A[i][1] = A[i+1][1];
                    A[i+1][1] = bufor;

                    bufor = A[i][2];
                    A[i][2] = A[i+1][2];
                    A[i+1][2] = bufor;
                }
            }
            iloscElementow--;
    }
}

//Algrorytm Kruskala, czyli szukamy najoptymalniejszego rozwiązania poprzez sprawdzanie kolejnych, posortowanych rosnąco elementów
int algorytmKruskala(){

    int i, koszt=0;

    for(i=0; i<n; i++){                                 //inicjacja algorytmu Kruskala
		C[i] = i;                                       //Na początku każdy wierzchołek jest osobnym drzewem
		T[i] = 1;                                       //ilość elementów w tym drzewie
	}

	for(i = 0; i < m; i++){                             //w tej pętki sprawdzamy nasze kolejne, posortowane elementy
        if(sprawdz(A[i][0], A[i][1])){                  //sprawdzanie, czy krawędzie łączą wierzchołki w tym samym drzewie
            koszt += A[i][2];                           //jezeli nie to dodajemy koszt tej gałęzi
        }
	}
	return koszt;
}

//utworzenie pliku out1.txt i zapisanie w nim wyniku
void zapisPliku(int i){
    fstream zapis;
    zapis.open( "out1.txt", ios::out );
    zapis<<i<<endl;
    zapis.close();
}

int main (){

    cout<<"Program wczytuje dane grafu z pliku in1.txt"<<endl;
    cout<<"Plik in1.txt koniecznie musi byc w tej samej lokalizaji co program!"<<endl;
    cout<<"Upewnij sie, ze tak jest!"<<endl;

    plik();                                             //wywyołanie fukncji wczytiującej dane z pliku

    cout<<"Odczytano nastepujace dane z pliku:"<<endl;
    cout<<"Ilosc wierzcholkow grafu: " << n;            //ilość wierzchołków grafu
    cout<<", ";
    cout<<"ilosc polaczen w grafie: " << m;             //ilość połączeń w grafie
    cout<<endl;

    odczytaneDane();                                    //wyrzucenie odczytanych danych

    cout<<"Sortowanie danych."<<endl;

    sortowanie();                                       //sortowanie krawędzi według ich wag

    cout<<"Uporzadkowane dane:"<<endl;

    odczytaneDane();                                    //wyrzucenie posortowanych danych według wag (kosztów)

    cout<<"Szukanie najtanszego wariantu, "<<endl<<"metoda minimalnego drzewa rozpinajacego."<<endl;
    cout<<"Algorytm Kruskala."<<endl;


    cout<<"Obliczono minimalna droge! Wynosi ona: ";
    int wynik = algorytmKruskala();                                 //wywołanie algorytmu Kruskala znajdowania minimalnej drogi w grafie
    cout<<wynik;
    cout<<"."<<endl;

    zapisPliku(wynik);

    cout<<"Wygenerowano plik C:\\lokalizacja_programu\\out1.txt z wynikiem!"<<endl;
    cin.get();
    return 0;
}

/**
 *  @file    Estymacja_widma_FFT.cpp
 *  @date    30/7/2018
 *	@author  Daniel Polak
 *           Mechatronika st. II, sem. 1
 *	         Wydzia³ Mechaniczny
 *	         Politechnika Gdañska
 *
 *  @brief badanie szybkoœci i dok³adnoœci dzia³ania algorytmu FFT
 *
 *
 *  Nieniejszy program zosta³ stworzony w ramach zaliczenia
 *  projektu P21 na przedmiot Programowanie Wspó³bie¿ne. Celem
 *	projektu by³o napisanie programu wielow¹tkowego,
 *	pozwalaj¹cego na wyznaczenie widma za pomoc¹ klasycznego
 *	algorytmu FFT oraz zbadanie wp³ywu postaci sygna³u
 *	wejœciowego na szybkoœæ wykonywania algorytmu. Zgodnie z
 *	za³o¿eniami, sygna³ wejœciowy mo¿e byæ w postaci ci¹gu
 *	liczb typu float (pojedynczej precyzji) lub double (podwójnej
 *  precyzji). D³ugoœæ ci¹gu oraz to dla jakiego typu sygna³u ma
 *  zostaæ wykonana analiza s¹ zadawane przez u¿ytkownika. Program
 *  podzielono na klika plików, zawieraj¹cych funkcje i klasy
 *  realizuj¹ce okreœlone zadania.
 *
 *
 *	W niniejszym pliku zawarty jest podprogram g³ówny int main().
 *	Zawiera on domyœln¹ inicjalizacje analizownego sygna³em,
 *	a tak¿e inicjalizacje zwi¹zane z ustawieniem folderu roboczego
 *	i jêzyka konsoli. Wszelkie pliki tekstowe, zawieraj¹ce wyniki
 *	dzia³ania programu, bêd¹ zapisane do folderu zgodnego ze
 *	œciê¿k¹ ..\..\Skrypty_Matlab (Projekt P21 - Daniel Polak\
 *	Skrypty_Matlab, je¿eli zosta³a zachowana struktura folderów).
 *	Zawarta jest tu tak¿e pêtla g³ówna, która wyœwietla menu
 *	g³ówne, z którego u¿ytkowanik ma mo¿liwoœæ wyboru
 *  funkcjonalnoœci, która ma zostaæ zrealizowana. Program
 *	pozwala na zrealizowanie 3-ech g³ównych funkcjonalnoœci:
 *	> estymacjê widma dla pojedynczego sygna³u, pomiar czasu jego
 *    wyznaczania i zapisanie go do pliku binarnego,
 *	> estymacjê widma dla dwóch sygna³ów, pomiar czasu ich
 *    wyznaczania i zapisanie ich do plików, dziêki czamu mo¿liwe
 *    jest ich póŸniejsze porównanie,
 *  > wykonanie pomiaru czasów dla serii sygna³ów o ró¿nych typach
 *    i d³ugoœciach i wyœwietlenie wyników w tabeli.
 *
 *
 *	Pozosta³e pliki nag³ówkowe i odpowiadaj¹ce pliki Ÿród³owe: 
 *	> DPinit.h
 *	  W pliku tym zawartych zosta³o kilka makr i zmiennych
 *	  globalnych, które u¿ywane s¹ w niemal wszystkich pozosta³ych
 *	  plikach.
 *	> DPfft.h
 *	  Plik zawiera funkcje realizuj¹c¹ algorytm fft oraz
 *	  ifft (ifft nie jest jednak wyznaczane w programie).
 *	> DPsignals.h i DPsignals.cpp
 *	  Zawarto tutaj funkcje, których celem jest manipulowanie
 *	  postaci¹ wektora zawieraj¹cego sygna³ (generowanie sygna³u,
 *	  przepisywanie do innego wektora itd.)
 *	> DPfiles.h i DPfiles.cpp
 *	  Pliki te zawieraj¹ funkcje do obs³ugi plików tekstowych -
 *	  zapis wektorów danych, odczyt itd.
 *	> DPthreads.h i DPthreads.c++
 *	  Pliki te stanowi¹ najwa¿niejsz¹ czêœæ programu. Zawieraj¹
 *	  funkcje oraz klasê, które pozwalaj¹ na realizacjê
 *	  wspomnianych wczeœniej podstawowych funkcjonalnoœci.
 *	Zgodnie ze standardem cia³a zwyk³ych funkcji zosta³y
 *	umieszczone w plikach Ÿród³owych, natomiast ciala szablonów
 *	klas i funkcji zosta³y w ca³oœci zawarte w plikach
 *	nag³ówkowych.
 *
 *
 *	Do projektu do³¹czych jest tak¿e kilka skryptów programu
 *	Matlab, pozwalaj¹cych na odczyt danych z plików i ich
 *	interpretacjê na wykresach. Znajduj¹ siê one w folderze
 *	Skrypty_Matlab. Ponadto w pliku
 *	Estymacja widma FFT - sprawozdanie.pdf zawarte zosta³y
 *	dodatkowe opisy i wnioski dotycz¹ce projektu i dzia³ania
 *  algorytmu FFT.
 */

#include "stdafx.h"
#include "DPsignals.h"
#include "DPinit.h"
#include "DPthreads.h"

#include <direct.h>		//_chdir
#include <locale.h>		//setlocale

int main()
{
	VectCxDbl	signalDbl;		///< wektor zawieraj¹cy próbki sygna³u wejœciowego
								///< signaDbl jest kluczowym wektorem wykorzystywanym
								///< w ca³ym programie. Jest przekazywany jako sta³y
								///< do funkcji nr 1, 2 i 3 z menu g³ównego, gdzie na 
								///< jego podstawie liczone s¹ widma FFT. W funkcjach 
								///< tych nie jest on modyfikowany. Jego modyfikacja 
								///< jest mo¿liwa jedynie w specjalnie przygotowanej
								///< do tego celu funkcji nr 4 z menu g³ównego.
	VectDbl		signalParams;	///< wektor parametrów do inicjalizacji sygna³u wejœciowego
	char		choiceChar;		///< zmienna znakowa do funkcji switch w menu g³ównym

								//Zmiana folderu roboczego na ..\..\Skrypty_Matlab
	_chdir("..\\..");			// przejœcie dwa katalogi w górê
	_chdir("Skrypty_Matlab");	// przejœcie do folderu

								//	Zmiana ustawieñ regionalnych - pozwala na
								//	wyœwietlanie polskich znaków w konsoli
	setlocale(LC_ALL, "polish");

	// Inicjalizacja domyœlnego sygna³u wejœciowego (sinus, 8192 próbek)
	//	Za³adowanie parametrów sygna³u z pliku. Je¿eli plik 
	//	nie istnieje inicjalizowane s¹ parametry domyœlne
	if (loadParams(signalParams)) {
		signalParams = { 80, 10, 1, -1, 0 };
		saveParams(signalParams);
	}
	generateSignal(signalDbl, 8192, 49, signalParams);

	// Wyœwielanie menu g³ównego
	//	Z menu u¿ytkowanik ma mo¿liwoœæ wyboru podstawowej 
	//	funkcjonalnoœci programu, która ma zostaæ zrealizowana.
	//	Jest ono zrealizowane za pomoc¹ funkcji switch i jest 
	//	wyœwieltane w pêtli do... while do momentu wprowadzenia 
	//	znaku "0", który koñczy dzia³anie programu.
	do {
		system("cls");	// Wyczyszczenie konsoli

		std::cout << "Proszê wybraæ jedn¹ z opcji:" << std::endl;
		std::cout << "> (1) Estymacja FFT dla pojedynczego sygna³u" << std::endl;
		std::cout << "> (2) Porównanie wykonania FFT dla dwóch sygna³ów" << std::endl;
		std::cout << "> (3) Test czasu wykonywania FFT dla serii sygna³ów o ró¿nych typach i d³ugoœciach" << std::endl;
		std::cout << "> (4) Zmiana sygna³u wejœciowego" << std::endl;
		std::cout << "> (0) Wyjœcie z programu" << std::endl;
		std::cin.clear();
		std::cin >> choiceChar;

		//	W zale¿noœci od znaku choiceChar, który zosta³ 
		//	wprowadzony wywo³ywana jest odpowiednia funkcja
		switch (choiceChar)
		{
		case 49:
			//	Znak "1" - estymacja FFT dla pojedynczego sygna³u
			executeFFT(signalDbl);
			break;
		case 50:
			//	Znak "2" - porównanie FFT dla dwóch sygna³ów
			compareFFT(signalDbl);
			break;
		case 51:
			//	Znak "3" - test czasów wykonania FFT dla ró¿nych sygna³ów
			testFFT(signalDbl);
			break;
		case 52:
			//	Znak "4" - przejœcie do menu wyboru sygna³u wejœciowego
			changeSignalMenu(signalDbl);
			break;
		default:
			break;
		}
	} while (choiceChar != 48);

	return 0;
}
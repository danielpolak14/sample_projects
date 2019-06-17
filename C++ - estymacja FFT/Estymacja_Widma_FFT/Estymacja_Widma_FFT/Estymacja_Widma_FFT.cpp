/**
 *  @file    Estymacja_widma_FFT.cpp
 *  @date    30/7/2018
 *	@author  Daniel Polak
 *           Mechatronika st. II, sem. 1
 *	         Wydzia� Mechaniczny
 *	         Politechnika Gda�ska
 *
 *  @brief badanie szybko�ci i dok�adno�ci dzia�ania algorytmu FFT
 *
 *
 *  Nieniejszy program zosta� stworzony w ramach zaliczenia
 *  projektu P21 na przedmiot Programowanie Wsp�bie�ne. Celem
 *	projektu by�o napisanie programu wielow�tkowego,
 *	pozwalaj�cego na wyznaczenie widma za pomoc� klasycznego
 *	algorytmu FFT oraz zbadanie wp�ywu postaci sygna�u
 *	wej�ciowego na szybko�� wykonywania algorytmu. Zgodnie z
 *	za�o�eniami, sygna� wej�ciowy mo�e by� w postaci ci�gu
 *	liczb typu float (pojedynczej precyzji) lub double (podw�jnej
 *  precyzji). D�ugo�� ci�gu oraz to dla jakiego typu sygna�u ma
 *  zosta� wykonana analiza s� zadawane przez u�ytkownika. Program
 *  podzielono na klika plik�w, zawieraj�cych funkcje i klasy
 *  realizuj�ce okre�lone zadania.
 *
 *
 *	W niniejszym pliku zawarty jest podprogram g��wny int main().
 *	Zawiera on domy�ln� inicjalizacje analizownego sygna�em,
 *	a tak�e inicjalizacje zwi�zane z ustawieniem folderu roboczego
 *	i j�zyka konsoli. Wszelkie pliki tekstowe, zawieraj�ce wyniki
 *	dzia�ania programu, b�d� zapisane do folderu zgodnego ze
 *	�ci�k� ..\..\Skrypty_Matlab (Projekt P21 - Daniel Polak\
 *	Skrypty_Matlab, je�eli zosta�a zachowana struktura folder�w).
 *	Zawarta jest tu tak�e p�tla g��wna, kt�ra wy�wietla menu
 *	g��wne, z kt�rego u�ytkowanik ma mo�liwo�� wyboru
 *  funkcjonalno�ci, kt�ra ma zosta� zrealizowana. Program
 *	pozwala na zrealizowanie 3-ech g��wnych funkcjonalno�ci:
 *	> estymacj� widma dla pojedynczego sygna�u, pomiar czasu jego
 *    wyznaczania i zapisanie go do pliku binarnego,
 *	> estymacj� widma dla dw�ch sygna��w, pomiar czasu ich
 *    wyznaczania i zapisanie ich do plik�w, dzi�ki czamu mo�liwe
 *    jest ich p�niejsze por�wnanie,
 *  > wykonanie pomiaru czas�w dla serii sygna��w o r�nych typach
 *    i d�ugo�ciach i wy�wietlenie wynik�w w tabeli.
 *
 *
 *	Pozosta�e pliki nag��wkowe i odpowiadaj�ce pliki �r�d�owe: 
 *	> DPinit.h
 *	  W pliku tym zawartych zosta�o kilka makr i zmiennych
 *	  globalnych, kt�re u�ywane s� w niemal wszystkich pozosta�ych
 *	  plikach.
 *	> DPfft.h
 *	  Plik zawiera funkcje realizuj�c� algorytm fft oraz
 *	  ifft (ifft nie jest jednak wyznaczane w programie).
 *	> DPsignals.h i DPsignals.cpp
 *	  Zawarto tutaj funkcje, kt�rych celem jest manipulowanie
 *	  postaci� wektora zawieraj�cego sygna� (generowanie sygna�u,
 *	  przepisywanie do innego wektora itd.)
 *	> DPfiles.h i DPfiles.cpp
 *	  Pliki te zawieraj� funkcje do obs�ugi plik�w tekstowych -
 *	  zapis wektor�w danych, odczyt itd.
 *	> DPthreads.h i DPthreads.c++
 *	  Pliki te stanowi� najwa�niejsz� cz�� programu. Zawieraj�
 *	  funkcje oraz klas�, kt�re pozwalaj� na realizacj�
 *	  wspomnianych wcze�niej podstawowych funkcjonalno�ci.
 *	Zgodnie ze standardem cia�a zwyk�ych funkcji zosta�y
 *	umieszczone w plikach �r�d�owych, natomiast ciala szablon�w
 *	klas i funkcji zosta�y w ca�o�ci zawarte w plikach
 *	nag��wkowych.
 *
 *
 *	Do projektu do��czych jest tak�e kilka skrypt�w programu
 *	Matlab, pozwalaj�cych na odczyt danych z plik�w i ich
 *	interpretacj� na wykresach. Znajduj� si� one w folderze
 *	Skrypty_Matlab. Ponadto w pliku
 *	Estymacja widma FFT - sprawozdanie.pdf zawarte zosta�y
 *	dodatkowe opisy i wnioski dotycz�ce projektu i dzia�ania
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
	VectCxDbl	signalDbl;		///< wektor zawieraj�cy pr�bki sygna�u wej�ciowego
								///< signaDbl jest kluczowym wektorem wykorzystywanym
								///< w ca�ym programie. Jest przekazywany jako sta�y
								///< do funkcji nr 1, 2 i 3 z menu g��wnego, gdzie na 
								///< jego podstawie liczone s� widma FFT. W funkcjach 
								///< tych nie jest on modyfikowany. Jego modyfikacja 
								///< jest mo�liwa jedynie w specjalnie przygotowanej
								///< do tego celu funkcji nr 4 z menu g��wnego.
	VectDbl		signalParams;	///< wektor parametr�w do inicjalizacji sygna�u wej�ciowego
	char		choiceChar;		///< zmienna znakowa do funkcji switch w menu g��wnym

								//Zmiana folderu roboczego na ..\..\Skrypty_Matlab
	_chdir("..\\..");			// przej�cie dwa katalogi w g�r�
	_chdir("Skrypty_Matlab");	// przej�cie do folderu

								//	Zmiana ustawie� regionalnych - pozwala na
								//	wy�wietlanie polskich znak�w w konsoli
	setlocale(LC_ALL, "polish");

	// Inicjalizacja domy�lnego sygna�u wej�ciowego (sinus, 8192 pr�bek)
	//	Za�adowanie parametr�w sygna�u z pliku. Je�eli plik 
	//	nie istnieje inicjalizowane s� parametry domy�lne
	if (loadParams(signalParams)) {
		signalParams = { 80, 10, 1, -1, 0 };
		saveParams(signalParams);
	}
	generateSignal(signalDbl, 8192, 49, signalParams);

	// Wy�wielanie menu g��wnego
	//	Z menu u�ytkowanik ma mo�liwo�� wyboru podstawowej 
	//	funkcjonalno�ci programu, kt�ra ma zosta� zrealizowana.
	//	Jest ono zrealizowane za pomoc� funkcji switch i jest 
	//	wy�wieltane w p�tli do... while do momentu wprowadzenia 
	//	znaku "0", kt�ry ko�czy dzia�anie programu.
	do {
		system("cls");	// Wyczyszczenie konsoli

		std::cout << "Prosz� wybra� jedn� z opcji:" << std::endl;
		std::cout << "> (1) Estymacja FFT dla pojedynczego sygna�u" << std::endl;
		std::cout << "> (2) Por�wnanie wykonania FFT dla dw�ch sygna��w" << std::endl;
		std::cout << "> (3) Test czasu wykonywania FFT dla serii sygna��w o r�nych typach i d�ugo�ciach" << std::endl;
		std::cout << "> (4) Zmiana sygna�u wej�ciowego" << std::endl;
		std::cout << "> (0) Wyj�cie z programu" << std::endl;
		std::cin.clear();
		std::cin >> choiceChar;

		//	W zale�no�ci od znaku choiceChar, kt�ry zosta� 
		//	wprowadzony wywo�ywana jest odpowiednia funkcja
		switch (choiceChar)
		{
		case 49:
			//	Znak "1" - estymacja FFT dla pojedynczego sygna�u
			executeFFT(signalDbl);
			break;
		case 50:
			//	Znak "2" - por�wnanie FFT dla dw�ch sygna��w
			compareFFT(signalDbl);
			break;
		case 51:
			//	Znak "3" - test czas�w wykonania FFT dla r�nych sygna��w
			testFFT(signalDbl);
			break;
		case 52:
			//	Znak "4" - przej�cie do menu wyboru sygna�u wej�ciowego
			changeSignalMenu(signalDbl);
			break;
		default:
			break;
		}
	} while (choiceChar != 48);

	return 0;
}
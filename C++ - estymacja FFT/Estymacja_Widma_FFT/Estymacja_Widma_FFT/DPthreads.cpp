#include "stdafx.h"
#include "DPinit.h"
#include "DPthreads.h"
#include "DPsignals.h"

#include <vector>
#include <iostream>
#include <thread>
#include <math.h>		//pow
#include <iomanip>      //std::setprecision, std::left, std::right, std::setw, std::setfill

/**
 *  @brief  bada czas i realizuje fft dla pojedycznego sygna�u o
 *          typie i d�ugo�ci zadanych przez u�ytkownika
 *
 *  @param  signalDbl jest wektorem zawieraj�cym liczby zespolone typu
 *          double, dla kt�rego maj� zosta� wykonane badania
 *  @return void
 */
void executeFFT(const VectCxDbl signalDbl) {
	system("cls");	// Wyczyszenie ekranu

	VectCxFlt signalFlt;		///< kopia wektora z sygna�em wej�ciowym zmieniona na typ std::complex<float
	// Przepisanie zawarto�ci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);
	unsigned int numSamples;	///< ilo�� pr�bek, dla kt�rej ma zosta� wykonana analiza fft
	char choiceChar;			///< zmienna znakowa do wyboru typu sygna�u
	double time;				///< uzyskany �redni czas wykonania fft dla okre�lonego sygna�u

	// Pobranie ilo�ci pr�bek, z uwzgl�dnieniem, �e podana
	// liczba musi by� pot�g� liczby 2
	numSamples = getNumSamples(true);
	std::cout << std::endl;

	// Wy�wietlenie menu do wyboru typu sygna�u
	do {
		std::cout << "Prosz� wybra� rodzaj zmiennej sygna�u:" << std::endl;
		std::cout << "> (d) sygna� typu double" << std::endl;
		std::cout << "> (f) sygna� typu float" << std::endl;
		std::cin.clear();
		std::cin >> choiceChar;
	} while (choiceChar != 100 && choiceChar != 102);

	std::cout << std::endl << "Prosz� czeka�, trwa pomiar czasu wykonywania FFT..." << std::endl;

	// Wywo�anie funkcji realizuj�cej fft i mierz�cej czas, dla
	// sygna�u o typie wybranym przez u�ytkownika. Poniewa�
	// executeFFTInThreads() jest szablonem funkcji, w zale�no�ci
	// od tego jaki typ ma pierwszy jej argument dla takiego typu
	// wykonywane s� obliczenia. Wykorzystanie tego szablonu
	// zmniejsza zatem ilo�� niezb�dnego kodu (kod nie musi by�
	// podwojony dla sygna��w typu float i double).
	if (choiceChar == 100)
		time = executeFFTInThreads(signalDbl, numSamples, "sygnal_wejsciowy.txt", "widmo_fft.txt");
	else if (choiceChar == 102)
		time = executeFFTInThreads(signalFlt, numSamples, "sygnal_wejsciowy.txt", "widmo_fft.txt");

	// Wy�wietlenie komunikat�w ko�cowych
	std::cout << "Zako�czono zapisywanie do danych plik�w sygnal_wejsciowy.txt i widmofft.txt." << std::endl;
	std::cout << "�redni czas wykonywania FFT w 100 pr�bach: " << time << " milisekund." << std::endl << std::endl;
	std::cout << "Prosz� wprowadzi� dowolny znak, aby powr�ci� do menu g��wnego." << std::endl;
	char p;
	std::cin >> p;
}

/**
 *  @brief  bada czas i realizuje fft dla dw�ch sygna��w o
 *          typach i d�ugo�ciach zadanych przez u�ytkownika
 *
 *  Dzia�anie tej funkcji jest w�a�ciwie identyczne z dzia�aniem
 *  funkcji executeFFT(), ale analiza fft jest wykonywana dla dw�ch
 *  przypadk�w. Ma ona za zadanie umo�liwi� p�niejsze por�wnanie
 *  wynik�w uzyskiwanych dla r�nych ilo�ci pr�bek i typ�w sygna��w.
 *
 *  @param  signalDbl jest wektorem zawieraj�cym liczby zespolone typu
 *          double, dla kt�rego maj� zosta� wykonane badania
 *  @return void
 */
void compareFFT(const VectCxDbl signalDbl) {
	// Makro zdefiniowane do ewnetualnej �atwej zmiany ilo�ci sygna��w
	// do por�wania.
	#define NUMBER_OF_SIGNALS 2		///< ilo�� sygna��w kt�ra ma zosta� przeanalizowana

	VectCxFlt signalFlt;			///< kopia wektora z sygna�em wej�ciowym zmieniona na typ std::complex<float
	// Przepisanie zawarto�ci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);

	unsigned int numSamples[NUMBER_OF_SIGNALS];		///< tablica ilo�� pr�bek dla kolejnych przypadk�w
	char chooseType[NUMBER_OF_SIGNALS];				///< tablica okre�laj�ca wybrane typy sygna�ow
	char fileName1[40], fileName2[40];				///< tablice z nazwami plik�w, do kt�rych maj� zosta� zapisane dane
	double time[NUMBER_OF_SIGNALS];					///< tablica uzyskanych czas�w dla kolejnych przypadk�w
	
	// Wykonanie analiz fft dla dw�ch (lub wi�cej, je�eli
	// NUMBER_OF_SIGNALS jest wi�ksze) przypadk�w sygna��w.
	for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
		system("cls");
		std::cout << "_____Sygna� nr " << i + 1 << "_____" << std::endl << std::endl;
		
		// Pobranie ilo�ci pr�bek dla przypadku nr i
		numSamples[i] = getNumSamples(true);
		std::cout << std::endl;

		// Wyb�r typu sygna�u dla przypadku nr i
		do {
			std::cout << "Prosz� wybra� rodzaj zmiennej sygna�u nr " << i + 1 <<":"<< std::endl;
			std::cout << "> (d) sygna� typu double" << std::endl;
			std::cout << "> (f) sygna� typu float" << std::endl;
			std::cin.clear();
			std::cin >> chooseType[i];
		} while (chooseType[i] != 100 && chooseType[i] != 102);

		std::cout << std::endl << "Prosz� czeka�, trwa pomiar czasu wykonywania FFT..." << std::endl;

		// Okre�lenie nazw plik�w, kt�re zmieniaj� si� w zale�no�ci
		// od numeru pr�by.
		sprintf_s(fileName1, "porownanie_sygnal_we%i.txt", i+1);
		sprintf_s(fileName2, "porownanie_widmo_fft%i.txt", i+1);

		// Wykonanie analizy fft dla danej pr�by
		if (chooseType[i] == 100)
			time[i] = executeFFTInThreads(signalDbl, numSamples[i], fileName1, fileName2);
		else if (chooseType[i] == 102)
			time[i] = executeFFTInThreads(signalFlt, numSamples[i], fileName1, fileName2);
	}

	// Wy�wietlenie komunikatu ko�cowego oraz czas�w trwania pr�b.
	system("cls");
	std::cout << "Dane zapisano do plik�w: porownanie_sygnal_we1/2.txt i porownanie_widmo_fft1/2.txt w folderze Skrypty Matlab" << std::endl << std::endl;
	for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
		std::cout << "�redni czas wykonywania FFT dla sygnalu nr " << i+1 << " (" << numSamples[i] << " pr�bek, typu ";
		if (chooseType[i] == 100) std::cout << "double) w 100 pr�bach: ";
		if (chooseType[i] == 102) std::cout << "float) w 100 pr�bach: ";
		std::cout << time[i] << " milisekund." << std::endl;
	}
	std::cout << std::endl << "Prosz� wprowadzi� dowolny znak, aby powr�ci� do menu g��wnego." << std::endl;
	char p;
	std::cin >> p;
}

/**
*  @brief  mierzy czas wykonania algorytmu fft dla kolejnych pot�g
*          liczby 2, wy�wietla je w tabeli i zapisuje do pliku
*
*
*  @param  signalDbl jest wektorem zawieraj�cym liczby zespolone typu
*          double, dla kt�rego maj� zosta� wykonane badania
*  @return void
*/
void testFFT(const VectCxDbl signalDbl) {
	// Makra zdefiniowane w celu u�atwienia testowania programu
	// i dostosowania ilo�ci w�tk�w i pot�g do potrzeb.
	#define MIN_POWER 8				///< minimalna pot�ga liczby 2 od kt�rej maj� zacz�� si� pomiary
	//#define MAX_POWER 24
	#define NUMBER_OF_THREADS 1	///< ilo�� pr�b (w�tk�w) jaka ma zosta� wykonana dla sygna�u o danych parametrach
	
	VectCxFlt signalFlt;			///< kopia wektora z sygna�em wej�ciowym zmieniona na typ std::complex<float
	// Przepisanie zawarto�ci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);
	VectDbl times;					///< wektor zawieraj�cy u�rednione czasy pr�b dla wszystkich sygna�ow
	std::vector<std::thread> threads;	///< wektor w�tk�w zwi�zanych z pojedynczymi pr�bami fft
	int i;							///< licznik p�tli
	int MAX_POWER;					///< minimalna pot�ga liczby 2 od kt�rej maj� zacz�� si� pomiary
	bool notFirst = false;			///< zmienna pomocnicza do menu w p�tli do... while

	// Wyb�r maksymalnej pot�gi liczby 2 przez u�ytkownika.
	//  Pot�ga jest ograniczona do 22, poniewa� powy�ej ilo�ci
	//  pr�bek 2^22 pojawiaja si� problemy z alokacj� pami�ci.
	do {
		system("cls");
		if (notFirst) std::cout << "Niepoprawna warto��." << std::endl;
		std::cout << "Program dokona pomiaru czas�w wykonywania algorytmu FFT dla kolejnych pot�g liczby 2.";
		std::cout << std::endl << std::endl << "Prosz� poda� maksymaln� pot�g� liczby 2, dla kt�rej czasy maj� ";
		std::cout << "zosta� uzystkane (minimum 9, maksimum 22)." << std::endl;
		std::cout << "Uwaga: obliczenia dla pot�g >18 znacz�co wyd�u�aj� czas oczekiwania oraz bardzo obci��aj� procesor." << std::endl;
		std::cin >> MAX_POWER;
		notFirst = true;
	} while (MAX_POWER < 9 || MAX_POWER>22);
	std::cout << "Prosz� czeka�, program wyznacza czasy oblicze� FFT..." << std::endl;

	// Rozszerzenie wektora zawieraj�cego czasy do opwiedniego
	// rozmiaru i wyzerowanie warto�ci.
	for (i = 0; i < (MAX_POWER - MIN_POWER + 1) * 2; i++) {
		times.push_back(0);
	}

	// Pomiary czas�w odb�d� si� przy u�yciu klasy TimeFFTClass.
	// W tym celu utowrzone s� dwa wektory obiekt�w tych klas, 
	// jeden z obiektami o typie std::complex<double>, a 
	// drugi o typie std::complex<float>. Z ka�dym obiektem
	// znajduj�cym si� w wektorze zwi�zany b�dzie unikalny sygna�
	// o okre�lonym typie i d�ugo�ci. Wszystkie obiekty tych wektor�w
	// b�d� r�ni�y si� mi�dzy sob� konfiguracj� tych dw�ch
	// parametr�w. Kolejne elementy tych wektor�w, zawieraj� sygna�y
	// o d�ugo�ciach r�wnych kolejnym pot�gom liczby 2. Nast�pnie
	// dla ka�dego z tych obiekt�w utworzonych zostanie
	// NUMBER_OF_THREADS w�tk�w wykonuj�cych pomiar czasu fft.
	std::vector<TimeFFTClass<CxDbl>> objectVectorDbl;	///< wektor obiekt�w klasy TimeFFTClass<std::complex<double
	std::vector<TimeFFTClass<CxFlt>> objectVectorFlt;	///< wektor obiekt�w klasy TimeFFTClass<std::complex<double

	// Wype�nienie wektor�w obiektami z sygna�ami o d�ugo�ciach
	// r�wnych kolejnym pot�gom liczby 2.
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		objectVectorDbl.push_back(TimeFFTClass<CxDbl>(signalDbl, (int)pow(2, i + MIN_POWER)));
		objectVectorFlt.push_back(TimeFFTClass<CxFlt>(signalFlt, (int)pow(2, i + MIN_POWER)));
	}

	// Utworzenie NUMBER_OF_THREADS w�tk�w wywo�uj�cych metod�
	// mierz�c� czas wykonania fft dla sygna�u zwi�zanego z
	// danym obiektem. Zaraz po utworzeniu dla jednego obiektu
	// wywo�ywana jest na nich metoda join(). Program czeka zatem
	// do momentu, a� wszystkie w�tki zostan� zako�czone i tym
	// samym zako�czony b�dzie pomiar czasu wykonywania fft dla
	// jednego sygna�u o danej d�ugo�ci i typie. Nast�pnie p�tla
	// for zwi�ksza iteracj� o jeden i pomiary s� wykonywane dla
	// nast�pnego sygna�u - operacje w w�tkach nigdy nie s�
	// wykonywane jednocze�nie dla r�nych sygna��w. Takie
	// rozwi�zanie jest podyktowane tym, �e podczas testowania
	// programu, dla takiego przypadku uzyskiwano najwi�ksz�
	// powtarzalno�� uzyskiwanych wynik�w. W przypadku, gdy
	// wszystkie w�tki dla wszystkich przypadk�w by�y wykonywane
	// jednocze�nie, powtarzalno�� by�a zmniejszona, co jest
	// najpewniej spowodowane nier�wnomiernym obci��eniem procesora.
	// Powtarzalno�� mo�na tak�e nieco zwi�kszy�, zwi�kszaj�c
	// ilo�� w�tk�w, jednak powoduje to znaczne wyd�u�enie czasu
	// trwania pomiar�w sygna��w o d�ugo�ciach r�wnych wy�szym
	// pot�gom liczby 2, dlatego ilo�� w�tk�w, w normalnym trybie
	// dzia�ania z u�ytkownikiem ograniczono tylko do 10.
	// Wykonanie pomiar�w w w�tkach dla obiekt�w z sygna�ami typu double.
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		for (int j = 0; j < NUMBER_OF_THREADS; j++) {
			threads.push_back(std::thread([&objectVectorDbl, i, &times]() {
				objectVectorDbl[i].measureTime();
			}));
		}

		for (auto& thread : threads) {
			thread.join();
		}
		threads.clear();
	}

	// Wykonanie pomiar�w w w�tkach dla obiekt�w z sygna�ami typu float.
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		for (int j = 0; j < NUMBER_OF_THREADS; j++) {
			threads.push_back(std::thread([&objectVectorFlt, i, &times]() {
				objectVectorFlt[i].measureTime();
			}));
		}

		for (auto& thread : threads) {
			thread.join();
		}
		threads.clear();
	}

	// Pobranie �rednich czas�w trwania fft z obiekt�w. 
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		times[i] = objectVectorDbl[i].returnTime();
		times[i + MAX_POWER - MIN_POWER + 1] = objectVectorFlt[i].returnTime();
	}


	// Wy�wietlenie komunikat�w ko�cowych i tabeli.
	system("cls");
	std::cout << "Czasy oblicze� dla procesora: " << std::endl << std::endl;
	system("echo %PROCESSOR_ARCHITECTURE% %PROCESSOR_IDENTIFIER% %PROCESSOR_LEVEL% %PROCESSOR_REVISION%");
	system("wmic cpu get name, CurrentClockSpeed, MaxClockSpeed, NumberOfCores, NumberOfLogicalProcessors");
	
	std::cout << "Czasy wykonywania algorytmu fft u�redniode dla serii " << NUMBER_OF_THREADS << " pr�b." << std::endl << std::endl;
	std::cout << "|" << std::setw(15) << std::left << " Ilo��  | " << "Czas (w milisekundach)" << std::setw(7) << std::right << "|" << std::endl;
	std::cout << "|" << std::setw(7) << " pr�bek |" << std::setw(12) << "double" << std::setw(6) << "|" << std::setw(10) << "float" << std::setw(7) << "|" << std::endl;
	std::cout << "|" << std::setfill('-') << std::setw(9) << std::right << "|" << std::setw(18) << "|" << std::setw(17) << "|" << "\n";
	std::cout << std::setprecision(4) << std::fixed << std::setfill(' ');
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		std::cout << "|  2^" << i + MIN_POWER;
		if(i+MIN_POWER<10)	std::cout << std::setw(4) << "|";
		else				std::cout << std::setw(3) << "|";
		std::cout << std::setw(16) << std::right << times[i] << " |";
		std::cout << std::setw(15) << times[i + MAX_POWER - MIN_POWER + 1] << " |" << std::endl;
	}

	// Zapis wektora do plik�w (plik zawiera najpierw wyniki
	// pomiar�w dla sygna��w typu double, a nast�pnie dla 
	// sygna��w typu float).
	saveVectorToFile(times, "czasy.txt");
	std::cout << std::endl << "Wyznaczone czasy zosta�y zapisane do pliku czasy.txt." << std::endl << std::endl;
	std::cout << "Prosz� wprowadzi� dowolny znak, aby powr�ci� do menu g��wnego.";
	char p;
	std::cin >> p;
}
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
 *  @brief  bada czas i realizuje fft dla pojedycznego sygna³u o
 *          typie i d³ugoœci zadanych przez u¿ytkownika
 *
 *  @param  signalDbl jest wektorem zawieraj¹cym liczby zespolone typu
 *          double, dla którego maj¹ zostaæ wykonane badania
 *  @return void
 */
void executeFFT(const VectCxDbl signalDbl) {
	system("cls");	// Wyczyszenie ekranu

	VectCxFlt signalFlt;		///< kopia wektora z sygna³em wejœciowym zmieniona na typ std::complex<float
	// Przepisanie zawartoœci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);
	unsigned int numSamples;	///< iloœæ próbek, dla której ma zostaæ wykonana analiza fft
	char choiceChar;			///< zmienna znakowa do wyboru typu sygna³u
	double time;				///< uzyskany œredni czas wykonania fft dla okreœlonego sygna³u

	// Pobranie iloœci próbek, z uwzglêdnieniem, ¿e podana
	// liczba musi byæ potêg¹ liczby 2
	numSamples = getNumSamples(true);
	std::cout << std::endl;

	// Wyœwietlenie menu do wyboru typu sygna³u
	do {
		std::cout << "Proszê wybraæ rodzaj zmiennej sygna³u:" << std::endl;
		std::cout << "> (d) sygna³ typu double" << std::endl;
		std::cout << "> (f) sygna³ typu float" << std::endl;
		std::cin.clear();
		std::cin >> choiceChar;
	} while (choiceChar != 100 && choiceChar != 102);

	std::cout << std::endl << "Proszê czekaæ, trwa pomiar czasu wykonywania FFT..." << std::endl;

	// Wywo³anie funkcji realizuj¹cej fft i mierz¹cej czas, dla
	// sygna³u o typie wybranym przez u¿ytkownika. Poniewa¿
	// executeFFTInThreads() jest szablonem funkcji, w zale¿noœci
	// od tego jaki typ ma pierwszy jej argument dla takiego typu
	// wykonywane s¹ obliczenia. Wykorzystanie tego szablonu
	// zmniejsza zatem iloœæ niezbêdnego kodu (kod nie musi byæ
	// podwojony dla sygna³ów typu float i double).
	if (choiceChar == 100)
		time = executeFFTInThreads(signalDbl, numSamples, "sygnal_wejsciowy.txt", "widmo_fft.txt");
	else if (choiceChar == 102)
		time = executeFFTInThreads(signalFlt, numSamples, "sygnal_wejsciowy.txt", "widmo_fft.txt");

	// Wyœwietlenie komunikatów koñcowych
	std::cout << "Zakoñczono zapisywanie do danych plików sygnal_wejsciowy.txt i widmofft.txt." << std::endl;
	std::cout << "Œredni czas wykonywania FFT w 100 próbach: " << time << " milisekund." << std::endl << std::endl;
	std::cout << "Proszê wprowadziæ dowolny znak, aby powróciæ do menu g³ównego." << std::endl;
	char p;
	std::cin >> p;
}

/**
 *  @brief  bada czas i realizuje fft dla dwóch sygna³ów o
 *          typach i d³ugoœciach zadanych przez u¿ytkownika
 *
 *  Dzia³anie tej funkcji jest w³aœciwie identyczne z dzia³aniem
 *  funkcji executeFFT(), ale analiza fft jest wykonywana dla dwóch
 *  przypadków. Ma ona za zadanie umo¿liwiæ póŸniejsze porównanie
 *  wyników uzyskiwanych dla ró¿nych iloœci próbek i typów sygna³ów.
 *
 *  @param  signalDbl jest wektorem zawieraj¹cym liczby zespolone typu
 *          double, dla którego maj¹ zostaæ wykonane badania
 *  @return void
 */
void compareFFT(const VectCxDbl signalDbl) {
	// Makro zdefiniowane do ewnetualnej ³atwej zmiany iloœci sygna³ów
	// do porówania.
	#define NUMBER_OF_SIGNALS 2		///< iloœæ sygna³ów która ma zostaæ przeanalizowana

	VectCxFlt signalFlt;			///< kopia wektora z sygna³em wejœciowym zmieniona na typ std::complex<float
	// Przepisanie zawartoœci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);

	unsigned int numSamples[NUMBER_OF_SIGNALS];		///< tablica iloœæ próbek dla kolejnych przypadków
	char chooseType[NUMBER_OF_SIGNALS];				///< tablica okreœlaj¹ca wybrane typy sygna³ow
	char fileName1[40], fileName2[40];				///< tablice z nazwami plików, do których maj¹ zostaæ zapisane dane
	double time[NUMBER_OF_SIGNALS];					///< tablica uzyskanych czasów dla kolejnych przypadków
	
	// Wykonanie analiz fft dla dwóch (lub wiêcej, je¿eli
	// NUMBER_OF_SIGNALS jest wiêksze) przypadków sygna³ów.
	for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
		system("cls");
		std::cout << "_____Sygna³ nr " << i + 1 << "_____" << std::endl << std::endl;
		
		// Pobranie iloœci próbek dla przypadku nr i
		numSamples[i] = getNumSamples(true);
		std::cout << std::endl;

		// Wybór typu sygna³u dla przypadku nr i
		do {
			std::cout << "Proszê wybraæ rodzaj zmiennej sygna³u nr " << i + 1 <<":"<< std::endl;
			std::cout << "> (d) sygna³ typu double" << std::endl;
			std::cout << "> (f) sygna³ typu float" << std::endl;
			std::cin.clear();
			std::cin >> chooseType[i];
		} while (chooseType[i] != 100 && chooseType[i] != 102);

		std::cout << std::endl << "Proszê czekaæ, trwa pomiar czasu wykonywania FFT..." << std::endl;

		// Okreœlenie nazw plików, które zmieniaj¹ siê w zale¿noœci
		// od numeru próby.
		sprintf_s(fileName1, "porownanie_sygnal_we%i.txt", i+1);
		sprintf_s(fileName2, "porownanie_widmo_fft%i.txt", i+1);

		// Wykonanie analizy fft dla danej próby
		if (chooseType[i] == 100)
			time[i] = executeFFTInThreads(signalDbl, numSamples[i], fileName1, fileName2);
		else if (chooseType[i] == 102)
			time[i] = executeFFTInThreads(signalFlt, numSamples[i], fileName1, fileName2);
	}

	// Wyœwietlenie komunikatu koñcowego oraz czasów trwania prób.
	system("cls");
	std::cout << "Dane zapisano do plików: porownanie_sygnal_we1/2.txt i porownanie_widmo_fft1/2.txt w folderze Skrypty Matlab" << std::endl << std::endl;
	for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
		std::cout << "Œredni czas wykonywania FFT dla sygnalu nr " << i+1 << " (" << numSamples[i] << " próbek, typu ";
		if (chooseType[i] == 100) std::cout << "double) w 100 próbach: ";
		if (chooseType[i] == 102) std::cout << "float) w 100 próbach: ";
		std::cout << time[i] << " milisekund." << std::endl;
	}
	std::cout << std::endl << "Proszê wprowadziæ dowolny znak, aby powróciæ do menu g³ównego." << std::endl;
	char p;
	std::cin >> p;
}

/**
*  @brief  mierzy czas wykonania algorytmu fft dla kolejnych potêg
*          liczby 2, wyœwietla je w tabeli i zapisuje do pliku
*
*
*  @param  signalDbl jest wektorem zawieraj¹cym liczby zespolone typu
*          double, dla którego maj¹ zostaæ wykonane badania
*  @return void
*/
void testFFT(const VectCxDbl signalDbl) {
	// Makra zdefiniowane w celu u³atwienia testowania programu
	// i dostosowania iloœci w¹tków i potêg do potrzeb.
	#define MIN_POWER 8				///< minimalna potêga liczby 2 od której maj¹ zacz¹æ siê pomiary
	//#define MAX_POWER 24
	#define NUMBER_OF_THREADS 1	///< iloœæ prób (w¹tków) jaka ma zostaæ wykonana dla sygna³u o danych parametrach
	
	VectCxFlt signalFlt;			///< kopia wektora z sygna³em wejœciowym zmieniona na typ std::complex<float
	// Przepisanie zawartoœci wektora signalDbl do wektora singalFlt
	//  (identyczna kopia, ale zawiera liczby typu std::complex<float>)
	vectorDoubleToFloat(signalDbl, signalFlt);
	VectDbl times;					///< wektor zawieraj¹cy uœrednione czasy prób dla wszystkich sygna³ow
	std::vector<std::thread> threads;	///< wektor w¹tków zwi¹zanych z pojedynczymi próbami fft
	int i;							///< licznik pêtli
	int MAX_POWER;					///< minimalna potêga liczby 2 od której maj¹ zacz¹æ siê pomiary
	bool notFirst = false;			///< zmienna pomocnicza do menu w pêtli do... while

	// Wybór maksymalnej potêgi liczby 2 przez u¿ytkownika.
	//  Potêga jest ograniczona do 22, poniewa¿ powy¿ej iloœci
	//  próbek 2^22 pojawiaja siê problemy z alokacj¹ pamiêci.
	do {
		system("cls");
		if (notFirst) std::cout << "Niepoprawna wartoœæ." << std::endl;
		std::cout << "Program dokona pomiaru czasów wykonywania algorytmu FFT dla kolejnych potêg liczby 2.";
		std::cout << std::endl << std::endl << "Proszê podaæ maksymaln¹ potêgê liczby 2, dla której czasy maj¹ ";
		std::cout << "zostaæ uzystkane (minimum 9, maksimum 22)." << std::endl;
		std::cout << "Uwaga: obliczenia dla potêg >18 znacz¹co wyd³u¿aj¹ czas oczekiwania oraz bardzo obci¹¿aj¹ procesor." << std::endl;
		std::cin >> MAX_POWER;
		notFirst = true;
	} while (MAX_POWER < 9 || MAX_POWER>22);
	std::cout << "Proszê czekaæ, program wyznacza czasy obliczeñ FFT..." << std::endl;

	// Rozszerzenie wektora zawieraj¹cego czasy do opwiedniego
	// rozmiaru i wyzerowanie wartoœci.
	for (i = 0; i < (MAX_POWER - MIN_POWER + 1) * 2; i++) {
		times.push_back(0);
	}

	// Pomiary czasów odbêd¹ siê przy u¿yciu klasy TimeFFTClass.
	// W tym celu utowrzone s¹ dwa wektory obiektów tych klas, 
	// jeden z obiektami o typie std::complex<double>, a 
	// drugi o typie std::complex<float>. Z ka¿dym obiektem
	// znajduj¹cym siê w wektorze zwi¹zany bêdzie unikalny sygna³
	// o okreœlonym typie i d³ugoœci. Wszystkie obiekty tych wektorów
	// bêd¹ ró¿ni³y siê miêdzy sob¹ konfiguracj¹ tych dwóch
	// parametrów. Kolejne elementy tych wektorów, zawieraj¹ sygna³y
	// o d³ugoœciach równych kolejnym potêgom liczby 2. Nastêpnie
	// dla ka¿dego z tych obiektów utworzonych zostanie
	// NUMBER_OF_THREADS w¹tków wykonuj¹cych pomiar czasu fft.
	std::vector<TimeFFTClass<CxDbl>> objectVectorDbl;	///< wektor obiektów klasy TimeFFTClass<std::complex<double
	std::vector<TimeFFTClass<CxFlt>> objectVectorFlt;	///< wektor obiektów klasy TimeFFTClass<std::complex<double

	// Wype³nienie wektorów obiektami z sygna³ami o d³ugoœciach
	// równych kolejnym potêgom liczby 2.
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		objectVectorDbl.push_back(TimeFFTClass<CxDbl>(signalDbl, (int)pow(2, i + MIN_POWER)));
		objectVectorFlt.push_back(TimeFFTClass<CxFlt>(signalFlt, (int)pow(2, i + MIN_POWER)));
	}

	// Utworzenie NUMBER_OF_THREADS w¹tków wywo³uj¹cych metodê
	// mierz¹c¹ czas wykonania fft dla sygna³u zwi¹zanego z
	// danym obiektem. Zaraz po utworzeniu dla jednego obiektu
	// wywo³ywana jest na nich metoda join(). Program czeka zatem
	// do momentu, a¿ wszystkie w¹tki zostan¹ zakoñczone i tym
	// samym zakoñczony bêdzie pomiar czasu wykonywania fft dla
	// jednego sygna³u o danej d³ugoœci i typie. Nastêpnie pêtla
	// for zwiêksza iteracjê o jeden i pomiary s¹ wykonywane dla
	// nastêpnego sygna³u - operacje w w¹tkach nigdy nie s¹
	// wykonywane jednoczeœnie dla ró¿nych sygna³ów. Takie
	// rozwi¹zanie jest podyktowane tym, ¿e podczas testowania
	// programu, dla takiego przypadku uzyskiwano najwiêksz¹
	// powtarzalnoœæ uzyskiwanych wyników. W przypadku, gdy
	// wszystkie w¹tki dla wszystkich przypadków by³y wykonywane
	// jednoczeœnie, powtarzalnoœæ by³a zmniejszona, co jest
	// najpewniej spowodowane nierównomiernym obci¹¿eniem procesora.
	// Powtarzalnoœæ mo¿na tak¿e nieco zwiêkszyæ, zwiêkszaj¹c
	// iloœæ w¹tków, jednak powoduje to znaczne wyd³u¿enie czasu
	// trwania pomiarów sygna³ów o d³ugoœciach równych wy¿szym
	// potêgom liczby 2, dlatego iloœæ w¹tków, w normalnym trybie
	// dzia³ania z u¿ytkownikiem ograniczono tylko do 10.
	// Wykonanie pomiarów w w¹tkach dla obiektów z sygna³ami typu double.
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

	// Wykonanie pomiarów w w¹tkach dla obiektów z sygna³ami typu float.
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

	// Pobranie œrednich czasów trwania fft z obiektów. 
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		times[i] = objectVectorDbl[i].returnTime();
		times[i + MAX_POWER - MIN_POWER + 1] = objectVectorFlt[i].returnTime();
	}


	// Wyœwietlenie komunikatów koñcowych i tabeli.
	system("cls");
	std::cout << "Czasy obliczeñ dla procesora: " << std::endl << std::endl;
	system("echo %PROCESSOR_ARCHITECTURE% %PROCESSOR_IDENTIFIER% %PROCESSOR_LEVEL% %PROCESSOR_REVISION%");
	system("wmic cpu get name, CurrentClockSpeed, MaxClockSpeed, NumberOfCores, NumberOfLogicalProcessors");
	
	std::cout << "Czasy wykonywania algorytmu fft uœredniode dla serii " << NUMBER_OF_THREADS << " prób." << std::endl << std::endl;
	std::cout << "|" << std::setw(15) << std::left << " Iloœæ  | " << "Czas (w milisekundach)" << std::setw(7) << std::right << "|" << std::endl;
	std::cout << "|" << std::setw(7) << " próbek |" << std::setw(12) << "double" << std::setw(6) << "|" << std::setw(10) << "float" << std::setw(7) << "|" << std::endl;
	std::cout << "|" << std::setfill('-') << std::setw(9) << std::right << "|" << std::setw(18) << "|" << std::setw(17) << "|" << "\n";
	std::cout << std::setprecision(4) << std::fixed << std::setfill(' ');
	for (i = 0; i < MAX_POWER - MIN_POWER + 1; i++) {
		std::cout << "|  2^" << i + MIN_POWER;
		if(i+MIN_POWER<10)	std::cout << std::setw(4) << "|";
		else				std::cout << std::setw(3) << "|";
		std::cout << std::setw(16) << std::right << times[i] << " |";
		std::cout << std::setw(15) << times[i + MAX_POWER - MIN_POWER + 1] << " |" << std::endl;
	}

	// Zapis wektora do plików (plik zawiera najpierw wyniki
	// pomiarów dla sygna³ów typu double, a nastêpnie dla 
	// sygna³ów typu float).
	saveVectorToFile(times, "czasy.txt");
	std::cout << std::endl << "Wyznaczone czasy zosta³y zapisane do pliku czasy.txt." << std::endl << std::endl;
	std::cout << "Proszê wprowadziæ dowolny znak, aby powróciæ do menu g³ównego.";
	char p;
	std::cin >> p;
}
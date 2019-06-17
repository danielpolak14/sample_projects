/**
 *  @file    DPthreads.h
 *  @author	 Daniel Polak
 *
 *  @brief   klasa i funkcje pozwalaj¹ce na wykonanie wielu prób fft w w¹tkach
 * 
 *  Niniejszy plik stanowi g³ówn¹ czêœæ programu pozwalaj¹c¹ na
 *  realizacjê za³o¿eñ projektu. Wykorzystywane s¹ tu funkcje
 *  w³asne z pozosta³ych plików. Funkcje executeFFT(),
 *  compareFFT() i testFFT() realizuj¹ 3 g³ówne zadania i odrêbne
 *  funkcjonalnoœci. S¹ one wywo³ywane z menu g³ównego stworzonego
 *  w podprogramie g³ównym w zale¿noœci od wyboru u¿ytkownika.
 *  Wyniki ich dzia³ania zapisywane s¹ do plików tekstowych do
 *  folderu /Skrypty_Matlab. 
 */
#pragma once
#include "stdafx.h"
#include "DPinit.h"
#include "DPfft.h"
#include "DPsignals.h"
#include "DPfiles.h"

#include <mutex>
#include <chrono>
#include <vector>
#include <type_traits>	//std::is_same()
#include <algorithm>	//std::copy()
#include <iterator>		//std::back_insterter()

 /**
  *  @brief  pomocniczy szablon funkcji do wykonania fft na sygnale
  *          o zadanym typie w w¹tkach
  *
  *  Funkcja pozwala na dokonanie pomiaru czasu wykonywania algorytmu
  *  fft dla serii 100 prób oraz zapisanie sygna³u wejœciowego i 
  *  uzyskanego widma do plików o zadanych nazwach. Ka¿da próba jest
  *  wykonywana w oddzielnym w¹tku. Sygna³em wejœciowym w funkcji jest
  *  kopia wektora liczb o okreœlonym typie, który jest podawany do niej
  *  jako argument. Elementy funkcji dzia³aj¹ na kopii tego wektora.
  *  W pocz¹tkowym etapie jej wykonywania kopia ta jest modyfikowana pod
  *  wzglêdem iloœci próbek przy wywo³aniu konstruktora obiektu klasy
  *  TimeFFTClass (próbki s¹ obcinane, a je¿eli d³ugoœæ wektora przekracza
  *  iloœæ próbek zadanych, dla której fft ma zostaæ wykonane, na koñcu
  *  sygna³u wejœciowego dopisywane s¹ zera). Tym sposobem mo¿liwe jest
  *  wykonanie testów dla sygna³u o okreœlonej iloœci próbek. Szablon
  *  funkcji zosta³ stworzony pomocniczo, w celu zmniejszenia iloœci kodu
  *  w funkcjach executeFFT() i compareFFT() oraz przede wszystkim
  *  umo¿liwienia wyboru typu sygna³u (float, double) przez u¿ytkownika
  *  (co w jêzyku C++ o statycznej kontroli typów jest utrudnione).
  *
  *  @tparam Type okreœla typ liczb w wektorze, dla jakich ma zostaæ
  *          wykonana analiza fft
  *  @param  signal wektor zawieraj¹cy próbki sygna³u
  *  @param  numSamples iloœæ próbek, jak¹ ma mieæ docelowy sygna³
  *          wejœciowy na którym za zostaæ wykonany alogrytm fft
  *  @param  firstFileName tablica z nazw¹ pliku, do którego ma
  *          zostaæ zapisany sygna³ wejœciowy
  *  @param  secondFileName tablica z nazw¹ pliku, do którego ma
  *          zostaæ zapisane widmo fft 
  *  @return œredni czas wykonywania fft ze wszystkich 100 prób
  */
template<typename Type>
double executeFFTInThreads(const std::vector<Type> signal, unsigned int numSamples, const char* firstFileName, const char* secondFileName) {
	TimeFFTClass<Type> timeFFTObject(signal, numSamples);	///< objekt do pomiaru czasu i wykonania fft
	std::vector<std::thread> timeThreads;					///< wektor w¹tków wykonuj¹cych próby czasu fft

	// Utworzenie za pomoc¹ funkcji lambda 100 w¹tków, które
	// dokonuj¹ pomiaru czasu wykonywania fft na sygnale 
	// zwi¹zanym z obiektem timeFFTObject oraz wywo³anie
	// metody join() na ka¿dym w¹tku.
	for (int i = 0; i < 100; i++) {
		timeThreads.push_back(std::thread([&timeFFTObject]() {
			timeFFTObject.measureTime();
		}));
	}
	for (auto& thread : timeThreads) {
		thread.join();
	}

	// Po wykonaniu pomiarów czasu, w podobny sposób tworzony
	// jest kolejny w¹tek, który zapisuje sygna³ wejœciowy
	// z obiektu TimeFFTObject do pliku, a nastepnie modyfikuje
	// zwi¹zany z nim wektor tak, aby zawiera³ on widmo tego
	// sygna³u po czy raz jeszcze dokonuje zapisu do pliku.
	// Do funkcji lambda przekazywane s¹ przez referencje
	// obiekt oraz nazwy plików.
	std::thread fileThread([&timeFFTObject,&firstFileName,&secondFileName]() {
		timeFFTObject.saveToFile(firstFileName);
		timeFFTObject.obtainFFT();
		timeFFTObject.saveToFile(secondFileName);
	});

	// Program wywo³uje metodê join() i czeka a¿ pliki zostan¹
	// zapisane.
	fileThread.join();

	return timeFFTObject.returnTime();
}

/**
 *  @brief  klasa dostosowana do wyznaczenia widma i pomiaru czasu
 *          wykonania algorytmu fft w w¹tkach
 *
 *  Klasa zawiera metodê, która pozwala na pomiar czasu wykonywania
 *  algorytmu fft dla serii kilku pomiarów. Zosta³a ona dostosowana
 *  do tego, aby mo¿liwy by³ pomiar dla serii wielu prób w sposób
 *  wielow¹tkowy. Dany obiekt klasy jest zwi¹zany z jednym sygna³em
 *  o œciœle okreœlonej d³ugoœci i typie. Parametry te s¹ okreœlane
 *  przy utworzeniu obiektu jako parametr szablonu i argument
 *  konstruktora i nie zmieniaj¹ siê do koñca ¿ycia obiektu. £¹czny
 *  W klasie zawarto tak¿e kilka dodatkowych metdod, omówionych 
 *  poni¿ej.
 * 
 *  @tparam Type okreœla typ zmiennych, które zawiera wektor
 */
template<class Type>
class TimeFFTClass
{
	std::mutex mutex;			///< mutex sekcji krytycznej zwi¹zanej z pomiarem czasu
	double time;				///< pole przechowuj¹ce ³¹czny czas wykonywania fft dla wszystkich prób
	int threadCounter;			///< licznik iloœci pomiarów - liczba wywo³añ metody measureTime()
	std::vector<Type> signal;	///< sygna³ dla którego wykonywane s¹ pomiary

public:
	/**
	 *  @brief  konstruktor
	 *  
	 *  @param  initialSignal wektor, zawieraj¹cy synga³ na podstawie którego
	 *          tworzony jest sygna³ zwi¹zany z obiektem
	 *  @param  numSamples stanowi iloœæ próbek, jak¹ ma mieæ docelowy sygna³
	 *          z wi¹zany z obiektem i dla której maj¹ zostaæ wykonane pomiary
	 */
	TimeFFTClass(std::vector<Type> initialSignal, unsigned int numSamples) {
		time = 0;
		threadCounter = 0;
	
		if (numSamples > initialSignal.size()) {
			std::copy(initialSignal.begin(), initialSignal.begin() + initialSignal.size(), std::back_inserter(signal));
			extendVectorWithZeros(signal, numSamples);
		}
		else
			std::copy(initialSignal.begin(), initialSignal.begin() + numSamples, std::back_inserter(signal));
		
	}

	/**
	 *  @brief  konstruktor kopiuj¹cy
	 *
	 *  Konieczny do utworzenia wektora obiektów klasy TimeFFTClass
	 */
	TimeFFTClass(const TimeFFTClass &object) {
		time = object.time;
		threadCounter = object.threadCounter;
		signal = object.signal;
	}

	/**
	 *  @brief  wyœwietla aktualn¹ postaæ sygna³u w konsoli
	 */
	void print() {
		printVector(this->signal);
	}

	/**
	 *  @brief  mierzy czas wykonania fft i powiêksza o tê wartoœæ pole time
     */
	void measureTime() {
		// Pomiar czasu z wykorzystaniem biblioteki <chrono>
		// Pobranie aktualnej wartoœci zegara - wartoœæ pocz¹tkowa
		auto start = std::chrono::steady_clock::now();
		// Wykonanie algorytmu fft. Funkcja fftTime nie zmienia postaci
		// sygna³u wejœciowego!
		fftTime(this->signal);
		// Pobranie aktualnej wartoœci zegara - wartoœæ po wykonaniu fft
		auto end = std::chrono::steady_clock::now();

		// Obliczenie czasu
		std::chrono::duration<double, std::milli> duration = end - start;

		// Zwiêkszenie licznika iloœci prób oraz powiêkszenie pola time o 
		// czas trwania aktualnej próby. Poniewa¿ w programie metoda 
		// measureTime jest wykonywana nawet dla 100 w¹tków i ich przewidywany
		// czas wykonywania powinien byæ zbli¿ony, aby ¿aden pomiar nie zosta³
		// pominiêty pola te zwiêkszane s¹ dopiero po wejœciu do sekcji
		// krytycznej zwi¹zanej z mutexem "mutex"
		mutex.lock();
		threadCounter++;
		this->time += duration.count();
		mutex.unlock();
	}

	/**
	 *  @brief  zwraca œredni czas wykonywania algorytmu fft
	 */
	double returnTime() {
		return time/static_cast<double>(threadCounter);
	}

	/**
	 *  @brief  zapisuje aktualn¹ postaæ pola signal do pliku
	 *
	 *  @param  fileName stanowi tablicê char z nazw¹ pliku, do którego
	 *          dane maj¹ zostaæ zapisane
	 */
	void saveToFile(const char* fileName) {
		saveVectorToFile(this->signal, fileName);
	}

	/**
	 *  @brief  zamienia sygna³ zwi¹zany z obiektem na widmo fft tego sygna³u
	 */
	void obtainFFT() {
		fft(this->signal);
	}
};

void executeFFT(const VectCxDbl signal);
void compareFFT(const VectCxDbl signal);
void testFFT(const VectCxDbl signal);
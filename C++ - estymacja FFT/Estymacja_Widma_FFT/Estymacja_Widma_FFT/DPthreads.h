/**
 *  @file    DPthreads.h
 *  @author	 Daniel Polak
 *
 *  @brief   klasa i funkcje pozwalaj�ce na wykonanie wielu pr�b fft w w�tkach
 * 
 *  Niniejszy plik stanowi g��wn� cz�� programu pozwalaj�c� na
 *  realizacj� za�o�e� projektu. Wykorzystywane s� tu funkcje
 *  w�asne z pozosta�ych plik�w. Funkcje executeFFT(),
 *  compareFFT() i testFFT() realizuj� 3 g��wne zadania i odr�bne
 *  funkcjonalno�ci. S� one wywo�ywane z menu g��wnego stworzonego
 *  w podprogramie g��wnym w zale�no�ci od wyboru u�ytkownika.
 *  Wyniki ich dzia�ania zapisywane s� do plik�w tekstowych do
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
  *          o zadanym typie w w�tkach
  *
  *  Funkcja pozwala na dokonanie pomiaru czasu wykonywania algorytmu
  *  fft dla serii 100 pr�b oraz zapisanie sygna�u wej�ciowego i 
  *  uzyskanego widma do plik�w o zadanych nazwach. Ka�da pr�ba jest
  *  wykonywana w oddzielnym w�tku. Sygna�em wej�ciowym w funkcji jest
  *  kopia wektora liczb o okre�lonym typie, kt�ry jest podawany do niej
  *  jako argument. Elementy funkcji dzia�aj� na kopii tego wektora.
  *  W pocz�tkowym etapie jej wykonywania kopia ta jest modyfikowana pod
  *  wzgl�dem ilo�ci pr�bek przy wywo�aniu konstruktora obiektu klasy
  *  TimeFFTClass (pr�bki s� obcinane, a je�eli d�ugo�� wektora przekracza
  *  ilo�� pr�bek zadanych, dla kt�rej fft ma zosta� wykonane, na ko�cu
  *  sygna�u wej�ciowego dopisywane s� zera). Tym sposobem mo�liwe jest
  *  wykonanie test�w dla sygna�u o okre�lonej ilo�ci pr�bek. Szablon
  *  funkcji zosta� stworzony pomocniczo, w celu zmniejszenia ilo�ci kodu
  *  w funkcjach executeFFT() i compareFFT() oraz przede wszystkim
  *  umo�liwienia wyboru typu sygna�u (float, double) przez u�ytkownika
  *  (co w j�zyku C++ o statycznej kontroli typ�w jest utrudnione).
  *
  *  @tparam Type okre�la typ liczb w wektorze, dla jakich ma zosta�
  *          wykonana analiza fft
  *  @param  signal wektor zawieraj�cy pr�bki sygna�u
  *  @param  numSamples ilo�� pr�bek, jak� ma mie� docelowy sygna�
  *          wej�ciowy na kt�rym za zosta� wykonany alogrytm fft
  *  @param  firstFileName tablica z nazw� pliku, do kt�rego ma
  *          zosta� zapisany sygna� wej�ciowy
  *  @param  secondFileName tablica z nazw� pliku, do kt�rego ma
  *          zosta� zapisane widmo fft 
  *  @return �redni czas wykonywania fft ze wszystkich 100 pr�b
  */
template<typename Type>
double executeFFTInThreads(const std::vector<Type> signal, unsigned int numSamples, const char* firstFileName, const char* secondFileName) {
	TimeFFTClass<Type> timeFFTObject(signal, numSamples);	///< objekt do pomiaru czasu i wykonania fft
	std::vector<std::thread> timeThreads;					///< wektor w�tk�w wykonuj�cych pr�by czasu fft

	// Utworzenie za pomoc� funkcji lambda 100 w�tk�w, kt�re
	// dokonuj� pomiaru czasu wykonywania fft na sygnale 
	// zwi�zanym z obiektem timeFFTObject oraz wywo�anie
	// metody join() na ka�dym w�tku.
	for (int i = 0; i < 100; i++) {
		timeThreads.push_back(std::thread([&timeFFTObject]() {
			timeFFTObject.measureTime();
		}));
	}
	for (auto& thread : timeThreads) {
		thread.join();
	}

	// Po wykonaniu pomiar�w czasu, w podobny spos�b tworzony
	// jest kolejny w�tek, kt�ry zapisuje sygna� wej�ciowy
	// z obiektu TimeFFTObject do pliku, a nastepnie modyfikuje
	// zwi�zany z nim wektor tak, aby zawiera� on widmo tego
	// sygna�u po czy raz jeszcze dokonuje zapisu do pliku.
	// Do funkcji lambda przekazywane s� przez referencje
	// obiekt oraz nazwy plik�w.
	std::thread fileThread([&timeFFTObject,&firstFileName,&secondFileName]() {
		timeFFTObject.saveToFile(firstFileName);
		timeFFTObject.obtainFFT();
		timeFFTObject.saveToFile(secondFileName);
	});

	// Program wywo�uje metod� join() i czeka a� pliki zostan�
	// zapisane.
	fileThread.join();

	return timeFFTObject.returnTime();
}

/**
 *  @brief  klasa dostosowana do wyznaczenia widma i pomiaru czasu
 *          wykonania algorytmu fft w w�tkach
 *
 *  Klasa zawiera metod�, kt�ra pozwala na pomiar czasu wykonywania
 *  algorytmu fft dla serii kilku pomiar�w. Zosta�a ona dostosowana
 *  do tego, aby mo�liwy by� pomiar dla serii wielu pr�b w spos�b
 *  wielow�tkowy. Dany obiekt klasy jest zwi�zany z jednym sygna�em
 *  o �ci�le okre�lonej d�ugo�ci i typie. Parametry te s� okre�lane
 *  przy utworzeniu obiektu jako parametr szablonu i argument
 *  konstruktora i nie zmieniaj� si� do ko�ca �ycia obiektu. ��czny
 *  W klasie zawarto tak�e kilka dodatkowych metdod, om�wionych 
 *  poni�ej.
 * 
 *  @tparam Type okre�la typ zmiennych, kt�re zawiera wektor
 */
template<class Type>
class TimeFFTClass
{
	std::mutex mutex;			///< mutex sekcji krytycznej zwi�zanej z pomiarem czasu
	double time;				///< pole przechowuj�ce ��czny czas wykonywania fft dla wszystkich pr�b
	int threadCounter;			///< licznik ilo�ci pomiar�w - liczba wywo�a� metody measureTime()
	std::vector<Type> signal;	///< sygna� dla kt�rego wykonywane s� pomiary

public:
	/**
	 *  @brief  konstruktor
	 *  
	 *  @param  initialSignal wektor, zawieraj�cy synga� na podstawie kt�rego
	 *          tworzony jest sygna� zwi�zany z obiektem
	 *  @param  numSamples stanowi ilo�� pr�bek, jak� ma mie� docelowy sygna�
	 *          z wi�zany z obiektem i dla kt�rej maj� zosta� wykonane pomiary
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
	 *  @brief  konstruktor kopiuj�cy
	 *
	 *  Konieczny do utworzenia wektora obiekt�w klasy TimeFFTClass
	 */
	TimeFFTClass(const TimeFFTClass &object) {
		time = object.time;
		threadCounter = object.threadCounter;
		signal = object.signal;
	}

	/**
	 *  @brief  wy�wietla aktualn� posta� sygna�u w konsoli
	 */
	void print() {
		printVector(this->signal);
	}

	/**
	 *  @brief  mierzy czas wykonania fft i powi�ksza o t� warto�� pole time
     */
	void measureTime() {
		// Pomiar czasu z wykorzystaniem biblioteki <chrono>
		// Pobranie aktualnej warto�ci zegara - warto�� pocz�tkowa
		auto start = std::chrono::steady_clock::now();
		// Wykonanie algorytmu fft. Funkcja fftTime nie zmienia postaci
		// sygna�u wej�ciowego!
		fftTime(this->signal);
		// Pobranie aktualnej warto�ci zegara - warto�� po wykonaniu fft
		auto end = std::chrono::steady_clock::now();

		// Obliczenie czasu
		std::chrono::duration<double, std::milli> duration = end - start;

		// Zwi�kszenie licznika ilo�ci pr�b oraz powi�kszenie pola time o 
		// czas trwania aktualnej pr�by. Poniewa� w programie metoda 
		// measureTime jest wykonywana nawet dla 100 w�tk�w i ich przewidywany
		// czas wykonywania powinien by� zbli�ony, aby �aden pomiar nie zosta�
		// pomini�ty pola te zwi�kszane s� dopiero po wej�ciu do sekcji
		// krytycznej zwi�zanej z mutexem "mutex"
		mutex.lock();
		threadCounter++;
		this->time += duration.count();
		mutex.unlock();
	}

	/**
	 *  @brief  zwraca �redni czas wykonywania algorytmu fft
	 */
	double returnTime() {
		return time/static_cast<double>(threadCounter);
	}

	/**
	 *  @brief  zapisuje aktualn� posta� pola signal do pliku
	 *
	 *  @param  fileName stanowi tablic� char z nazw� pliku, do kt�rego
	 *          dane maj� zosta� zapisane
	 */
	void saveToFile(const char* fileName) {
		saveVectorToFile(this->signal, fileName);
	}

	/**
	 *  @brief  zamienia sygna� zwi�zany z obiektem na widmo fft tego sygna�u
	 */
	void obtainFFT() {
		fft(this->signal);
	}
};

void executeFFT(const VectCxDbl signal);
void compareFFT(const VectCxDbl signal);
void testFFT(const VectCxDbl signal);
#include "stdafx.h"
#include "DPsignals.h"
#include "DPfiles.h"
#include "DPinit.h"

#include <complex>
#include <vector>
#include <iostream>

/**
 *  @brief  wyœwietla menu czêœci programu do modyfikacji sygna³u
 *          wejœciowego
 *
 *  @param  signal wskazuje na wektor zawieraj¹cy sygna³ wejœciowy
 *  @return void
 */
void changeSignalMenu(VectCxDbl& signal) {
	char	choiceChar=0;		///< zmienna znakowa do funkcji switch w menu
	VectDbl signalParameters;	///< wektor zawieraj¹cy parametry generowanego sygna³u
	
	// Za³adowanie parametrów sygna³u z pliku. Je¿eli plik 
	// nie istnieje lub nie mo¿na go odczytaæ inicjalizowane
	// s¹ parametry domyœlne i wpisywane s¹ do pliku
	if (loadParams(signalParameters)) {
		signalParameters = { 80, 10, 1, -1, 0 };
		saveParams(signalParameters);
	}

	// Wyœwietlanie menu programu do modyfikacji sygna³u
	//  Menu jest wyœwietlane do momentu, kiedy u¿ytkownik wprowadza znaki 
	//  od "0" do "6".
	do {
		system("cls");	// Wyczyszczenie konsoli

		std::cout << "Proszê wybraæ rodzaj sygna³u lub jedn¹ z opcji:" << std::endl;
		std::cout << "> (0) zmiana parametrów generowanego sygna³u" << std::endl;
		std::cout << "> (1) sinus" << std::endl;
		std::cout << "> (2) sygna³ prostok¹tny" << std::endl;
		std::cout << "> (3) sygna³ pi³okszta³tny" << std::endl;
		std::cout << "> (4) sygna³ œwiergotowy (sin(y)*sin(x))" << std::endl;
		std::cout << "> (5) szum bia³y" << std::endl << std::endl;
		std::cout << "> (6) odczyt sygna³u z pliku sygnal_wlasny.txt" << std::endl;
		std::cout << "      dozwolony format próbek w pliku: " << std::endl;
		std::cout << "      x1 x2 x3...   lub   ";
		std::cout << "(Rex1,Imx1) (Rex2,Imx2) (Rex3,Imx3)..." << std::endl << std::endl;
		std::cout << "> dowolny inny znak, aby powróciæ do menu g³ównego" << std::endl;
		std::cin >> choiceChar;
		system("cls");
		if (choiceChar == 48)
			// Znak "0" - otwarcie menu czêœci programu pozwalaj¹cej na zmianê
			// parametrów sygna³u
			changeParamsMenu(signalParameters);
		else if (choiceChar > 48 && choiceChar < 54)
			// Znaki od "1" do "5" - wywo³anie funkcji generuj¹cej sygna³ okreœlonego
			// przez u¿ytkownika rodzaju. Proszê zauwa¿yæ, ¿e jako jeden z  argumentów
			// funkcji generateSignal(), u¿yta jest inna funkcja tj. getNumSamples().
			// Powoduje to, ¿e przed wygenerowaniem sygna³u w konsoli wyœwietlane jest
			// menu prosz¹ce u¿ytkownika o wprowadzenie iloœci próbek.
			generateSignal(signal, getNumSamples(false), choiceChar, signalParameters);
		else if (choiceChar == 54)
			// Znak "6" - przepisanie próbek sygna³u z pliku tekstowego. Je¿eli plik
			// nie istnieje wyœwietlany jest stosowny komunikat.
			if (loadFromFile(signal, "sygnal_wlasny.txt")) {
				std::cout << std::endl <<"Plik sygnal_wlasny.txt nie istnieje lub nie mo¿na go otworzyæ. ";
				std::cout << "Sygna³ nie zosta³ zmodyfikowany." << std::endl;
			}
	} while (choiceChar > 47 && choiceChar < 55);
}

/**
 *  @brief  generuje sygna³ okreœlonego rodzaju i wpisuje go
 *          do wskazanego wektora
 *  
 *  Dostêpne rodzaje sygna³ów, okreœlone przez signalType:
 *  > "49" sygna³ sinusoidalny
 *  > "50" sygna³ prostok¹tny
 *  > "51" sygna³ pi³okszta³tny
 *  > "52" sygna³ œwiergotowy typu sin(x)*sin(t)
 *  > "53" sygna³ losowy - szum bia³y
 *  
 *  @param  signal wskazuje wektor typu std::complex<double>,
 *          do którego sygna³ ma zostaæ wpisany
 *  @param  numSamples zawiera informacjê o d³ugoœci wektora, 
 *          który ma zostaæ wygenerowany
 *  @param  signalType zawiera informacjê o typie sygna³u,
 *          który ma zostaæ wygenerowany.
 *  @param  params wskazuje wektor 5-ciu parametów sygna³, kolejno
 *          czêst. próbkowania, czêst. sygna³u, amp. max, amp. min
 *          przesuniêcie w fazie
 *  @return void
 */
void generateSignal(VectCxDbl& signal, unsigned int numSamples, char signalType, VectDbl& params) {
	unsigned int i = 0;		///< licznik pêtli
	// Przepisanie parametrów do zmiennych u³atwiaj¹cych ich wykorzystanie
	int samplesForPeriod = (int)(params[0] / params[1]);
	double fn = params[1] / params[0];
	double ampMax = params[2], ampMin = params[3], phase = params[4];
	double signalValue, temp1, temp2;

	// Je¿eli rodzaj sygna³u zosta³ wskazany poprawnie nastêpuje wyczyszczenie wektora
	if (signalType > 48 && signalType < 54) signal.clear();

	// Wywo³anie odpowiedniego bloku funkcji switch i wygenerowanie sygna³u.
	//  Bloki case zawieraj¹ proste operacje numeryczne pozwalaj¹ce na utworzenie
	//  wektora z sygna³em o okreœlonym typie.
	switch (signalType)
	{
		// Sygnal sinusoidalny
	case 49:
		for (i = 0; i < numSamples; i++) {
			temp1 = ((ampMax - ampMin) / 2)*sin(static_cast<double>(i) * fn * Pi2 + phase);
			temp1 += (ampMax + ampMin) / 2;
			signal.push_back(temp1);
		}
		break;
		// Sygna³ prostok¹tny
	case 50:
		unsigned int phaseSamples;
		temp1 = phase / Pi2;
		temp2 = 0;
		// Obliczenie przesuniêcia fazowego wyra¿onego w iloœci próbek
		// (wartoœæ mniejsza ni¿ iloœæ próbek na okres)
		phaseSamples = (unsigned int)((double)samplesForPeriod*modf(temp1, &temp2));
		i = phaseSamples;
		while (i < numSamples + phaseSamples) {
			if ((i / (samplesForPeriod / 2)) % 2)
				signal.push_back(ampMin);
			else
				signal.push_back(ampMax);
			i++;
		}
		break;
		// Sygna³ pi³okszta³tny
	case 51:
		signalValue = ((ampMax - ampMin) / 2)*(phase / Pi2);
		while (i < numSamples) {
			if (signalValue > ampMax) signalValue = ampMin;
			signal.push_back(signalValue);
			signalValue += (ampMax - ampMin) / static_cast<double>(samplesForPeriod);
			i++;
		}
		break;
		// Sygna³ œwiergotowy
	case 52:
		for (i = 0; i < numSamples; i++) {
			signalValue = ((ampMax - ampMin) / 2)*sin(static_cast<double>(i) * fn * Pi2 + phase);
			signal.push_back(sin(0.05*static_cast<double>(i) + phase)*signalValue);
		}
		break;
		// Szum bia³y
	case 53:
		for (i = 0; i < numSamples; i++) {
			signalValue = (double)rand() / RAND_MAX;
			signalValue = ampMin + signalValue * (ampMax - ampMin);
			signal.push_back(signalValue);
		}
		break;
	default:
		break;
	}

}

/**
 *  @brief  kopiuje wartoœci z wektora typu std::complex<double>
 *          do wektora typu std::complex<float>
 *
 *  @param  vectorDbl wskazuje wektor, z którego wartoœci maj¹ zostaæ
 *          przekopoiowane
 *  @param  vectorFlt wskazuje wektor, do którego wartoœci maj¹ zostaæ
 *          przekopoiowane
 *  @return void
 */
void vectorDoubleToFloat(const VectCxDbl vectorDbl, VectCxFlt& vectorFlt) {
	
	vectorFlt.clear();

	for (unsigned int i = 0; i < vectorDbl.size(); i++) {
		vectorFlt.push_back(static_cast<CxFlt>(vectorDbl[i]));
	}
}

/**
 *  @brief  pobiera od u¿ytkownika iloœæ próbek
 *  
 *  Iloœæ próbek, jaka mo¿e zostaæ przeanalizowana w programie
 *  zosta³a ograniczona do 2^21 = 2097152. Powy¿ej tej wartoœci
 *  pojawiaj¹ siê problemy z alokacj¹ pamiêci dla tworzonych 
 *  wektorów, a ponadto czasy oczekiwania na wyniki s¹ doœæ d³ugie.
 *
 *  @param  powerOF2 okreœla, czy podana przez u¿ytkownika
 *          liczba musi byæ potêg¹ liczby 2
 *  @return void
 */
unsigned int getNumSamples(bool powerOf2) {
	unsigned int numSamples = 0;
	unsigned int bitsInWord = 0;	///< zawiera informacjê o iloœci bitów w s³owie 
	                                ///< okreœlaj¹cym zmienn¹ numSamples
	unsigned int tempNumSamples;	///< kopia podanej iloœci próbek, na której
	                                ///< wykonywane s¹ przesuniêcia bitowe
	bool notFirst = false;			///< zmienna pomocnicza do sprawdzenia, czy
	                                ///< u¿ytkownik wprowadzi³ niepoprawn¹ liczbê
	// Wyœwietlenie stosownego menu i pobranie od u¿ytkownika iloœci próbek.
	//  Ta czêsæ programu jest wykonywana do momentu podania przez u¿ytkownika
	//  prawid³owej wartoœci. Wyœwietlanie komunikaty mog¹ mieæ dwie ró¿ne
	//  postacie, w zale¿noœci od wartoœci zmiennej logicznej powerOf2. Je¿eli
	//  zmienna ta przyjmuje wartoœæ true, wprowadzana liczba musi byæ tak¿e 
	//  potêg¹ liczby 2.
	do {
		bitsInWord = 0;		// Wyzerowanie zmiennej w kolejnej iteracji pêtli

		// Wyœwietlenie komunikatu, je¿eli u¿ytkownik poda³ poprzednio niew³aœciw¹
		// wartoœæ.
		if (notFirst)
		{
			system("cls");
			std::cout << "Niepoprawna wartoœæ." << std::endl;
		}
		if (powerOf2) {
			std::cout << "Proszê podaæ iloœæ próbek, która ma zostaæ przeanalizowana (minimum 2 maksimum, 2^21=2097152):" << std::endl;
			std::cout << std::endl << "Uwaga:" << std::endl << "- Aby widmo FFT mog³o zostaæ wyznaczone poprawnie, iloœæ próbek musi byæ równa potêdze liczby 2." << std::endl;
			std::cout << "- Je¿eli podana wartoœæ przekracza iloœæ próbek w analizowanym sygnale, zostanie on uzupe³niony zerami." << std::endl;
		}
		else
		{
			std::cout << "Proszê podaæ iloœæ próbek (minimum 2, maksimum 2^21=2097152):" << std::endl;
		}
		std::cin >> numSamples;

		// Je¿eli liczba ma byæ potêg¹ liczby dwa, zostaje wykonana czêœæ programu
		// sprawdzaj¹ca ten warunek. Sprawdzenie to polega na zliczeniu iloœci bitów
		// w s³owie. Je¿eli liczba ta jest potêg¹ liczby 2, to suma bitów mo¿e byæ
		// równa tylko i wy³¹cznie 1. Je¿eli nie jest to prawda, liczba bitów zostaje
		// ustawiona na 0, tak aby pêtla do... while zosta³a wykonana ponownie. 
		if (powerOf2) {
			tempNumSamples = numSamples;
			for (; tempNumSamples != 0; tempNumSamples >>= 1) bitsInWord += tempNumSamples & 1;
			if (bitsInWord > 1) numSamples = 0;
		}

		notFirst = true;

	} while (numSamples < 2 || numSamples > 2097152);
	return numSamples;
}

/**
*   @brief  wyœwietlenie menu do modyfikacji parametrów sygna³u
*
*   @param  params wskazuje wektor, do którego parametry maj¹
            zostaæ wpisane
*   @return void
*/
void changeParamsMenu(VectDbl& params) {
	char choiceChar=49;		///< zmienna znakowa do funkcji switch w menu
	bool notFirst;			///< zmienna pomocnicza do sprawdzenia, czy
	                        ///< u¿ytkownik wprowadzi³ niepoprawn¹ liczb
	// Wyœwietlenie menu i modyfikacja danego parametru w zale¿noœci od
	// wybranej opcji. 
	while (choiceChar > 48 && choiceChar < 54) {
		system("cls");
		std::cout << "Proszê wybraæ jedn¹ z opcji:" << std::endl;
		std::cout << "> (1) zmiana czêstotliwosci próbkowania sygna³u" << std::endl;
		std::cout << "> (2) zmiana czêstotliwoœci sygna³u" << std::endl;
		std::cout << "> (3) zmiana amplitudy maksymalnej" << std::endl;
		std::cout << "> (4) zmiana amplitudy minimalnej" << std::endl;
		std::cout << "> (5) zmiana przesuniêcia fazowego" << std::endl;
		std::cout << "> dowolny inny znak, aby powróciæ" << std::endl << std::endl;
		std::cout << "Aktualne parametry generowanego sygna³u:" << std::endl;
		std::cout << "> Czêœtotliwoœæ próbkowania fs = " << params[0] << " Hz, ";
		std::cout << "czas próbkowania Ts = " << 1/params[0] << " s" << std::endl;
		std::cout << "> Czêstotliwoœæ sygna³u f = " << params[1] << " Hz" << std::endl;
		std::cout << "> Czêstotliwoœæ unormowana fn = f/fs = " << params[1] / params[0] << std::endl;
		std::cout << "> Amplituda maksymalna AmpMax = " << params[2] << std::endl;
		std::cout << "> Amplituda minimalna AmpMin = " << params[3] << std::endl;
		std::cout << "> Przesuniêcie w fazie fi = " << params[4] << " rad" << std::endl;
		std::cin >> choiceChar;

		// Modyfikacja parametru, w zale¿noœci od wprowadzonego znaku
		switch (choiceChar)
		{
		case 49:
			// Znak "1" - zmiana czêstotliwoœci próbkowania
			std::cout << "Proszê podaæ wartoœæ czêstotliwoœci próbkowania sygna³u:" << std::endl;
			std::cout << "(uwaga: fs powinno byæ równe conajmniej " << 2 * params[1];
			std::cout << " Hz, aby by³o spe³nione tw. o próbkowaniu)" << std::endl;
			std::cin >> params[0];
			break;
		case 50:
			// Znak "2" - zmiana czêstotliwoœci sygna³u
			std::cout << "Proszê podaæ wartoœæ czêstotliwoœci sygna³u:" << std::endl;
			std::cin >> params[1];
			break;
		case 51:
			// Znak "3" - zmiana amplitudy maksymalnej 
			notFirst = false;
			do {
				if (notFirst) std::cout << "Niepoprawna wartoœæ. AmpMax musi byæ wiêksze ni¿ " << params[3] << std::endl;
				std::cout << "Proszê podaæ wartoœæ amplitudy maksymalnej (AmpMax > AmpMin):" << std::endl;
				std::cin >> params[2];
				notFirst = true;
			} while (params[2] <= params[3]);
			break;
		case 52:
			// Znak "4" - zmiana amplitudy minimalnej
			notFirst = false;
			do {
				if (notFirst) std::cout << "Niepoprawna wartoœæ. AmpMin musi byæ mniejsze ni¿ " << params[2] << std::endl;
				std::cout << "Proszê podaæ wartoœæ amplitudy minimalnej (AmpMin < AmpMax):" << std::endl;
				std::cin >> params[3];
				notFirst = true;
			} while (params[3] >= params[2]);
			break;
		case 53:
			// Znak "5" - zmiana wartoœci przesuniêcia fazowego
			std::cout << "Proszê podaæ wartoœæ przesuniêcia fazowego w rad:" << std::endl;
			std::cin >> params[4];
			break;
		default:
			break;
		}
	}
	saveParams(params);
}
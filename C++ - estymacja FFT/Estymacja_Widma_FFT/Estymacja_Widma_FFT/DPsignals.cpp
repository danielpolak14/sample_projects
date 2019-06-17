#include "stdafx.h"
#include "DPsignals.h"
#include "DPfiles.h"
#include "DPinit.h"

#include <complex>
#include <vector>
#include <iostream>

/**
 *  @brief  wy�wietla menu cz�ci programu do modyfikacji sygna�u
 *          wej�ciowego
 *
 *  @param  signal wskazuje na wektor zawieraj�cy sygna� wej�ciowy
 *  @return void
 */
void changeSignalMenu(VectCxDbl& signal) {
	char	choiceChar=0;		///< zmienna znakowa do funkcji switch w menu
	VectDbl signalParameters;	///< wektor zawieraj�cy parametry generowanego sygna�u
	
	// Za�adowanie parametr�w sygna�u z pliku. Je�eli plik 
	// nie istnieje lub nie mo�na go odczyta� inicjalizowane
	// s� parametry domy�lne i wpisywane s� do pliku
	if (loadParams(signalParameters)) {
		signalParameters = { 80, 10, 1, -1, 0 };
		saveParams(signalParameters);
	}

	// Wy�wietlanie menu programu do modyfikacji sygna�u
	//  Menu jest wy�wietlane do momentu, kiedy u�ytkownik wprowadza znaki 
	//  od "0" do "6".
	do {
		system("cls");	// Wyczyszczenie konsoli

		std::cout << "Prosz� wybra� rodzaj sygna�u lub jedn� z opcji:" << std::endl;
		std::cout << "> (0) zmiana parametr�w generowanego sygna�u" << std::endl;
		std::cout << "> (1) sinus" << std::endl;
		std::cout << "> (2) sygna� prostok�tny" << std::endl;
		std::cout << "> (3) sygna� pi�okszta�tny" << std::endl;
		std::cout << "> (4) sygna� �wiergotowy (sin(y)*sin(x))" << std::endl;
		std::cout << "> (5) szum bia�y" << std::endl << std::endl;
		std::cout << "> (6) odczyt sygna�u z pliku sygnal_wlasny.txt" << std::endl;
		std::cout << "      dozwolony format pr�bek w pliku: " << std::endl;
		std::cout << "      x1 x2 x3...   lub   ";
		std::cout << "(Rex1,Imx1) (Rex2,Imx2) (Rex3,Imx3)..." << std::endl << std::endl;
		std::cout << "> dowolny inny znak, aby powr�ci� do menu g��wnego" << std::endl;
		std::cin >> choiceChar;
		system("cls");
		if (choiceChar == 48)
			// Znak "0" - otwarcie menu cz�ci programu pozwalaj�cej na zmian�
			// parametr�w sygna�u
			changeParamsMenu(signalParameters);
		else if (choiceChar > 48 && choiceChar < 54)
			// Znaki od "1" do "5" - wywo�anie funkcji generuj�cej sygna� okre�lonego
			// przez u�ytkownika rodzaju. Prosz� zauwa�y�, �e jako jeden z  argument�w
			// funkcji generateSignal(), u�yta jest inna funkcja tj. getNumSamples().
			// Powoduje to, �e przed wygenerowaniem sygna�u w konsoli wy�wietlane jest
			// menu prosz�ce u�ytkownika o wprowadzenie ilo�ci pr�bek.
			generateSignal(signal, getNumSamples(false), choiceChar, signalParameters);
		else if (choiceChar == 54)
			// Znak "6" - przepisanie pr�bek sygna�u z pliku tekstowego. Je�eli plik
			// nie istnieje wy�wietlany jest stosowny komunikat.
			if (loadFromFile(signal, "sygnal_wlasny.txt")) {
				std::cout << std::endl <<"Plik sygnal_wlasny.txt nie istnieje lub nie mo�na go otworzy�. ";
				std::cout << "Sygna� nie zosta� zmodyfikowany." << std::endl;
			}
	} while (choiceChar > 47 && choiceChar < 55);
}

/**
 *  @brief  generuje sygna� okre�lonego rodzaju i wpisuje go
 *          do wskazanego wektora
 *  
 *  Dost�pne rodzaje sygna��w, okre�lone przez signalType:
 *  > "49" sygna� sinusoidalny
 *  > "50" sygna� prostok�tny
 *  > "51" sygna� pi�okszta�tny
 *  > "52" sygna� �wiergotowy typu sin(x)*sin(t)
 *  > "53" sygna� losowy - szum bia�y
 *  
 *  @param  signal wskazuje wektor typu std::complex<double>,
 *          do kt�rego sygna� ma zosta� wpisany
 *  @param  numSamples zawiera informacj� o d�ugo�ci wektora, 
 *          kt�ry ma zosta� wygenerowany
 *  @param  signalType zawiera informacj� o typie sygna�u,
 *          kt�ry ma zosta� wygenerowany.
 *  @param  params wskazuje wektor 5-ciu paramet�w sygna�, kolejno
 *          cz�st. pr�bkowania, cz�st. sygna�u, amp. max, amp. min
 *          przesuni�cie w fazie
 *  @return void
 */
void generateSignal(VectCxDbl& signal, unsigned int numSamples, char signalType, VectDbl& params) {
	unsigned int i = 0;		///< licznik p�tli
	// Przepisanie parametr�w do zmiennych u�atwiaj�cych ich wykorzystanie
	int samplesForPeriod = (int)(params[0] / params[1]);
	double fn = params[1] / params[0];
	double ampMax = params[2], ampMin = params[3], phase = params[4];
	double signalValue, temp1, temp2;

	// Je�eli rodzaj sygna�u zosta� wskazany poprawnie nast�puje wyczyszczenie wektora
	if (signalType > 48 && signalType < 54) signal.clear();

	// Wywo�anie odpowiedniego bloku funkcji switch i wygenerowanie sygna�u.
	//  Bloki case zawieraj� proste operacje numeryczne pozwalaj�ce na utworzenie
	//  wektora z sygna�em o okre�lonym typie.
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
		// Sygna� prostok�tny
	case 50:
		unsigned int phaseSamples;
		temp1 = phase / Pi2;
		temp2 = 0;
		// Obliczenie przesuni�cia fazowego wyra�onego w ilo�ci pr�bek
		// (warto�� mniejsza ni� ilo�� pr�bek na okres)
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
		// Sygna� pi�okszta�tny
	case 51:
		signalValue = ((ampMax - ampMin) / 2)*(phase / Pi2);
		while (i < numSamples) {
			if (signalValue > ampMax) signalValue = ampMin;
			signal.push_back(signalValue);
			signalValue += (ampMax - ampMin) / static_cast<double>(samplesForPeriod);
			i++;
		}
		break;
		// Sygna� �wiergotowy
	case 52:
		for (i = 0; i < numSamples; i++) {
			signalValue = ((ampMax - ampMin) / 2)*sin(static_cast<double>(i) * fn * Pi2 + phase);
			signal.push_back(sin(0.05*static_cast<double>(i) + phase)*signalValue);
		}
		break;
		// Szum bia�y
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
 *  @brief  kopiuje warto�ci z wektora typu std::complex<double>
 *          do wektora typu std::complex<float>
 *
 *  @param  vectorDbl wskazuje wektor, z kt�rego warto�ci maj� zosta�
 *          przekopoiowane
 *  @param  vectorFlt wskazuje wektor, do kt�rego warto�ci maj� zosta�
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
 *  @brief  pobiera od u�ytkownika ilo�� pr�bek
 *  
 *  Ilo�� pr�bek, jaka mo�e zosta� przeanalizowana w programie
 *  zosta�a ograniczona do 2^21 = 2097152. Powy�ej tej warto�ci
 *  pojawiaj� si� problemy z alokacj� pami�ci dla tworzonych 
 *  wektor�w, a ponadto czasy oczekiwania na wyniki s� do�� d�ugie.
 *
 *  @param  powerOF2 okre�la, czy podana przez u�ytkownika
 *          liczba musi by� pot�g� liczby 2
 *  @return void
 */
unsigned int getNumSamples(bool powerOf2) {
	unsigned int numSamples = 0;
	unsigned int bitsInWord = 0;	///< zawiera informacj� o ilo�ci bit�w w s�owie 
	                                ///< okre�laj�cym zmienn� numSamples
	unsigned int tempNumSamples;	///< kopia podanej ilo�ci pr�bek, na kt�rej
	                                ///< wykonywane s� przesuni�cia bitowe
	bool notFirst = false;			///< zmienna pomocnicza do sprawdzenia, czy
	                                ///< u�ytkownik wprowadzi� niepoprawn� liczb�
	// Wy�wietlenie stosownego menu i pobranie od u�ytkownika ilo�ci pr�bek.
	//  Ta cz�s� programu jest wykonywana do momentu podania przez u�ytkownika
	//  prawid�owej warto�ci. Wy�wietlanie komunikaty mog� mie� dwie r�ne
	//  postacie, w zale�no�ci od warto�ci zmiennej logicznej powerOf2. Je�eli
	//  zmienna ta przyjmuje warto�� true, wprowadzana liczba musi by� tak�e 
	//  pot�g� liczby 2.
	do {
		bitsInWord = 0;		// Wyzerowanie zmiennej w kolejnej iteracji p�tli

		// Wy�wietlenie komunikatu, je�eli u�ytkownik poda� poprzednio niew�a�ciw�
		// warto��.
		if (notFirst)
		{
			system("cls");
			std::cout << "Niepoprawna warto��." << std::endl;
		}
		if (powerOf2) {
			std::cout << "Prosz� poda� ilo�� pr�bek, kt�ra ma zosta� przeanalizowana (minimum 2 maksimum, 2^21=2097152):" << std::endl;
			std::cout << std::endl << "Uwaga:" << std::endl << "- Aby widmo FFT mog�o zosta� wyznaczone poprawnie, ilo�� pr�bek musi by� r�wna pot�dze liczby 2." << std::endl;
			std::cout << "- Je�eli podana warto�� przekracza ilo�� pr�bek w analizowanym sygnale, zostanie on uzupe�niony zerami." << std::endl;
		}
		else
		{
			std::cout << "Prosz� poda� ilo�� pr�bek (minimum 2, maksimum 2^21=2097152):" << std::endl;
		}
		std::cin >> numSamples;

		// Je�eli liczba ma by� pot�g� liczby dwa, zostaje wykonana cz�� programu
		// sprawdzaj�ca ten warunek. Sprawdzenie to polega na zliczeniu ilo�ci bit�w
		// w s�owie. Je�eli liczba ta jest pot�g� liczby 2, to suma bit�w mo�e by�
		// r�wna tylko i wy��cznie 1. Je�eli nie jest to prawda, liczba bit�w zostaje
		// ustawiona na 0, tak aby p�tla do... while zosta�a wykonana ponownie. 
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
*   @brief  wy�wietlenie menu do modyfikacji parametr�w sygna�u
*
*   @param  params wskazuje wektor, do kt�rego parametry maj�
            zosta� wpisane
*   @return void
*/
void changeParamsMenu(VectDbl& params) {
	char choiceChar=49;		///< zmienna znakowa do funkcji switch w menu
	bool notFirst;			///< zmienna pomocnicza do sprawdzenia, czy
	                        ///< u�ytkownik wprowadzi� niepoprawn� liczb
	// Wy�wietlenie menu i modyfikacja danego parametru w zale�no�ci od
	// wybranej opcji. 
	while (choiceChar > 48 && choiceChar < 54) {
		system("cls");
		std::cout << "Prosz� wybra� jedn� z opcji:" << std::endl;
		std::cout << "> (1) zmiana cz�stotliwosci pr�bkowania sygna�u" << std::endl;
		std::cout << "> (2) zmiana cz�stotliwo�ci sygna�u" << std::endl;
		std::cout << "> (3) zmiana amplitudy maksymalnej" << std::endl;
		std::cout << "> (4) zmiana amplitudy minimalnej" << std::endl;
		std::cout << "> (5) zmiana przesuni�cia fazowego" << std::endl;
		std::cout << "> dowolny inny znak, aby powr�ci�" << std::endl << std::endl;
		std::cout << "Aktualne parametry generowanego sygna�u:" << std::endl;
		std::cout << "> Cz�totliwo�� pr�bkowania fs = " << params[0] << " Hz, ";
		std::cout << "czas pr�bkowania Ts = " << 1/params[0] << " s" << std::endl;
		std::cout << "> Cz�stotliwo�� sygna�u f = " << params[1] << " Hz" << std::endl;
		std::cout << "> Cz�stotliwo�� unormowana fn = f/fs = " << params[1] / params[0] << std::endl;
		std::cout << "> Amplituda maksymalna AmpMax = " << params[2] << std::endl;
		std::cout << "> Amplituda minimalna AmpMin = " << params[3] << std::endl;
		std::cout << "> Przesuni�cie w fazie fi = " << params[4] << " rad" << std::endl;
		std::cin >> choiceChar;

		// Modyfikacja parametru, w zale�no�ci od wprowadzonego znaku
		switch (choiceChar)
		{
		case 49:
			// Znak "1" - zmiana cz�stotliwo�ci pr�bkowania
			std::cout << "Prosz� poda� warto�� cz�stotliwo�ci pr�bkowania sygna�u:" << std::endl;
			std::cout << "(uwaga: fs powinno by� r�wne conajmniej " << 2 * params[1];
			std::cout << " Hz, aby by�o spe�nione tw. o pr�bkowaniu)" << std::endl;
			std::cin >> params[0];
			break;
		case 50:
			// Znak "2" - zmiana cz�stotliwo�ci sygna�u
			std::cout << "Prosz� poda� warto�� cz�stotliwo�ci sygna�u:" << std::endl;
			std::cin >> params[1];
			break;
		case 51:
			// Znak "3" - zmiana amplitudy maksymalnej 
			notFirst = false;
			do {
				if (notFirst) std::cout << "Niepoprawna warto��. AmpMax musi by� wi�ksze ni� " << params[3] << std::endl;
				std::cout << "Prosz� poda� warto�� amplitudy maksymalnej (AmpMax > AmpMin):" << std::endl;
				std::cin >> params[2];
				notFirst = true;
			} while (params[2] <= params[3]);
			break;
		case 52:
			// Znak "4" - zmiana amplitudy minimalnej
			notFirst = false;
			do {
				if (notFirst) std::cout << "Niepoprawna warto��. AmpMin musi by� mniejsze ni� " << params[2] << std::endl;
				std::cout << "Prosz� poda� warto�� amplitudy minimalnej (AmpMin < AmpMax):" << std::endl;
				std::cin >> params[3];
				notFirst = true;
			} while (params[3] >= params[2]);
			break;
		case 53:
			// Znak "5" - zmiana warto�ci przesuni�cia fazowego
			std::cout << "Prosz� poda� warto�� przesuni�cia fazowego w rad:" << std::endl;
			std::cin >> params[4];
			break;
		default:
			break;
		}
	}
	saveParams(params);
}
#include "stdafx.h"
#include "DPfiles.h"
#include "DPinit.h"

#include <vector>	//std::vector
#include <complex>	//std::complex
#include <fstream>

/**
 *  @brief  wpisuje ci�g liczb odczytany z pliku binarnego
 *          do wektora liczb zespolonych typu double
 *
 *	W pliku dozwolone s� dwie formy zapisu liczb:
 *  - tylko liczby rzeczywiste oddzielone spacjami: 
 *    x1 x2 x3...
 *  - liczby zespolone oddzielone spacjami zapisane w 
 *    formacie zgodnym z formatem strumienia:
 *    (Rex1,Imx1) (Rex2,Imx2) (Rex3,Imx3)...
 *
 *  @param  loadedVector wskazuje na wektor, do kt�rego 
 *          dane maj� zosta� wpisane
 *  @param  fileName wskazuje tablic� znak�w, kt�ra okre�la nazw�
 *          pliku, z kt�rego ma nast�pi� zapis
 *  @return warto�� logiczna, okre�laj�ca czy podczas odczytu
 *          wyst�pi� b��d (g��wnie czy plik istnieje)
 */
bool loadFromFile(VectCxDbl& loadedVector, const char *fileName) {
	bool fileError = true;	///< zmienna okre�laj�ca czy odczyt z pliku by� mo�liwy
	std::fstream file;		///< strumie� wej�cia/wyj�cia do pliku
	CxDbl tempCxDbl;		///< zmienna zespolona typu double, przechowuj�ca chwilow� zawarto�� strumienia 
	double tempDbl;			///< zmienna typu double, przechowuj�ca chwilow� zawarto�� strumienia
	char firstSign;			///< zmienna pomocnicza, zawieraj�ca pierwszy znak znajduj�cy si� w pliku 

	// Otwarcie pliku binarnego w trybie do odczytu
	file.open(fileName, std::ios::in |
						std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku si� powiod�o
	if (file.is_open()) {
		fileError = false;		// je�eli plik zosta� otwarty, zmienna 
								// wskazuj�ca na b��d przyjmuje warto�� false
		loadedVector.clear();	// Wyczyszenie wektora

		file >> firstSign;		// Przekazanie pierwszego znaku do zmiennej firsChar
		file.seekg(0);			// Ustawienie kursora na pocz�tek pliku

		// Odczyt danych i wpisanie ich do wektora. 
		// Je�eli pierwszym znakiem w pliku jest nawias "(", warto�ci odczytywane s�
		// w formacie zgodnym z <complex>, tj. (Re(X),Im(X)), w przeciwnym wypadku
		// w formacie zgodnym ze zwyk�� zmienn� typu double
		if (firstSign == 40) {
			while (file >> tempCxDbl)
				loadedVector.push_back(tempCxDbl);
		}
		else {
			while (file >> tempDbl)
				loadedVector.push_back(tempDbl);
		}
	}
	// Zamkni�cie pliku binarnego
	file.close();
	return fileError;
}

/**
 *  @brief  wpisuje ci�g liczb odczytany z pliku binarnego
 *          do wektora liczb typu double
 *
 *  @param  loadedVector wskazuje na wektor, do kt�rego
 *          dane maj� zosta� wpisane
 *  @param  fileName wskazuje tablic� znak�w, kt�ra okre�la nazw�
 *          pliku, z kt�rego ma nast�pi� zapis
 *  @return warto�� logiczna, okre�laj�ca czy podczas odczytu
 *          wyst�pi� b��d (g��wnie czy plik istnieje)
 */
bool loadFromFile(VectDbl& loadedVector, const char *fileName) {
	bool fileError = true;	///< zmienna okre�laj�ca czy odczyt z pliku by� mo�liwy
	std::fstream file;		///< strumie� wej�cia/wyj�cia do pliku
	double temp;			///< zmienna przechowuj�ca chwilow� zawarto�� strumienia

	// Otwarcie pliku binarnego w trybie do odczytu
	file.open(fileName, std::ios::in |
						std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku si� powiod�o
	if (file.is_open()) {
		fileError = false;		// je�eli plik zosta� otwarty, zmienna 
								// wskazuj�ca na b��d przyjmuje warto�� false
		loadedVector.clear();	// Wyczyszenie wektora

		// Odczyt danych i wpisanie ich do wektora. 
		while (file >> temp)
		{
			loadedVector.push_back(temp);
		}
	}
	// Zamkni�cie pliku binarnego
	file.close();
	return fileError;
}

/**
 *  @brief  zapisuje wektor zawieraj�cy 5 element�w do pliku
 *          tekstowego parametry_sygnalu.txt o �ci�le okre�lonej
 *          strukturze
 *
 *  W pliku przechowywane s� parametry sygna�u wykorzystywanego
 *  w programie. Dzi�ki ich zapisowi mo�liwy jest ich p�niejszy
 *  odczyt tak�e w programie Matlab, w celu umo�liwienia
 *  interpretacji. Wektor zawiera kolejno: cz�stotliwo��
 *  pr�bkowania, cz�stotliwo�� sygna�u, amplitud� maksymaln�,
 *  amplitud� minimaln� i przesuni�cie fazie.
 *
 *  @param  params wskazuje 5-cio elementowy wektor zawieraj�cy
 *          parametry
 *  @return warto�� logiczna, okre�laj�ca czy podczas odczytu
 *          wyst�pi� b��d (g��wnie czy plik istnieje)
 */
bool saveParams(const VectDbl& params) {
	bool fileError = true;		///< zmienna okre�laj�ca czy odczyt z pliku by� mo�liwy
	std::fstream file;			///< strumie� wej�cia/wyj�cia do pliku
	// Otwarcie pliku w trybie zapisu
	file.open("parametry_sygnalu.txt",	std::ios::out |
										std::ios::binary);
	// Wpisanie danych do pliku
	if (file.is_open()) {
		fileError = false;
		file << "Czestotliwosc probkowania: " << params[0] << "\r\n";
		file << "Czestotliwosc sygnalu: " << params[1] << "\r\n";
		file << "Amplituda maksymalna: " << params[2] << "\r\n";
		file << "Amplituda minimalna: " << params[3] << "\r\n";
		file << "Przesuniecie w fazie: " << params[4] << "\r\n";
	}
	// Zamkni�cie pliku tekstowego
	file.close();
	return fileError;
}

/**
 *  @brief  odczytuje 5 liczb z pliku parametry_sygnalu.txt
 *          o �ci�le okre�lonej strukturze i wpisuje je do
 *          wektora
 *
 *  @param  params wskazuje wektor, do kt�rego parametry maj� 
 *          zosta� wpisane
 *  @return warto�� logiczna, okre�laj�ca czy podczas odczytu
 *          wyst�pi� b��d (g��wnie czy plik istnieje)
 */
bool loadParams(VectDbl& params) {
	bool fileError = true;	///< zmienna okre�laj�ca czy odczyt z pliku by� mo�liwy
	std::fstream file;		///< strumie� wej�cia/wyj�cia do pliku
	double temp;			///< zmienna przechowuj�ca chwilow� zawarto�� strumienia

	// Otwarcie pliku parametry_sygnalu.txt w trybie do odczytu
	file.open("parametry_sygnalu.txt",	std::ios::in |
										std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku si� powiod�o
	if (file.is_open()) {
		fileError = false;		// je�eli plik zosta� otwarty, zmienna 
								// wskazuj�ca na b��d przyjmuje warto�� false

		// Odczyt danych z pliku.
		// Parametry w pliku znajduj� si� w �ci�le okre�lonych miejscach,
		// st�d zachodzi tutaj potrzeba poruszania si� po pliku za pomoc�
		// metody seekg()
		file.seekg(27);
		file >> temp;
		params.push_back(temp);
		file.seekg(25,std::ios::cur);
		file >> temp;
		params.push_back(temp);
		file.seekg(24, std::ios::cur);
		file >> temp;
		params.push_back(temp);
		file.seekg(23, std::ios::cur);
		file >> temp;
		params.push_back(temp);
		file.seekg(24, std::ios::cur);
		file >> temp;
		params.push_back(temp);
	}
	// Zamkni�cie pliku binarnego
	file.close();
	return fileError;
}
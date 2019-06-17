#include "stdafx.h"
#include "DPfiles.h"
#include "DPinit.h"

#include <vector>	//std::vector
#include <complex>	//std::complex
#include <fstream>

/**
 *  @brief  wpisuje ci¹g liczb odczytany z pliku binarnego
 *          do wektora liczb zespolonych typu double
 *
 *	W pliku dozwolone s¹ dwie formy zapisu liczb:
 *  - tylko liczby rzeczywiste oddzielone spacjami: 
 *    x1 x2 x3...
 *  - liczby zespolone oddzielone spacjami zapisane w 
 *    formacie zgodnym z formatem strumienia:
 *    (Rex1,Imx1) (Rex2,Imx2) (Rex3,Imx3)...
 *
 *  @param  loadedVector wskazuje na wektor, do którego 
 *          dane maj¹ zostaæ wpisane
 *  @param  fileName wskazuje tablicê znaków, która okreœla nazwê
 *          pliku, z którego ma nast¹piæ zapis
 *  @return wartoœæ logiczna, okreœlaj¹ca czy podczas odczytu
 *          wyst¹pi³ b³¹d (g³ównie czy plik istnieje)
 */
bool loadFromFile(VectCxDbl& loadedVector, const char *fileName) {
	bool fileError = true;	///< zmienna okreœlaj¹ca czy odczyt z pliku by³ mo¿liwy
	std::fstream file;		///< strumieñ wejœcia/wyjœcia do pliku
	CxDbl tempCxDbl;		///< zmienna zespolona typu double, przechowuj¹ca chwilow¹ zawartoœæ strumienia 
	double tempDbl;			///< zmienna typu double, przechowuj¹ca chwilow¹ zawartoœæ strumienia
	char firstSign;			///< zmienna pomocnicza, zawieraj¹ca pierwszy znak znajduj¹cy siê w pliku 

	// Otwarcie pliku binarnego w trybie do odczytu
	file.open(fileName, std::ios::in |
						std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku siê powiod³o
	if (file.is_open()) {
		fileError = false;		// je¿eli plik zosta³ otwarty, zmienna 
								// wskazuj¹ca na b³¹d przyjmuje wartoœæ false
		loadedVector.clear();	// Wyczyszenie wektora

		file >> firstSign;		// Przekazanie pierwszego znaku do zmiennej firsChar
		file.seekg(0);			// Ustawienie kursora na pocz¹tek pliku

		// Odczyt danych i wpisanie ich do wektora. 
		// Je¿eli pierwszym znakiem w pliku jest nawias "(", wartoœci odczytywane s¹
		// w formacie zgodnym z <complex>, tj. (Re(X),Im(X)), w przeciwnym wypadku
		// w formacie zgodnym ze zwyk³¹ zmienn¹ typu double
		if (firstSign == 40) {
			while (file >> tempCxDbl)
				loadedVector.push_back(tempCxDbl);
		}
		else {
			while (file >> tempDbl)
				loadedVector.push_back(tempDbl);
		}
	}
	// Zamkniêcie pliku binarnego
	file.close();
	return fileError;
}

/**
 *  @brief  wpisuje ci¹g liczb odczytany z pliku binarnego
 *          do wektora liczb typu double
 *
 *  @param  loadedVector wskazuje na wektor, do którego
 *          dane maj¹ zostaæ wpisane
 *  @param  fileName wskazuje tablicê znaków, która okreœla nazwê
 *          pliku, z którego ma nast¹piæ zapis
 *  @return wartoœæ logiczna, okreœlaj¹ca czy podczas odczytu
 *          wyst¹pi³ b³¹d (g³ównie czy plik istnieje)
 */
bool loadFromFile(VectDbl& loadedVector, const char *fileName) {
	bool fileError = true;	///< zmienna okreœlaj¹ca czy odczyt z pliku by³ mo¿liwy
	std::fstream file;		///< strumieñ wejœcia/wyjœcia do pliku
	double temp;			///< zmienna przechowuj¹ca chwilow¹ zawartoœæ strumienia

	// Otwarcie pliku binarnego w trybie do odczytu
	file.open(fileName, std::ios::in |
						std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku siê powiod³o
	if (file.is_open()) {
		fileError = false;		// je¿eli plik zosta³ otwarty, zmienna 
								// wskazuj¹ca na b³¹d przyjmuje wartoœæ false
		loadedVector.clear();	// Wyczyszenie wektora

		// Odczyt danych i wpisanie ich do wektora. 
		while (file >> temp)
		{
			loadedVector.push_back(temp);
		}
	}
	// Zamkniêcie pliku binarnego
	file.close();
	return fileError;
}

/**
 *  @brief  zapisuje wektor zawieraj¹cy 5 elementów do pliku
 *          tekstowego parametry_sygnalu.txt o œciœle okreœlonej
 *          strukturze
 *
 *  W pliku przechowywane s¹ parametry sygna³u wykorzystywanego
 *  w programie. Dziêki ich zapisowi mo¿liwy jest ich póŸniejszy
 *  odczyt tak¿e w programie Matlab, w celu umo¿liwienia
 *  interpretacji. Wektor zawiera kolejno: czêstotliwoœæ
 *  próbkowania, czêstotliwoœæ sygna³u, amplitudê maksymaln¹,
 *  amplitudê minimaln¹ i przesuniêcie fazie.
 *
 *  @param  params wskazuje 5-cio elementowy wektor zawieraj¹cy
 *          parametry
 *  @return wartoœæ logiczna, okreœlaj¹ca czy podczas odczytu
 *          wyst¹pi³ b³¹d (g³ównie czy plik istnieje)
 */
bool saveParams(const VectDbl& params) {
	bool fileError = true;		///< zmienna okreœlaj¹ca czy odczyt z pliku by³ mo¿liwy
	std::fstream file;			///< strumieñ wejœcia/wyjœcia do pliku
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
	// Zamkniêcie pliku tekstowego
	file.close();
	return fileError;
}

/**
 *  @brief  odczytuje 5 liczb z pliku parametry_sygnalu.txt
 *          o œciœle okreœlonej strukturze i wpisuje je do
 *          wektora
 *
 *  @param  params wskazuje wektor, do którego parametry maj¹ 
 *          zostaæ wpisane
 *  @return wartoœæ logiczna, okreœlaj¹ca czy podczas odczytu
 *          wyst¹pi³ b³¹d (g³ównie czy plik istnieje)
 */
bool loadParams(VectDbl& params) {
	bool fileError = true;	///< zmienna okreœlaj¹ca czy odczyt z pliku by³ mo¿liwy
	std::fstream file;		///< strumieñ wejœcia/wyjœcia do pliku
	double temp;			///< zmienna przechowuj¹ca chwilow¹ zawartoœæ strumienia

	// Otwarcie pliku parametry_sygnalu.txt w trybie do odczytu
	file.open("parametry_sygnalu.txt",	std::ios::in |
										std::ios::binary);
	// Sprawdzenie, czy otwarcie pliku siê powiod³o
	if (file.is_open()) {
		fileError = false;		// je¿eli plik zosta³ otwarty, zmienna 
								// wskazuj¹ca na b³¹d przyjmuje wartoœæ false

		// Odczyt danych z pliku.
		// Parametry w pliku znajduj¹ siê w œciœle okreœlonych miejscach,
		// st¹d zachodzi tutaj potrzeba poruszania siê po pliku za pomoc¹
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
	// Zamkniêcie pliku binarnego
	file.close();
	return fileError;
}
/**
 *  @file    DPfiles.h
 *	@author	 Daniel Polak
 *
 *  @brief funkcje do odczytu i zapisu danych w plikach tekstowych
 *
 *  Plik zawiera funckje s³u¿¹ce do obs³ugi danych w zewnêtrznych
 *  plikach binarnych. Dane te zapisywane s¹ do i z wektorów o
 *  okreœlonych lub nieokreœlonych typach.
 *
 */
#pragma once
#include "stdafx.h"
#include "DPinit.h"

#include <vector>	// std::vector
#include <complex>	// std::complex
#include <fstream>


/**
 *   @brief  Zapisuje dane oddzielone spacjami z wektora do
 *			 pliku binarnego
 *			 
 *   @tparam Type okreœla typ zmiennych, które zawiera wektor
 *   @param  savedVector wskazuje wektor, który ma zostaæ zapisany
 *   @param  fileName wskazuje tablicê znaków, która okreœla nazwê
 *           pliku, do którego ma nast¹piæ zapiœ
 *   @return wartoœæ logiczna, okreœlaj¹ca czy podczas zapisu
 *           wyst¹pi³ b³¹d
 */
template<typename Type>
bool saveVectorToFile(const std::vector<Type>& savedVector, const char *fileName) {
	
	bool fileError = true;		///< zmienna okreœlaj¹ca czy zapis do pliku by³ mo¿liwy
	std::fstream file;			///< strumieñ wejœcia/wyjœcia do pliku

	// Otwarcie pliku binarnego w trybie zapisu
	file.open(fileName, std::ios::out |
						std::ios::binary);
	
	// Sprawdzenie, czy otwarcie pliku siê powiod³o
	if (file.is_open()) {
		fileError = false;	// je¿eli plik zosta³ otwarty, zmienna 
							// wskazuj¹ca na b³¹d przyjmuje wartoœæ false

		// Zapis wszystkich danych z wektora i oddzielenie ich spacj¹
		for (unsigned int i = 0; i < savedVector.size(); i++) {
			file << savedVector.at(i) << " ";
		}
	}
	// Zamkniêcie pliku binarnego
	file.close();
	return fileError;
}

bool loadFromFile(VectCxDbl& loadedVector, const char *fileName);
bool loadFromFile(VectDbl& loadedVector, const char *fileName);
bool saveParams(const VectDbl& params);
bool loadParams(VectDbl& params);
/**
 *  @file    DPfiles.h
 *	@author	 Daniel Polak
 *
 *  @brief funkcje do odczytu i zapisu danych w plikach tekstowych
 *
 *  Plik zawiera funckje s�u��ce do obs�ugi danych w zewn�trznych
 *  plikach binarnych. Dane te zapisywane s� do i z wektor�w o
 *  okre�lonych lub nieokre�lonych typach.
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
 *   @tparam Type okre�la typ zmiennych, kt�re zawiera wektor
 *   @param  savedVector wskazuje wektor, kt�ry ma zosta� zapisany
 *   @param  fileName wskazuje tablic� znak�w, kt�ra okre�la nazw�
 *           pliku, do kt�rego ma nast�pi� zapi�
 *   @return warto�� logiczna, okre�laj�ca czy podczas zapisu
 *           wyst�pi� b��d
 */
template<typename Type>
bool saveVectorToFile(const std::vector<Type>& savedVector, const char *fileName) {
	
	bool fileError = true;		///< zmienna okre�laj�ca czy zapis do pliku by� mo�liwy
	std::fstream file;			///< strumie� wej�cia/wyj�cia do pliku

	// Otwarcie pliku binarnego w trybie zapisu
	file.open(fileName, std::ios::out |
						std::ios::binary);
	
	// Sprawdzenie, czy otwarcie pliku si� powiod�o
	if (file.is_open()) {
		fileError = false;	// je�eli plik zosta� otwarty, zmienna 
							// wskazuj�ca na b��d przyjmuje warto�� false

		// Zapis wszystkich danych z wektora i oddzielenie ich spacj�
		for (unsigned int i = 0; i < savedVector.size(); i++) {
			file << savedVector.at(i) << " ";
		}
	}
	// Zamkni�cie pliku binarnego
	file.close();
	return fileError;
}

bool loadFromFile(VectCxDbl& loadedVector, const char *fileName);
bool loadFromFile(VectDbl& loadedVector, const char *fileName);
bool saveParams(const VectDbl& params);
bool loadParams(VectDbl& params);
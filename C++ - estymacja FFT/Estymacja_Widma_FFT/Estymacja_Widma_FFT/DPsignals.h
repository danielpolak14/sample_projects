/**
 *  @file    DPsignals.h
 *  @author	 Daniel Polak
 *
 *  @brief   funkcje do obs�ugi i generowania wektora z sygna�em
 *
 *  Plik zawiera szereg funkcji umo�liwaj�cych generowanie,
 *  modyfikowanie i zamienianie wektora zawieraj�cego sygna�
 *  w ci�gu liczb. Funkcje modyfikuj�ce sygna� dzia�aja jedynie
 *  dla wektora liczb typu std::complex<double>.
 */
#pragma once
#include "stdafx.h"
#include "DPinit.h"

#include <math.h>
#include <iostream>
#include <vector>

/**
 *  @brief  funkcja dopisuje zera na ko�cu wektora, tak aby
 *          jego d�ugo�� by�a r�wna zadanej warto�ci
 *
 *  @tparam Type okre�la typ zmiennych, kt�re zawiera wektor
 *  @param  vector wskazuje wektor, kt�ry ma zosta� rozszerzony
 *  @param  length zawiera informacj�, jak� wielko�� ma mie�
 *          wektor po rozszerzeniu
 *  @return void
 */
template<typename Type>
void extendVectorWithZeros(std::vector<Type>& vector, unsigned int length) {
	for (unsigned int i = vector.size(); i < length; i++)
		vector.push_back(0);
}

/**
 *  @brief  wy�wietla wektor na ekranie konsoli
 *
 *  @tparam Type okre�la typ zmiennych, kt�re zawiera wektor
 *  @param  vector wskazuje wektor, kt�ry ma zosta� rozszerzony
 *  @return void
 */
template<typename Type>
void printVector(std::vector<Type>& vector) {
	for (unsigned int i = 0; i < vector.size(); i++)
		std::cout << vector[i] << std::endl;
}

void changeSignalMenu(VectCxDbl& signal);
void generateSignal(VectCxDbl& signal, unsigned int numSamples, char signalType, VectDbl& params);
void changeParamsMenu(VectDbl& params);
unsigned int getNumSamples(bool powerOf2);
void vectorDoubleToFloat(const VectCxDbl vectorDouble, VectCxFlt& vectorFloat);
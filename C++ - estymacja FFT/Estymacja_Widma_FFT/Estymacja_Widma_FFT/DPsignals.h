/**
 *  @file    DPsignals.h
 *  @author	 Daniel Polak
 *
 *  @brief   funkcje do obs³ugi i generowania wektora z sygna³em
 *
 *  Plik zawiera szereg funkcji umo¿liwaj¹cych generowanie,
 *  modyfikowanie i zamienianie wektora zawieraj¹cego sygna³
 *  w ci¹gu liczb. Funkcje modyfikuj¹ce sygna³ dzia³aja jedynie
 *  dla wektora liczb typu std::complex<double>.
 */
#pragma once
#include "stdafx.h"
#include "DPinit.h"

#include <math.h>
#include <iostream>
#include <vector>

/**
 *  @brief  funkcja dopisuje zera na koñcu wektora, tak aby
 *          jego d³ugoœæ by³a równa zadanej wartoœci
 *
 *  @tparam Type okreœla typ zmiennych, które zawiera wektor
 *  @param  vector wskazuje wektor, który ma zostaæ rozszerzony
 *  @param  length zawiera informacjê, jak¹ wielkoœæ ma mieæ
 *          wektor po rozszerzeniu
 *  @return void
 */
template<typename Type>
void extendVectorWithZeros(std::vector<Type>& vector, unsigned int length) {
	for (unsigned int i = vector.size(); i < length; i++)
		vector.push_back(0);
}

/**
 *  @brief  wyœwietla wektor na ekranie konsoli
 *
 *  @tparam Type okreœla typ zmiennych, które zawiera wektor
 *  @param  vector wskazuje wektor, który ma zostaæ rozszerzony
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
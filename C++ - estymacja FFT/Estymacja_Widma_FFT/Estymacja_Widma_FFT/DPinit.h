/**
 *  @file    DPinit.h
 *  @author	Daniel Polak
 *
 *  @brief definicja makr i zmiennych globalnych u¿ywanych w programie
 *
 *
 */
#pragma once
#include "stdafx.h"

#include <vector>
#include <complex>

// Defnicja makr - u¿ywane w ca³ym programie w celu skrócenia zapisu
// Dla zmiennych typu double
typedef std::vector<double> VectDbl;
typedef std::complex<double> CxDbl;
typedef std::vector<std::complex<double>> VectCxDbl;
// Dla zmiennych typu float
typedef std::vector<float> VectFlt;
typedef std::complex<float> CxFlt;
typedef std::vector<std::complex<float>> VectCxFlt;

// Zmienne globalne
const double Pi = 3.14159265358979323846264338328;
const double Pi2 = 2 * Pi;
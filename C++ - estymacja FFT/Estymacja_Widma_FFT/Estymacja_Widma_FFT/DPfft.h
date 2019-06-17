/**
 *  @file    DPfft.h
 *  @author	 Daniel Polak
 *
 *  @brief   funkcje wykonuj¹ce fft i odwrotne fft
 *
 */
#pragma once
#include "stdafx.h"
#include "DPinit.h"

#include <math.h>
#include <vector> 
#include <complex>		// std::conj
#include <algorithm>	// std::for_each

/**
 *  @brief  oblicza widmo fft wskazanego wektora liczb zespolonych
 * 
 *  Funkcja zosta³a zaczerpniêta ze strony:
 *  https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
 *  Zosta³a ona jedynie dostosowana do tego, aby mo¿liwe by³o 
 *  u¿ywanie w niej wektorów
 *
 *  @tparam Type okreœla typ liczb zespolonych, które zawiera wektor
 *  @param  x wskazuje wektor, zawieraj¹cy wektor liczb zespolonych
 *          dla którego ma zostaæ wykonane widmo. Po wykonaniu funkcji
 *          zawiera widmo fft tego sygna³u
 *  @return void
 */
template<typename Type>
void fft(std::vector<std::complex<Type>>& x)
{
	unsigned int N = x.size(), k = N, n;
	Type thetaT = static_cast<Type>(Pi / N);
	std::complex<Type> phiT = std::complex<Type>(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				std::complex<Type> t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			std::complex<Type> t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
}

/**
 *  @brief  wykonuje operacje obliczenia widmo fft wskazanego
 *          wektora liczb zespolonych
 *
 *  Cia³o funkcji jest identyczne z cia³em funkcji fft(). Ró¿nica w
 *  ich dzia³aniu polega na tym, ¿e funkcja fftTime() nie modyfikuje
 *  wektora wejœciowego. Po jej wywo³aniu na stos przekazywana zostaje
 *  kopia tego wektora i na niej wykonywane s¹ operacje. Funkcja taka
 *  jest konieczna, poniewaz program podczas pomiaru czasu wielokrotnie
 *  wykonuje tê funkcjê. W zwi¹zku z tym, operowanie wielokrotnie na
 *  oryginalnym sygnale prowadzi³oby do b³êdów.
 *
 *  @tparam Type okreœla typ liczb zespolonych, które zawiera wektor
 *  @param  x wskazuje wektor, zawieraj¹cy wektor liczb zespolonych
 *          dla którego ma zostaæ wykonane widmo.
 *  @return void
 */
template<typename Type>
void fftTime(std::vector<std::complex<Type>> x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	Type thetaT = static_cast<Type>(Pi / N);
	std::complex<Type> phiT = std::complex<Type>(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				std::complex<Type> t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			std::complex<Type> t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}

/**
 *  @brief  wyznacza odwrotne fft wskazanego wektora liczb zespolonych
 *
 *  Funkcja zosta³a zaczerpniêta ze strony:
 *  https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
 *  Zosta³a ona jedynie dostosowana do tego, aby mo¿liwe by³o
 *  u¿ywanie w niej wektorów
 *  Funkcja nie jest wykorzystywana w pozosta³ych czêœciach programu.
 *
 *  @tparam Type okreœla typ liczb zespolonych, które zawiera wektor
 *  @param  x wskazuje wektor zawieraj¹cy widmo, dla którego ma zostaæ
 *          wykonane odwrotne fft. Po wykonaniu funkcji zawiera ci¹g 
 *          próbek pierwotnego sygna³u
 *  @return void
 */
template<typename Type>
void ifft(std::vector<Type>& x)
{
	// conjugate the complex numbers
	std::for_each(x.begin(), x.end(), std::conj<Type>);

	// forward fft
	fft(x);

	// conjugate the complex numbers again
	std::for_each(x.begin(), x.end(), std::conj<Type>);

	// scale the numbers
	for (unsigned int i = 0; i < x.size(); i++) {
		x[i] /= x.size();
	}
}
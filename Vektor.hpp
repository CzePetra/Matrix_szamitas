#pragma once
#include <iostream>
#include <cmath>
#include <fstream>





template <class T>
class Vektor {
private:
	T* numbers;
	int dim;
public:
	Vektor(int dim_szam = 1); // dim_szam dimenziós nullvektor
	Vektor(int dim_szam, const T tomb[]);

	Vektor(const Vektor& other);
	Vektor& operator=(const Vektor& other);

	~Vektor();

	//matekos mûveletek
	Vektor operator+(const Vektor& other) const;
	Vektor operator-(const Vektor& other) const;
	Vektor operator-() const; // (-1)-el való szorzás
	T operator*(const Vektor& other) const; //skalár szorzás
	Vektor operator*(T szorzo) const;
	template <class T>
	friend Vektor operator*(T szorzo, const Vektor&);
	Vektor& operator+=(const Vektor& other);
	Vektor& operator-=(const Vektor& other);
	T Hossz() const;
	Vektor Egyseg() const;

	T& operator[](unsigned int index);
	T operator[](unsigned int index) const;

	void Print() const;
	int getDim() const;
};

template <class T>
Vektor<T>::Vektor(int dim_szam)
{
	dim = dim_szam;
	try {
		numbers = new T[dim];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < dim; i++)
	{
		numbers[i] = 0;
	}

}

template <class T>
Vektor<T>::Vektor(int dim_szam, const T tomb[]) :dim(dim_szam)
{
	try {
		numbers = new T[dim];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < dim; i++)
	{
		numbers[i] = tomb[i];
	}
}

template <class T>
Vektor<T>::Vektor(const Vektor& other)
{
	dim = other.dim;
	try {
		numbers = new T[dim];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < dim; i++)
	{
		numbers[i] = other[i];
	}
}

template <class T>
Vektor<T>& Vektor<T>::operator=(const Vektor& other)
{
	if (this == &other)
		return *this;

	delete[] numbers;

	dim = other.dim;
	try {
		numbers = new T[dim];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < dim; i++)
	{
		numbers[i] = other[i];
	}
	return *this;
}

template <class T>
Vektor<T>::~Vektor()
{
	dim = 0;
	delete[] numbers;
}

template <class T>
Vektor<T> Vektor<T>::operator+(const Vektor& other) const
{
	if (dim != other.dim)
		throw std::length_error("Nem egyezik a ket vektor dimenzioja, igy nem vegezheto el a muvelet.");

	Vektor eredmeny(dim);
	for (int i = 0; i < dim; i++)
	{
		eredmeny[i] = (*this)[i] + other[i];
	}

	return eredmeny;
}

template <class T>
Vektor<T> Vektor<T>::operator-(const Vektor& other) const
{
	if (dim != other.dim)
		throw std::length_error("Nem egyezik a ket vektor dimenzioja, igy nem vegezheto el a muvelet.");

	return *this + (-other);
}

template <class T>
Vektor<T> Vektor<T>::operator-() const
{
	Vektor eredmeny(*this);
	for (int i = 0; i < dim; i++)
	{
		eredmeny[i] *= (-1);
	}
	return eredmeny;
}

template <class T>
T Vektor<T>::operator*(const Vektor& other) const //skalár szorzás
{
	if (dim != other.dim)
		throw std::length_error("Nem egyezik a ket vektor dimenzioja, igy nem vegezheto el a muvelet.");
	T sum = 0;

	for (int i = 0; i < dim; i++)
	{
		sum += (*this)[i] * other[i];
	}

	return sum;
}

template <class T>
Vektor<T> Vektor<T>::operator*(T szorzo) const
{
	Vektor eredmeny(dim);
	for (int i = 0; i < dim; i++)
	{
		eredmeny[i] = (*this)[i] * szorzo;
	}
	return eredmeny;
}

template <class T>
Vektor<T>& Vektor<T>::operator+=(const Vektor& other)
{
	*this = *this + other;
	return *this;
}

template <class T>
Vektor<T>& Vektor<T>::operator-=(const Vektor& other)
{
	*this = *this - other;
	return *this;
}

template <class T>
T Vektor<T>::Hossz() const
{
	T sum = 0;

	for (int i = 0; i < dim; i++)
	{
		sum += numbers[i] * numbers[i];
	}
	return sqrt(sum);
}

template <class T>
Vektor<T> Vektor<T>::Egyseg() const
{
	Vektor eredmeny;
	T hossz = Hossz();

	eredmeny = *this * (1 / hossz);
	return eredmeny;
}

template <class T>
T& Vektor<T>::operator[](unsigned int index)
{
	return numbers[index];
}

template <class T>
T Vektor<T>::operator[](unsigned int index) const
{
	return numbers[index];
}

template <class T>
void Vektor<T>::Print() const
{
	std::cout << "(";
	for (int i = 0; i < (dim - 1); i++)
	{
		std::cout << numbers[i] << ", ";
	}
	std::cout << numbers[dim - 1] << ")";
}

template <class T>
int Vektor<T>::getDim() const
{
	return dim;
}

template<class T>
inline Vektor<T> operator*(T szorzo, const Vektor<T>& ez)
{
	return ez * szorzo;
}

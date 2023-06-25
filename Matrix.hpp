#pragma once
#include <iostream>
#include <cmath>
#include <fstream>
#include "Vektor.hpp"





// mxn-es mátrixot reprezentál az osztály
template <class T>
class Matrix {
private:
	int m; // sorok száma ->vektor dim_szam
	int n; // oszlopok száma ->data tömb elemszáma
	Vektor<T>* data; // ez a "tömb" tárolja a mátrix oszlopait

	//ellenõrzõ függvények
	bool SameSizeWith(const Matrix<T>& other) const; //megnézi, hogy megeggyezik-e a méretük
	bool Szorozhato(const Matrix<T>& other) const; //megnézi, hogy elvégezhetõ-e a szorzás
	bool Negyzetes() const;

public:
	Matrix(int m = 1, int n = 1); // "nullmátrix"
	Matrix(int m, int n, const Vektor<T>* const data);
	Matrix(int m, int n, const T tomb[]);

	Matrix(const Matrix& other);
	Matrix& operator=(const Matrix& other);

	~Matrix();

	//matekos mûveletek
	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other) const;
	Matrix operator-() const; // (-1)-el való szorzás
	Matrix operator*(const Matrix<T>& other) const;
	Matrix operator*(T szam) const;
	Vektor<T> operator*(const Vektor<T>& v) const;
	template <class T>
	friend Matrix operator*(T n, const Matrix& that);
	Matrix Inverz() const;
	Matrix Tr() const;
	Matrix Adjungalt() const;
	Matrix Gauss()const;
	Matrix VezeregyesKialakitas(int i, int j, bool csere)const;
	unsigned int Rang() const;
	T Det() const;
	Matrix alMatrix(unsigned int sor, unsigned int oszlop) const;
	Vektor<T>& operator[](unsigned int n);
	Vektor<T> operator[](unsigned int n) const;

	void Print() const;
	void Save(std::ofstream& fout) const;
	void Load(std::ifstream& fin);
};
template <class T>
Matrix<T> egysegMatrix(unsigned int n); //létrehoz egy nxn-es egységmátrixot





//függvények definiálása:

template<class T>
inline bool Matrix<T>::SameSizeWith(const Matrix& other) const
{
	return (this->m == other.m && this->n == other.n);
}

template<class T>
inline bool Matrix<T>::Szorozhato(const Matrix& other) const
{
	return (this->n == other.m);
}

template<class T>
inline bool Matrix<T>::Negyzetes() const
{
	return (n == m);
}

template<class T>
inline Matrix<T>::Matrix(int m, int n) : m(m), n(n)
{
	try {
		data = new Vektor<T>[n];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < n; i++)
	{
		Vektor<T> v(m);
		data[i] = v;
	}
}

template<class T>
inline Matrix<T>::Matrix(int m, int n, const Vektor<T>* const data) : m(m), n(n)
{
	try {
		this->data = new Vektor<T>[n];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < n; i++)
	{
		this->data[i] = data[i];
	}

}



template<class T>
inline Matrix<T>::Matrix(const Matrix<T>& other) : m(other.m), n(other.n)
{
	try {
		data = new Vektor<T>[n];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}

	for (int i = 0; i < n; i++)
	{
		data[i] = other.data[i];
	}
}

template<class T>
inline Matrix<T>& Matrix<T>::operator=(const Matrix& other)
{
	if (this == &other)
		return *this;

	delete[] data;

	n = other.n;
	m = other.m;
	try {
		data = new Vektor<T>[n];
	}
	catch (const std::bad_alloc& hiba) {
		throw hiba;
	}

	for (int i = 0; i < n; i++)
	{
		data[i] = other.data[i];
	}

	return *this;
}

template<class T>
Matrix<T>::~Matrix()
{
	m = 0;
	n = 0;
	delete[] data;
}

template<class T>
inline Matrix<T> Matrix<T>::operator+(const Matrix& other) const
{
	if (!SameSizeWith(other))
		throw std::length_error("Nem egyeznek a matrixok meretei, igy nem vegezheto el a muvelet.");
	Matrix eredmeny(m, n);

	for (int i = 0; i < n; i++)
	{
		eredmeny[i] = data[i] + other[i];
	}
	return eredmeny;
}

template<class T>
inline Matrix<T> Matrix<T>::operator-(const Matrix& other) const
{
	if (!SameSizeWith(other))
		throw std::length_error("Nem egyeznek a matrixok meretei, igy nem vegezheto el a muvelet.");
	Matrix eredmeny(m, n);

	for (int i = 0; i < n; i++)
	{
		eredmeny[i] = data[i] - other[i];
	}
	return eredmeny;
}

template<class T>
inline Matrix<T> Matrix<T>::operator-() const
{
	Matrix eredmeny(*this);
	eredmeny = eredmeny * (-1);

	return eredmeny;
}

template<class T>
inline Matrix<T> Matrix<T>::operator*(const Matrix& other) const
{
	if (!Szorozhato(other))
		throw std::length_error("Nem egyeznek a matrixok meretei, igy nem vegezheto el a muvelet.");
	Matrix eredmeny(this->n, other.m);

	Matrix seged = this->Tr();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < other.n; j++)
		{
			eredmeny[i][j] = seged[i] * other[j];
		}
	}
	return eredmeny.Tr();
}

template<class T>
inline Matrix<T> Matrix<T>::operator*(T szam) const
{
	Matrix eredmeny(m, n);
	for (int i = 0; i < n; i++)
	{
		eredmeny[i] = data[i] * szam;
	}
	return eredmeny;
}

template<class T>
inline Vektor<T> Matrix<T>::operator*(const Vektor<T>& v) const
{
	if (n != v.getDim())
		throw std::length_error("Nem vegezheto el a muvelet, a matrix es a vektor nem osszeszorozhato.");
	Vektor<T> eredmeny(m);

	Matrix seged = this->Tr();
	for (int i = 0; i < m; i++)
	{
		eredmeny[i] = seged[i] * v;
	}

	return eredmeny;
}

template<class T> 
Matrix<T> Matrix<T>::Inverz() const
{
	if (!Negyzetes())
		throw std::length_error("Csak negyzetes matrix invertalhato!");
	if (Det() == 0)
		throw std::domain_error("Matrix determinansa nulla, igy nem vegezheto el az invertalas.");

	Matrix inverz(*this); //a függvény nem módosíthatja a tagváltozókat ezért kell lemásolni

	return inverz = (inverz.Adjungalt()) * (1.0 / (inverz.Det()));
}

template<class T>
inline Matrix<T> Matrix<T>::Tr() const
{
	T* tomb = new T[m * n];

	for (int i = 0; i < m; i++) //vektor eleme->sort nézünk (m db)
	{
		for (int j = 0; j < n; j++) //hanyadik vektor->oszlopot számolunk (n)
		{
			tomb[i * n + j] = data[j][i];
		}
	}
	Matrix<T> eredmeny(n, m, tomb);

	delete[] tomb;
	return eredmeny;
}

template<class T>
Matrix<T> Matrix<T>::Adjungalt() const
{
	if (!Negyzetes() || Det()==0)
		throw std::length_error("Csak invertalhato (nem nulla determinansu) matrix adjungalhato!");
	Matrix seged(n, n);

	if (n > 9)
		std::cout << "9x9-es matrixnal nagyobb matrix eseten sajnos a program futasa sok idot vesz igenybe.";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			T sign = ((i + j) % 2 == 0) ? 1 : (-1);
			seged[j][i] = sign * alMatrix(j, i).Det();
		}
	}
	return seged;
}

template<class T>
Matrix<T> Matrix<T>::Gauss() const
{
	if (m > 9 || n > 9)
		std::cout << "A folyamat tobb idot vehet igenybe, mert tul nagy a matrix, kerem legyen turelmes!" << std::endl;
	Matrix M = (*this);
	int i = 0; //számontartja, hogy melyik sor-oszlop helyen szeretnénk kialakítani a vezéregyest
	int j = 0;
	while (i < n && j < m) //ameddig nem éri el a mátrix szélét
	{
		if (M[j][i] == 0)
		{
			M = (M).VezeregyesKialakitas(i, j, true);
		}
		else
		{
			M = (M).VezeregyesKialakitas(i, j, false);
		}
		i++;
		j++;
	}
	return M;
}

template<class T>
Matrix<T> Matrix<T>::VezeregyesKialakitas(int O, int S, bool csere) const
{
	Matrix seged = (*this).Tr(); //vektor mûveletek elvégezhetõek legyenek S-sor, O-oszlop a transzponálás után
	T arany;
	int vezer = O; //aktuális vezéregyest tartalmazó oszlop

	if (csere) //megkeresi az adott sorhoz alkalmas vezéregyes jelöltet
	{
		int i = O;
		int nullas = 0;
		while (seged[i + nullas][S] == 0 && nullas + i < (n - 1))//egyrészt -1 mivel az indexelés 0-val kezdõdik
		{
			nullas++;
		}
		i += nullas;

		if (seged[i][S] == 0) return seged.Tr();//van benne egy telsejes nulla sor

		Vektor<T> cserebere = seged[i];
		seged[i] = seged[vezer];
		seged[vezer] = cserebere;
	}
	for (int i = (vezer + 1); i < data->getDim(); i++)
	{
		if (seged[i][S] != 0)
		{
			arany = seged[vezer][S] / seged[i][S];
			seged[i] = (seged[i] * arany) - seged[vezer];
		}
	}
	seged[vezer] = seged[vezer] * (1 / seged[vezer][S]);//kinullázódott a sor ->kialakult a vezéregyes
	return seged.Tr();
}

template<class T>
inline unsigned int Matrix<T>::Rang() const
{
	Matrix M = (*this).Gauss();
	int rang = 0;
	for (int i = 0; i < __min(n, m); i++)
	{
		if (M[i][i] != 0)
			rang++;
	}
	return rang;
}

template<class T>
T Matrix<T>::Det() const
{
	if (!Negyzetes())
		throw std::length_error("Csak negyzetes matrixnak van determinansa!");
	if (n == 1)
		return data[0][0];
	if (n == 2)
		return (data[0][0] * data[1][1] - data[1][0] * data[0][1]);
	T det = 0;
	for (int i = 0; i < n; i++)
	{
		Matrix seged = this->alMatrix(0, i);
		T sign = (i % 2 == 0) ? 1 : (-1);

		det += sign * (*this)[i][0] * seged.Det();
	}

	return det;
}

template<class T>
Matrix<T> Matrix<T>::alMatrix(unsigned int sor, unsigned int oszlop) const
{
	T* tomb = new T[(n - 1) * (n - 1)];
	unsigned int index = 0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i != sor && j != oszlop)
			{
				tomb[index] = (*this)[j][i];
				index++;
			}
		}
	}
	Matrix<T> eredmeny((n - 1), (n - 1), tomb);
	delete[] tomb;
	return eredmeny;
}

template<class T>
inline Vektor<T>& Matrix<T>::operator[](unsigned int n)
{
	return data[n];
}

template<class T>
inline Vektor<T> Matrix<T>::operator[](unsigned int n) const
{
	return data[n];
}

template<class T>
inline void Matrix<T>::Print() const
{
	std::cout << std::endl;

	for (int i = 0; i < m; i++) //sor
	{
		for (int j = 0; j < n; j++) //oszlop
		{
			std::cout << (*this)[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

template<class T>
inline void Matrix<T>::Save(std::ofstream& fout) const
{
	fout << this->m << " " << this->n << std::endl;


	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < m; i++)
		{
			fout << (*this)[j][i] << " ";
		}
	}
	fout << std::endl;
}

template<class T>
inline void Matrix<T>::Load(std::ifstream& fin)
{
	fin >> this->m >> this->n;
	try {
		T* tomb = new T[m * n];

		for (int i = 0; i < m * n; i++)
		{
			fin >> tomb[i];
		}
		*this = Matrix<T>(m, n, tomb);
		delete[] tomb;
	}

	catch (const std::bad_alloc& hiba) {
		throw;
	}
}

template<class T>
inline Matrix<T>::Matrix(int m, int n, const T tomb[]) : m(m), n(n)
{
	try {
		data = new Vektor<T>[n];
	}
	catch (const std::bad_alloc& hiba) {
		throw;
	}
	for (int i = 0; i < n; i++)
	{
		data[i] = Vektor<T>(m);
	}

	for (int i = 0; i < (m * n); i++)
	{
		data[i / m][i % m] = tomb[i];
	}
}





template<class T>
Matrix<T> operator*(T n, const Matrix<T>& that)
{
	return that * n;
}

template<class T>
Matrix<T> egysegMatrix(unsigned int n)
{
	Matrix<T> matrix(n, n);
	for (unsigned int i = 0; i < n; i++)
	{
		matrix[i][i] = 1;
	}
	return matrix;
}

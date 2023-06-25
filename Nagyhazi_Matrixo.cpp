#include <iostream>
#include <iomanip>
#include "Matrix.hpp"
#include "Vektor.hpp"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>



/*
A program els�sorban az�rt �r�dott, hogy programoz�k haszn�lj�k, mint be�p�tett k�nyvt�r.
Ez a f�program a helyes m�k�d�s tesztel�s�re szolg�l.
*/
int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    //M�trixokat egy f�jlb�l lehet kiolvasni, hogy ut�na sz�molni lehessen vel�k:

    std::cout << std::setprecision(2) << std::fixed; //a sz�mol�s pontoss�g�t teszi lehet�v�

    std::ifstream fin("in.txt"); //megnyitott teszt f�jl tartalmazza a tesztel�shez haszn�lt (4x4)-as k�t m�trixot, helyes form�ban
    Matrix<double> a, b, eredmeny;
    try {
        a.Load(fin);
        b.Load(fin);
    }
    catch (const std::bad_alloc& hiba)
    {
        std::cout << hiba.what() << std::endl;
    }
    double tomb[] = { 1,2,3,4 }; //a tesztel�s sor�n hazsn�lt vektor
    Vektor<double> v1(4, tomb), v2;

    std::cout << "Kiirja a betoltott matrixokat:\n";
    a.Print();
    b.Print();
    std::cout << std::endl;

    //m�veleti f�ggv�nyek kipr�b�l�sa:
    std::cout << "Kulonbozo muveletek kovetkeznek:\n";

    std::cout << "a es b matrix osszege:\n";
    try {
        (a + b).Print();
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "a es b matrix kulonbsege:\n";
    try {
        (eredmeny = a - b).Print();
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "eredmeny (-1)szerese:\n"; //eredmeny az a �s b m�trix k�l�nbs�ge
    (eredmeny = -eredmeny).Print();
    std::cout << std::endl;
    

    std::cout << "a es b matrix szorzata:\n";
    try {
        (eredmeny = a * b).Print();
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    std::cout << std::endl;

    //ide�g nincs sziv�rg�s
    std::cout << "matrix szorzasa egy szammal:\n";
    //az a �s b matrixot is beszorozza, egyiket jobbr�l m�sikat balr�l
    (eredmeny = b * 5).Print();
    //(eredmeny = 10 * a).Print(); //friend
    std::cout << std::endl;

    std::cout << "matrix es vektor szorzata, ahol v1=";
    v1.Print(); std::cout << "ha elvegezheto, akkor az eredmeny erteket v2 kapja" << std::endl;
    try {
        (v2 = a * v1).Print();
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }

    std::cout << std::endl;

    std::cout << "b matrix inverze, es ellenorzese ha van:\n";
    try {
        (eredmeny = b.Inverz()).Print();
        Matrix<double> Ellenorzo = eredmeny * b;
        Ellenorzo.Print(); //ha j�l sz�moltunk akkor az Ellenorzo egy nxn-es egys�gm�trix lesz
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    catch (const std::domain_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "a matrix es transzponaltja:\n";
    a.Print();
    (eredmeny = a.Tr()).Print();
    std::cout << std::endl;

    std::cout << "a matrix adjungaltja:\n";
    try {
        (eredmeny = eredmeny.Adjungalt()).Print();
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Gauss eliminacio elvegzese a b matrixon:\n";
    (eredmeny = b.Gauss()).Print();
    std::cout << std::endl;

    std::cout << "b matrix rangja:\n";
    unsigned int d = b.Rang();
    std::cout << d << std::endl;
    std::cout << std::endl;

    std::cout << "kiszamolja a harom matrix determinansat:\n";
    try {
        double q1 = a.Det(), q2 = b.Det(), q3 = eredmeny.Det();
        std::cout << q1 << " " << q2 << " " << q3 << std::endl;
    }
    catch (const std::length_error& rossz)
    {
        std::cout << rossz.what() << std::endl;
    }
    
    //matrixok f�jlba mentese:
    std::ofstream fout("out.txt");
    a.Save(fout);
    b.Save(fout);
    eredmeny.Save(fout);
    std::cout << "a, b es eredmeny matrixokat elmentette egy fajlba\n" << std::endl;

    //egys�g m�trix l�trehoz�sa
    unsigned int n;
    std::cout << "Kerem adja meg mekkora egysegmatrixot szeretne\n";
    std::cin >> n;
    egysegMatrix<double>(n).Print();
 
    std::cout << "Teszt program vege :)";
    return 0;
}



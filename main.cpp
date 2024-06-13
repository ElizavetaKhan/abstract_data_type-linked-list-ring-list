#include <iostream>
#include <fstream>
#include <cstdlib>

#include "list_set.h"
#include "ringlist_set.h"
#include "BitFields_set.h"

namespace PACKAGE
{
    //static list_set SET;
    //static list_set SET2;

    //static BitFields_set SET(-99, 144); // -99 – 144 // 85 273
    //static BitFields_set SET2(0, 187);

    static ringlist_set SET;
    static ringlist_set SET2;
}

// -99, -96, -32, -1, 0, 2, 3, 5, 7, 89, 92, 111, 144
// 7, 40, 42, 49, 58, 72, 73, 78, 92, 103, 109, 123, 127, 129, 130, 144, 165, 187

using namespace PACKAGE;

void fillFromFile(std::ifstream& in)
{
    int num;

    while (in >> num) {
        SET.insert(num);
    }
}

// рандомными числами заполняем второе множество
void randomFill(std::ifstream& in) {
    int num;

    while (in >> num) {
        SET2.insert(num);
    }

    /*for (int i = 0; i < 18; ++i) {
        SET2.insert(rand() % 200);
    }*/
}

int main()
{
    std::ifstream in("input.txt");

    fillFromFile(in);
    std::cout << std::endl << "A:";
    SET.print();

    std::ifstream in2("input2.txt");
    randomFill(in2);
    std::cout << std::endl << "В:";
    SET2.print();

    std::cout << std::endl << "A + B:";
    (SET + SET2).print();

    std::cout << std::endl << "A ^ B:";
    (SET ^ SET2).print();

    std::cout << std::endl << "A / B:";
    (SET / SET2).print();

    //std::cout << std::endl << SET.member(-90) << ' ' << SET2.member(50);
}

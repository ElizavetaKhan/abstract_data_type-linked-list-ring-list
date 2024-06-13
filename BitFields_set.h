#ifndef INC_5_LAB_BITFIELDS_SET_H
#define INC_5_LAB_BITFIELDS_SET_H
#include <iostream>
#include <cmath>

struct resultIsUndefineD : public std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
        std::cout << std::endl << std::endl;
        return "!! невыполнимое действие над множествами – множества пересекаются !!";
    }
};

class BitFields_set {
private:
    unsigned int *array{};
    int amountOfBitFields{};
    int start{};
    int end{};
    bool isDifferentSign{};

private:
    // объединяет множества при этом first - множество, которое начнется раньше second
    void combiner(const BitFields_set& first, const BitFields_set& second);

    // поиск элемента массива, в котором находится число Х
    // поиск разряда числа в битовом поле - читаем число справа налево (в digit заносит)
    // возвращает -1, если число вне границ start end
    int searchBitField_and_Digit(int x, int& digit) const;

    // возвращает номер битового поля, в минимальном бите которого 0
    // если вернулось число больше, чем элементов в множестве – отрицательные числа только
    // если число отрицательное - число целых полей до 0
    int whereZero() const;

    void copy(BitFields_set& copy);

    // какое десятичное число находится в элементе массива arrayElement и в разряде digit
    int whatsNumber(int arrayElement, int digit) const;

    // деструктор
    void destructor();

public:
    // конструктор по умолчанию (множество задаем)
    BitFields_set(int start, int end);

    // копирующий конструктор
    BitFields_set(BitFields_set& B);

    ~BitFields_set();

    // UNION новое множество возвращает и записывается в С (проверка на наличие множеств) – отправка в UniMerge
    BitFields_set& operator+(BitFields_set& B);

    // INTERSECTION пересечение множеств this и В (проверка на наличие множеств) – отправка в crosser
    BitFields_set& operator^(const BitFields_set& B);

    // DIFFERENCE разность множеств this и В
    BitFields_set& operator/(const BitFields_set& B);

    // слияние множеств А и В, результат в С. На непересекающихся множествах!!!!
    BitFields_set& merge(BitFields_set& B);

    // true - х принадлежит А, false - не принадлежит
    bool member(int x);

    // вставить в множество значение
    void insert(int v);

    // из множества А удаляется значение х, если оно там есть, иначе ничего не делаем
    void deleTe(int x);

    // ASSIGN присваивание А мн-ва В. В не меняется
    BitFields_set& operator= (const BitFields_set B);

    // минимальный элемент списка - tail->next
    int min();

    // последний элемент списка - tail
    int max() const;

    //  EQUAL true, если множества A и B эквивалентны. false, если множества A и B  не эквивалентны.
    bool operator==(const BitFields_set& B);

    // возвращает то множество, в котором обнаружен объект х. Если не найден, то возвращаем пустое множество - NULLPTR (ни А, ни В).
    BitFields_set& find(BitFields_set& B, int x);

    // доп. метод - проверяет пересекаются множества или нет.
    bool isIntersect(const BitFields_set& A, const BitFields_set& B);

    // делаем объект - множество пустым
    void makeNull();

    void print();
};


#endif //INC_5_LAB_BITFIELDS_SET_H

#include "BitFields_set.h"

BitFields_set::BitFields_set(int start, int end)
    : start {start}
    , end {end}
{
    isDifferentSign = start < 0 && end > 0;

    if (isDifferentSign) // если разных знаков
    {
        int a = abs(start);
        int b = abs(end) + 1;

        a = (a % 32 == 0) ? (a / 32) : (a / 32 + 1);
        b = (b+1) % 32 == 0 ? ((b+1) / 32) : ((b+1) / 32 + 1);

        amountOfBitFields = a+b;
    } else if (start >= 0) {
        amountOfBitFields = abs(end) + 1;
        amountOfBitFields = ((amountOfBitFields + 1) % 32 == 0) ? ((amountOfBitFields+1) / 32 - 1) : ((amountOfBitFields+1) / 32);

        // вычитаем то, что не будет использоваться (до 0 элементы)
        amountOfBitFields -= (start+1) % 32 == 0 ? (start+1) / 32 - 2 : (start+1) / 32 - 1;

    } else {
        amountOfBitFields = abs(start);
        amountOfBitFields = (amountOfBitFields % 32 == 0) ? (amountOfBitFields / 32 - 1) : (amountOfBitFields / 32);

        // вычитаем то, что не будет использоваться (до 0 элементы)
        amountOfBitFields -= abs(end) % 32 == 0 ? (abs(end) / 32) - 2 : (abs(end) / 32) - 1;
    }

    array = new unsigned int[amountOfBitFields];
}

BitFields_set::BitFields_set(BitFields_set& B)
{
    if (this == &B)
        return;

    this->copy(B);
}

void BitFields_set::copy(BitFields_set& copy)
{
    this->array = new unsigned int[copy.amountOfBitFields];

    for (int i = 0; i < copy.amountOfBitFields; ++i)
        this->array[i] = copy.array[i];

    this->start = copy.start;
    this->end = copy.end;
    this->amountOfBitFields = copy.amountOfBitFields;
    this->isDifferentSign = copy.isDifferentSign;
}

BitFields_set::~BitFields_set()
{
    this->destructor();
}

void BitFields_set::destructor()
{
    delete[] array;
}

BitFields_set& BitFields_set::operator+(BitFields_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    // проверка на наличие множеств
    if (this->array && !B.array)
        return *this;
    if (!this->array && B.array)
        return B;
    if (!this->array && !B.array)
    {
        BitFields_set* nuLL;
        nuLL = new BitFields_set(0,0);
        return *nuLL;
    }

    BitFields_set* C;
    // мин - B, макс - В
    if (this->start >= B.start && this->end <= B.end)
        C = new BitFields_set(B.start,B.end);
        // мин - B, макс - this
    else if (this->start >= B.start && this->end >= B.end)
        C = new BitFields_set(B.start,this->end);
        // мин - this, макс - this
    else if (this->start <= B.start && this->end >= B.end)
        C = new BitFields_set(this->start,this->end);
        // мин - this, макс - B
    else
        C = new BitFields_set(this->start,B.end);

    // если this начнется первее
    if (this->start <= B.start)
        C->combiner(*this,B);

        // иначе если В начнется первее this
    else
        C->combiner(B, *this);

    return *C;
}

void BitFields_set::combiner(const BitFields_set& first, const BitFields_set& second)
{
    // в каком элементе массива находится 0 (или находился БЫ)
    // если число > amountOfBitFields – то множество отрицательных чисел без нуля
    // если число < 0 – множество положительных чисел без нуля
    int zeroA = first.whereZero();
    int zeroB = second.whereZero();

    // |___this___|___0___|___B___| – 1 случай
    // 0___|___this___|____|___B___| – 2 случай
    // |___this___|____|___B___|___0 – 3 случай
    int j = 0;
    for (int i = 0; i < amountOfBitFields; ++i)
    {
        // рассматриваем с 1 по 3 случаи
        bool case1 =  isDifferentSign && (i >= zeroA + abs(zeroB));
        bool case2 = !isDifferentSign && start < 0 && (i >= zeroA - zeroB);
        bool case3 = !isDifferentSign && start >= 0 && i >= abs(zeroB);

        if (case1 || case2 || case3)
        {
            // если еще не кончилось множество first
            if (i < first.amountOfBitFields)
                array[i] = first.array[i] | second.array[j];

                // если оно кончилось, то присваиваем только элементы second
            else
                array[i] = second.array[j];
            ++j;

            // если пока не дошли до множества second и не кончилось множество first
        } else if (i < first.amountOfBitFields)
            array[i] = first.array[i];
    }
}

BitFields_set& BitFields_set::operator^(const BitFields_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    // проверка на наличие множеств и пересекаются ли вообще
    if (!this->array || !B.array || this->start > B.end || B.start > this->end)
    {
        BitFields_set* nuLL;
        nuLL = new BitFields_set(0,0);
        return *nuLL;
    }

    BitFields_set* C;
    // берем минимум из максимума и максимум из минимума
    // мин - B, макс - В
    if (this->start >= B.start && this->end <= B.end)
        C = new BitFields_set(this->start,this->end);
        // мин - B, макс - this
    else if (this->start >= B.start && this->end >= B.end)
        C = new BitFields_set(this->start,B.end);
        // мин - this, макс - this
    else if (this->start <= B.start && this->end >= B.end)
        C = new BitFields_set(B.start,B.end);
        // мин - this, макс - B
    else
        C = new BitFields_set(B.start,this->end);

    int beforeInThis = 0, beforeInB = 0;
    int digit;

    // если this начнется первее
    if (start <= B.start)
        // то, что "отрезали" в this
        beforeInThis = this->searchBitField_and_Digit(B.start, digit);

        // иначе если В начнется первее this
    else
        // то, что "отрезали" в В
        beforeInB = B.searchBitField_and_Digit(start, digit);

    for (int i = 0; i < C->amountOfBitFields; ++i)
        C->array[i] = this->array[beforeInThis + i] & B.array[beforeInB + i];

    return *C;
}

BitFields_set& BitFields_set::operator/(const BitFields_set &B)
{
    // если это одно и то же множество
    if (this == &B)
    {
        BitFields_set* nuLL;
        nuLL = new BitFields_set(0,0);
        return *nuLL;
    }

    // копирующий конструктор
    BitFields_set* C;
    C = new BitFields_set(*this);

    // проверка на наличие и пересечение множеств
    if (!this->array || !B.array || this->start > B.end || B.start > this->end)
        return *C;

    int digit;

    // с какого элемента массива С начинается В (если start у В больше С)
    int startB = searchBitField_and_Digit(B.start, digit);

    // с какого элемента массива В начинается С (если start у С больше В)
    int startC = B.searchBitField_and_Digit(C->start, digit);
    int j = (startC != -1) ? startC : 0;
    int i = (startB != -1) ? startB : 0;

    while (i < amountOfBitFields && j < B.amountOfBitFields)
    {
        C->array[i] = array[i] & ~(array[i] & B.array[j]);
        ++i;
        ++j;
    }
    return *C;
}

BitFields_set& BitFields_set::merge(BitFields_set &B)
{
    // если пересекаются - выбрасываем исключение
    if (this->isIntersect(*this,B))
        throw resultIsUndefineD();

    // проверка на наличие множеств
    if (this->array && !B.array)
        return *this;
    if (!this->array && B.array)
        return B;
    if (!this->array && !B.array)
    {
        BitFields_set* nuLL;
        nuLL = new BitFields_set(0,0);
        return *nuLL;
    }

    BitFields_set* C;
    // мин - B, макс - В
    if (this->start >= B.start && this->end <= B.end)
        C = new BitFields_set(B.start,B.end);
        // мин - B, макс - this
    else if (this->start >= B.start && this->end >= B.end)
        C = new BitFields_set(B.start,this->end);
        // мин - this, макс - this
    else if (this->start <= B.start && this->end >= B.end)
        C = new BitFields_set(this->start,this->end);
        // мин - this, макс - B
    else
        C = new BitFields_set(this->start,B.end);

    // если this начнется первее
    if (this->start <= B.start)
        C->combiner(*this,B);

        // иначе если В начнется первее this
    else
        C->combiner(B, *this);

    return *C;
}

bool BitFields_set::member(int x)
{
    if (!array)
        return false;

    int digit;
    unsigned int temp = 1 << 31;
    // вернется номер элемента массива и разряд, в котором должна быть 1
    int num = searchBitField_and_Digit(x,digit);
    if (num != 1 && ((temp >> digit | array[num]) == array[num]))
        return true;

    return false;
}

void BitFields_set::insert(int v)
{
    int digit;
    int numOfArrayElem = searchBitField_and_Digit(v,digit);

    if (numOfArrayElem != -1)
    {
        unsigned int temp = 1 << 31;
        temp >>= digit;
        array[numOfArrayElem] |= temp;
    }
}

void BitFields_set::deleTe(int x)
{
    if (!this->array)
        return;

    // получаем номер элемента массива, где должен находиться х и его разряд
    int digit;
    int arrayElement = this->searchBitField_and_Digit(x,digit);

    if (arrayElement != -1)
    {
        unsigned int temp = 1 << 31;
        temp >>= digit;
        array[arrayElement] = array[arrayElement] & ~temp;
    }
}

BitFields_set &BitFields_set::operator=(BitFields_set B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    this->destructor();
    this->copy(B);

    return *this;
}

int BitFields_set::min()
{
    // если массив не заполнен – возвращаем минимум для int
    if (!this->array)
        return -2147483647;

    unsigned int temp = 1 << 31;
    for (int i = 0; i < amountOfBitFields; ++i)
        for (int j = 0; j < 32; ++j)
            // если бит 1 и элемент массива не равен 0
            if (array[i] != 0 && (temp >> j | array[i]) == array[i])
                return whatsNumber(i,j);
    return -2147483647;
}

int BitFields_set::max() const
{
    // если массив не заполнен – возвращаем максимум для int
    if (!this->array)
        return 2147483647;

    for (int i = amountOfBitFields - 1; i >= 0; --i)
        for (int j = 31; j >= 0; --j)
            // если бит 1 и элемент массива не равен 0
            if (array[i] != 0 && (1 << j | array[i]) == array[i])
                return whatsNumber(i,j+1);

    return 2147483647;
}

bool BitFields_set::operator==(const BitFields_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return true;

    // проверка на наличие множеств и пересекаются ли вообще
    if (!this->array || !B.array || this->start > B.end || B.start > this->end)
        return false;

    // |___this___||_|_____B___||  – 1 случай – this закончится раньше
    // |___this___||___B___||___|  – 2 случай – В - подмножество this

    // запоминаем элемент массива this, с которого начинается В
    int digit;
    int whereStartB = start <= B.start ? searchBitField_and_Digit(B.start, digit) : searchBitField_and_Digit(start, digit);

    // идем до конца множества, у которого end больше
    for (int i = 0; i < (end >= B.end ? amountOfBitFields : whereStartB + B.amountOfBitFields); ++i) {
        // проверяем элементы от 0 до whereStartB – чтобы они были равны 0
        if (i < whereStartB) {
            if (array[i] != 0)
                return false;
            continue;
        }

        // проверяем элементы от конца первого закончившегося до конца – чтобы они были равны 0
        if (end >= B.end && (i >= whereStartB + B.amountOfBitFields)) {
            if (array[i] != 0)
                return false;
            continue;
        } else if (end < B.end && i >= amountOfBitFields) {
            if (B.array[i-whereStartB] != 0)
                return false;
            continue;
        }

        // проверяем на равенство совпавший отрезок
        if (array[i] != B.array[i - whereStartB])
            return false;
    }

    return true;
}

BitFields_set& BitFields_set::find(BitFields_set &B, int x)
{
     // получаем номер элемента массива, где должен находиться х и его разряд
     int digit;
     int num = this->searchBitField_and_Digit(x,digit);
     unsigned int temp = 1 << 31;

     // прибавляем единицу в нужный бит - число изменилось - был 0, иначе 1
     if (num != -1 && ((temp >> digit || array[num]) == array[num]))
         return *this;

     num = B.searchBitField_and_Digit(x,digit);
     if (num != -1 && ((temp >> digit || B.array[num]) == B.array[num]))
         return B;
}

int BitFields_set::searchBitField_and_Digit(int x, int& digit) const
{
    int arrayElement;
    int beforeZero = (abs(start) % 32 == 0) ? (abs(start) / 32) : (abs(start) / 32 + 1) - 1;

    // если есть вообще смысл искать
    if (x <= this->end && x >= this->start)
    {
        // если множество из отрицательных чисел или лежит в подмножестве < 0
        if (this->isDifferentSign && x < 0 || x < 0) {
            arrayElement = (abs(x) % 32 == 0) ? beforeZero - ((abs(x) - 1) / 32) : beforeZero - (abs(x) / 32);
            digit = abs(x) % 32 == 0 ? 0 : 32 - (abs(x) % 32);
            return arrayElement;

            // если лежит в подмножестве >= 0 или только положительные числа
        } else {
            arrayElement = (x+1) % 32 == 0 ? ((x+1) / 32) : ((x+1) / 32 + 1);
            digit = (x+1) % 32 - 1;

            if (isDifferentSign)
                return arrayElement + beforeZero;
            else {
                // вычитаем неиспользованные биты до 0, если start у нас начинается не с 0
                arrayElement -= (start+1) % 32 == 0 ? (start+1) / 32 - 1 : (start+1) / 32;
                return arrayElement - 1;
            }
        }
    }

    return -1;
}

void BitFields_set::makeNull()
{
    this->destructor();
}

void BitFields_set::print()
{
    if (!this->array)
        return;

    std::cout << std::endl << "{";
    unsigned int temp = 1 << 31;

    for (int i = 0; i < amountOfBitFields; ++i)
        for (int j = 0; j < 32; ++j)
            if ((temp >> j | array[i]) == array[i])
                if (whatsNumber(i,j) == max())
                    std::cout << whatsNumber(i,j);
                else
                    std::cout << whatsNumber(i,j) << ", ";

    std::cout << "}" << std::endl;
}

bool BitFields_set::isIntersect(const BitFields_set& A, const BitFields_set& B)
{
    // если это одно и то же множество
    if (&A == &B)
        return true;

    // если какого-то из множеств нет
    if (!A.array || !B.array)
        return false;

    int digit;
    int i = 0, j = 0;
    // с какого элемента начинаем
    A.start <= B.start ? i = searchBitField_and_Digit(B.start, digit) : j = B.searchBitField_and_Digit(A.start, digit);

    while (i < A.amountOfBitFields && j < B.amountOfBitFields) {
        if ((A.array[i] & B.array[j]) != 0)
            return true;
        ++i;
        ++j;
    }

    return false;
}

int BitFields_set::whereZero() const
{
    // разные знаки
    if (this->isDifferentSign) {
        int beforeZero = (abs(start) % 32 == 0) ? (abs(start) / 32) : (abs(start) / 32 + 1) - 1;
        return beforeZero + 1;

    // только отрицательные
    } else if (start < 0) {
        return abs(end) % 32 == 0 ? this->amountOfBitFields + (abs(end) / 32 - 1) : this->amountOfBitFields + (abs(end) / 32);

    // положительные и 0
    } else
        return (start+1) % 32 == 0 ? -((start+1) / 32 - 1) : -((start+1) / 32);
}

int BitFields_set::whatsNumber(int arrayElement, int digit) const
{
    int from;
    if (abs(start) % 32 == 0)
        from = start;
    else
        from = (start >= 0) ? (start / 32) * 32 : ((start / 32) - 1) * 32;

    return from + (arrayElement * 32) + digit;
}

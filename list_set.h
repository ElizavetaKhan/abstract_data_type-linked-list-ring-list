#ifndef INC_5_LAB_LIST_SET_H
#define INC_5_LAB_LIST_SET_H
#include "item.h"
#include <iostream>

struct resultIsUndefined : public std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
        std::cout << std::endl << std::endl;
        return "!! невыполнимое действие над множествами !!";
    }
};

class list_set {
private:
    Item* head;

private:
    // объединяет множества
    static Item* combiner(Item* A, Item* B);

    // объединение непересекающихся множеств
    static Item* merger(Item* A, Item* B);

    // перечение множеств – создает множество, возвращает заполненным
    static Item* crosser(Item* A, Item* B);

    // разность
    static Item* subtractor(Item* A, Item* B);

    // равны ли множества
    static bool equals(Item* A, Item* B);

    // ищем х от позиции pos, возвращаем всегда предыдущий
    Item* findX(int x, Item* pos);

    void destructor();

public:
    // конструктор по умолчанию (пустое множество)
    list_set();

    // копирующий конструктор либо всего списка, либо его части(присоединение к хвосту)
    // присваивает голову скопируемой части
    explicit list_set(Item* copy);

    ~list_set();

    // UNION новое множество возвращает и записывается в С
    list_set& operator+(list_set& B);

    // INTERSECTION пересечение множеств this и В
    list_set& operator^(list_set& B);

    // DIFFERENCE разность множеств this и В
    list_set& operator/(list_set& B);

    // слияние множеств А и В, результат в С. На непересекающихся множествах!!!!
    list_set& merge(list_set& B);

    // true - х принадлежит А, false - не принадлежит
    bool member(int x);

    // вставить в множество значение
    void insert(int v);

    // из множества А удаляется значение х, если оно там есть, иначе ничего не делаем
    void deleTe(int x);

    // ASSIGN присваивание А мн-ва В. В не меняется
    list_set& operator= (list_set B);

    // минимальный элемент списка - head
    int min();

    // последний элемент списка
    int max();

    //  EQUAL true, если множества A и B эквивалентны. false, если множества A и B  не эквивалентны.
    bool operator==(list_set& B);

    // возвращает то множество, в котором обнаружен объект х. Если не найден, то возвращаем пустое множество - NULLPTR (ни А, ни В).
    list_set& find(list_set& B, int x);

    // делаем объект - множество пустым
    void makeNull();

    // проверяет пересекаются множества или нет.
    bool isIntersect(const list_set& B);

    void print();
};


#endif //INC_5_LAB_LIST_SET_H

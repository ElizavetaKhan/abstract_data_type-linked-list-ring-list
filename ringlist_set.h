#ifndef INC_5_LAB_RINGLIST_SET_H
#define INC_5_LAB_RINGLIST_SET_H
#include "item2.h"
#include <iostream>

struct ResultIsUndefined : public std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
        std::cout << std::endl << std::endl;
        return "!! невыполнимое действие над множествами !!";
    }
};

class ringlist_set {
private:
    item* tail;

private:
    // объединяет множества
    static item* combiner(item* A, item* B);

    // объединение непересекающихся множеств
    static item* merger(item* A, item* B);

    // перечение множеств
    static item* crosser(item* A, item* B);

    // разность
    static item* subtractor(item* A, item* B);

    static bool equals(item* A, item* B);

    // ищем х от позиции pos, возвращаем всегда предыдущий
    item* findX(int x, item* pos);

    void destructor();

public:
    // конструктор по умолчанию (пустое множество)
    ringlist_set();

    // на выход приходит голова сразу, от которой начнется копирование
    explicit ringlist_set(item* copy);

    ~ringlist_set();

    // UNION новое множество возвращает и записывается в С
    ringlist_set& operator+(ringlist_set& B);

    // INTERSECTION пересечение множеств this и В
    ringlist_set& operator^(ringlist_set& B);

    // DIFFERENCE разность множеств this и В
    ringlist_set& operator/(ringlist_set& B);

    // слияние множеств А и В, результат в С. На непересекающихся множествах!!!!
    ringlist_set& merge(ringlist_set& B);

    // true - х принадлежит А, false - не принадлежит
    bool member(int x);

    // вставить в множество значение
    void insert(int x);

    // из множества А удаляется значение х, если оно там есть, иначе ничего не делаем
    void deleTe(int x);

    // ASSIGN присваивание А мн-ва В. В не меняется
    ringlist_set& operator= (ringlist_set B);

    // минимальный элемент списка - tail->next
    int min();

    // последний элемент списка - tail
    int max();

    //  EQUAL true, если множества A и B эквивалентны. false, если множества A и B  не эквивалентны.
    bool operator==(ringlist_set& B);

    // возвращает то множество, в котором обнаружен объект х. Если не найден, то возвращаем пустое множество - NULLPTR (ни А, ни В).
    ringlist_set& find(ringlist_set& B, int x);

    // доп. метод - проверяет пересекаются множества или нет.
    bool isIntersect(ringlist_set B);

    // делаем объект - множество пустым
    void makeNull();

    void print();
};


#endif //INC_5_LAB_RINGLIST_SET_H

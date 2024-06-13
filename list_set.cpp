#include "list_set.h"

list_set::list_set()
{
    head = nullptr;
}

list_set::list_set(Item* copy)
{
    Item* heaD = nullptr;
    Item* prev;
    for (Item* i = copy; i != nullptr; i = i->next) {
        Item* neW = new Item(i->val);

        if (!heaD)
            heaD = neW;
        else {
            prev->next = neW;
        }
        prev = neW;
    }

    this->head = heaD;
}

list_set::~list_set()
{
    this->destructor();
}

list_set& list_set::operator+(list_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    // если пустое множество this или В
    if (!this->head && B.head)
        return B;
    if (!B.head && this->head)
        return *this;

    // возвращаем пустое множество если оба пустые
    if (!this->head && B.head) {
        list_set* nuLL;
        nuLL = new list_set();
        return *nuLL;
    }

    list_set* C;
    C = new list_set();
    C->head = combiner(this->head, B.head);

    return *C;
}

Item* list_set::combiner(Item* A, Item* B)
{
    Item* cur; // храним текущий элемент
    Item* heaD;
    Item* i = A; Item* j = B;

    // заполняем первый элемент
    if (i->val <= j->val) {
        heaD = new Item(i->val);
        cur = heaD;
        i = i->next;
    } else {
        heaD = new Item(j->val);
        cur = heaD;
        j = j->next;
    }

    while (j && i)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем
             cur->next = new Item(i->val);
             cur = cur->next;

             j = j->next;
             i = i->next;
             continue;
        }

        if (i->val > j->val) { // добавляем после cur
             cur->next = new Item(j->val);
             cur = cur->next;

             j = j->next;
             continue;
        }

        if (i->val < j->val) {
            cur->next = new Item(i->val);
            cur = cur->next;
            i = i->next;
            continue;
        }
    }

    // если закончилось одно из множеств – добавляем элементы другого до конца в С
    if (!i) {
        list_set* temp = new list_set(j);
        cur->next = temp->head;
    } else {
        list_set* temp = new list_set(i);
        cur->next = temp->head;
    }
    return heaD;
}

list_set& list_set::operator^(list_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    // проверка на наличие множеств
    if (!this->head || !B.head) {
        list_set* nuLL;
        nuLL = new list_set();
        return *nuLL;
    }

    list_set* C;
    C = new list_set();
    C->head = crosser(this->head, B.head);

    return *C;
}

Item* list_set::crosser(Item* A, Item* B)
{
    Item* cur; // храним текущий элемент
    Item* heaD = nullptr;
    Item* i = A; Item* j = B;

    // заполняем первый элемент
    while (!heaD) {
        if (i->val == j->val) { // при совпадении itemов - добавляем
            heaD = new Item(i->val);
            cur = heaD;

            j = j->next;
            i = i->next;
            continue;
        }

        if (i->val < j->val) { // если в списке A элемент меньше
            i = i->next;
            continue;
        }

        if (i->val > j->val) // идем дальше по списку В
            j = j->next;
    }

    // если конец любого из списков
    while (j && i)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем
             cur->next = new Item(i->val);
             cur = cur->next;

             j = j->next;
             i = i->next;
             continue;
        }

        if (i->val < j->val) { // если в списке A элемент меньше
            i = i->next;
            continue;
        }

        if (i->val > j->val) // идем дальше по списку В
            j = j->next;
    }
    return heaD;
}

list_set& list_set::operator/(list_set &B)
{
    // проверка на наличие множеств
    if (!this->head || this == &B) {
        list_set* nuLL;
        nuLL = new list_set();
        return *nuLL;
    }
    if (!B.head)
        return *this;

    list_set* C;
    C = new list_set(this->head);
    C->head = subtractor(C->head,B.head);

    return *C;
}

Item* list_set::subtractor(Item* A, Item* B)
{
    Item* prev; // храним предыдущий элемент
    Item* i = A; Item* j = B;

    while (j && i)
    {
        if (i->val == j->val) { // при совпадении itemов - удаляем
            j = j->next;
            if (i == A) // если голову надо удалить
            {
                A = A->next;
                delete i;
                i = A;
                continue;
            }
            prev->next = i->next;
            delete i;
            i = prev->next;
            continue;
        }

        if (i->val < j->val) { // если в текущем списке элемент меньше - делаем next
            prev = i;
            i = i->next;
            continue;
        }

        if (i->val > j->val) { // идем дальше по списку В
            j = j->next;
        }
    }
    return A;
}

list_set& list_set::merge(list_set &B)
{
    // если пустое множество this или В
    if (!this->head && B.head)
        return B;
    if (!B.head && this->head)
        return *this;

    // возвращаем пустое множество если оба пустые
    if (!this->head && B.head) {
        list_set* nuLL;
        nuLL = new list_set();
        return *nuLL;
    }

    list_set* C;
    C = new list_set();
    C->head = merger(this->head, B.head);
    return *C;
}

Item *list_set::merger(Item *A, Item *B)
{
    Item* cur; // храним текущий элемент
    Item* heaD;
    Item* i = A; Item* j = B;

    // заполняем первый элемент
    if (i->val < j->val) {
        heaD = new Item(i->val);
        cur = heaD;
        i = i->next;
    } else {
        heaD = new Item(j->val);
        cur = heaD;
        j = j->next;
    }

    while (j && i)
    {
        if (i->val > j->val) { // добавляем после cur
            cur->next = new Item(j->val);
            cur = cur->next;

            j = j->next;
            continue;
        }

        if (i->val < j->val) {
            cur->next = new Item(i->val);
            cur = cur->next;
            i = i->next;
            continue;
        }
    }

    // если закончилось одно из множеств – добавляем элементы другого до конца в С
    if (!i) {
        list_set* temp = new list_set(j);
        cur->next = temp->head;
    } else {
        list_set* temp = new list_set(i);
        cur->next = temp->head;
    }
    return heaD;
}

bool list_set::member(int x)
{
    // проверка на наличие множества
    if (!this->head)
        return false;

    Item* temp = findX(x,head);
    return temp->next->val == x;
}

void list_set::insert (int v)
{
    // создаем элемент с val = v
    Item* neW = new Item(v);

    // вставка в пустое множество - создается head - первый и последний элемент
    if (!head) {
        head = neW;
        return;
    }
    // если v < head.val то вставка в голову
    if (v < head->val) {
        neW->next = head;
        head = neW;
        return;
    }

    // поиск значения v
    Item* temp = findX(neW->val,head);

    // вернули prev элемента neW.val - если prev.next это не х - то надо добавить
    if (temp->next && (temp->next->val != neW->val)) {
        Item* nexT = temp->next;
        temp->next = neW;

        if (nexT)
            neW->next = nexT;

    // иначе если temp - последний элемент
    } else if (!temp->next)
        temp->next = neW;
}

void list_set::deleTe (int x)
{
    // проверка на наличие множества
    if (!this->head)
        return;

    Item* i = head;

    if (i->val == x) // если голова
    {
        if (head->next) { // если есть еще элементы помимо головы
            delete i;
            head = head->next;
        } else {          // если один элемент в списке
            delete i;
            head = nullptr;
        }
        return;
    }

    // иначе ищем в findX и возвращаем предыдущий
    Item* prev = findX(x,head);

    // если вернулся !prev.next, то есть хвост – такого элемента нет в списке
    if (prev->next) {
        Item* cur = prev->next;
        prev->next = cur->next;

        delete cur;
    }
}

list_set& list_set::operator= (list_set B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    this->destructor();
    list_set* temp = new list_set(B.head);

    this->head = temp->head;
    return *this;
}

int list_set::min()
{
    // проверка на наличие множества
    if (!this->head)
        return -2147483647;

    return this->head->val;
}

int list_set::max()
{
    // проверка на наличие множества
    if (!this->head)
        return 2147483647;

    Item* tail;
    for (Item* i = this->head; i != nullptr; i = i->next) {
        tail = i;
    }
    return tail->val;
}

bool list_set::operator==(list_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return true;

    // проверка на наличие множеств
    if (!this->head || !B.head)
        return false;

    return equals(head,B.head);
}

bool list_set::equals(Item *A, Item *B)
{
    Item* j = B;
    Item* i;
    for (i = A; i && j; i = i->next, j = j->next)
        if (i->val != j->val)
            return false;

    if (!i && !j)
        return true;
    else
        return false;
}

list_set& list_set::find(list_set &B, int x)
{
     Item* temp = findX(x, this->head);

     // вернулся prev от Х - и при этом prev.next == x
     if (temp->next->val == x)
         return *this;

     temp = B.findX(x,B.head);
     if (temp->next->val == x)
         return B;

     std::cout << "!! такого элемента нет ни в одном из списков !!" << std::endl;
}

Item *list_set::findX(int x, Item* pos)
{
    Item* prev;
    for (Item* i = pos; i != nullptr; i = i->next) {
        // если совпало или уже перепрыгнуло имеющееся значение
        if (i->val >= x)
            return prev;

        prev = i;
    }

    return prev;
}

void list_set::makeNull()
{
    this->destructor();
}

bool list_set::isIntersect(const list_set& B)
{
    // если это одно и то же множество
    if (this->head == B.head)
        return true;

    Item* i = this->head; Item* j = B.head;
    while (i && j) {
        if (i->val == j->val)
            return true;
        if (i->val > j->val)
            j = j->next;
        if (i->val < j->val)
            i = i->next;
    }
    return false;
}

void list_set::print()
{
    std::cout << std::endl << "{";

    for (Item* i = head; i != nullptr; i = i->next)
    {
        if (i->next)
            std::cout << i->val << ", ";
        else
            std::cout << i->val;
    }

    std::cout << "}" << std::endl;
}

void list_set::destructor()
{
    if (!head)
        return;

    Item* current = head;
    while (current) {
        Item* nexT = current->next;
        delete current;
        current = nexT;
    }

    this->head = nullptr;
}
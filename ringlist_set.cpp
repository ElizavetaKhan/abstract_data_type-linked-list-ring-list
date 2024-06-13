#include "ringlist_set.h"

ringlist_set::ringlist_set()
{
    tail = nullptr;
}

ringlist_set::ringlist_set(item* copy)
{
    item* neW;
    item* i;
    item* newTail = new item(copy->val);

    for (i = copy->next; i->val < i->next->val; i = i->next)
    {
        neW = new item(i->val,newTail->next); // ссылаемся на голову

        newTail->next = neW;
        newTail = newTail->next;
    }

    neW = new item(i->val,newTail->next); // ссылаемся на голову

    newTail->next = neW;
    newTail = newTail->next;

    this->tail = newTail;
}

ringlist_set::~ringlist_set()
{
    this->destructor();
}

ringlist_set& ringlist_set::operator+(ringlist_set &B)
{
    // если это одно и то же множество
    if (this == &B)
        return *this;

    // если пустое множество this или В
    if (!this->tail && B.tail)
        return B;
    if (!B.tail && this->tail)
        return *this;

    // возвращаем пустое множество если оба пустые
    if (!this->tail && B.tail) {
        ringlist_set* nuLL;
        nuLL = new ringlist_set();
        return *nuLL;
    }

    ringlist_set* C;
    C = new ringlist_set();
    C->tail = combiner(tail,B.tail);

    return *C;
}

item* ringlist_set::combiner(item *A, item *B)
{
    item* i = A->next; item* j = B->next;
    item* taiL;

    // заполняем первый элемент
    if (i->val <= j->val) {
        taiL = new item(i->val);
        i = i->next;
    } else {
        taiL = new item(j->val);
        j = j->next;
    }

    while (i != A && j != B)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем элемент
             item* neW = new item(i->val, taiL->next); // указатель на голову переносим
             taiL->next = neW;

             taiL = taiL->next;
             j = j->next;
             i = i->next;
            continue;
        }

        if (i->val > j->val) { // если i больше - создаем элемент и добавляем после cur
             item* neW = new item(j->val, taiL->next); // указатель на голову переносим
             taiL->next = neW;

             taiL = taiL->next;
             j = j->next;
             continue;
        }

        if (i->val < j->val) { // если j больше - создаем элемент и добавляем после cur
            item* neW = new item(i->val, taiL->next); // указатель на голову переносим
            taiL->next = neW;

            i = i->next;
            taiL = taiL->next;
            continue;
        }
    }

    // отдельно обрабатываем хвост – пока списком(который не закончился) догоняем другой
    // I - всегда тот, который еще не кончился

    if (i == A) {
        // если оказалось, что i короче – меняем
        std::swap(i,j);
        std::swap(A,B);
    }

    while (i != A)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем элемент
            item* neW = new item(i->val, taiL->next); // указатель на голову переносим
            taiL->next = neW;

            taiL = taiL->next;
            i = i->next;
            break;
        }

        if (i->val > j->val) { // если i больше - создаем элемент и добавляем после taiL
            item* neW = new item(j->val, taiL->next); // указатель на голову переносим
            taiL->next = neW;

            taiL = taiL->next;
            break;
        }

        if (i->val < j->val) { // если j больше - создаем элемент и добавляем после cur
            item* neW = new item(i->val,taiL->next); // указатель на голову переносим
            taiL->next = neW;

            i = i->next;
            taiL = taiL->next;
            continue;
        }
    }

    // если закончилось одно из множеств – добавляем элементы другого до конца в С
    if (i == A) {
        // поместили оставшиеся элементы в новый список
        ringlist_set* temp = new ringlist_set(j);
        item* newHead = temp->tail->next;

        temp->tail->next = taiL->next;
        taiL->next = newHead;
        taiL = temp->tail;

    } else if (j == B) {
        // поместили оставшиеся элементы в новый список
        ringlist_set* temp = new ringlist_set(i);
        item* newHead = temp->tail->next;

        temp->tail->next = taiL->next;
        taiL->next = newHead;
        taiL = temp->tail;
    }
    return taiL;
}

ringlist_set& ringlist_set::operator^(ringlist_set &B)
{
    // если this == B
    if (this == &B)
        return *this;

    // проверка на наличие множеств ИЛИ если промежутки у них разные
    if (!this->tail || !B.tail || this->tail->val < B.tail->next->val || B.tail->val < this->tail->next->val) {
        ringlist_set* nuLL;
        nuLL = new ringlist_set();
        return *nuLL;
    }

    ringlist_set* C;
    C = new ringlist_set(this->tail->next);

    C->tail = crosser(C->tail,B.tail);
    return *C;
}

item* ringlist_set::crosser(item *A, item *B)
{
    item* i = A->next; item* j = B->next;
    item* taiL = nullptr;

    // отдельно добавляем голову
    while (!taiL)
    {
        if (i->val == j->val) {
             taiL = new item(i->val);
             i = i->next;
             j = j->next;
             continue;
        }
        if (i->val < j->val) { // если в текущем списке элемент меньше
            i = i->next;
            continue;
        }

        if (i->val > j->val) { // идем дальше по списку В
            j = j->next;
        }
    }

    while (i != A && j != B)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем
            item* neW = new item(i->val,taiL->next);
            taiL->next = neW;

            taiL = taiL->next;
            j = j->next;
            i = i->next;
            continue;
        }

        if (i->val < j->val) { // если в текущем списке элемент меньше
            i = i->next;
            continue;
        }

        if (i->val > j->val) { // идем дальше по списку В
            j = j->next;
        }
    }


    // отдельно обрабатываем хвост – пока списком(который не закончился) догоняем другой
    // I - всегда тот, который еще не кончился
    if (i == A) {
        // если оказалось, что i короче – меняем
        std::swap(i,j);
        std::swap(A,B);
    }

    while (i != A)
    {
        if (i->val == j->val) { // при совпадении itemов - добавляем
            item* neW = new item(i->val,taiL->next);
            taiL->next = neW;
            taiL = taiL->next;
            break;
        }

        if (i->val < j->val) { // если в текущем списке элемент меньше
            i = i->next;
            continue;
        }

        if (i->val > j->val)  // выходим
            break;
    }

    return taiL;
}

ringlist_set& ringlist_set::operator/(ringlist_set &B)
{
    // проверка на наличие множеств ИЛИ если this == B
    if (!this->tail || (this == &B)) {
        ringlist_set* nuLL;
        nuLL = new ringlist_set();
        return *nuLL;
    }
    if (!B.tail)
        return *this;

    // если промежутки у них разные
    if (this->tail->val < B.tail->next->val || B.tail->val < this->tail->next->val)
        return *this;

    ringlist_set* C;
    C = new ringlist_set(this->tail->next);

    C->tail = subtractor(C->tail,B.tail);
    return *C;
}

item* ringlist_set::subtractor(item* A, item* B)
{
    item* i = A->next; item* j = B->next;
    item* prev; // храним предыдущий элемент

    while (i != A && j != B)
    {
        if (i->val == j->val) { // при совпадении itemов - удаляем i, по j идем дальше
            j = j->next;
            prev->next = i->next;
            delete i;
            i = prev->next;
            continue;
        }

        if (i->val < j->val) { // если в текущем списке элемент меньше - делаем next, догоняем j
            prev = i;
            i = i->next;
            continue;
        }

        if (i->val > j->val) // идем дальше по списку В
            j = j->next;
    }


    // проверяем хвост отдельно
    // если закончился список i
    if (i == A)
        while (j != B) {

            if (i->val == j->val) { // при совпадении itemов - удаляем i
                prev->next = i->next;

                // удаляем хвост
                A = prev;
                delete i;
                return A;
            }

            if (i->val < j->val) // если в текущем списке элемент меньше - делаем next, догоняем j
                break;

            if (i->val > j->val) // идем дальше по списку В
                j = j->next;
        }

    // если закончился список j
    else {
        while (i != A) {

            if (i->val == j->val) { // при совпадении itemов - удаляем i
                prev->next = i->next;

                // удаляем элемент и выходим
                delete i;
                return A;
            }

            if (i->val < j->val) { // если в текущем списке элемент меньше - делаем next, догоняем j
                prev = i;
                i = i->next;

                continue;
            }

            if (i->val > j->val) // выходим
                break;
        }
    }

    return A;
}

ringlist_set& ringlist_set::merge(ringlist_set &B)
{
    // если пустое множество this или В
    if (!this->tail && B.tail)
        return B;
    if (!B.tail && this->tail)
        return *this;

    // возвращаем пустое множество если оба пустые
    if (!this->tail && B.tail) {
        ringlist_set* nuLL;
        nuLL = new ringlist_set();
        return *nuLL;
    }

    ringlist_set* C;
    C = new ringlist_set();
    C->tail = merger(tail,B.tail);

    return *C;
}

item *ringlist_set::merger(item *A, item *B)
{
    item* i = A->next; item* j = B->next;
    item* taiL;

    // заполняем первый элемент
    if (i->val < j->val) {
        taiL = new item(i->val);
        i = i->next;
    } else {
        taiL = new item(j->val);
        j = j->next;
    }

    while (i != A && j != B)
    {
        if (i->val > j->val) { // если i больше - создаем элемент и добавляем после cur
            item* neW = new item(j->val,taiL->next); // указатель на голову переносим
            taiL->next = neW;

            taiL = taiL->next;
            j = j->next;
            continue;
        }

        if (i->val < j->val) { // если j больше - создаем элемент и добавляем после cur
            item* neW = new item(i->val, taiL->next); // указатель на голову переносим
            taiL->next = neW;

            i = i->next;
            taiL = taiL->next;
            continue;
        }
    }


    // отдельно обрабатываем хвост – пока списком(который не закончился) догоняем другой
    // I - всегда тот, который еще не кончился
    if (i == A) {
        // если оказалось, что i короче – меняем
        std::swap(i,j);
        std::swap(A,B);
    }

    while (i != A)
    {
        if (i->val > j->val) { // если i больше - создаем элемент и добавляем после taiL
            item* neW = new item(j->val, taiL->next); // указатель на голову переносим
            taiL->next = neW;

            taiL = taiL->next;
            break;
        }

        if (i->val < j->val) { // если j больше - создаем элемент и добавляем после cur
            item* neW = new item(i->val,taiL->next); // указатель на голову переносим
            taiL->next = neW;

            i = i->next;
            taiL = taiL->next;
            continue;
        }
    }

    // если закончилось одно из множеств – добавляем элементы другого до конца в С
    if (i->val == A->val) {
        // поместили оставшиеся элементы в новый список
        ringlist_set* temp = new ringlist_set(j);
        item* newHead = temp->tail->next;

        temp->tail->next = taiL->next;
        taiL->next = newHead;
        taiL = temp->tail;

    } else if (j == B) {
        // поместили оставшиеся элементы в новый список
        ringlist_set* temp = new ringlist_set(i);
        item* newHead = temp->tail->next;

        temp->tail->next = taiL->next;
        taiL->next = newHead;
        taiL = temp->tail;
    }
    return taiL;
}

bool ringlist_set::member(int x)
{
    // проверка на наличие множества
    if (!this->tail)
        return false;

    item* temp = findX(x,tail->next);
    return temp->next->val == x;
}

void ringlist_set::insert(int x)
{
    // создаем элемент с val = х
    item* neW = new item(x);

    // вставка в пустое множество - создается tail - первый и последний элемент
    if (!tail) {
        tail = neW;
        return;
    }

    item* head = tail->next;
    // если х < head.val то вставка в голову
    if (x < head->val) {
        neW->next = head;
        tail->next = neW;

        return;
    }

    // поиск значения х в списке
    item* temp = findX(neW->val,tail->next);

    // вернули prev элемента neW.val - если prev.next это не х - то надо добавить
    if (temp->next->val != neW->val) {
        item* nexT = temp->next;
        temp->next = neW;
        neW->next = nexT;

        // если вставка в хвост
        if (neW->val > neW->next->val) {
            tail = neW;
        }
    }
}

void ringlist_set::deleTe(int x)
{
    // проверка на наличие множества
    if (!this->tail)
        return;

    if (x == tail->val) // если это хвост
    {
        // и всего один элемент
        if (tail->next == tail)
        {
            delete tail;
            tail = nullptr;
            return;
        }
    }

    item* prev = findX(x,tail->next);

    // если такой х вообще есть
    if (prev != tail) {
        item* cur = prev->next;
        prev->next = cur->next;

        delete cur;
    }
}

ringlist_set& ringlist_set::operator=(ringlist_set B)
{
    // если this == B
    if (this == &B)
        return *this;

    this->destructor();

    ringlist_set* temp = new ringlist_set(B.tail->next);
    this->tail = temp->tail;
    return *this;
}

int ringlist_set::min()
{
    return tail->next->val;
}

int ringlist_set::max()
{
    return tail->val;
}

bool ringlist_set::operator==(ringlist_set &B)
{
    // если this == B
    if (this == &B)
        return true;

    // проверка на наличие множеств
    if (!this->tail || !B.tail)
        return false;

    return equals(tail,B.tail);
}

bool ringlist_set::equals(item *A, item *B)
{
    item* j = B->next;
    item* i;
    for (i = A->next; i != A; i = i->next)
        if (i->val != j->val)
            return false;

    // если хвосты не равны или j вообще не хвост (то есть множество больше)
    if (i->val != j->val || j->val != B->val)
        return false;

    return true;
}

ringlist_set& ringlist_set::find(ringlist_set &B, int x)
{
    item* temp = findX(x,tail->next);

    // вернулся prev от Х - и при этом prev.next == x
    if (temp->next->val == x)
        return *this;

    temp = B.findX(x, B.tail->next);
    if (temp->next->val == x)
        return B;
}

item *ringlist_set::findX(int x, item* pos)
{
    item* prev = tail;
    for (item* i = pos; i != this->tail; i = i->next)
    {
        // если совпало или уже перепрыгнуло имеющееся значение
        if (i->val >= x)
            return prev;

        prev = i;
    }

    // проверяем хвост
    if (tail->val >= x)
        return prev;

    return tail;
}

bool ringlist_set::isIntersect(ringlist_set B)
{
    // если this == B
    if (this->tail == B.tail)
        return true;

    // если промежутки у них разные
    if (this->tail->val < B.tail->next->val || B.tail->val < this->tail->next->val)
        return false;

    item* i = this->tail->next; item* j = B.tail->next;
    while (i != this->tail && j != B.tail) {
        if (i->val == j->val)
            return true;
        if (i->val > j->val)
            j = j->next;
        if (i->val < j->val)
            i = i->next;
    }

    // разбираемся с хвостами и если что "догоняем" множество, которое дошло до хвоста
    if (i->val == j->val)
        return true;
    else if ((i == this->tail) && (j->val < i->val))
    {
        // отправляемся на поиск хвоста одного множества в остатках второго
        if (B.findX(i->val,j) != B.tail) // если нашли
            return true;

    } else if ((j == B.tail) && (j->val > i->val))
        // отправляемся на поиск хвоста одного множества в остатках второго
        if (findX(j->val,i) != tail) // если нашли
            return true;

    return false;
}

void ringlist_set::makeNull()
{
    this->destructor();
}

void ringlist_set::print()
{
    if (!tail)
        return;

    std::cout << std::endl << "{";

    for (item* i = tail->next; i != tail; i = i->next)
        std::cout << i->val << ", ";

    std::cout << tail->val << "}" << std::endl;
}

void ringlist_set::destructor()
{
    if (!tail)
        return;

    item* current = tail->next;
    while (current != tail)
    {
        item* nexT = current->next;
        delete current;
        current = nexT;
    }
    delete tail;
    tail = nullptr;
}
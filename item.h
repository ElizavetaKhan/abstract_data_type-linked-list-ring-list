#ifndef INC_5_LAB_ITEM_H
#define INC_5_LAB_ITEM_H

struct Item
{
    int val{};
    Item* next; // указатель на следующий

    explicit Item (int val)
    {
        this->val = val;
        this->next = nullptr;
    }

};

#endif //INC_5_LAB_ITEM_H

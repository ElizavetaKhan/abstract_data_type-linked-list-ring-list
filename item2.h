#ifndef INC_5_LAB_ITEM2_H
#define INC_5_LAB_ITEM2_H

struct item
{
    int val{};
    item* next;

    explicit item (int val)
    {
        this->val = val;
        this->next = this;
    }

    item (int val, item* next)
    {
        this->val = val;
        this->next = next;
    }

};

#endif //INC_5_LAB_ITEM2_H

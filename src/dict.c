#include <stdlib.h>
#include "librune.h"

rn_sint rn_dictCompare(rn_mem oldKey, rn_mem newKey)
{
    rn_unicode* old = oldKey;
    rn_unicode* new = newKey;
    rn_uint i = 0;
    rn_sint flag;
    while (rn_true)
    {
        if (!old[i] && !new[i]) return 0;
        flag = new[i] - old[i];
        if (flag || !new[i] || !old[i]) return flag;
        i++;
    }
}

void rn_dictKeyRelease(rn_mem _key)
{
    rn_unicode* key = _key;
    rn_uint i = 0;
    do {
        free(key[i++]);
    } while(key[i]);
}

void rn_dictValueRelease(rn_mem value)
{
    rn_obj obj = value;
    obj->release(obj);
}

rn_rbt rn_dictNew()
{
    rn_rbt dict = rn_rbtNew();
    dict->compare = rn_dictCompare;
    dict->keyRelease = rn_dictKeyRelease;
    dict->valueRelease = rn_dictValueRelease;
    return dict;
}

rn_obj rn_dictGet(rn_rbt rbt, rn_unicode* string)
{
    return rn_rbtGet(rbt, string);
}

void rn_dictInsert(rn_rbt rbt, rn_unicode* string, rn_obj obj)
{
    rn_rbtInsert(rbt, string, obj);
}

rn_obj rn_dictDelete(rn_rbt rbt, rn_unicode* string)
{
    rn_rbtDelete(rbt, string);
}

void rn_dictRelease(rn_rbt rbt)
{
    rn_rbtRelease(rbt);
}

/**
 * 
 */
#include <stdlib.h>
#include "librune.h"

rn_obj rn_ObjRoot, rn_ObjMap;

void rn_objDefaultInit(rn_obj obj)
{
    obj->size = 0;
    obj->data = rn_null;
}

rn_obj rn_objDefaultCall(rn_obj obj, rn_obj param)
{
    if (obj->super)
    {
        return obj->super->call(obj, param);
    }
    else
    {
        return param;
    }
}

rn_obj rn_objDefaultGet(rn_obj obj, rn_obj param)
{
    if (obj->super)
    {
        return obj->super->get(obj, param);
    }
    else
    {
        return param;
    }
}

void rn_objDefaultSet(rn_obj obj, rn_obj key, rn_obj value)
{
    if (obj->super) obj->super->set(obj, key, value);
}

void rn_objDefaultRelease(rn_obj obj)
{
    if (obj->super) obj->super->release(obj);
}

rn_obj rn_objNew(rn_obj superObj)
{
    rn_obj obj = malloc(sizeof(rn_ObjBody));
    *obj = rn_ObjRoot;
    obj->super = superObj;
    superObj->init(obj);
    obj->init = superObj->init;
    return obj;
}

rn_sint rn_mapRBTCompare(rn_mem oldKey, rn_mem newKey)
{
    rn_sint sizeFlag, dataFlag;
    rn_uint minSize, i;
    rn_obj old = oldKey, new = newKey;

    sizeFlag = new->size - old->size;
    minSize = MIN(new->size, old->size);

    for (i = 0; i < minSize; i++)
    {
        dataFlag = new->data[i] - old->data[i];
        if (dataFlag) return dataFlag;
    }

    return sizeFlag;
}

void rn_mapRBTRelease(rn_mem data)
{
    rn_obj obj = data;
    obj->release(obj);
}

void rn_objMapInit(rn_obj obj)
{
    rn_rbt rbt = rn_rbtNew();
    rbt->compare = rn_mapRBTCompare;
    rbt->keyRelease = rn_mapRBTRelease;
    rbt->valueRelease = rn_mapRBTRelease;

    obj->data = rbt;
    obj->size = sizeof(rn_RBTBody);
}

rn_obj rn_objMapGet(rn_obj obj, rn_obj param)
{
    return (rn_obj) rn_rbtGet(obj->data, param);
}

void rn_objMapSet(rn_obj obj, rn_obj key, rn_obj value)
{
    rn_rbtInsert(obj->data, key, value);
}

void rn_objMapRelease(rn_obj obj)
{

}

rn_obj rn_objMapNew()
{
    return rn_objNew(rn_ObjMap);
}

void rn_initObj()
{
    *rn_ObjRoot = {0, rn_null, rn_null, rn_objDefaultInit, rn_objDefaultCall,
                     rn_objDefaultGet, rn_objDefaultSet, rn_objDefaultRelease};

    *rn_ObjMap = {sizeof(rn_RBTBody), rn_rbtNew(), rn_ObjRoot, rn_objMapInit, rn_objDefaultCall,
                     rn_objMapGet, rn_objMapSet, rn_objMapRelease};
}

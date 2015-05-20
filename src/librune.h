#ifndef LIBRUNE_H_
#define LIBRUNE_H_

/* util.c */

typedef signed char rn_schar;
typedef signed short rn_sshort;
typedef signed int rn_sint;
typedef signed long long rn_slong;

typedef unsigned char rn_uchar;
typedef unsigned short rn_ushort;
typedef unsigned int rn_uint;
typedef unsigned long long rn_ulong;

typedef rn_uchar rn_bool;
typedef rn_uchar rn_byte;
typedef void* rn_mem;

extern const rn_bool rn_true;
extern const rn_bool rn_false;
extern const rn_mem rn_null;

extern const rn_schar rn_SCHAR_MIN;
extern const rn_sshort rn_SSHORT_MIN;
extern const rn_sint rn_SINT_MIN;
extern const rn_slong rn_SLONG_MIN;
extern const rn_schar rn_SCHAR_MAX;
extern const rn_sshort rn_SSHORT_MAX;
extern const rn_sint rn_SINT_MAX;
extern const rn_slong rn_SLONG_MAX;
extern const rn_uchar rn_UCHAR_MAX;
extern const rn_ushort rn_USHORT_MAX;
extern const rn_uint rn_UINT_MAX;
extern const rn_ulong rn_ULONG_MAX;


/* lins.c */

typedef struct rn_linsNode{
    struct rn_linsNode* prev;
    struct rn_linsNode* next;
    rn_mem value;
}rn_LinsNode;

typedef struct rn_linsBody{
    struct rn_linsNode* first;
    struct rn_linsNode* last;
}rn_LinsBody;

typedef rn_LinsBody* rn_lins;
typedef void (*rn_LinsForEachHandler) (rn_uint, rn_mem, rn_mem);

rn_lins rn_linsNew();
rn_uint rn_linsSize(rn_lins);
void rn_linsPush(rn_lins, rn_mem);
rn_mem rn_linsPull(rn_lins);
rn_mem rn_linsPop(rn_lins);
rn_mem linsGet(rn_lins, rn_sint);
void linsSet(rn_lins, rn_sint, rn_mem);
void linsDeleteIndex(rn_lins, rn_sint);
void linsDelete(rn_lins, rn_mem);
void linsForEach(rn_lins, rn_LinsForEachHandler, rn_mem);
void linsFree(rn_lins);


/* rbt.c */

typedef rn_sint (*rn_RBTCompare) (rn_mem, rn_mem);
typedef void (*rn_RBTContentFree) (rn_mem);
typedef void (*rn_RBTForEachHandler) (rn_uint, rn_mem, rn_mem, rn_mem);

typedef struct rn_rbtNode{
    struct rn_rbtNode* up;
    struct rn_rbtNode* left;
    struct rn_rbtNode* right;
    rn_uchar color;
    rn_mem key;
    rn_mem value;
}rn_RBTNode;

typedef struct rn_rbtBody{
    struct rn_rbtNode* root;
    rn_RBTCompare compare;
    rn_RBTContentFree keyFree;
    rn_RBTContentFree valueFree;
}rn_RBTBody;

typedef struct rn_defaultRBTValue{
    rn_uint length;
    rn_byte* data;
}rn_DefaultRBTValue;

typedef rn_RBTBody* rn_rbt;

rn_rbt rn_rbtNew();
rn_uint rn_rbtSize(rn_rbt);
rn_mem rn_rbtGet(rn_rbt, rn_mem);
void rn_rbtInsert(rn_rbt, rn_mem, rn_mem);
rn_mem rn_rbtDelete(rn_rbt, rn_mem);
void rn_rbtForEach(rn_rbt, rn_RBTForEachHandler, rn_mem);
void rn_rbtFree(rn_rbt);


/* utf8.c */

typedef struct rn_utf8Buffer{
    rn_uchar size;
    rn_byte bytes[3];
}rn_UTF8Buffer;

typedef rn_ushort rn_unicode;
typedef rn_UTF8Buffer* rn_utf8;

rn_utf8 rn_utf8Encode(rn_unicode);
rn_utf8 rn_utf8DecodeStart();
rn_unicode rn_utf8DecodeNext(rn_utf8, rn_byte);


#endif /* LIBRUNE_H_ */

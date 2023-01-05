#define __NEED_Size
#define __NEED_UniversalType
#define __NEED_Status
#include <covenant/std.h>
#include <covenant/bits/everytype.h>
#include <covenant/fn.h>

typedef struct TailQueue TailQueue;
typedef struct TailQueueNode TailQueueNode;

struct TailQueueNode {
    UniversalType data;
    TailQueueNode* next;
};

struct TailQueue {
    Size size;
    TailQueueNode* head;
    TailQueueNode* tail;
    CompareFunction cmp;
    DestructorFunction destructor;
    Size obj_size;
};

Status c_tq_init(TailQueue*, Size, CompareFunction, DestructorFunction);

// Add an element at the tail of the linked list
Status c_tq_add(TailQueue*, UniversalType);

/* Node operations */
Status c_tq_node_init(UniversalType, TailQueueNode*);

Status c_tq_removeat(TailQueue*, Size);

Status c_tq_pop_head(TailQueue*);
Status c_tq_pop_tail(TailQueue*);

UniversalType c_tq_getv(TailQueue*, UniversalType);
TailQueueNode* c_tq_getn(TailQueue*, UniversalType);

/* Get node and value at index */
UniversalType c_tq_getvat(TailQueue*, Size);
TailQueueNode* c_tq_getnat(TailQueue*, Size);

// Insert to TailQueue, at index and element
Status c_tq_insert(TailQueue*, Size, UniversalType);

// Add an element at the head of the linked list
Status c_tq_push(TailQueue*, UniversalType);

void c_tq_destroy(TailQueue*);
// Enables the header to only be included
#pragma once

// Include guard for compilers not knowing the pragma directive.
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "importantIncludes.h"

// Just copied this from stack overflow
// https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit/263738#263738https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit/263738#263738
/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b)                                                        \
  (!!((a) & (1ULL << (b)))) // '!!' to make sure this returns 0 or 1

/* x=target variable, y=mask */
#define BITMASK_SET(x, y) ((x) |= (y))
#define BITMASK_CLEAR(x, y) ((x) &= (~(y)))
#define BITMASK_FLIP(x, y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x, y) (!(~(x) & (y)))
#define BITMASK_CHECK_ANY(x, y) ((x) & (y))

// Data structures:
typedef struct {
  uint64_t id;
  void *data;
  uint64_t dataSize;
  struct node *next;
} node;

typedef struct {
  uint64_t globalId;
  node *back;
  node *front;
} LinkedList;

typedef struct {
  void *data;
  size_t dataSize;
  struct Stack *next;
} Stack;

typedef struct {
  void *data;
  struct DLNode *prev;
  struct DLNode *next;
} DLNode;

typedef struct dllist {
  size_t dataSize;
  uint32_t size;
  DLNode *head;
  DLNode *tail;
} DLinkedList;

typedef DLinkedList Queue;

typedef struct {
  uint64_t highestPosition;
  uint64_t size;
  size_t dataSize;
  void **data;
  void (*datadeletefuncion)(void **data);
} DynamicArray;

// Dynamic array functions:
DynamicArray *dynarr_initialize(void *data, size_t dataSize,
                                uint64_t initialSize,
                                void (*datadeletefunc)(void **data));
void *dynarr_at(DynamicArray *list, uint64_t position);
uint64_t dynarr_size(DynamicArray *list);
uint64_t dynarr_resize(DynamicArray *list, int mode);
int dynarr_add(DynamicArray *list, void *data);
void *dynarr_remove(DynamicArray *list);
int dynarr_delete(DynamicArray **list);
void *dynarr_removeAt(DynamicArray *list, uint64_t position);
int dynarr_addAt(DynamicArray *list, void *data, uint64_t position);

// Function prototypes:
// Prime number computation:
uint8_t *prime_so_prime(int);
int prime_checksoPrimes(uint8_t *, int);
uint8_t *prime_me_prime(uint32_t n);
int prime_checkmePrimes(uint8_t *bools, int n);
int prime_prime(int);
uint8_t *prime_boolarr(int n);
uint64_t primes_calcPrimesNaive(uint64_t primesMax);
uint8_t *prime_satkins(uint64_t limit);

// Game of life methods.
int **gol_generateEmptyField(int width, int height);
int **gol_nextGen(int **oldGen, int rows, int cols);
void gol_printField(int **field, int width, int height);
void gol_setField(int **field, int width, int height);
void gol_nextGeni(int ***oldGen, int rows, int cols);

// Singly linked list:
LinkedList *llist_createList(void *data, size_t dataSize);
uint64_t llist_appendItem(LinkedList *list, void *data, uint64_t dataSize);
int llist_printList(LinkedList *list, void (*outputData)(node *));
node *llist_findItem(LinkedList *list, uint32_t id);
void llist_deleteItem(LinkedList *list, uint32_t id,
                      void (*freefunc)(void *data));
void llist_deleteList(LinkedList *list, void (*freefunc)(void *data));
int llist_merge(LinkedList *start, LinkedList *appendedList);

// Dynamic stack implementation:
Stack *stack_create(void *data, size_t dataSize);
uint32_t stack_push(Stack **stack, void *data, size_t dataSize);
Stack *stack_pop(Stack **stack);
uint32_t stack_empty(Stack **stack);
Stack *stack_top(Stack *stack);

// TODO:
int llist_sort(LinkedList *list, void (*sortfunc)(void *data));

// Doubly linked list:
DLinkedList *dllist_initialization(void *data, size_t dataSize);
uint32_t dllist_size(DLinkedList *list);
int dllist_push(DLinkedList *list, void *data, size_t dataSize);
DLNode *dllist_pop(DLinkedList *list);
int dllist_isEmpty(DLinkedList *list);
int dllist_merge(DLinkedList *list);

// TODO:
// TODO: implement id for the dllist elements.
DLNode *dllist_popStart(DLinkedList *list);
DLNode *dllist_pushEnd(DLinkedList *list);
int dllist_removeData(DLinkedList *list, void *data);
int dllist_deleteList(DLinkedList *list, void (*deletefunc)(void *data));
int dllist_sort(DLinkedList *list, void (*comparefunc)(DLNode *node));
int dllist_print(DLinkedList *list, void (*printfunc)(void *data));

// Queue:
Queue *queue_initialize(void *data, size_t dataSize);
int queue_enqueue(Queue *queue, void *data, size_t dataSize);
DLNode *queue_dequeue(Queue *queue);

// Various other algorithms:
void xorswap(int *a, int *b);
uint64_t factorial(uint64_t number);
uint64_t factorialNR(uint64_t number);

#endif
#include "algorithms.h"

// Create the dynamic array with the intial data in the argument and an initial
// size.
DynamicArray *dynarr_initialize(void *data, size_t dataSize,
                                uint64_t initialSize,
                                void (*datadeletefunc)(void **data)) {
  if (data == NULL || dataSize == 0) {
    return 0;
  } else {
    // Create the list structure at runtime.
    DynamicArray *list = malloc(sizeof(DynamicArray));
    // Initialize the size members.
    list->dataSize = dataSize;
    list->size = initialSize;
    list->highestPosition = 0;

    list->datadeletefuncion = datadeletefunc;
    // Allocate space for the data and then copy it in there.
    list->data = calloc(initialSize, sizeof(void *));
    list->data[list->highestPosition] = malloc(dataSize);
    memcpy(list->data[0], data, dataSize);
    return list;
  }
}

// Returns the data at the position in the array.
void *dynarr_at(DynamicArray *list, uint64_t position) {
  if (!(!(list == NULL) && !(position > (list->size - 1)))) {
    return NULL;
  } else {
    // Return the data at the position.
    return list->data[position];
  }
}

// Returns the size of the list.
uint64_t dynarr_size(DynamicArray *list) {
  if (list == NULL) {
    return 0;
  } else {
    // Return the numerical size of the list.
    return list->size;
  }
}

// Adds a new element at the end of the list.
int dynarr_add(DynamicArray *list, void *data) {
  if (list == NULL || data == NULL) {
    fprintf(stderr, "The item can not be added to the list.\n");
    return 0;
  } else {
    if ((list->size - 1) == list->highestPosition) {
      // Increase the size of the list.
      dynarr_resize(list, 1);
    }
    list->data[list->highestPosition + 1] = data;
    list->highestPosition++;
    return 1;
  }
}

// Uses the lifo principle to remove the data from the tail of the list.
void *dynarr_remove(DynamicArray *list) {
  if (!(!(list == NULL) && !(list->highestPosition == 0))) {
    return 0;
  } else {
    if ((ceil((double)(list->size / 2))) < list->highestPosition) {
      // The list is now smaller than half its size, therefore we need to
      // descrease the size.
      dynarr_resize(list, 0);
    }
    // Acces the data that needs to be removed.
    void *temp = list->data[list->highestPosition];
    // Remove the item from the list.
    list->data[list->highestPosition] = NULL;
    // Update the highest item member.
    list->highestPosition--;
    return 1;
  }
}

// There is some error in here and i am crying.

// Resize the list and returns the new size.
// Mode 0: decrease list size, Mode >0: increase list size
uint64_t dynarr_resize(DynamicArray *list, int mode) {
  // Checks for some basic assertions, such as the size of the list and that the
  // list is non-null.
  if (list == NULL || mode < 0) {
    fprintf(stderr, "The list cant be resized with these parameters!");
    return 0;
  } else {
    // Create the new size of the list.
    double newSize = 0;
    if (mode) {
      // Increase the list by a power of two.
      list->data =
          realloc(list->data, newSize = ceil((double)(list->size * 2)));
      // Allocate the rest of the new array with null so we dont have to worry
      // about the garabage values present after realloc, as it is implemented
      // using malloc and not calloc.
      for (int i = list->highestPosition + 1; i < newSize; i++) {
        list->data[i] = NULL;
      }
      // If the new size is now smaller than the previous biggest position, we
      // need to set the new biggest position to be the size of the new array
      // - 1. (The highest accessible member in the array.)
      if (newSize <= (list->highestPosition - 1)) {
        list->highestPosition = newSize - 1;
      }
    } else {
      // Shrink the list by a factor of two.
      list->data =
          realloc(list->data, newSize = ceil((double)(list->size / 2)));
    }
    // Set the size member of the list to accurately represent the actual new
    // size.
    list->size = newSize;
    return 1;
  }
}

// Removes the item from a specific position in the array.
void *dynarr_removeAt(DynamicArray *list, uint64_t position) {
  // Test some basic assertions about the validity of the parameters.
  if (!(!(list == NULL) && !(list->highestPosition < position))) {
    return NULL;
  } else {
    // Allocate a new temporary variable that gets returned later.
    void **newList = calloc(list->size, list->dataSize);
    // Save the data before removing it.
    void *temp = list->data[position];
    if (position != 0) {
      // The item is not at the start.
      // Copy the items before the removed one to the new list.
      memcpy(newList, list->data, list->dataSize * position);
    }
    if (position != (list->size - 1)) {
      // The item is not at the end.
      // Copy the items after the removed one to the new list.
      memcpy(newList + position, list->data + position + 1,
             (list->size - position - 1) * list->dataSize);
    }
    list->data = newList;
    // Decrease the list size members.
    list->highestPosition--;
    // Return the removed element.
    return temp;
  }
}
// Adds the item in the parameter to the list at the specified position.
int dynarr_addAt(DynamicArray *list, void *data, uint64_t position) {
  if (!(!(list == NULL) && !(list->data == NULL) && !(data == NULL) &&
        !(position > list->highestPosition))) {
    return 1;
  } else {
    // Check whether the list is still big enough to contain a new element, and
    // resize if this is not the case.
    if (list->size == (list->highestPosition - 1)) {
      dynarr_resize(list, 1);
    }
    // Create the new list with the size of the old one.
    void **newList = calloc(list->size, list->dataSize);

    if (position != 0) {
      // The item shall be added at the start of the list.
      // Copy the items from till the position of the item that needs to be
      // added.
      memcpy(newList, list->data, list->dataSize * position);
    }
    if (position != (list->size - 1)) {
      // The item shall be added at the end of the list.
      // Copy the items after the position where the item should be added.
      memcpy(newList + position + 1, list->data + position,
             (list->size - position) * list->dataSize);
    }
    newList[position] = data;
    // Add the item to the list.
    // Update the list struct.
    list->data = newList;
    list->highestPosition++;
    return 1;
  }
}
// Delete the entire list.
int dynarr_delete(DynamicArray **list) {
  if (*list == NULL) {
    return 0;
  } else {
    // Delete the data.
    for (int i = 0; i < (*list)->size; i++) {
      (*list)->datadeletefuncion((*list)->data[i]);
    }
    // Delete the rest of the list.
    free((*list)->data);
    free((*list)->datadeletefuncion);
    free(*list);
    *list = NULL;
    return 1;
  }
}

// Prime calculations:

uint8_t *prime_boolarr(int n) {
  uint8_t *bools =
      (uint8_t *)calloc(((n % 8) ? (n / 8) : ((n / 8) + 1)), sizeof(uint8_t));
  return bools;
}

// Returns the number of primes that are in the array previously calculated by
// the prime_so_prime method.
int prime_checksoPrimes(uint8_t *primesArray, int n) {
  uint32_t amount = 0;
  uint8_t *itr = primesArray + 2;
  for (int i = 2; i < n; i++) {
    if (!*(itr++)) {
      amount++;
    }
  }
  return amount;
}

int prime_prime(int n) {
  // Checks if the number is valid and does a few efficient checks before
  // starting to do the rest with stupid, inefficient modulos.
  if (n < 2 || !((int)n == n) || !(n & 1)) {
    return 0;
  } else {
    if (n == 2) {
      return 1;
    }
    int sqt = floor(sqrt(n));
    for (int i = 3; i <= sqt; i += 2) {
      if (!(n % i)) {
        return 0;
      }
    }
    return 1;
  }
}

int prime_checkmePrimes(uint8_t *bools, int n) {
  uint32_t amount = 0;
  if (bools == NULL || n < 0) {
    return 0;
  } else {
    for (int i = 2; i < n; i++) {
      if (!(BIT_CHECK(bools[i / 8], i % 8))) {
        amount++;
      }
    }
    return amount;
  }
}

uint8_t *prime_so_prime(int n) {
  uint8_t *primes = (uint8_t *)calloc((n + 1), sizeof(uint8_t));
  if (n < 2) {
    return primes;
  } else {
    if (n == 2) {
      primes[1] = 1;
      return primes;
    } else {
      int sqt = (uint32_t)sqrt(n);
      uint8_t *itr = primes + 2;
      for (int pos = 2; pos <= sqt; pos++) {
        if (!*(itr++)) {
          for (int j = pos * pos; j <= n; j += pos) {
            primes[j] = 1;
          }
        }
      }
      return primes;
    }
  }
}

uint8_t *prime_me_prime(uint32_t n) {
  uint8_t *primes = prime_boolarr(n);
  if (n == 1) {
    BIT_SET(primes[1 / 8], 1 % 8);
    return primes;
  } else {
    uint32_t sqt = (uint32_t)sqrt(n);
    for (uint32_t pos = 2; pos <= sqt; pos++) {
      if (!BIT_CHECK(primes[pos / 8], pos % 8)) {
        for (uint32_t j = pos * pos; j <= n; j += pos) {
          BIT_SET(primes[j / 8], j % 8);
        }
      }
    }
    return primes;
  }
}
// Naive way to calculate the prime number under a certain limit specified in
// the argument. The second argument is a bool specifying if the number should
// be printed or not. Memory usage: O(n) Time-complexity: O(n log n) (all cases)
uint64_t *prime_calcPrimeNaive(uint64_t limit) {
  // Allocate a dynamic array with the size of the limit.
  uint64_t *primes = (uint64_t *)calloc(limit, sizeof(uint64_t));
  // Position at which the next prime caclulated will be put in the array.
  uint64_t position = 1;
  // Check the case that there is not a single prime in there.
  if (limit < 2) {
    return primes;
  }
  // As the naive algorithm does not quite work well with 2, we just short
  // circuit it with a constant time operation.
  primes[0] = 2;
  for (uint64_t i = 3; i < limit; i++) {
    for (uint64_t j = 2; j < limit; j++) {
      if (!(i % j)) {
        // I is divisible by j,
        // therfore by definition not a prime number
        // and we can continue with the next number.
        break;
      } else {
        if (j == (i - 1)) {
          // The loop ran and found no number dividing i,
          // therefore i is prime by definition.
          // Thus we set the current position in the array equal to i and
          // increase the position variable afterwards.
          primes[position++] = i;
          break;
        }
      }
    }
  }
  return primes;
}

// Uses the sieve of atkin algorithm to calculate
// Only works for limit >= 6.
uint8_t *prime_satkins(uint64_t limit) {
  // Create an array of numbers up to limit initially marked as prime.
  uint8_t *bools = calloc(limit, sizeof(uint8_t));
  // Computes the floor of the square root of the limit.
  uint64_t sqt = sqrt(limit);
  for (uint64_t x = 1; x <= sqt; x++) {
    for (uint64_t y = 1; y <= sqt; y++) {
      uint64_t n = (4 * x * x) + (y * y);
      if (n <= limit &&
          (n % 60 == 1 || n % 60 == 13 || n % 60 == 17 || n % 60 == 29 ||
           n % 60 == 37 || n % 60 == 41 || n % 60 == 49 || n % 60 == 53)) {
        bools[n] = !bools[n];
      }
      n = (3 * x * x) + (y * y);
      if (n <= limit &&
          (n % 60 == 7 || n % 60 == 19 || n % 60 == 31 || n % 60 == 43)) {
        bools[n] = !bools[n];
      }
      n = (3 * x * x) - (y * y);
      if (x > y && n <= limit &&
          (n % 60 == 11 || n % 60 == 23 || n % 60 == 47 || n % 60 == 59)) {
        bools[n] = !bools[n];
      }
    }
  }

  // Sets the composite number to 0, as they are non-prime by definition.
  for (uint64_t i = 5; i <= sqt; i++) {
    if (bools[i] == 1) {
      for (uint64_t j = 1; j * i * i <= limit; j++) {
        bools[j * i * i] = 0;
      }
    }
  }

  // Return the prime array.
  return bools;
}

int **gol_generateEmptyField(int width, int height) {
  int **nextGen = (int **)calloc(width, sizeof(int *));
  for (int i = 0; i < width; i++) {
    nextGen[i] = (int *)calloc(height, sizeof(int));
  }
  return nextGen;
}

// Game of life:

int **gol_nextGen(int **oldGen, int rows, int cols) {
  int **nextGen = gol_generateEmptyField(cols, rows);

  for (int i = 1; i < rows - 1; i++) {
    for (int j = 1; j < cols - 1; j++) {
      int aliveNeighbours = 0;
      for (int l = -1; l <= 1; l++) {
        for (int m = -1; m <= 1; m++) {
          aliveNeighbours += oldGen[i + l][m + j];
        }
      }

      // Apply the rules of the standart CGOL rulesset.
      aliveNeighbours -= oldGen[i][j];
      if ((oldGen[i][j]) && (aliveNeighbours < 2)) {
        nextGen[i][j] = 0;
      } else {
        if ((oldGen[i][j]) && (aliveNeighbours > 3)) {
          nextGen[i][j] = 0;
        } else {
          if ((!oldGen[i][j]) && (aliveNeighbours == 3)) {
            nextGen[i][j] = 1;
          } else {
            nextGen[i][j] = oldGen[i][j];
          }
        }
      }
    }
  }
  // Free the memory and return the new field.
  free(oldGen);
  return nextGen;
}

void gol_nextGeni(int ***oldGen, int rows, int cols) {
  int **nextGen = gol_generateEmptyField(cols, rows);

  for (int i = 1; i < rows - 1; i++) {
    for (int j = 1; j < cols - 1; j++) {
      int aliveNeighbours = 0;
      for (int l = -1; l <= 1; l++) {
        for (int m = -1; m <= 1; m++) {
          aliveNeighbours += oldGen[0][i + l][m + j];
        }
      }
      // Apply the rules of the standart CGOL rulesset.
      aliveNeighbours -= oldGen[0][i][j];
      if ((oldGen[0][i][j]) && (aliveNeighbours < 2)) {
        nextGen[i][j] = 0;
      } else {
        if ((oldGen[0][i][j]) && (aliveNeighbours > 3)) {
          nextGen[i][j] = 0;
        } else {
          if ((!oldGen[0][i][j]) && (aliveNeighbours == 3)) {
            nextGen[i][j] = 1;
          } else {
            nextGen[i][j] = oldGen[0][i][j];
          }
        }
      }
    }
  }
  // Free the memory and return the new field.
  free(*(oldGen));
  *oldGen = nextGen;
}

void gol_printField(int **field, int width, int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (!field[i][j]) {
        printf(" .");
      } else {
        printf(" #");
      }
    }
    printf("\n");
  }
}

// Does not currently work as well as it should.
void gol_setField(int **field, int width, int height) {
  while (1) {
    printf("BItte geben sie die position ein: ");
    int row = 0, col = 0, val = 0;
    scanf("%d.%d %d", &row, &col, &val);
    if ((col < 0) || (col > width) || (col < 0)) {
      return;
    }
    if ((row < 0) || (row > height) || (row < 0)) {
      return;
    }
    if ((val < 0) || (val > 1) || val == -1) {
      return;
    }
    field[row][col] = val;
  }
}

// Create the linked list.
LinkedList *llist_createList(void *data, size_t dataSize) {
  if (data == NULL) {
    return NULL;
  } else {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->front = (node *)malloc(sizeof(node));
    list->back = list->front;
    list->globalId = 0;

    // Allocate the data pointer.
    list->front->data = malloc(dataSize);
    memcpy(list->front->data, data, dataSize);

    list->front->dataSize = dataSize;
    list->front->id = list->globalId;
    list->front->next = NULL;

    return list;
  }
}

uint64_t llist_appendItem(LinkedList *list, void *data, uint64_t dataSize) {
  node *temp = NULL;
  node *front = list->front;
  uint64_t id = ++list->globalId;
  if (front == NULL) {
    list->front = (node *)malloc(sizeof(node));
    list->back = list->front;
    list->globalId = 0;

    // Allocate the data pointer.
    list->front->data = malloc((size_t)dataSize);
    memcpy(list->front->data, data, dataSize);

    list->front->dataSize = dataSize;
    list->front->id = list->globalId;
    list->front->next = NULL;
  } else {
    // Allocation and declaration of the list item at the temporary pointer
    // temp.
    temp = (node *)malloc(sizeof(node));
    temp->id = id;
    temp->next = NULL;
    temp->data = malloc((size_t)dataSize);
    memcpy(temp->data, data, dataSize);

    list->back->next = temp;
    list->back = temp;
    temp = NULL;
  }
  return id;
}

int llist_printList(LinkedList *list, void (*outputData)(node *)) {
  int position = 0;
  node *itrVar = list->front;
  if (itrVar == NULL) {
    fprintf(stderr, "The list is empty!\n");
    return 0;
  } else {
    while (itrVar != NULL) {
      printf("\nPosition: %d,  \n", position++);
      printf("ID: %d\n", itrVar->id);
      outputData(itrVar);
      itrVar = itrVar->next;
    }
  }
  itrVar = NULL;
  return 1;
}

node *llist_findItem(LinkedList *list, uint32_t id) {
  node *itrVar = list->front;
  while (itrVar != NULL) {
    if (itrVar->id == id) {
      return itrVar;
    } else {
      itrVar = itrVar->next;
    }
  }
  return NULL;
}
void llist_deleteList(LinkedList *list, void (*freefunc)(void *data)) {
  // Initializing the variables used for the iteration and the deletion.
  node *itrVar = list->front;
  node *freeVar = NULL;

  while (itrVar != NULL) {
    freeVar = itrVar;
    itrVar = itrVar->next;
    freefunc(freeVar->data);
    free(freeVar);
  };
  // Sets the pointers front and back to point to nothing as the final step.
  list->back = NULL;
  list->front = NULL;
  list->globalId = 0;
}

void llist_deleteItem(LinkedList *list, uint32_t id,
                      void (*freefunc)(void *data)) {
  node *temp = NULL, *itrVar = NULL;
  // If the id is not existing or
  if (list->front == NULL || id > list->globalId || id < list->front->id) {
    // end the function.
    return;
    // If the id is in the front.
  } else if (list->front->id == id) {
    temp = list->front;
    list->front = list->front->next;
    freefunc(temp->data);
    free(temp);

    // If the id is in the back.
  } else if (list->back->id == id) {
    itrVar = list->front;
    while (itrVar->next != NULL) {
      temp = itrVar;
      itrVar = itrVar->next;
    }
    list->back = temp;
    temp->next = NULL;
    freefunc(itrVar->data);
    free(itrVar);
    // If the id is somewhere between front and back.
  } else {
    itrVar = list->front;
    while (itrVar->id != id) {
      temp = itrVar;
      itrVar = itrVar->next;
    }
    temp->next = itrVar->next;
    freefunc(itrVar->data);
    free(itrVar);
  }
  temp = NULL;
  itrVar = NULL;
}

// Appends a linked list to the end of the second linked list.
int llist_merge(LinkedList *start, LinkedList *appendedList) {
  if (start == NULL) {
    return 0;
  } else {
    start->back->next = appendedList->front;
    return 1;
  }
}

Stack *stack_initialize(void *data, size_t dataSize) {
  if (data == NULL) {
    return NULL;
  }
  Stack *stack = malloc(sizeof(Stack));
  stack->dataSize = dataSize;
  stack->next = NULL;

  stack->data = malloc(dataSize);
  memcpy(stack->data, data, dataSize);

  return stack;
}

uint32_t stack_push(Stack **stack, void *data, size_t dataSize) {
  if (stack == NULL || data == NULL || *stack == NULL) {
    return 0;
  } else {
    Stack *temp = malloc(sizeof(Stack));
    temp->dataSize = dataSize;
    temp->data = malloc(dataSize);
    memcpy(temp->data, data, dataSize);

    temp->next = *stack;
    *stack = temp;

    return 1;
  }
}
Stack *stack_pop(Stack **stack) {
  if (stack == NULL) {
    return NULL;
  } else {
    Stack *temp = *stack;
    *stack = (*stack)->next;
    return temp;
  }
}

uint32_t stack_empty(Stack **stack) { return *stack == NULL; }

Stack *stack_top(Stack *stack) {
  if (stack == NULL) {
    return NULL;
  } else {
    return stack;
  }
}

// Create a new doubly linked list.
DLinkedList *dllist_initialization(void *data, size_t dataSize) {
  if (data == NULL) {
    return NULL;
  } else {
    // Allocates the space for the constant container of the list.S
    DLinkedList *list = malloc(sizeof(DLinkedList));
    list->dataSize = dataSize;
    list->size = 1;
    // Allocates the space for the node.
    list->head = malloc(sizeof(DLNode));
    list->head->prev = NULL;
    list->head->next = NULL;

    // Allocate the space for the data.
    list->head->data = malloc(dataSize);
    // Copy the data into the space that was allocated in the previous line.
    memcpy(list->head->data, data, dataSize);

    list->tail = list->head;
    return list;
  }
}

// Adds a new element to the head of the llist.
int dllist_push(DLinkedList *list, void *data, size_t dataSize) {
  if (list == NULL || data == NULL) {
    return 0;
  } else {
    // Allocate the space for the new node
    DLNode *node = malloc(sizeof(DLNode));
    node->data = malloc(dataSize);
    // Copy the data into the new node.
    memcpy(node->data, data, list->dataSize);
    if (list->size == 0) {
      list->head = node;
      list->tail = node;
    } else {
      // Set the next node after the current tail to be the new node.
      list->tail->next = node;
      // Set the pointer prev of the new node to point to the old tail.
      node->prev = list->tail;
      // Set the tail variable to the new node, as it is now the new tail.
      list->tail = node;
    }
    // Increase the size of the list before returning to the caller.
    list->size++;
    return 1;
  }
}
// Removes the element from the tail.
DLNode *dllist_pop(DLinkedList *list) {
  // Error checking.
  if (list == NULL || list->size == 0) {
    return NULL;
  } else {
    // Sets the temporary variable to point to the head of the list.
    DLNode *temp = list->head;
    // Case 1: the list has two members.
    if (list->size == 1) {
      list->head = NULL;
      list->tail = NULL;
      // Case 2: the list has > 2 members.
    } else {
      list->head = temp->next;
      list->head->prev = NULL;
      temp->next = NULL;
    }
    list->size--;
    return temp;
  }
}

uint32_t dllist_size(DLinkedList *list) { return list->size; }

int dllist_isEmpty(DLinkedList *list) { return dllist_size(list); }

// Initializes a new queue with the data in the parameter.
Queue *queue_initialize(void *data, size_t dataSize) {
  return dllist_initialization(data, dataSize);
}

// Enqueues a new item at the start of the queue.
int queue_enqueue(Queue *queue, void *data, size_t dataSize) {
  return dllist_push(queue, data, dataSize);
}

// Dequeues an item from the end of the queue.
DLNode *queue_dequeue(Queue *queue) { return dllist_pop(queue); }

// Exexcutes a simple xor swap on the variables a and b.
void xorswap(int *a, int *b) {
  // Set the value of a to the xor of a and b
  *a = *a ^ *b;
  // Set the value of b to the xor of itself and the xor of the new a and the
  // previous b.
  *b = *a ^ *b;
  // xor the new values of a and b again.
  *a = *a ^ *b;
}

// Recursively calculates the factorial of the number in the argument.
uint64_t factorial(uint64_t number) {
  if (number == 0 || number == 1) {
    return 1;
  } else {
    return factorial(number - 1) * number;
  }
}
// Calculate the factorial of the number in the argument with iteration.
uint64_t factorialNR(uint64_t number) {
  uint64_t fact = 1;
  for (uint64_t i = 2; i <= number; i++) {
    fact *= i;
  }
  return fact;
}

static void dynarr_deltefuncioninteger(void **data) { free(data); }
static void printfunc(DynamicArray *array, int length) {
  printf("[ ");
  for (int i = 0; i <= length; i++)
    printf("%d ", *((int *)dynarr_at(array, i)));
  printf("]\n");
}
int main(void) {
  int integers[] = {0, 1, 2, 3, 4, 5, 6, 7};
}
// https://en.wikipedia.org/wiki/Karatsuba_algorithm#:~:text=The%20Karatsuba%20algorithm%20was%20the,faster%2C%20for%20sufficiently%20large%20n.
// https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm
// http://www.cs.cornell.edu/courses/cs3410/2019sp/labs/lab10/
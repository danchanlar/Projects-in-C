#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#define N 3

// Structures

typedef struct State State;
struct State {
    int puzzle[N][N];
    int x, y;
    int cost;
    State *parent;
    int parentCost;
};

typedef struct {
    State *state;
    int priority;
} QueueNode;

typedef struct {
    QueueNode *nodes;
    int capacity;
    int size;
} PriorityQueue;

int goal[N][N] = {{6, 5, 4}, {7, 0, 3}, {8, 1, 2}};

//Prototypes of functions

PriorityQueue *createPriorityQueue(int capacity);
void heapifyUp(PriorityQueue *queue, int index);
void heapifyDown(PriorityQueue *queue, int index);
void enqueue(PriorityQueue *queue, State *state, int priority);
QueueNode dequeue(PriorityQueue *queue);
int misplaced_tiles(State *state, int goal[N][N]);
void printPuzzle(int puzzle[N][N]);
int compareStates(State *s1, State *s2);
int isInQueue(PriorityQueue *queue, State *s, int priority, int parentCost);
int isExpanded(State *s);
void markAsExpanded(State *s);
int isGoal(State *s);
void printPath(State *s);
void expand(State *current, PriorityQueue *open, int cost);
void expandAStar(State *current, PriorityQueue *open);
void UCS(State *initial);
void AStar(State *initial, int goal[N][N]);

// Creates a priority queue with a given capacity

PriorityQueue *createPriorityQueue(int capacity) {
    PriorityQueue *queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->nodes = (QueueNode *) malloc(capacity * sizeof(QueueNode));
    return queue;
}

// Ensures the min-heap property is maintained after insertion

void heapifyUp(PriorityQueue *queue, int index) {
    if (index == 0) {
        return;
    }
    
    int parent = (index - 1) / 2;
    
    // Swap with parent if the current node's priority is less than the parent's priority
    
    if (queue->nodes[index].priority < queue->nodes[parent].priority) {
        QueueNode temp = queue->nodes[index];
        queue->nodes[index] = queue->nodes[parent];
        queue->nodes[parent] = temp;
        
        heapifyUp(queue, parent);
    }
}

// Ensures the min-heap property is maintained after removal

void heapifyDown(PriorityQueue *queue, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int size = queue->size;

    // Check if left or right child has a smaller priority
    
    while (left < size) {
        if (queue->nodes[left].priority < queue->nodes[smallest].priority) {
            smallest = left;
        }
        if (right < size && queue->nodes[right].priority < queue->nodes[smallest].priority) {
            smallest = right;
        }
        if (smallest != index) {
            QueueNode temp = queue->nodes[index];
            queue->nodes[index] = queue->nodes[smallest];
            queue->nodes[smallest] = temp;
            index = smallest;
            left = 2 * index + 1;
            right = 2 * index + 2;
        } else {
            break;
        }
    }
}

// Inserts a new state into the priority queue with the given priority

void enqueue(PriorityQueue *queue, State *state, int priority) {
    if (queue->size == queue->capacity) {
        return;
    }
    QueueNode newNode = {state, priority};
    queue->nodes[queue->size++] = newNode;
    heapifyUp(queue, queue->size - 1);
}

// Removes and returns the state with the highest priority (lowest value)

QueueNode dequeue(PriorityQueue *queue) {
    QueueNode node = queue->nodes[0];
    queue->nodes[0] = queue->nodes[--queue->size];
    heapifyDown(queue, 0);
    return node;
}

//The heurestic function is about how many misplaced tiles away from the final state

int misplaced_tiles(State *state, int goal[N][N]) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state->puzzle[i][j] != -1 && state->puzzle[i][j] != goal[i][j]) {
                count++;
            }
        }
    }
    return count;
}

void printPuzzle(int puzzle[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", puzzle[i][j]);
        }
        printf("\n");
    }
}

int compareStates(State *s1, State *s2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (s1->puzzle[i][j] != s2->puzzle[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// Checks if a state with a given priority is already in the priority queue

int isInQueue(PriorityQueue *queue, State *s, int priority, int parentCost) {
    for (int i = 0; i < queue->size; i++) {
        if (compareStates(s, queue->nodes[i].state) && queue->nodes[i].priority <= priority + parentCost) {
            return 1;
        }
    }
    return 0;
}

State *expandedStates[200];
int numExpanded = 0;

// Checks if a state has already been expanded

int isExpanded(State *s) {
    for (int i = 0; i < numExpanded; i++) {
        if (compareStates(s, expandedStates[i])) {
            return 1;
        }
    }
    return 0;
}

// Marks a state as expanded by adding it to the expanded states list

void markAsExpanded(State *s) {
    expandedStates[numExpanded++] = s;
}

// Check if the current state is the goal or not

int isGoal(State *s) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (s->puzzle[i][j] != goal[i][j]) {
                printf("State is not the goal:\n");
                printPuzzle(s->puzzle);
                printf("\n");
                return 0;
            }
        }
    }
    printf("State is the goal:\n");
    printPuzzle(s->puzzle);
    printf("\n");
    return 1;
}

void printPath(State *s) {
    if (s == NULL) {
        return;
    }
    printPath((State *) s->parent);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", s->puzzle[i][j]);
        }
        printf("\n");
    }
    printf("Cost: %d\n", s->cost);
    printf("\n");

    free(s);
}

// Expands the current state, generating new states and adding them to the priority queue

void expand(State *current, PriorityQueue *open, int cost) {
    // All the possible moves in the puzzle
    int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[8] = {0, 0, -1, 1, -1, 1, -1, 1};
    for (int i = 0; i < 8; i++) {
        int newX = current->x + dx[i];
        int newY = current->y + dy[i];
        if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
            State *newState = (State *) malloc(sizeof(State));
            *newState = *current;
            newState->puzzle[current->x][current->y] = newState->puzzle[newX][newY];
            newState->puzzle[newX][newY] = 0;
            newState->x = newX;
            newState->y = newY;
            newState->cost++;
            newState->parent = current;
            newState->parentCost = current->cost;

            printf("Generated new state (cost %d):\n", newState->cost);
            printPuzzle(newState->puzzle);

            if (!isExpanded(newState) && !isInQueue(open, newState, newState->cost, newState->parentCost)) {
                enqueue(open, newState, newState->cost);
                markAsExpanded(newState);
            } else {
                free(newState);
            }
        }
    }
}

/* Expands the current state using the A* algorithm, generating new states and adding them to the priority queue 
with calculated priorities */

void expandAStar(State *current, PriorityQueue *open) {
    int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[8] = {0, 0, -1, 1, -1, 1, -1, 1};
    for (int i = 0; i < 8; i++) {
        int newX = current->x + dx[i];
        int newY = current->y + dy[i];
        if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
            State *newState = (State *) malloc(sizeof(State));
            *newState = *current;
            newState->puzzle[current->x][current->y] = newState->puzzle[newX][newY];
            newState->puzzle[newX][newY] = 0;
            newState->x = newX;
            newState->y = newY;
            newState->cost++;
            newState->parent = current;
            newState->parentCost = current->cost;

            int priority = newState->cost + misplaced_tiles(newState, goal);
            
            printf("Generated new state (cost %d, priority %d):\n", newState->cost, priority);
            printPuzzle(newState->puzzle);

            if (!isExpanded(newState) && !isInQueue(open, newState, priority, newState->parentCost)) {
                enqueue(open, newState, priority);
                markAsExpanded(newState);
            } else {
                free(newState);
            }
        }
    }
}

// UCS algorithm

void UCS(State *initial) {
    PriorityQueue *open = createPriorityQueue(1000);
    enqueue(open, initial, 0);

    while (open->size > 0) {
        QueueNode node = dequeue(open);
        State *current = node.state;

        expand(current, open, current->cost + 1);

        if (memcmp(current->puzzle, goal, sizeof(int) * N * N) == 0) {
            printf("Reached the goal state using UCS.\n");
            printPath(current);
            printf("Cost: %d\n", current->cost);
            printf("Number of expansions: %d\n", open->size);
            return;
        } else {
            printf("Did not reach the goal state.\n");
        }
    }

    free(initial);
}

// A* algorithm

void AStar(State *initial, int goal[N][N]) {
    PriorityQueue *open = createPriorityQueue(1000);
    enqueue(open, initial, 0 + misplaced_tiles(initial, goal));

    while (open->size > 0) {
        QueueNode node = dequeue(open);
        State *current = node.state;

        expandAStar(current, open);

        if (memcmp(current->puzzle, goal, sizeof(int) * N * N) == 0) {
            printf("Reached the goal state using A*.\n");
            printPath(current);
            printf("Cost: %d\n", current->cost);
            printf("Number of expansions: %d\n", open->size);
            return;
        } else {
            printf("Did not reach the goal state.\n");
        }
    }

    free(initial);
}

int main() {
    State *initial = (State *) malloc(sizeof(State));
    char input[100];
    printf("Enter initial state in a single line (e.g., 6 5 4 7 1 3 8 0 2):\n");
    fgets(input, 100, stdin);

    char *token = strtok(input, " ");
    int k = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (token != NULL) {
                initial->puzzle[i][j] = atoi(token);
                if (initial->puzzle[i][j] == 0) {
                    initial->x = i;
                    initial->y = j;
                }
                token = strtok(NULL, " ");
            }
        }
    }
    initial->cost = 0;
    initial->parent = NULL;

    printf("Initial state:\n");
    printPuzzle(initial->puzzle);

    int choice;
    printf("Choose algorithm:\n");
    printf("1. UCS\n");
    printf("2. A*\n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nUsing UCS:\n");
        UCS(initial);
    } else if (choice == 2) {
        printf("\nUsing A*:\n");
        AStar(initial, goal);
    } else {
        printf("Invalid choice.\n");
    }

    free(initial);

    return 0;
}


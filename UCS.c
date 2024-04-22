#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_STATES 362880 // Maximum number of possible states for 3x3 puzzle (9!)
#define PRIME 31  // A prime number used for hashing
#define N 3
int i,j;

// Structure to represent each state of the puzzle
typedef struct PuzzleState {
    int board[N][N];
    int zero_row, zero_col; // Position of the empty tile
    int cost; // Cost to reach this state
    struct PuzzleState* parent; // Parent state
} PuzzleState;

// Structure to represent a node in the priority queue
typedef struct {
    PuzzleState* state;
    int priority;
} QueueNode;

// Structure for the priority queue
typedef struct {
    QueueNode* array[MAX_STATES];
    int size;
} PriorityQueue;


// Function to create a new puzzle state
PuzzleState* createPuzzleState(int board[N][N], int zero_row, int zero_col, int cost, PuzzleState* parent) {
    PuzzleState* state = (PuzzleState*)malloc(sizeof(PuzzleState));
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            state->board[i][j] = board[i][j];
        }
    }
    state->zero_row = zero_row;
    state->zero_col = zero_col;
    state->cost = cost;
    state->parent = parent;
    return state;
}

// Function to check if two states are equal
bool areStatesEqual(PuzzleState* state1, PuzzleState* state2) {
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (state1->board[i][j] != state2->board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to check if a state is the goal state
bool isGoalState(PuzzleState* state, int goal[N][N]) {
    return areStatesEqual(state, createPuzzleState(goal, 0, 0, 0, NULL));
}

// Function to swap two tiles in the puzzle board
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Function to perform a move in the puzzle
PuzzleState* performMove(PuzzleState* state, int new_row, int new_col) {
    int board[N][N];
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            board[i][j] = state->board[i][j];
        }
    }
    swap(&board[state->zero_row][state->zero_col], &board[new_row][new_col]);
    return createPuzzleState(board, new_row, new_col, state->cost + 1, state);
}


// Function to print the puzzle board
void printBoard(int board[N][N]) {
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to print the solution path
void printSolutionPath(PuzzleState* goalState) {
    if (goalState == NULL) return;
    printSolutionPath(goalState->parent);
    printBoard(goalState->board);
    printf("\n");
}

// Function to create an empty priority queue
PriorityQueue* createPriorityQueue() {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->size = 0;
    return queue;
}

// Function to check if the priority queue is empty
bool isQueueEmpty(PriorityQueue* queue) {
    return queue->size == 0;
}

// Function to enqueue a state with its priority
void enqueue(PriorityQueue* queue, PuzzleState* state, int priority) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->state = state;
    newNode->priority = priority;
    queue->array[queue->size] = newNode;
    queue->size++;
}

// Function to dequeue the state with the lowest priority
PuzzleState* dequeueMin(PriorityQueue* queue) {
    if (isQueueEmpty(queue)) return NULL;
    int min_priority = INT_MAX;
    int min_index = 0;
    for (i = 0; i < queue->size; i++) {
        if (queue->array[i]->priority < min_priority) {
            min_priority = queue->array[i]->priority;
            min_index = i;
        }
    }
    PuzzleState* minState = queue->array[min_index]->state;
    free(queue->array[min_index]);
    for (i = min_index; i < queue->size - 1; i++) {
        queue->array[i] = queue->array[i + 1];
    }
    queue->size--;
    return minState;
}

// Function to create an empty explored set
bool exploredSet[MAX_STATES] = {false};


// Function to calculate a hash value for a puzzle state
int calculateHash(PuzzleState* state) {
    int hash = 0;
    int multiplier = 1;
    
    // Iterate over each element in the puzzle board
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            // Update the hash value using the current element
            hash = (hash + state->board[i][j] * multiplier) % MAX_STATES;
            multiplier = (multiplier * PRIME) % MAX_STATES;
        }
    }
    
    return hash;
}

// Function to mark a state as explored
void markAsExplored(PuzzleState* state) {
    // Calculate a unique identifier for the state (e.g., hash function)
    int hash = calculateHash(state);
    exploredSet[hash] = true;
}

// Function to check if a state has been explored
bool isExplored(PuzzleState* state) {
    // Calculate a unique identifier for the state (e.g., hash function)
    int hash = calculateHash(state);
    return exploredSet[hash];
}

// Function to apply Uniform Cost Search algorithm
void UCS(int initial[N][N], int goal[N][N]) {
	int dr,dc;
    // Define initial state and goal state
    PuzzleState* initialState = createPuzzleState(initial, 0, 0, 0, NULL);
    // Create an empty priority queue
    PriorityQueue* priorityQueue = createPriorityQueue();

    // Enqueue initial state with priority 0
    enqueue(priorityQueue, createPuzzleState(initial, 0, 0, 0, NULL), 0);
    
    // Priority queue for storing states to be explored
    // Implement your priority queue here

    // Implement enqueue function for your priority queue
    
    // Define explored set
    bool exploredSet[MAX_STATES] = {false};
    // Implement your explored set here

    // Implement UCS algorithm here
    while (!isQueueEmpty(priorityQueue)) {
        // Dequeue the state with the lowest cost
        PuzzleState* currentState = dequeueMin(priorityQueue);
        
        // If the current state is the goal state, print the solution path and return
        if (isGoalState(currentState, goal)) {
            printf("Solution Path:\n");
            printSolutionPath(currentState);
            return;
        }
	
        
    	// Explore all possible moves from the current state
		for (dr = -1; dr <= 1; dr++) {
    		for (dc = -1; dc <= 1; dc++) {
        		// Ignore moves that are not valid
        		if (abs(dr) + abs(dc) != 1 && abs(dr) + abs(dc) != 2) continue;
        
        		int new_row = currentState->zero_row + dr;
        		int new_col = currentState->zero_col + dc;
        
        		// Ignore moves that are out of bounds
        		if (new_row < 0 || new_row >= N || new_col < 0 || new_col >= N) continue;
        
        		// Perform the move and create a new state
        		PuzzleState* newState = performMove(currentState, new_row, new_col);
        
        		// Check if the new state has already been explored
        		// If not, enqueue it into the priority queue
        		if (!isExplored(newState)) {
            		enqueue(priorityQueue, newState, newState->cost);
            		markAsExplored(newState);
        		} else {
            		// Free memory if the state has been explored
            		free(newState);
        		}
        	}
    	}
    }
    
    // If no solution is found
    printf("No solution found!\n");

    // Free memory
    free(initialState);
}


int main(){
    // Define initial and goal states
    int initial[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    int goal[N][N] = {{6, 5, 4}, {7, 0, 3}, {8, 1, 2}};
    
    // Call UCS algorithm
    UCS(initial, goal);
    
    return 0;
}


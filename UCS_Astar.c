#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 3
#define MAX_STATES 362880

int goal[N][N] = {{6, 5, 4}, {7, 0, 3}, {8, 1, 2}};

int dr, dc;

typedef struct boardState {
    int board[N][N];
    int row, col;
    int cost;                
    int heuristic;          
    struct boardState *parent;
} boardState;

typedef struct {
    boardState *array[MAX_STATES];
    int size;
} PriorityQueue;

int misplacedTiles(int board[N][N], int goal[N][N]);

boardState *createBoardState(int board[N][N], int row, int col, int cost, boardState *parent) {
    boardState *state = (boardState *)malloc(sizeof(boardState));
    if (state == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            state->board[i][j] = board[i][j];
        }
    }

    state->row = row;
    state->col = col;
    state->cost = cost;
    state->heuristic = misplacedTiles(state->board, goal);
    state->parent = parent;

    return state;
}

bool areStatesEqual(boardState *state1, boardState *state2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state1->board[i][j] != state2->board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool isGoalState(boardState *state, int goal[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state->board[i][j] != goal[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

boardState *performMove(boardState *state, int new_row, int new_col) {
    int board[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = state->board[i][j];
        }
    }
    swap(&board[state->row][state->col], &board[new_row][new_col]);
    return createBoardState(board, new_row, new_col, state->cost + 1, state);
}

int misplacedTiles(int board[N][N], int goal[N][N]) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != goal[i][j] && board[i][j] != 0) {
                count++;
            }
        }
    }
    return count;
}

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void printSolutionPath(boardState *goalState) {
    if (goalState == NULL) return;
    printSolutionPath(goalState->parent);
    printBoard(goalState->board);
    printf("Cost: %d\n\n", goalState->cost);
}

PriorityQueue *createPriorityQueue() {
    PriorityQueue *queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->size = 0;
    return queue;
}

bool isPriorityQueueEmpty(PriorityQueue *queue) {
    return queue->size == 0;
}

void enqueue(PriorityQueue *queue, boardState *state) {
    if (queue->size == MAX_STATES) {
        fprintf(stderr, "Queue overflow\n");
        exit(EXIT_FAILURE);
    }
    queue->array[queue->size++] = state;
}

boardState *dequeue(PriorityQueue *queue) {
    if (isPriorityQueueEmpty(queue)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }
    int min_cost_index = 0;
    for (int i = 1; i < queue->size; i++) {
        if ((queue->array[i]->cost + queue->array[i]->heuristic) < (queue->array[min_cost_index]->cost + queue->array[min_cost_index]->heuristic)) {
            min_cost_index = i;
        }
    }

    boardState *state = queue->array[min_cost_index];
    queue->array[min_cost_index] = queue->array[queue->size - 1];
    queue->size--;

    return state;
}

// Function to apply A* algorithm
void aStarSearch(boardState *initial, int goal[N][N]) {
    // Create an empty priority queue
    PriorityQueue *queue = createPriorityQueue();
    int statesExplored = 0; // Counter for states explored

    // Enqueue initial state
    enqueue(queue, initial);

    // Implement A* algorithm here
    while (!isPriorityQueueEmpty(queue)) {
        // Dequeue a state
        boardState *currentState = dequeue(queue);
        statesExplored++;

        // If the current state is the goal state, print the solution path and return
        if (isGoalState(currentState, goal)) {
            printf("Solution Path:\n");
            printSolutionPath(currentState);
            printf("Total Cost: %d\n", currentState->cost); // Print total cost
            printf("States explored: %d\n", statesExplored);
            return;
        }

        // Explore all possible moves from the current state
        for (dr = -1; dr <= 1; dr++) {
            for (dc = -1; dc <= 1; dc++) {
                // Ignore moves that are not valid or diagonal
                if ((dr == 0 && dc == 0) || (abs(dr) + abs(dc) != 1))
                    continue;

                int new_row = currentState->row + dr;
                int new_col = currentState->col + dc;

                // Ignore moves that are out of bounds
                if (new_row < 0 || new_row >= N || new_col < 0 || new_col >= N)
                    continue;

                // Perform the move and create a new state
                boardState *newState = performMove(currentState, new_row, new_col);

                // Enqueue the new state
                enqueue(queue, newState);
            }
        }
    }

    // If no solution is found
    printf("No solution found!\n");
    printf("States explored: %d\n", statesExplored);
}


void initializeBoardState(boardState *initial, int goal[N][N]) {
    char input[100];
    printf("Enter initial state in a single line (e.g., 6 5 4 7 1 3 8 0 2):\n");
    fgets(input, 100, stdin);

    char *token = strtok(input, " ");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (token != NULL) {
                initial->board[i][j] = atoi(token);
                if (initial->board[i][j] == 0) {
                    initial->row = i;
                    initial->col = j;
                }
                token = strtok(NULL, " ");
            }
        }
    }
    initial->cost = 0;
    initial->heuristic = misplacedTiles(initial->board, goal);
    initial->parent = NULL;
}

void uniformCostSearch(boardState *initial, int goal[N][N]) {
    // Create an empty priority queue
    PriorityQueue *queue = createPriorityQueue();
    int statesExplored = 0; // Counter for states explored

    // Enqueue initial state
    enqueue(queue, initial);

    // Implement UCS algorithm here
    while (!isPriorityQueueEmpty(queue)) {
        // Dequeue a state
        boardState *currentState = dequeue(queue);
        statesExplored++;

        // If the current state is the goal state, print the solution path and return
        if (isGoalState(currentState, goal)) {
            printf("Solution Path:\n");
            printSolutionPath(currentState);
            printf("Total Cost: %d\n", currentState->cost); // Print total cost
            printf("States explored: %d\n", statesExplored);
            return;
        }

        // Explore all possible moves from the current state
        for (dr = -1; dr <= 1; dr++) {
            for (dc = -1; dc <= 1; dc++) {
                // Ignore the move that does not change the position
                if (dr == 0 && dc == 0)
                    continue;

                int new_row = currentState->row + dr;
                int new_col = currentState->col + dc;

                // Ignore moves that are out of bounds
                if (new_row < 0 || new_row >= N || new_col < 0 || new_col >= N)
                    continue;

                // Perform the move and create a new state
                boardState *newState = performMove(currentState, new_row, new_col);

                // Enqueue the new state
                enqueue(queue, newState);
            }
        }
    }

    // If no solution is found
    printf("No solution found!\n");
    printf("States explored: %d\n", statesExplored);
}

int main() {
    int choice;
    printf("*****************************************************************************\n");
    printf("-----------------------------------------------------------------------------\n");
    int goal[N][N] = {{6, 5, 4}, {7, 0, 3}, {8, 1, 2}};
    boardState *initial = (boardState *)malloc(sizeof(boardState));
    initializeBoardState(initial, goal);
    printf("Choose algorithm\n");
    printf("1.UCS\n");
    printf("2.A*\n");
    scanf("%d", &choice);
    if(choice == 1){
        printf("Initial state:\n");
        uniformCostSearch(initial, goal);
    }else if(choice==2){
        printf("Initial state:\n");
        aStarSearch(initial, goal);
    }else{
        printf("Invalid input\n");
        printf("Choose algorithm\n");
        printf("1.UCS\n");
        printf("2.A*\n");
    }
    printBoard(initial->board);
    free(initial);
    printf("*****************************************************************************\n");
    printf("-----------------------------------------------------------------------------");
    return 0;
}


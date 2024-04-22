#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3
int i,j,k;
// Define possible moves (up, down, left, right, and diagonal)
int moves[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

// Define the goal state
int goal_state[N][N] = {{6, 5, 4}, {7, 0 , 3}, {8, 1, 2}};

// Calculate the number of misplaced tiles (heuristic function)
int misplaced_tiles(int state[N][N]) {
    int count = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (state[i][j] != goal_state[i][j]) {
                count++;
            }
        }
    }
    return count;
}

// A* Algorithm
int astar(int start[N][N]) {
    int visited[N][N];
    memset(visited, 0, sizeof(visited));
    
    typedef struct {
        int priority;
        int state[N][N];
    } Node;
    
    Node current_node;
    current_node.priority = misplaced_tiles(start);
    memcpy(current_node.state, start, sizeof(start));
    
    while (1) {
        // Check if current state is the goal state
        int is_goal = 1;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (current_node.state[i][j] != goal_state[i][j]) {
                    is_goal = 0;
                    break;
                }
            }
            if (!is_goal) break;
        }
        if (is_goal) return 1;
        
        // Explore possible moves
        for (k = 0; k < 8; k++) {
            int new_state[N][N];
            memcpy(new_state, current_node.state, sizeof(current_node.state));
            int x = 0, y = 0;
            // Find the position of empty cell
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++) {
                    if (current_node.state[i][j] == 0) {
                        x = i + moves[k][0];
                        y = j + moves[k][1];
                        break;
                    }
                }
            }
            if (x >= 0 && x < N && y >= 0 && y < N) {
                // Swap empty cell with neighboring cell
                int temp = new_state[x][y];
                new_state[x][y] = new_state[x - moves[k][0]][y - moves[k][1]];
                new_state[x - moves[k][0]][y - moves[k][1]] = temp;
                // Check if the new state has been visited
                int is_visited = 0;
                for (i = 0; i < N; i++) {
                    for (j = 0; j < N; j++) {
                        if (visited[i][j] == 0 && new_state[i][j] != current_node.state[i][j]) {
                            is_visited = 1;
                            break;
                        }
                    }
                    if (is_visited) break;
                }
                if (!is_visited) {
                    // Update priority and add to visited
                    int priority = misplaced_tiles(new_state);
                    visited[x][y] = 1;
                    Node new_node;
                    new_node.priority = priority;
                    memcpy(new_node.state, new_state, sizeof(new_state));
                    if (priority < current_node.priority) {
                        current_node = new_node;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

// Example usage
int main() {
    int initial_state[N][N] = {{1, 2, 0}, {3, 4, 5}, {6, 7, 8}};

    if (astar(initial_state)) {
        printf("Solution found:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", initial_state[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("No solution exists.\n");
    }

    return 0;
}


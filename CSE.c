#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NICKNAME_SIZE_MAX 5

int i,j=0;

typedef struct player_properties{
    char nickname[NICKNAME_SIZE_MAX + 1];
    char mark;
    int score;
    int status;
    int row;
    int col;
} PLAYER;

void get_nickname(char *nickname, int size);
void get_move(char board[3][3], char *nickname, int *row, int *col);
void display_board(char board[3][3]);
int is_winner(char board[3][3], PLAYER player);

int main(void){
    const int MAX_MOVES = 9;
    int keepPlaying = 1;
    char answer, board[3][3];
    
    // Get info for player 1
    PLAYER player1;
    printf("Player 1:\n");
    get_nickname(player1.nickname, NICKNAME_SIZE_MAX);
    player1.mark = 'x';
    player1.score = 0;
    
    // Get info for player 2
    PLAYER player2;
    system("clear"); // For Windows use system("cls");
    printf("Player 2:\n");
    get_nickname(player2.nickname, NICKNAME_SIZE_MAX);
    player2.mark = 'o';
    player2.score = 0;

    while (keepPlaying){
        // (Re)-initialize board
        for (i=0; i < 3; i++){
            for (j=0; j < 3; j++){
            board[i][j] = ' ';
            }
        }
        // (Re)-set moves played in game
        int moves = 0;
        
        // (Re)-set players' properties
        player1.status = -1;
        player1.row = player1.col = -1;
        player2.status = -1;
        player2.row = player2.col = -1;
        
        while (moves < MAX_MOVES){
            if (moves == 0){
                system("clear"); // For Windows use system("cls");
                display_board(board); // display empty board
            }
            
            /* Player 1 makes a move */
            get_move(board, player1.nickname, &player1.row, &player1.col);
            board[player1.row][player1.col] = player1.mark;
            moves++;
            system("clear"); // For Windows use system("cls");
            display_board(board);
            
            /* needs minimum of 5 moves before someone can win */
            if (moves >= 5){
                /* check if player 1 wins */
                if (is_winner(board, player1)){
                    player1.score++;
                    player1.status = 1;
                    player2.status = 0;
                    goto RESULT;
                }
            }
            
            /* Player 2 makes a move */ 
            if (moves == MAX_MOVES) goto RESULT; // last move goes to player 1
            get_move(board, player2.nickname, &player2.row, &player2.col);
            board[player2.row][player2.col] = player2.mark;
            moves++;
            system("clear"); // For Windows use system("cls");
            display_board(board);
            
            /* needs minimum of 5 moves before someone can win */
            if (moves >= 5){
                /* check if player 2 wins */
                if (is_winner(board, player2)){
                    player2.score++;
                    player2.status = 1;
                    player1.status = 0;
                    goto RESULT;
                }
            }
        }
        RESULT:
            if (player1.status == 1) {
                printf("%s WINS!\n", player1.nickname);
            }
            else if (player2.status == 1) {
                printf("%s WINS!\n", player2.nickname);
            }
            else {
                printf("It's a draw!\n");
            }
            EXIT:
                printf("Do you want to play again? [y/n] ");
                scanf(" %c", &answer);
                switch (answer){
                    case 'N':
                    case 'n':
                        keepPlaying = 0;
                        printf("Final score: \n");
                        printf("-----------\n");
                        printf("%s : %d pts\n", player1.nickname, player1.score);
                        printf("%s : %d pts\n", player2.nickname, player2.score);
                        break;
                    case 'Y':
                    case 'y':
                        break;
                    default:
                        printf("ERROR: Invalid answer\n");
                        goto EXIT;
                }
    }

    return 0;
}


void get_move(char board[3][3], char *nickname, int *row, int *col){
    int i, j;
    do {
        printf("%s, it's your turn! Please give coordinates (row column): ", nickname);
        scanf(" %d %d", &i, &j);
        while(getchar() != '\n'); // empty stdin buffer
    } while (i < 0 || i > 2 || j < 0 || j > 2 || board[i][j] != ' '); /* Valid move cannot be out of board boundaries, or occupied position */
    *row = i;
    *col = j;
    return;
}


int is_winner(char board[3][3], PLAYER player){
    // Check rows
    for (i=0; i < 3; i++){
        if ((board[i][0] == board[i][1]) && (board[i][0] == board[i][2]) && (board[i][0] == player.mark)){
            return 1;
        }
    }
    // Check columns
    for (j =0; j < 3; j++){
        if ((board[0][j] == board[1][j]) && (board[0][j] == board[2][j]) && (board[0][j] == player.mark)){
            return 1;
        }
    }
    // Check diagonals
    if ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2]) && (board[0][0] == player.mark)){
        return 1;
    }
    else if ((board[0][2] == board[1][1]) && (board[0][2] == board[2][0]) && (board[0][2] == player.mark)){
        return 1;
    }
    return 0;
}


void display_board(char board[3][3]){
    printf("\t0\t1\t2\t\n");
    printf("0\t%c   |\t%c   |\t%c\t\n", board[0][0], board[0][1], board[0][2]);
    printf("     -----------------------\n");
    printf("1\t%c   |\t%c   |\t%c\t\n", board[1][0], board[1][1], board[1][2]);
    printf("     -----------------------\n");
    printf("2\t%c   |\t%c   |\t%c\t\n", board[2][0], board[2][1], board[2][2]);
    return;
}


void get_nickname(char *nickname, int size){
    while (1){
        printf("Enter a nickname up to %d characters long: ", size);
        scanf(" %s", nickname);
        while(getchar() != '\n'); // empty stdin buffer
        if (strlen(nickname) <= size) break;
        printf("Nickname cannot be longer than %d characters\n", size);
    }
    return;
}

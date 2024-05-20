#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum { MAX = 2, MIN = 1,N = 3, TIE = 0,GAME_OVER = 2,CONTINUE = 1 };


struct inform
{  struct inform *kinisis[21];
   int next;
   int vacancies; 
   char board[N][N]; 
};

int finalStateCheck(char board[N][N]) {
    for (int i = 0; i < 3; i++) {
        if ((board[0][i] == 'C' && board[1][i] == 'S' && board[2][i] == 'E') 
        ||(board[0][i] == 'E' && board[1][i] == 'S' && board[2][i] == 'C') 
        ||(board[i][0] == 'C' && board[i][1] == 'S' && board[i][2] == 'E') 
        ||(board[i][0] == 'E' && board[i][1] == 'S' && board[i][2] == 'C')) {
            return GAME_OVER;
        }
    }
    if ((board[0][0] == 'C' && board[1][1] == 'S' && board[2][2] == 'E') 
    ||(board[0][0] == 'E' && board[1][1] == 'S' && board[2][2] == 'C') 
    ||(board[2][0] == 'C' && board[1][1] == 'S' && board[0][2] == 'E') 
    ||(board[2][0] == 'E' && board[1][1] == 'S' && board[0][2] == 'C')) {
        return GAME_OVER;
    }
    return CONTINUE;
}

int makeChildren(struct inform *tree, int seira, int yorai, int evaluation[21]);
int tree_MinMax_tree(struct inform *tree, int seira)
{ 
    int yorai;
    int bestPlay;
    int bestChild; 
    int position = 0;
    int evaluation[21]; 
    int i;

    if (finalStateCheck(tree->board) == GAME_OVER && seira == MAX) 
        return MIN;
    else if (finalStateCheck(tree->board) == GAME_OVER && seira == MIN) 
        return MAX;
    else if (tree->vacancies == 0) 
        return TIE;

    
    if (seira == MAX)
        yorai = MIN;
    else
        yorai = MAX;

    position = makeChildren(tree, seira, yorai,evaluation); 

    if (seira == MAX) 
    {  
        bestPlay= evaluation[0];
        bestChild= 0;
        for (i = 0; i < position; i++)
        {  
            if (bestPlay < evaluation[i]) 
            {  
                bestPlay = evaluation[i];
                bestChild= i;
            }
        }
    }
    else 
    {  
        bestPlay =evaluation[0];
        bestChild= 0;
        for (i = 1; i < position; i++)
        {  
            if (evaluation[i] < bestPlay) 
            {  
                bestPlay=evaluation[i];
                bestChild= i;
            }
        }
    }
    tree->next = bestChild;
    return bestPlay;
}

int makeChildren(struct inform *tree, int seira, int yorai, int evaluation[21])
{
    
    int position = 0;
    int i,j,k;
    char options[]={'E','S','C'};
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            if (tree->board[i][j] == '?'){
                for (int k = 0; k < 3; k++) {
                   tree->kinisis[position] = (struct inform *)malloc(sizeof(struct inform));


                    void copy_board(char A[3][3],char B[3][3])
                        {  int i,j;
                            for(i=0;i<3;i++)
                                 for(j=0;j<3;j++)
                                     B[i][j]=A[i][j];

 }
                    
                    
                    tree->kinisis[position]->vacancies = tree->vacancies - 1;
                    copy_board(tree->board, tree->kinisis[position]->board);
                    tree->kinisis[position]->board[i][j] = options[k];
                    evaluation[position] = tree_MinMax_tree(tree->kinisis[position],yorai);
                    position++;
                }
            }
        }
    }

    return position; 
}


void user_move(struct inform *state) {
    int row, col;
    char letter;

    printf("Choose the row for the first letter (0-first row/1-second row/2-third row): ");
    scanf("%d", &row);
    printf("Choose the col for the first letter (0-first col/1-second col/2-third col): ");
    scanf("%d", &col);

    while (state->board[row][col] != '?') {
        printf("The position is occupied or you are outside the coordinates of the table.\nChoose the row for the first letter (0-first row/1-second row/2-third row): ");
        scanf("%d", &row);
        printf("Choose the col for the first letter (0-first col/1-second col/2-third col): ");
        scanf("%d", &col);
    }

    printf("Choose between C , S , E: ");
    scanf(" %c", &letter);


    while (letter != 'C' && letter != 'S' && letter != 'E') {
        printf("Wrong letter.Choose between C , S , E:");
        scanf(" %c ", &letter);
        
    }

    state->board[row][col] = letter;
    state->vacancies--;
}

void print_board(char A[N][N])
{  int i,j;
   for(i=0;i<3;i++)
   {  for(j=0;j<3;j++)
      {  printf("%c ",A[i][j]);
      }
      printf("\n");
   }
   printf("_______________________\n");
}

void game_preparation(struct inform *state)
{
    int i, j;

    state->vacancies = 8;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            state->board[i][j] = '?';
        }
    }

    printf("The table should start with S in one of the positions left or right of the middle one.\nPress 1 for left or 2 for right: ");
    int grammi;
    scanf("%d", &grammi);
    while (grammi != 1 && grammi != 2)
    {
        printf("Incorrect input. Choose between 1 or 2: ");
        scanf("%d", &grammi);
    }

    if (grammi == 1)
        state->board[1][0] = 'S';
    else
        state->board[1][2] = 'S';

    printf("GET READY. THE GAME HAS JUST STARTED!!! \n\n");
    print_board(state->board);
}
void game_start() {
    struct inform *state = (struct inform *)malloc(sizeof(struct inform));
    int seira = MIN;
    
    game_preparation(state);

    while (1) {
        if (seira == MAX && finalStateCheck(state->board) == GAME_OVER) {
            printf("USER WON\n\n ");
            printf("GAME OVER\n\n ");
            break;
        } else if (seira == MIN && finalStateCheck(state->board) == GAME_OVER) {
            printf("COMPUTER WON\n\n ");
            printf("GAME OVER\n\n ");
            break;
        } else if (state->vacancies == 0) {
            printf("TIE\n\n ");
            printf("GAME OVER\n\n ");
            break;
        }

        if (seira == MAX) {
            printf("COMPUTER TURN\n______________________\n");
            tree_MinMax_tree(state, MAX);
            state = state->kinisis[state->next];
            print_board(state->board);
            seira = MIN;
        } else {
            printf("USER TURN\n_________________________\n");
            user_move(state);
            print_board(state->board);
            seira = MAX;
        }
    }

    
    free(state);
}


int main() {
    printf("*****************************************************************************\n-----------------------------------------------------------------------------\n");
    game_start();
    printf("*****************************************************************************\n-----------------------------------------------------------------------------");
    return 0;
}


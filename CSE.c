#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum { MAX = 2, MIN = 1,N = 3, TIE = 0,GAME_OVER = 2,CONTINUE = 1 };

int yorai;
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
int gameOvercheck(struct inform *tree, int turn){
    if (finalStateCheck(tree->board) == GAME_OVER && turn == MAX) 
        return MIN;
    else if (finalStateCheck(tree->board) == GAME_OVER && turn == MIN) 
        return MAX;
    else if (tree->vacancies == 0) 
        return TIE;

    return -1;
}
int nextMove (int turn,int yorai){
   if (turn == MAX)
        yorai = MIN;
    else
        yorai = MAX; 
}
int makeChildren(struct inform *tree, int turn, int yorai, int evaluation[21]);
int tree_MinMax_tree(struct inform *tree, int turn)
{ 

    int bestPlay;
    int bestChild; 
    int evaluation[21]; 
    int position = 0;
    int i;

    
    int gameOvercheck(struct inform *tree, int turn);
    
    int nextMove (int turn,int yorai);

    position = makeChildren(tree, turn, yorai,evaluation); 

    if (turn == MAX) 
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

int makeChildren(struct inform *tree, int turn, int yorai, int evaluation[21])
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


void user_move(struct inform *cond) {
    int row, col;
    char letter;

    printf("?p????te t? ??aµµ? p?? ?a t?p??et?sete t? ???µµa (0-???t? G?aµµ?/1-?e?te?? G?aµµ?/2-???t? G?aµµ?): ");
    scanf("%d", &row);
    printf("?p????te t? st??? p?? ?a t?p??et?sete t? ???µµa (0-???t? St???/1-?e?te?? St???/2-???t? St???): ");
    scanf("%d", &col);

    while (cond->board[row][col] != '?') {
        printf("? ??s? e??a? ?ate???µ??? ? ß??s?este e?t?? t?? s??teta?µ???? t?? p??a?a.\n?p????te t? ??aµµ? p?? ?a t?p??et?sete t? ???µµa (0-???t? G?aµµ?/1-?e?te?? G?aµµ?/2-???t? G?aµµ?): ");
        scanf("%d", &row);
        printf("?p????te ?a?? t? st??? p?? ?a t?p??et?sete t? ???µµa (0-???t? St???/1-?e?te?? St???/2-???t? St???): ");
        scanf("%d", &col);
    }

    printf("?p????te C ? S ? E: ");
    scanf(" %c", &letter);


    while (letter != 'C' && letter != 'S' && letter != 'E') {
        printf("????? ???µµa. ?p????te C ? S ? E");
        scanf(" %c ", &letter);
        
    }

    cond->board[row][col] = letter;
    cond->vacancies--;
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

void game_preparation(struct inform *cond)
{
    int i, j;

    cond->vacancies = 9;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            cond->board[i][j] = '?';
        }
    }

    printf("? p??a?a? ?a p??pe? ?a ?e????e? µe t? S se µ?a ap? t?? ??se?? a??ste?? ? de??? t?? µesa?a? .\n??ta 1 ??a t?? a??ste?? ? 2 ??a t?? de???: ");
    int grammi;
    scanf("%d", &grammi);
    while (grammi != 1 && grammi != 2)
    {
        printf("????? e?s?d??. ??a???te a??µesa se 1 ? 2: ");
        scanf("%d", &grammi);
    }

    if (grammi == 1)
        cond->board[1][0] = 'S';
    else
        cond->board[1][2] = 'S';
    cond->vacancies--;
    printf("??????S?????. ?? ???????? ????S ??????S?!!! \n\n");
    print_board(cond->board);
}
void game_start() {
    struct inform *cond = (struct inform *)malloc(sizeof(struct inform));
    int turn = MIN;
    
    game_preparation(cond);

    while (1) {
        if (turn == MAX && finalStateCheck(cond->board) == GAME_OVER) {
            printf("? ???S??S ?????S?\n\n ");
            printf("????S ??????????\n\n ");
            break;
        } else if (turn == MIN && finalStateCheck(cond->board) == GAME_OVER) {
            printf("? ?????G?S??S ?????S?\n\n ");
            printf("????S ??????????\n\n ");
            break;
        } else if (cond->vacancies == 0) {
            printf("?S??????\n\n ");
            printf("????S ??????????\n\n ");
            break;
        }

        if (turn == MAX) {
            printf("S???? ??? ?????G?S??\n______________________\n");
            tree_MinMax_tree(cond, MAX);
            cond = cond->kinisis[cond->next];
            print_board(cond->board);
            turn = MIN;
        } else {
            printf("S???? ??? ???S??\n_________________________\n");
            user_move(cond);
            print_board(cond->board);
            turn = MAX;
        }
    }

    
    free(cond);
}


int main() {
    printf("*****************************************************************************\n-----------------------------------------------------------------------------\n");
    game_start();
    printf("*****************************************************************************\n-----------------------------------------------------------------------------");
    return 0;
}


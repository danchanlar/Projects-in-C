#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1 // enallaktika +C (kerdise o MAX)
#define MIN -1 // enallaktika -C (kerdise o MIN)
#define TIE 0  // (isopalia)

#define GAME_END 100 // dikia sas timi
#define GAME_CONTINUE 200 // dikia sas timi

struct min_max
{  struct min_max *pointers[24]; // tha vrethoun stin prosomoiosi kai pointers einai 24 to poli paidia (24 to poli kiniseis)
   int next;   // tha vrethei stin prosomoiosi "dentro_MINMAX" kai next einai to index (0 ews 23 to poli) tou paidiou pou einai i epomeni kinisi
   int free_boxes;  // plithos kenon mesa sto plegma 3x3 (ara otan ginei 0, simainei oti den mporei na sinexistei to paihnidi)
   char table[3][3]; // plegma 3x3 (periexei kapoia apo ta C,S,E stis 9 theseis)
};

int check_CSE_ESC(char table[3][3])
{  if (table[0][0]=='C' && table[0][1]=='S' && table[0][2]=='E')  // grammi: 1 to CSE
   {  return (GAME_END);
   }
   else if (table[0][0]=='E' && table[0][1]=='S' && table[0][2]=='C')  // grammi: 1 to ESC
   {  return (GAME_END);
   }
   else if (table[2][0]=='C' && table[2][1]=='S' && table[2][2]=='E') // grammi: 3 to CSE
   {  return (GAME_END);
   }
   else if (table[2][0]=='E' && table[2][1]=='S' && table[2][2]=='C') // grammi: 3 to ESC
   {  return (GAME_END);
   }
   else if (table[0][0]=='C' && table[1][0]=='S' && table[2][0]=='E') // stili: 1 to CSE
   {  return (GAME_END);
   }
   else if (table[0][0]=='E' && table[1][0]=='S' && table[2][0]=='C') // stili: 1 to ESC
   {  return (GAME_END);
   }
   else if (table[0][1]=='C' && table[1][1]=='S' && table[2][1]=='E') // stili: 2 to CSE
   {  return (GAME_END);
   }
   else if (table[0][1]=='E' && table[1][1]=='S' && table[2][1]=='C') // stili: 2 to ESC
   {  return (GAME_END);
   }
   else if (table[0][2]=='C' && table[1][2]=='S' && table[2][2]=='E') // stili: 3 to CSE
   {  return (GAME_END);
   }
   else if (table[0][2]=='E' && table[1][2]=='S' && table[2][2]=='C') // stili: 3 to ESC
   {  return (GAME_END);
   }
   else if (table[0][0]=='C' && table[1][1]=='S' && table[2][2]=='E') // kiria diagonios to CSE
   {  return (GAME_END);
   }
   else if (table[0][0]=='E' && table[1][1]=='S' && table[2][2]=='C') // kiria diagonios to ESC
   {  return (GAME_END);
   }
   else if (table[2][0]=='C' && table[1][1]=='S' && table[0][2]=='E') // alli diagonios to CSE
   {  return (GAME_END);
   }
   else if (table[2][0]=='E' && table[1][1]=='S' && table[0][2]=='C') // alli diagonios to ESC
   {  return (GAME_END);
   }
   else
      return (GAME_CONTINUE);
   
}

void copy_table(char A[3][3],char B[3][3])
{  int i,j;
   for(i=0;i<3;i++)
     for(j=0;j<3;j++)
        B[i][j]=A[i][j];

}

int dentro_MINMAX(struct min_max *tree,int seira);

int make_children(struct min_max *tree, int seira, int epomenos, int test[24])
{
    int position = 0, i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (tree->table[i][j] == '-')
            {
                // Allocate memory for the child node
                tree->pointers[position] = (struct min_max *)malloc(sizeof(struct min_max));

                // Check if memory allocation succeeded
                if (tree->pointers[position] == NULL)
                {
                    // Handle memory allocation failure
                    printf("Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }

                // Initialize the child node
                tree->pointers[position]->free_boxes = tree->free_boxes - 1;
                copy_table(tree->table, tree->pointers[position]->table);
                tree->pointers[position]->table[i][j] = 'C';
                test[position] = dentro_MINMAX(tree->pointers[position], epomenos);
                position++;

                // Allocate memory for the child node
                tree->pointers[position] = (struct min_max *)malloc(sizeof(struct min_max));

                // Check if memory allocation succeeded
                if (tree->pointers[position] == NULL)
                {
                    // Handle memory allocation failure
                    printf("Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }

                // Initialize the child node
                tree->pointers[position]->free_boxes = tree->free_boxes - 1;
                copy_table(tree->table, tree->pointers[position]->table);
                tree->pointers[position]->table[i][j] = 'S';
                test[position] = dentro_MINMAX(tree->pointers[position], epomenos);
                position++;

                // Allocate memory for the child node
                tree->pointers[position] = (struct min_max *)malloc(sizeof(struct min_max));

                // Check if memory allocation succeeded
                if (tree->pointers[position] == NULL)
                {
                    // Handle memory allocation failure
                    printf("Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }

                // Initialize the child node
                tree->pointers[position]->free_boxes = tree->free_boxes - 1;
                copy_table(tree->table, tree->pointers[position]->table);
                tree->pointers[position]->table[i][j] = 'E';
                test[position] = dentro_MINMAX(tree->pointers[position], epomenos);
                position++;
            }
        }
    }

    return position; // position = plithos twn paidiwn
}

void user_move(struct min_max *state) {
    int row, col;
    char letter;

    // Prompt the user to enter the row and column for their move
    printf("Enter the row and column for your move (0-2, space-separated): ");
    scanf("%d %d", &row, &col);
    
    // Prompt the user to enter the letter ('C', 'S', 'E') for their move
    printf("Enter Letter ('C', 'S', 'E'): ");
    scanf(" %c", &letter); // Note the space before %c to consume any leading whitespace

    // Update the table with the user's move
    state->table[row][col] = letter;
    state->free_boxes--; // Decrease the number of free cells
}


int dentro_MINMAX(struct min_max *tree,int seira)
{  int epomenos;
   int position=0;
   int test[24]; // o pinakas test periexei tis times twn paidiwn tis katastasis "tree"
   int kaliteri_timi;
   int best; // to index tou paidiou poy exei tin kaliteri(analoga to epipedo) timi "test"
   int i;

   if(seira==MAX && check_CSE_ESC(tree->table)==GAME_END) // an einai i seira MAX kai eimai se fyllo (GAME_END), ara kerdise o MIN
      return (MIN);
   else if(seira==MIN && check_CSE_ESC(tree->table)==GAME_END) // an einai i seira MIN kai eimai se fyllo (GAME_END), ara kerdise o MAX
      return (MAX);
   else if(tree->free_boxes==0) // allios, aneksartitos poios paizei, DEN yparxoun kena, ara isopalia
      return (TIE);
   // an vrethei edw, i katastasi "tree" DEN einai fyllo, ara prepei na ginoun ta paidia tis
   if(seira==MAX)
      epomenos=MIN;
   else
      epomenos=MAX;

   position= make_children(tree,seira,epomenos,test); // i katastasi "tree" ftiaxnei ta paidia tis

   if(seira==MAX) // seira = epipedo sto dentro
   {  kaliteri_timi=test[0];
      best=0;
      for(i=0;i<position;i++)
      {  if(test[i]>kaliteri_timi) // euresi MEGISTIS timis apo ta paidia 
         {  kaliteri_timi=test[i];
            best=i;
         }
      }
   }
   else // seira == MIN (seira = epipedo sto dentro)
   {  kaliteri_timi=test[0];
      best=0;
      for(i=1;i<position;i++)
      {  if(test[i]<kaliteri_timi) // euresi ELAXISTIS timis apo ta paidia 
         {  kaliteri_timi=test[i];
            best=i;
         }
      }
   }
   tree->next=best;
   return kaliteri_timi;
}

void print_table(char A[3][3])
{  int i,j;
   for(i=0;i<3;i++)
   {  for(j=0;j<3;j++)
      {  printf("%c ",A[i][j]);
      }
      printf("\n");
   }
   printf("\n\n\n");
}

int main()
{
    struct min_max *state = (struct min_max *)malloc(sizeof(struct min_max));
    int seira = MIN, i, j, grammi, stili;
    char haraktiras;

    state->free_boxes = 7;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            state->table[i][j] = '-';
        }
    }
    
    

    state->table[1][2] = 'S';

    printf("ARXI PAIXNIDIOU \n\n");
    print_table(state->table);

    while (1)
    {
        if (seira == MAX && check_CSE_ESC(state->table) == GAME_END)
        {
            printf("NIKHSE O ANTIPALOS \n\n\n");
            break;
        }
        else if (seira == MIN && check_CSE_ESC(state->table) == GAME_END)
        {
            printf("NIKHSE TO PROGRAMMA \n\n\n");
            break;
        }
        else if (state->free_boxes == 0)
        {
            printf("ISOPALIA \n\n\n");
            break;
        }

        if (seira == MAX)
        {
            printf("PROGRAMMA paizei \n\n");
            dentro_MINMAX(state, MAX);
            state = state->pointers[state->next];
            print_table(state->table);
            seira = MIN;
        }
        else
        {
            printf("ANTIPALOS paizei \n\n");
            user_move(state);
            print_table(state->table);
            seira = MAX;
        }
    }

    // Free the allocated memory
    free(state);

    return 0;
}

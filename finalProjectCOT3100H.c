//John Cuono - COT3100H Final Project Code
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 8 //length or height of chessboard. SIZE * SIZE is the size of a full 8x8 chessboard

void permutePieces(int piecesArray[], int helperArray[], int counter, int size, char pieces[]); //calculates all possible permutations for fischer random rules
int fischerCheck(int piecesArray[], int size, char pieces[]); //check permutation conditions for fischer random
int totalPermutations = 0; //total permutations for fischer random

void eightQueens(int queensArray[], int helperArray[], int counter, int size); //eight queens permutations calculated recursively
int queensValidity(int queensArray[], int size, int row, int counter); //checks if a permutation of queens is valid for 8 queens puzzle
int totalSolutions = 0; //total permutations for 8 queens puzzle

void knightsTour();
void printChessBoardKnights(int knightsBoard[SIZE][SIZE]);
int knightsValidity(int xCord, int yCord, int knightsBoard[SIZE][SIZE]);
int knightsPossibleSquares(int xCord, int yCord, int knightsBoard[SIZE][SIZE]);
int solveKnightsRec(int xCord, int yCord, int moveCounter, int knightsBoard[SIZE][SIZE]);
int movesArray[8][2] = {{2,1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}}; //possible knight moves

int main()
{
    int n = -1; //switch variable

    printf("=============================\n");
    printf("Welcome to the chess simulator.\n");

    while(n != 4)
    {
        printf("=============================\n");
        printf("Here are some simulations you can run:\n");
        printf("1: Simulate all permutations of the opening position in Fischer Random.\n");
        printf("2: Simulate all solutions of the 8 queens puzzle.\n");
        printf("3: Simulate a move order for the Knight's Tour.\n");
        printf("4: Close the program.\n");
        printf("=============================\n");
        printf("Please enter a value corresponding to the list: ");
        scanf("%d", &n);

        switch(n)
        {
            case 1:
            {
                totalPermutations = 0; //reset the variable
                char pieces[SIZE] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
                int piecesArray[SIZE] = {0};
                int helperArray[SIZE] = {0};
                permutePieces(piecesArray, helperArray, 0, SIZE, pieces);
                printf("\n");
                printf("Total Permutations In Fischer Random: %d\n", totalPermutations);
                break;
            }
            case 2:
            {
                int queensArray[SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1};
                int helperArray[SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
                eightQueens(queensArray, helperArray, 0, SIZE);
                printf("\n");
                printf("Total Solutions Of The Standard 8 Queens Puzzle: %d\n", totalSolutions);
                break;
            }
            case 3:
            {
                knightsTour();
                break;
            }
            case 4:
            {
                printf("Goodbye!\n");
                break;
            }
            default:
            {
                printf("Please enter a valid section: ");
                scanf("%d", &n);
            }
        }
    }


    return 0;
}

void permutePieces(int piecesArray[], int helperArray[], int counter, int size, char pieces[])
{
    if(counter == size && fischerCheck(piecesArray, size, pieces))
    {
        totalPermutations++;
    }

    for(int i = 0; i < size; i++)
    {
        if(!helperArray[i])
        {
            helperArray[i] = 1;
            piecesArray[counter] = i;
            permutePieces(piecesArray, helperArray, counter + 1, size, pieces);
            helperArray[i] = 0;
        }
    }
}

int fischerCheck(int piecesArray[], int size, char pieces[])
{
    int bishopTracker = 1; //this code block eliminates duplicate permutations
    int knightTracker = 1; //since there's no way to check if a permutation is a duplicate of another entirely different permutation efficiently, we use this logic
    int rookTracker = 1; //these trackers will disable after a matching piece is found unless it meets the "return 0" condition
    for(int i = 0; i < size; i++)
    {
        if((pieces[piecesArray[i]] == 'B' && bishopTracker) || (pieces[piecesArray[i]] == 'N' && knightTracker) || (pieces[piecesArray[i]] == 'R' && rookTracker))
        {
            if(piecesArray[i] > SIZE / 2)
            {
                return 0; //by returning 0 for every piece with index > size / 2, we are guaranteed to avoid exactly half of the technically valid permutations
            }
            else if(pieces[piecesArray[i]] == 'B')
            {
                bishopTracker = 0; //disable the bishop tracker
                continue;
            }
            else if(pieces[piecesArray[i]] == 'N')
            {
                knightTracker = 0; //disable the knight tracker
                continue;
            }
            else if(pieces[piecesArray[i]] == 'R')
            {
                rookTracker = 0; //disable the rook tracker
                continue;
            }
        }
    }

    if(pieces[piecesArray[0]] == 'K' || pieces[piecesArray[size - 1]] == 'K')
    {
        return 0; //king cannot be on the ends because it must be surrounded by rooks
    }

    for(int i = 0; i < size - 1; i++) //checking bishops
    {
        if(pieces[piecesArray[i]] == 'B') //first bishop
        {
            for(int j = i + 1; j < size; j++)
            {
                if(pieces[piecesArray[j]] == 'B') //second bishop
                {
                   if(i % 2 == j % 2) //if bishops are on the same color square
                   {
                       return 0; //bishops have to be on opposite colored squares
                   }
                }
            }
        }
    }

    for(int i = 1; i < size - 1; i++) //dealing with kings and rooks problem
    {
        if(pieces[piecesArray[i]] == 'K') //find position of king
        {
            for(int j = 0; j < i + 1; j++) //search for rook before king
            {
                if(j == i)
                {
                    return 0;
                }
                else if(pieces[piecesArray[j]] == 'R') //if rook before king is found
                {
                    for(int k = i + 1; k < size + 1; k++) //search for rook after king
                    {
                        if(k == size) //if we reach the end of the array without success
                        {
                            return 0; //no rook was found after the king
                        }
                        else if(pieces[piecesArray[k]] == 'R')
                        {
                            break; //successful combination found so break
                        }
                    }
                    break; //finishing breaking
                }
            }
        }
    }

    printf("\nArray %d:", totalPermutations + 1);
    for(int i = 0; i < size; i++)
    {
        printf(" %c", pieces[piecesArray[i]]);
    }
    printf("\n");
    printf("Array %d:", totalPermutations + 1);
    for(int i = 0; i < size; i++)
    {
        printf(" %d", piecesArray[i]);
    }
    printf("\n");

    return 1;
}

void eightQueens(int queensArray[], int helperArray[], int counter, int size)
{
    if(counter == size)
    {
        totalSolutions++;
        printf("Solution %d:\n", totalSolutions); //update and print solutions
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(queensArray[j] == i)
                {
                    printf("Q ");
                }
                else
                {
                    printf("/ ");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    for(int row = 0; row < size; row++) //perform recursion
    {
        if(!helperArray[row] && queensValidity(queensArray, size, row, counter))
        {
            helperArray[row] = 1;
            queensArray[counter] = row;
            eightQueens(queensArray, helperArray, counter + 1, size);
            helperArray[row] = 0;
        }
    }
}

int queensValidity(int queensArray[], int size, int row, int counter)
{
    for(int column = 0; column < counter; column++)
    {
        if(abs(counter - column) == abs(queensArray[column] - row))
        {
            return 0; //if the distance between two points has the same x and y, it must be diagonal so it can't be a solution
        }
    }

    return 1;
}

void knightsTour()
{
    int knightsBoard[SIZE][SIZE]; //board to store move order
    for(int i = 0; i < SIZE; i++) //initialize the board
    {
        for(int j = 0; j < SIZE; j++)
        {
           knightsBoard[i][j] = -1;
        }
    }
    int moveCounter = 0;

    int xCord;
    int yCord;

    printf("Enter a square to begin on using coordinate notation: ");
    scanf("%d %d", &yCord, &xCord);

    while(yCord < 0 || yCord > 7 || xCord < 0 || xCord > 7)
    {
        printf("Enter a square to begin on using coordinate notation: ");
        scanf("%d %d", &yCord, &xCord);
    }

    knightsBoard[xCord][yCord] = moveCounter; //starting square
    printChessBoardKnights(knightsBoard);
    moveCounter++;

    solveKnightsRec(xCord, yCord, moveCounter, knightsBoard);
}

int solveKnightsRec(int xCord, int yCord, int moveCounter, int knightsBoard[SIZE][SIZE])
{
    printChessBoardKnights(knightsBoard);
    if(moveCounter == SIZE * SIZE) //if all squares have been visited
    {
        printChessBoardKnights(knightsBoard);
        return 1;
    }

    int maxPossibleMoves = 9; //This utilizes Warndorf's rule. Since a knight can't have 9 options ever, we initialize to 9 for comparison later

    for(int i = 0; i < 8; i++) //tries to find the next square with the least number of moves, so that backtracking can be more efficient
    {
        int nextXCord = xCord + movesArray[i][0]; //temp coordinates to check each next square
        int nextYCord = yCord + movesArray[i][1];

        if(knightsValidity(nextXCord, nextYCord, knightsBoard) == 1) //if we can move to the square
        {
            int lowestPossibleMoves = knightsPossibleSquares(nextXCord, nextYCord, knightsBoard); //set to the number of possible moves on a particular check
            if(lowestPossibleMoves < maxPossibleMoves) //if this new number is lower than anything we've found thus far
            {
                maxPossibleMoves = lowestPossibleMoves; //update the new max possible moves for later
            }
        }
    }

    for(int i = 0; i < 8; i++) //this actually performs the recursion
    {
        int nextXCord = xCord + movesArray[i][0]; //temp coordinates to check each next square
        int nextYCord = yCord + movesArray[i][1];

        if(knightsValidity(nextXCord, nextYCord, knightsBoard) == 1 && knightsPossibleSquares(nextXCord, nextYCord, knightsBoard) == maxPossibleMoves) //if the square being checked matches the maxPossibleMoves variable
        {
            knightsBoard[nextXCord][nextYCord] = moveCounter; //this move is guaranteed to be successful
            if(solveKnightsRec(nextXCord, nextYCord, moveCounter + 1, knightsBoard) == 1) //recursion
            {
                return 1;
            }
        }
    }

    return 0;
}

int knightsPossibleSquares(int xCord, int yCord, int knightsBoard[SIZE][SIZE])
{
    int possibleCounter = 0;

    for(int i = 0; i < 8; i++) //check how many of the next moves possible are valid
    {
        int nextXCord = xCord + movesArray[i][0];
        int nextYCord = yCord + movesArray[i][1];
        if(knightsValidity(nextXCord, nextYCord, knightsBoard) == 1)
        {
            possibleCounter++;
        }
    }
    return possibleCounter;
}
int knightsValidity(int xCord, int yCord, int knightsBoard[SIZE][SIZE])
{
   if(xCord >= 0 && xCord <= 7 && yCord >= 0 && yCord <= 7 && knightsBoard[xCord][yCord] == -1)
   {
       return 1; //if in bounds and square hasn't already been visited
   }
   return 0;
}

void printChessBoardKnights(int knightsBoard[SIZE][SIZE])
{
    char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int ranks[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int counter = 7;

    printf("=============================\n");
    for(int i = SIZE - 1; i >= 0; i--)
    {
        printf("%d | ", ranks[counter]);
        counter--;
        for(int j = 0; j < 8; j++)
        {
            if(knightsBoard[i][j] < 10 && knightsBoard[i][j] > -1)
            {
                printf("0%d ", knightsBoard[i][j]);
            }
            else
            {
                printf("%d ", knightsBoard[i][j]);
            }
        }
        printf("\n");
        if(i == 0)
        {
            counter = 0;
            printf("  X--------------------------\n");
            printf("    ");
            for(int k = 0; k < SIZE; k++)
            {
                printf("%c  ", files[counter]);
                counter++;
            }
            printf("\n");
        }
    }
    printf("=============================\n");
}

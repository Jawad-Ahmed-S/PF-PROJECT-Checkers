#include <stdio.h>
int turn = 0;
void PrintArray(char[8][8]);
void initialPositions(char[8][8]);
char Playerturn();
void Movement(char[8][8]);
int main()
{
    char gameBoard[8][8];
    initialPositions(gameBoard);
    PrintArray(gameBoard);
    while (1)
    {

        Movement(gameBoard);
        PrintArray(gameBoard);
    }
    return 0;
}
void Movement(char gameBoard[8][8])
{
    int row, col;
    printf("Enter the index of the peice");
    scanf("%d  ", &row);
    scanf("%d", &col);

    row -= 1;
    col -= 1;
    char active = Playerturn();
    if (gameBoard[row][col] != active)
    {
        printf("Invalid index!\n");
        turn--;
    }
    if (active == 'B') // Black ki turn
    {
        char side;
        printf("Which side");
        scanf(" %c", &side);
        if (side == 'd')
        { // rightaa
            if ((gameBoard[row + 1][col + 1] == 'B' || gameBoard[row + 1][col + 1] == 'W') && (row + 1) < 8 && (col + 1) < 8)
            {
                if ((row + 1) < 8 && (col + 1) < 8)
                {
                    printf(".................Index occupied.................");
                }
                printf("Index occupied.\n");
                return;
            }
            else
            {

                char temp = gameBoard[row + 1][col + 1];
                gameBoard[row + 1][col + 1] = gameBoard[row][col];
                gameBoard[row][col] = temp;
            }
        }
        else if (side == 'a') // left
        {
            if ((gameBoard[row + 1][col - 1] == 'B' || gameBoard[row + 1][col - 1] == 'W') && (row + 1) < 8 && (col - 1) >= 0)
            {
                printf("Index occupied.\n");
                turn--;
                return;
            }
            else
            {

                char temp = gameBoard[row + 1][col - 1];
                gameBoard[row + 1][col - 1] = gameBoard[row][col];
                gameBoard[row][col] = temp;
            }
        }
        else
        {
            printf("invalid side");
            turn--;
        }
    }
    else // White ki turn
    {
        char side;
        printf("Which side");
        scanf(" %c", &side);
        if (side == 'd') // right
        {
            if ((gameBoard[row - 1][col + 1] == 'B' || gameBoard[row - 1][col + 1] == 'W') && (row - 1) >= 0 && (col + 1) < 8)
            {
                printf("Index occupied.\n");
                turn--;
                return;
            }
            else
            {
                char temp = gameBoard[row - 1][col + 1];
                gameBoard[row - 1][col + 1] = gameBoard[row][col];
                gameBoard[row][col] = temp;
            }
        }
        else if (side == 'a') // left
        {
            if ((gameBoard[row - 1][col - 1] == 'B' || gameBoard[row - 1][col - 1] == 'W') && row - 1 >= 0 && col - 1 >= 0)
            {
                printf("Index occupied.\n");
                turn--;
                return;
            }
            else
            {

                char temp = gameBoard[row - 1][col - 1];
                gameBoard[row - 1][col - 1] = gameBoard[row][col];
                gameBoard[row][col] = temp;
            }
        }
        else
        {
            printf("invalid side");
            turn--;
        }
    }
}
char Playerturn()
{
    char active;
    if (turn % 2 == 0)
    {
        active = 'B';
    }
    else
    {
        active = 'W';
    }
    turn++;
    return active;
}
void PrintArray(char arr[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf(" %c ", arr[i][j]);
        }
        printf("\n");
    }
}
void initialPositions(char gameboard[8][8])
{
    char initialBoard[8][8] = {
        {'B', '-', 'B', '-', 'B', '-', 'B', '-'},
        {'-', 'B', '-', 'B', '-', 'B', '-', 'B'},
        {'B', '-', 'B', '-', 'B', '-', 'B', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'W', '-', 'W', '-', 'W', '-', 'W', '-'},
        {'-', 'W', '-', 'W', '-', 'W', '-', 'W'},
        {'W', '-', 'W', '-', 'W', '-', 'W', '-'},
    };
    for (int i = 0; i < 8; i++)
    {

        for (int j = 0; j < 8; j++)
        {
            gameboard[i][j] = initialBoard[i][j];
        }
    }
}

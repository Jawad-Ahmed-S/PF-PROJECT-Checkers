#include <stdio.h>
void initializeGameBoard(char[8][8]);
void PrintArray(char[8][8]);
void initialPositions(char gameboard[8][8]);
int main()
{
    char gameBoard[8][8];
    // initializeGameBoard(gameBoard);
    initialPositions(gameBoard);
    PrintArray(gameBoard);
    return 0;
}
// void initializeGameBoard(char arr[8][8])
// {
//     // for (int i = 0; i < 8; i++)
//     // {
//     //     for (int j = 0; j < 8; j++)
//     //     {
//     //         arr[i][j] = '-';
//     //     }
//     // }
//     arr[8][8] = {{'-',}}
// }
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
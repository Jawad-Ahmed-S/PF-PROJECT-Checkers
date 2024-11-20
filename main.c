#include <stdio.h>
#include <stdbool.h>

//tracks current turn
char curr_turn = 'W'; //'W' or 'B'

//position struct
struct position
{
    int x;
    int y;
    //rows are y, columns are x
};

//pieces struct
struct piece
{
    struct position position;
    char color;
    bool king;
    bool captured;
} pieces[24];

//prints array each turn
void printArray(char[8][8]);

//sets initial positions
void initialPositions(char[8][8]);

//updates array after each move
void updateArray(char[8][8]);

//manages movement of pieces for each turn
void movement();

//checks if piece at selected position
int selectionValid (struct position);

//determines type of move: (normal)move, capture, or no move
int moveValid(int, struct position);

//translates keyboard inputs (chars) to changes in position
struct position translateMove (char);

//flips curr_turn
void changeTurn ();

int main()
{
    char gameBoard[8][8];
    
    initialPositions(gameBoard);
    printArray(gameBoard);

    while (true)
    {
        //win checking function called here (must be first so can identify if current_turn cannot move and hence, loses)
        movement();
        updateArray(gameBoard);
        printArray(gameBoard);
    }
    return 0;
}
void movement()
{
    struct position selection; //input var for selection
    struct position move_vals; //var for storing return value of translate_move
    char input_move; //input var for move, takes char
    int piece; //stores selected piece index for pieces[] array
    int what_move; //move, no move, or captured (to be) piece index
    
    do
    {
        printf("select piece (row col): ");
        scanf("%d %d", &selection.y, &selection.x);
        piece = selectionValid(selection);

        //repeat until selection valid   
    } while (piece == -1);
    
    do
    {
        printf("move: ");
        scanf(" %c", &input_move);
        move_vals = translateMove(input_move);
        what_move = moveValid(piece, move_vals);

        //repeat until move valid
    } while (what_move == -1 || ((input_move == 'z' || input_move == 'c') && (!pieces[piece].king)));

    //move piece
    if (what_move == -2)
    {
        pieces[piece].position.x += move_vals.x;
        pieces[piece].position.y += move_vals.y;

        //check if king
        if ((pieces[piece].position.y == 7 && pieces[piece].color == 'W') || (pieces[piece].position.y == 0 && pieces[piece].color == 'B'))
        {
            //king is piece from one side is on the other side
            pieces[piece].king = true;
        }
    }

    //capture piece
    else
    {
        while (what_move >= 0)
        {
            pieces[what_move].captured = true;

            pieces[piece].position.x += move_vals.x * 2;
            pieces[piece].position.y += move_vals.y * 2;

            what_move = moveValid(piece, move_vals);
            //code left for checking both sides for capture and prompt if both capture for both
        }
        
        
    }

    changeTurn();
}

void changeTurn ()
{
    if (curr_turn == 'W')
    {
        curr_turn = 'B';
    }

    else
    {
        curr_turn = 'W';
    }
}

int selectionValid (struct position selection) //returns -1 if invalid, index of selected if valid
{
    //iterate over all pieces
    for (int i = 0; i < 24; i++)
    {
        //if piece captured, cannot be selected, move on
        if (pieces[i].captured)
        {
            continue;
        }

        //check only pieces that belong to current turn
        if (pieces[i].color == curr_turn)
        {
            
            //true if position of piece same as selected
            if (pieces[i].position.x == selection.x && pieces[i].position.y == selection.y)
            {
                return i;
            }
        }
    }
    //if no pieces match, then default
    return -1; //cannot return 0, could be selected index value
}

//takes index of selected piece and its change in position
int moveValid(int index, struct position move) //returns 0 if invalid, 1 if move, index of piece to be captured if capture
{
    struct position tmp;
    int maybe_capture;

    tmp.x = pieces[index].position.x + move.x;
    tmp.y = pieces[index].position.y + move.y;

    //check if out of bounds
    if (tmp.x > 7 || tmp.y > 7)
    {
        return -1;
    }

    bool occupied = false;

    //check if square already occupied (collision)
    for (int i = 0; i < 24; i++)
    {
        //if piece captured, cannot occupy square, move on
        if (pieces[i].captured)
        {
            continue;
        }

        if (pieces[i].color == curr_turn)
        {
            if (pieces[i].position.x == tmp.x || pieces[i].position.y == tmp.y)
            {
                maybe_capture = i;
                occupied  = true;
                break;
            } 
        }
    }

    //check if capture
    if (occupied)
    {
        tmp.x += move.x;
        tmp.y += move.y;


        //checks if square to jump to also occupied
        for (int i = 0; i < 24; i++)
        {
            //if piece captured, cannot occupy square, move on
            if (pieces[i].captured)
            {
                continue;
            }
            
            //check only pieces that belong to current turn
            if (pieces[i].color == curr_turn)
            {
                if (pieces[i].position.x == tmp.x || pieces[i].position.y == tmp.y)
                {
                    //collision with piece, cannot capture
                    return -1;
                    break;
                } 
            }
        }
        //if no collision, can capture
        return maybe_capture;
    }
    //if not occupied, can move
    return -2;
}

struct position translateMove (char move)
{
    struct position re;

    if (move == 'a' || move == 'z')
    {
        re.x = -1;
    }

    else if (move == 'd' || move == 'c')
    {
        re.x = 1;
    }

    if (move == 'a' || move == 'd')
    {
        re.y = -1;
    }

    else if (move == 'z' || move == 'c')
    {
        re.y = 1;
    }

    if (curr_turn == 'B')
    {
        re.y *= -1;
    }

    return re;
}

void updateArray(char gameBoard[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            gameBoard[i][j] = '-';
        }
    }

    for (int i = 0; i < 24; i++)
    {
        gameBoard[pieces[i].position.y][pieces[i].position.x] = pieces[i].color; //x is rows y is columns
    }
}

void printArray(char gameBoard[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf(" %c ", gameBoard[i][j]);
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

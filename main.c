#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define YEL "\e[0;33m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define BOLD_TEXT "\e[1m"
#define UNDERLINE_TEXT "\e[4m"

// tracks current turn
char curr_turn[2] = {'W', 'K'}; //'W' or 'B'
bool load = false;

// position struct
struct position
{
    int x;
    int y;
    // rows are y, columns are x
};

// pieces struct
struct piece
{
    struct position position;
    char color;
    bool king;
    bool captured;
} pieces[24];

// sets initial positions and piece colors
void initializeValues();

// prints array each turn
void printArray(char[8][8]);

// takes input and manages movement of pieces for each turn
void movement();

// updates array after each move
void updateArray(char[8][8]);

// checks if piece at selected position and if piece mobile
int selectionValid(struct position);

// determines type of move: (normal) move, capture, or no move
int moveValid(int, struct position);

// translates keyboard inputs (chars) to changes in position
struct position translateMove(char);

// checks if any player has won
int won();
// Saving
void SaveFile();
// loading game
int LoadGame();
// display the highlighted and the rest of menu items
void displayMenu(const char *menuItems[], int selected, int totalItems);
// menu function
void Menu();
// flips curr_turn
void changeTurn();
// mainGame
void mainGame();
// Actual Main for Gameplay-----
void mainGame()
{
    char gameBoard[8][8];
    if (!load)
    {
        initializeValues(gameBoard);
    }

    updateArray(gameBoard);
    printArray(gameBoard);

    while (true)
    {
        if (won())
        {
            break;
        }
        movement();
        updateArray(gameBoard);
        printArray(gameBoard);
    }
}

void Instructions()
{
    char back;
    printf("\033[1mInstructions:\033[0m\n");
    printf("\033[1mNormal Piece\033[0m: Use 'a' to move left top, 'd' to move right top.\n");
    printf("\033[1mKing\033[0m: Use 'z' to move left bottom, 'c' to move right bottom.\n");
    printf("\n\n <- Back");

    printf("\npress any key");
    scanf("%c", &back);
    if (back == 'b')
    {
        Menu();
    }
    else
    {
        Menu();
    }
}
void Credits()
{
    char back;
    printf("\033[1mCredits:\033[0m\n");

    // Yousha Faisal's contribution
    printf("\033[1mYousha Faisal:\033[0m\n");
    printf(" - Contributed to Game Logic\n");
    printf(" - Acted as Tester\n");

    // Jawad Ahmed's contribution
    printf("\033[1mJawad Ahmed:\033[0m\n");
    printf(" - Developed the Basic Structure\n");
    printf(" - Implemented Save State Game Feature with Bugs\n");
    printf(" - Resolved Various Frustrating Bugs\n");

    // Hammad Haider's contribution
    printf("\033[1mHammad Haider:\033[0m\n");
    printf(" - Implemented Color Functionality\n");
    printf(" - Developed the Win Condition Function\n");

    // Abdul Rafay's contribution
    printf("\033[1mAbdul Rafay:\033[0m\n");
    printf(" - Worked on GUI Development (Unfortunately, not integrated due to illness)\n");

    printf("\npress any key");
    scanf("%c", &back);
    if (back == 'b')
    {
        Menu();
    }
    else
    {
        Menu();
    }
}
// only menu-------------

int main()
{

    Menu();
}
void movement()
{
    struct position selection; // input var for selection
    struct position move_vals; //(move values) var for storing return value of translate_move
    char input_move;           // input var for move, takes char
    int piece;                 // stores selected piece index for pieces[] array
    int what_move;             // move, no move, or captured (to be) piece index

    do
    {

        printf("select piece (row col): ");
        // Check the return value of scanf
        if (scanf("%d %d", &selection.y, &selection.x) != 2)
        {
            printf("Invalid input. Please enter two integers.\n");

            // Clear the input buffer
            while (getchar() != '\n')
                ;       // Discard invalid input
            piece = -1; // Force loop to repeat
            continue;   // Restart the loop
        }

        // Validate selection
        piece = selectionValid(selection);

        if (piece == -1)
        {
            printf("Invalid selection. Please try again.\n");
        }

    } while (piece == -1);

    while (true)
    {
        printf("MOVE or press 'x' to go to MAIN MENU: ");
        scanf(" %c", &input_move);
        if (input_move == 'x')
        {
            Menu();
        }
        if ((input_move == 'z' || input_move == 'c') && (!pieces[piece].king))
        {
            printf("piece not King :| Try Again\n");
            continue;
        }

        // get movement values by translateMove
        move_vals = translateMove(input_move);
        what_move = moveValid(piece, move_vals);

        // repeat until move valid
        if (what_move != -1)
            break;
    }

    // move piece
    if (what_move == -2)
    {
        pieces[piece].position.x += move_vals.x;
        pieces[piece].position.y += move_vals.y;
    }

    // capture piece
    else
    {
        // capture piece
        pieces[what_move].captured = true;

        // change position of selected piece (jump)
        pieces[piece].position.x += (move_vals.x * 2);
        pieces[piece].position.y += (move_vals.y * 2);
    }

    // check if king
    if ((pieces[piece].position.y == 0 && pieces[piece].color == 'W') || (pieces[piece].position.y == 7 && pieces[piece].color == 'B'))
    {
        // king is when piece from one side is on the other side
        pieces[piece].king = true;

        if (pieces[piece].color == 'W')
            pieces[piece].color = 'K'; // king if white

        else
            pieces[piece].color = 'Q'; // queen if black

        // king and queen function exactly the same. distinction made due to display limitations
    }

    changeTurn();
    // update:
}

void changeTurn()
{
    if (curr_turn[0] == 'W')
    {
        curr_turn[0] = 'B';
        curr_turn[1] = 'Q';
    }

    else
    {
        curr_turn[0] = 'W';
        curr_turn[1] = 'K';
    }
}

int selectionValid(struct position selection) // returns -1 if invalid, index of selected if valid
{
    // iterate over all pieces
    for (int i = 0; i < 24; i++)
    {
        // if piece captured, cannot be selected, move on
        if (pieces[i].captured)
        {
            continue;
        }

        // check only pieces that belong to current turn
        if (pieces[i].color == curr_turn[0] || pieces[i].color == curr_turn[1])
        {

            // true if position of piece same as selected
            if (pieces[i].position.x == selection.x && pieces[i].position.y == selection.y)
            {
                if (moveValid(i, translateMove('a')) == -1 && moveValid(i, translateMove('d')) == -1)
                {
                    if (pieces[i].king)
                    {
                        if (moveValid(i, translateMove('z')) == -1 && moveValid(i, translateMove('c')) == -1)
                        {
                            return -1;
                        }
                    }

                    else
                    {
                        return -1;
                    }
                }

                return i;
            }
        }
    }
    // if no pieces match, then default
    return -1; // cannot return 0, could be selected index value
}

// takes index of selected piece and its change in position
int moveValid(int index, struct position move) // returns -1 if invalid, -2 if move, index of piece to be captured if capture
{
    struct position tmp;
    int maybe_capture = -1;

    tmp.x = pieces[index].position.x + move.x;
    tmp.y = pieces[index].position.y + move.y;

    // check if out of bounds
    if (tmp.x > 7 || tmp.x < 0 || tmp.y > 7 || tmp.y < 0)
    {
        return -1;
    }

    bool occupied = false;

    // check if square already occupied (collision)
    for (int i = 0; i < 24; i++)
    {
        // if piece captured, cannot occupy square, move on
        if (pieces[i].captured)
        {
            continue;
        }

        if (pieces[i].position.x == tmp.x && pieces[i].position.y == tmp.y)
        {
            if (pieces[i].color != curr_turn[0])
            {
                maybe_capture = i;
            }

            occupied = true;
            break;
        }
    }

    // check if capture
    if (occupied)
    {
        if (maybe_capture > -1)
        {
            tmp.x += move.x;
            tmp.y += move.y;

            // check if out of bounds
            if (tmp.x > 7 || tmp.x < 0 || tmp.y > 7 || tmp.y < 0)
            {
                return -1;
            }

            // check if square to jump to also occupied
            for (int i = 0; i < 24; i++)
            {
                // if piece captured, cannot occupy square, move on
                if (pieces[i].captured)
                {
                    continue;
                }

                if (pieces[i].position.x == tmp.x && pieces[i].position.y == tmp.y)
                {
                    // collision with piece, cannot capture
                    return -1;
                }
            }
            // if no collision, can capture
            return maybe_capture;
        }

        else
            return -1;
    }
    // if not occupied, can move
    return -2;
}

struct position translateMove(char move)
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

    if (curr_turn[0] == 'B')
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
        if (!pieces[i].captured)
            gameBoard[pieces[i].position.y][pieces[i].position.x] = pieces[i].color; // x is rows y is columns
    }
}

void printArray(char gameBoard[8][8])
{
    system("cls");
    printf(CYN "\t\t\t\t\t\t\t\t Black : Cyan\n" WHT);
    printf(YEL "\t\t\t\t\t\t\t\t White : Yellow\n" WHT);
    for (int i = -1; i < 8; i++)
    {
        if (i >= 0)
            printf("%d ", i);
        else
            printf("  ");

        for (int j = 0; j < 8; j++)
        {
            if (i == -1)
            {
                printf(" %d ", j);
                continue;
            }

            if (gameBoard[i][j] != '-')
            {
                if (gameBoard[i][j] == 'W' || gameBoard[i][j] == 'K')
                {
                    if (gameBoard[i][j] == 'K')
                    {
                        printf(BOLD_TEXT YEL " %c " WHT, gameBoard[i][j]);
                        continue;
                    }
                    printf(YEL " %c " WHT, gameBoard[i][j]);
                    continue;
                }

                else
                {
                    if (gameBoard[i][j] == 'Q')
                    {
                        printf(BOLD_TEXT CYN " %c " WHT, gameBoard[i][j]);
                        continue;
                    }
                    printf(CYN " %c " WHT, gameBoard[i][j]);
                    continue;
                }
            }

            printf(" %c ", gameBoard[i][j]);
        }
        printf("\n");
    }

    // which turn?
    if (curr_turn[0] == 'W')
    {
        printf(YEL "\t\t\t\t\t\t%c playing...\n" WHT, curr_turn[0]);
    }
    else if (curr_turn[0] == 'B')
    {
        printf(CYN "\t\t\t\t\t\t%c playing...\n" WHT, curr_turn[0]);
    }
}

void initializeValues()
{
    char initialBoard[8][8] = {
        {'-', 'B', '-', 'B', '-', 'B', '-', 'B'},
        {'B', '-', 'B', '-', 'B', '-', 'B', '-'},
        {'-', 'B', '-', 'B', '-', 'B', '-', 'B'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'W', '-', 'W', '-', 'W', '-', 'W', '-'},
        {'-', 'W', '-', 'W', '-', 'W', '-', 'W'},
        {'W', '-', 'W', '-', 'W', '-', 'W', '-'},
    };

    int piece_counter = 0;

    // set initial positions and colors
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (initialBoard[i][j] != '-')
            {
                // assign color
                pieces[piece_counter].color = initialBoard[i][j];

                // assign positions
                pieces[piece_counter].position.x = j;
                pieces[piece_counter].position.y = i;

                // update counter
                piece_counter++;
            }
        }
    }
}

int won()
{
    int countW = 0, countB = 0;

    for (int i = 0; i < 24; i++)
    {
        if (pieces[i].captured)
        {
            if (pieces[i].color == 'W')
                countW++;

            else
                countB++;
        }
    }

    if (countW == 12)
    {
        printf("Black Wins!\n");
        return 1;
    }

    if (countB == 12)
    {
        printf("White Wins!\n");
        return 1;
    }

    return 0;
}
void SaveFile()
{
    FILE *fptr = fopen("temp.dat", "wb+");
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }

    if (fwrite(pieces, sizeof(struct piece), 24, fptr) != 24)
    {
        perror("Error writing to file");
        fclose(fptr);
        return;
    }

    fclose(fptr);
}
int LoadGame()
{

    FILE *fptr1 = fopen("temp.dat", "rb");
    if (fread(pieces, sizeof(struct piece), 24, fptr1) != 24)
    {
        perror("Error reading from file");

        fclose(fptr1);
        return 0;
    }
    fclose(fptr1);

    return 1;
}
void Menu()
{
    const char *menuItems[] = {
        "Play Game",
        "Instructions",
        "Credits",
    };
    const int totalItems = 3; // Number of menu items
    int selected = 0;         // Currently selected item

    while (1)
    {
        system("cls"); // Clear the screen
        printf(YEL BOLD_TEXT UNDERLINE_TEXT "MAIN MENU\n\n\n\n" WHT);

        displayMenu(menuItems, selected, totalItems); // Display menu with current selection highlighted

        char key = getch(); // Get user input

        char arrow = getch();
        if (arrow == 72)
        { // Up arrow
            selected = (selected - 1 + totalItems) % totalItems;
        }
        else if (arrow == 80)
        { // Down arrow
            selected = (selected + 1) % totalItems;
        }

        else if (key == '\r')
        { // Enter key
            // Call different functions based on selection
            switch (selected)
            {
            case 0:
                mainGame();
                break;
            case 1:
                Instructions();
                break;
            case 2:
                Credits();
                break;
            default:
                break;
            }
            break;
        }
    }
}

void displayMenu(const char *menuItems[], int selected, int totalItems)
{
    for (int i = 0; i < totalItems; i++)
    {
        if (i == selected)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf(">> %s\n", menuItems[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else
        {
            printf("   %s\n", menuItems[i]);
        }
    }
}

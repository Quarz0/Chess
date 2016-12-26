/****************************************************************************
 * Chess Library
 *
 * Alexandria University, Faculty of Engineering
 * First year, Fall 2015
 * Computer and Systems Engineering Department
 *
 * Programming I Final Project
 *
 * Marwan Tammam <darknight-x@hotmail.com>
 * Ahmed Yakout <iyakout@hotmail.com>
 *
 ***************************************************************************/
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include "chess.h"

void
newBoard(void)
{
    char board2[8][8] = {
        {'R' , 'N' , 'B' , 'Q' , 'K' , 'B' , 'N' , 'R'},
        {'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P'},
        {WHT , BLK , WHT , BLK , WHT , BLK , WHT , BLK},
        {BLK , WHT , BLK , WHT , BLK , WHT , BLK , WHT},
        {WHT , BLK , WHT , BLK , WHT , BLK , WHT , BLK},
        {BLK , WHT , BLK , WHT , BLK , WHT , BLK , WHT},
        {'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p'},
        {'r' , 'n' , 'b' , 'q' , 'k' , 'b' , 'n' , 'r'}
    };

    memcpy(board, board2, sizeof(char) * 8 * 8);
    kw = 0, kb = 0, turn = 1, redos = 0, moves = 0;

    int i,j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 16; j++)
            killed[i][j] = ' ';

    whiteKingPos[0] = 7;
    whiteKingPos[1] = 4;
    blackKingPos[0] = 0;
    blackKingPos[1] = 4;

    R00Moved = 0;
    R07Moved = 0;
    r70Moved = 0;
    r77Moved = 0;
    whiteKingMoved = 0;
    blackKingMoved = 0;

    whiteTime = 0;
    blackTime = 0;

    f = fopen("undodata", "w+");

    return;
}

void
printBoard()
{
    char menu[8][20] = {"undo\t\t-> z", "redo\t\t-> y", "save\t\t-> s", "load\t\t-> l", "quit\t\t-> q", "new game\t-> n", "surrender\t-> x", "offer draw\t-> d"};

    system("cls"); /* or system("clear") for UNIX */
    int i, j;
    // print Top ABCDEFGH
    printf("\n     ");
    for (j = 0; j < 8; j++)
        printf("%c", 'A' + j);
    printf("\t\tList of commands:");
    printf("\n");
    // print top border
    printf("    %c", 218);
    for (j = 0; j < 8; j++)
        printf("%c", 196);
    printf("%c", 191);
    printf("\n");
    // print the board, left and right killed pieces, numbers and borders
    for (i = 0; i < 8; i++)
    {
        printf("%c%c %d%c", killed[0][i*2], killed[0][i*2+1], 8-i, 179);
        for (j = 0; j < 8; j++)
            printf("%c", board[i][j]);
        printf("%c%d %c%c", 179, 8-i, killed[1][i*2], killed[1][i*2+1]);
        printf("\t-%s", menu[i]);
        printf("\n");
    }
    // print bottom border
    printf("    %c", 192);
    for (j = 0; j < 8; j++)
        printf("%c", 196);
    printf("%c", 217);
    printf("\n");
    // print bottom ABCDEFGH
    printf("     ");
    for (j = 0; j < 8; j++)
        printf("%c", 'A' + j);
    printf("\t\tLast Move: %s", lastMove);
    printf("\n\n");
   // printf("%d%d %d%d %d%d\n", R00Moved,R07Moved,r70Moved,r77Moved,whiteKingMoved,blackKingMoved);
    return;
}

void
save(void)
{
    FILE* save = fopen("save", "w");
    int i, j;
    // save the board
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            fputc(board[i][j], save);
        }
    }
    fputc('*', save);

    // save killed pieces
    for (i = 0; i < 16 && killed[0][i] != ' '; i++)
    {
        fputc(killed[0][i], save);
    }
    fputc('*', save);
    for (i = 0; i < 16 && killed[1][i] != ' '; i++)
    {
        fputc(killed[1][i], save);
    }
    fputc('*', save);
    fprintf(save, "%d%d%d%d", whiteKingPos[0], whiteKingPos[1], blackKingPos[0], blackKingPos[1]);
    fputc('*', save);
    fprintf(save, "%d", turn);
    fputc('*', save);
    fprintf(save, "%d%d%d%d%d%d", R00Moved, R07Moved, r70Moved, r77Moved, whiteKingMoved, blackKingMoved);
    fputc('*', save);
    fprintf(save, "%d*%d*%d%c", whiteTime, blackTime, mode, t);


    fclose(save);
}

bool
load(void)
{
    FILE* load = fopen("save", "r");
    if (load == NULL)
    {
        printf("No saved game found.\n");
        Sleep(1000);
        return false;
    }
    newBoard();
    int i, j;
    // scan board
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            fscanf(load, "%c", &board[i][j]);

    char c = fgetc(load);
    c = fgetc(load);
    for (kw = 0; c != '*'; kw++)
    {
        killed[0][kw] = c;
        c = fgetc(load);
    }
    c = fgetc(load);
    for (kb = 0; c != '*'; kb++)
    {
        killed[1][kb] = c;
        c = fgetc(load);
    }
    fscanf(load, "%1d%1d%1d%1d", &whiteKingPos[0], &whiteKingPos[1], &blackKingPos[0], &blackKingPos[1]);
    c = fgetc(load);
    fscanf(load, "%d", &turn);
    c = fgetc(load);
    fscanf(load, "%1d%1d%1d%1d%1d%1d", &R00Moved, &R07Moved, &r70Moved, &r77Moved, &whiteKingMoved, &blackKingMoved);
    c = fgetc(load);
    fscanf(load, "%d", &whiteTime);
    c = fgetc(load);
    fscanf(load, "%d", &blackTime);
    c = fgetc(load);
    fscanf(load, "%d%c", &mode, &t);

    fclose(load);
    redos = 0, moves = 0;
    return true;
}

void
saveMoves(int move[], char i)
{
    fseek(f, moves * (8 * sizeof(char)), SEEK_SET);
    if (i != BLK && i != WHT)
    {
        fprintf(f, "%c", i);
    }
    else
        fprintf(f, "%c", WHT);
    fprintf(f, "%d%d%d%d", move[0], move[1], move[2], move[3]);
    if (isalpha(move[4]))
        fprintf(f, "%c", move[4]);
    else
        fprintf(f, "%c", '0');

    fputc('\n', f);
}

void
redo(void)
{
    if (redos == 0)
    {
        printf("Nothing to be redone.\n");
        Sleep(1000);
        return;
    }

    fseek(f, moves * (8 * sizeof(char)), SEEK_SET);
    fgetc(f);
    char move[6];
    fscanf(f, "%s", move);

    int move2[5];
    move2[0] = move[0] - '0';
    move2[1] = move[1] - '0';
    move2[2] = move[2] - '0';
    move2[3] = move[3] - '0';
    move2[4] = move[4] == '0' ? 0 : move[4];

    int i = redos;

    makeMove(move2);
    redos = i-1;
}

void
undo(void)
{
    if (moves == 0)
    {
        printf("Nothing to be undone.\n");
        Sleep(1000);
        return;
    }
    fseek(f, (moves-1) * (8 * sizeof(char)), SEEK_SET);
    char c;
    fscanf(f, "%c", &c);
    char move[6];
    fscanf(f, "%s", move);

    int move2[5];
    move2[0] = move[0] - '0';
    move2[1] = move[1] - '0';
    move2[2] = move[2] - '0';
    move2[3] = move[3] - '0';
    move2[4] = move[4] == '0' ? 0 : move[4];

    tempUndo(move2, c);
    turn *= -1;
    c = fgetc(f);

    redos++;
    moves--;
}

void makeMoveHelper(char move[])
{
    if(strcmp(move, "0-0") == 0 || strcmp(move, "0-0-0") == 0);
    else if ( !(move[1] <= '8' && move[1] >= '1' && move[3] <= '8' && move[3] >= '1' && move[0] >='A' && move[0] <='H' && move[2] >='A' && move[2] <='H') || strlen(move) > 5)
    {
        printf("INVALID MOVE.\n");
        Sleep(1000);
        return;
    }
    int move2[5] = {0};

    if(strcmp(move, "0-0") == 0){
        move2[1] = 'I' - 'A';
        move2[3] = 'I' - 'A';
        move2[0] = 0;
        move2[2] = 0;
    }
    else if (strcmp(move, "0-0-0") == 0){
        move2[1] = 'J' - 'A';
        move2[3] = 'J' - 'A';
        move2[0] = 0;
        move2[2] = 0;
    }
    else
    {
        move2[1] = move[0]-'A';
        move2[3] = move[2]-'A';
        move2[0] = '8' - move[1];
        move2[2] = '8' - move[3];

        if (isalpha(move[4]))
        {
            move2[4] = tolower(move[4]);
            if (move2[4] != 'b' && move2[4] != 'r' && move2[4] != 'q' && move2[4] != 'n')
            {
                printf("INVALID MOVE.\n");
                Sleep(1000);
                return;
            }
        }
        else
            move[4] = 0;
    }

    makeMove(move2);
}

void
makeMove(int move2[])
{
    if (isValid(move2))
    {
        char i = tempMove(move2);

        if (isCheck())
        {
            tempUndo(move2, i);
            printf("Invalid move. Attempting to move king into check\n");
            Sleep(2000);
            return;
        }

        turn *= -1;
        saveMoves(move2, i);
        moves++;
        redos = 0;

        if (isCheck()){
            if (isCheckMate(turn))
            {
                printBoard();
                printf("CHECKMATE! \n*** %s Wins!! ***\n\n", turn == -1 ? "White" : "Black");
                exit(0);
            }
            else{
                printBoard();
                printf("CHECK!\n");
                Sleep(1000);
            }
        }
        else if (isCheckMate(turn))
        {
            printBoard();
            printf("StaleMate\n\n");
            exit(0);
        }

    }
    else
    {
        printf("INVALID MOVE.\n");
        Sleep(1000);
    }
    return;
}

void
tempUndo(int move2[], char i)
{
    if (move2[1] == 8 || move2[1] == 9)
    {
        uncastle(move2[1]);
        return;
    }
    board[move2[0]][move2[1]] = board[move2[2]][move2[3]];
    if (i == BLK || i == WHT)
        board[move2[2]][move2[3]] = ((move2[2]) + (move2[3])) % 2 == 0 ? WHT : BLK;
    else
    {
        killed[islower(i) ? 0 : 1][islower(i) ? --kw : --kb] = ' ';
        board[move2[2]][move2[3]] = i;
    }
    if (board[move2[0]][move2[1]] == 'k')
    {
        whiteKingPos[0] = move2[0];
        whiteKingPos[1] = move2[1];
        whiteKingMoved = 0;
    }
    else if (board[move2[0]][move2[1]] == 'K')
    {
        blackKingPos[0] = move2[0];
        blackKingPos[1] = move2[1];
        blackKingMoved = 0;
    }
    else if (move2[0] == 0 && move2[1] == 0 && board[move2[0]][move2[1]] == 'R')
        R00Moved = 0;
    else if (move2[0] == 0 && move2[1] == 7 && board[move2[0]][move2[1]] == 'R')
        R07Moved = 0;
    else if (move2[0] == 7 && move2[1] == 0 && board[move2[0]][move2[1]] == 'r')
        r70Moved = 0;
    else if (move2[0] == 7 && move2[1] == 7 && board[move2[0]][move2[1]] == 'r')
        r77Moved = 0;


    if (move2[4] != 0)
    {
        board[move2[0]][move2[1]] = islower(board[move2[0]][move2[1]]) ? 'p' : 'P';
    }

}

char
tempMove(int move2[])
{
    char i = WHT;


    if (move2[1] == 9)
    {
        board[turn == 1 ? 7 : 0][2] = board[turn == 1 ? 7 : 0][4];
        if (turn == 1)
        {
            whiteKingPos[0] = 7;
            whiteKingPos[1] = 2;
            whiteKingMoved = 1;
            r70Moved = 1;
        }
        else
        {
            blackKingPos[0] = 0;
            blackKingPos[1] = 2;
            blackKingMoved = 1;
            R00Moved = 1;
        }
        board[turn == 1 ? 7 : 0][3] = board[turn == 1 ? 7 : 0][0];

        board[turn == 1 ? 7 : 0][0] = (turn == 1 ? 7 : 0) % 2 == 0 ? WHT : BLK;
        board[turn == 1 ? 7 : 0][4] = ((turn == 1 ? 7 : 0) + (4)) % 2 == 0 ? WHT : BLK;
    }

    else if (move2[1] == 8)
    {
        board[turn == 1 ? 7 : 0][6] = board[turn == 1 ? 7 : 0][4];
        if (turn == 1)
        {
            whiteKingPos[0] = 7;
            whiteKingPos[1] = 6;
            whiteKingMoved = 1;
            r77Moved = 1;
        }
        else
        {
            blackKingPos[0] = 0;
            blackKingPos[1] = 6;
            blackKingMoved = 1;
            R07Moved = 1;
        }
        board[turn == 1 ? 7 : 0][5] = board[turn == 1 ? 7 : 0][7];

        board[turn == 1 ? 7 : 0][7] = ((turn == 1 ? 7 : 0) + (7)) % 2 == 0 ? WHT : BLK;
        board[turn == 1 ? 7 : 0][4] = ((turn == 1 ? 7 : 0) + (4)) % 2 == 0 ? WHT : BLK;

    }
    else{

        if (board[move2[2]][move2[3]] != BLK && board[move2[2]][move2[3]] != WHT)
        {
            killed[turn == -1 ? 0 : 1][turn == -1 ? kw++ : kb++] = board[move2[2]][move2[3]];
            i = board[move2[2]][move2[3]];
        }

        board[move2[2]][move2[3]] = board[move2[0]][move2[1]];
        board[move2[0]][move2[1]] = ((move2[0]) + (move2[1])) % 2 == 0 ? WHT : BLK;

        if (board[move2[2]][move2[3]] == 'k')
        {
            whiteKingPos[0] = move2[2];
            whiteKingPos[1] = move2[3];
            whiteKingMoved = 1;
        }
        else if (board[move2[2]][move2[3]] == 'K')
        {
            blackKingPos[0] = move2[2];
            blackKingPos[1] = move2[3];
            blackKingMoved = 1;
        }
        else if (move2[0] == 0 && move2[1] == 0 && board[move2[2]][move2[3]] == 'R')
            R00Moved = 1;
        else if (move2[0] == 0 && move2[1] == 7 && board[move2[2]][move2[3]] == 'R')
            R07Moved = 1;
        else if (move2[0] == 7 && move2[1] == 0 && board[move2[2]][move2[3]] == 'r')
            r70Moved = 1;
        else if (move2[0] == 7 && move2[1] == 7 && board[move2[2]][move2[3]] == 'r')
            r77Moved = 1;
    }
    return i;
}

void
uncastle(int i)
{
    turn*=-1;
    if (i == 9)
    {
        board[turn == 1 ? 7 : 0][4] = board[turn == 1 ? 7 : 0][2];
        if (turn == 1)
        {
            whiteKingPos[0] = 7;
            whiteKingPos[1] = 4;
            whiteKingMoved = 0;
            r70Moved = 0;
        }
        else
        {
            blackKingPos[0] = 0;
            blackKingPos[1] = 4;
            blackKingMoved = 0;
            R00Moved = 0;
        }
        board[turn == 1 ? 7 : 0][0] = board[turn == 1 ? 7 : 0][3];

        board[turn == 1 ? 7 : 0][2] = ((turn == 1 ? 7 : 0) + 2) % 2 == 0 ? WHT : BLK;
        board[turn == 1 ? 7 : 0][3] = ((turn == 1 ? 7 : 0) + (3)) % 2 == 0 ? WHT : BLK;
    }

    else if (i == 8)
    {
        board[turn == 1 ? 7 : 0][4] = board[turn == 1 ? 7 : 0][6];
        if (turn == 1)
        {
            whiteKingPos[0] = 7;
            whiteKingPos[1] = 4;
            whiteKingMoved = 0;
            r77Moved = 0;
        }
        else
        {
            blackKingPos[0] = 0;
            blackKingPos[1] = 4;
            blackKingMoved = 0;
            R07Moved = 0;
        }
        board[turn == 1 ? 7 : 0][7] = board[turn == 1 ? 7 : 0][5];

        board[turn == 1 ? 7 : 0][6] = ((turn == 1 ? 7 : 0) + (6)) % 2 == 0 ? WHT : BLK;
        board[turn == 1 ? 7 : 0][5] = ((turn == 1 ? 7 : 0) + (5)) % 2 == 0 ? WHT : BLK;

    }
    turn*=-1;
}


char*
GetTimedString(int* t)
{
    time_t a = time(NULL);
    int z = 0, l=0;
    // growable buffer for chars
    char* buffer = NULL;

    // capacity of buffer
    unsigned int capacity = 0;

    // number of chars actually in buffer
    unsigned int n = 0;

    // character read or EOF
    int c;
    // iteratively get chars from standard input
    int minutes = (*t) / 60;
    int seconds = (*t) - minutes * 60;
    printf("\rTime Left(%02d:%02d)  ", minutes, seconds);
    while (c != EOF)
    {
        time_t b = time(NULL);
        if (b-a-z >= 1)
        {
            z = b-a;
            (*t)--;
            minutes = (*t) / 60;
            seconds = (*t) - minutes * 60;
            printf("\rTime Left(%02d:%02d)  ", minutes, seconds);
            if (buffer != NULL)
            {
                buffer[n] = '\0';
                printf("%s", buffer);
            }

        }
        if ((*t) <= 0)
            return NULL;

        if (_kbhit())
        {
            c = _getch();
            l++;
            if (c == '\r')
            {
                printf("\n");
                break;
            }
            else if(c == '\b')
            {
                if (buffer == NULL)
                    continue;
                else if (buffer[0] == 0)
                    continue;

                buffer[--n] = '\0';
                printf("\b \b");
                continue;
            }
            else
            printf("%c", c);
        }
        else
            continue;

        // grow buffer if necessary
        if (n + 1 > capacity)
        {
            // determine new capacity: start at 32 then double
            if (capacity == 0)
                capacity = 32;
            else if (capacity <= (UINT_MAX / 2))
                capacity *= 2;
            else
            {
                free(buffer);
                return NULL;
            }

            // extend buffer's capacity
            char* temp = realloc(buffer, capacity * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        // append current character to buffer
        buffer[n++] = c;
    }

    // return NULL if user provided no input
    if (n == 0 && c == EOF)
        return NULL;

    // minimize buffer
    char* minimal = malloc((n + 1) * sizeof(char));
    strncpy(minimal, buffer, n);
    free(buffer);

    // terminate char*
    minimal[n] = '\0';

    // return char*
    return minimal;
}

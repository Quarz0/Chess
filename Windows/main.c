/*
 * Final_Project ( Chess game )
 * Authors: Ahmed Yakout & Marwan Tammam.
 *
 * Created on December 4, 2015, 10:00 PM
 * Last Update: ...
 *
 *******************************************************************************/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include "cs50.h"
#include "chess.h"

int main()
{
newGame:
    newBoard();

    if (f == NULL)
        return 1;

    printf("WELCOME TO \"YT CHESS\"\n\n");
    printf("Choose Game Mode (1) or (2):\n1- Player vs Computer\n2- Player vs Player\n\n");
    printf("> ");

    do
    {
        mode = GetInt();
    }
    while (mode != 1 && mode != 2);
    //FILE* run = fopen("game2.txt", "r");

    system("cls");
    printf("Enable Time Control(y/n): ");

    do
    {
        t = tolower(GetChar());
    }
    while (t != 'y' && t != 'n');

    if (t == 'y')
    {
        printf("Enter each player's amount of time in minutes for the whole game(1-60): ");
        int time;
        do
        {
            time = GetInt();
        }
        while (time < 1 || time > 60);

        whiteTime = 60 * time;
        blackTime = 60 * time;
    }

    do
    {
        printBoard();
        printf("%s> %c", turn == 1 ? "White's Turn" : "Black's Turn", t == 'y' ? '\n' : ' ');
//        char move[8];
//        fscanf(run, "%s", move);
//        printf("%s", move);
//        getchar();
        //char* move = GetTimedString(turn == 1 ? &whiteTime : &blackTime);
//
//        if (move == NULL)
//        {
//            printf("\nTime UP!\n*** %s Wins!!! ***\n", turn == 1 ? "BLACK" : "WHITE");
//            Sleep(2000);
//            break;
//        }
        char* move = malloc(sizeof(char) * 8);

        if (turn == 1)
        {
            move = (t == 'y' ? GetTimedString(&whiteTime) : GetString());
            if (move == NULL)
            {
                printf("\nTime UP!\n*** BLACK Wins!!! ***\n");
                Sleep(2000);
                break;
            }
        }
        else
        {
            if (mode == 2)
            {
                move = (t == 'y' ? GetTimedString(&blackTime) : GetString());
                if (move == NULL)
                {
                    printf("\nTime UP!\n*** WHITE Wins!!! ***\n");
                    Sleep(2000);
                    break;
                }
            }

            else
                move = AIMove();
        }


        strncpy(lastMove, move, 6*sizeof(char));
        unsigned int i;
        for (i = 0; i < strlen(move); i++)
            move[i] = toupper(move[i]);

        if (strcmp(move, "Q") == 0)
        {
            printf("Save before quitting?(yes/no/cancel) ");
            char* c = GetString();
            for (i = 0; i < strlen(c); i++)
                c[i] = toupper(c[i]);

            if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0 || strcmp(c, "N") == 0 || strcmp(c, "NO") == 0)
            {
                if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0)
                    save();
                free(c);
                free(move);
                break;
            }
            free(c);
            free(move);
            continue;
        }

        if (strcmp(move, "X") == 0)
        {
            printf("Are you sure you want to surrender?(yes/no) ");
            char* c = GetString();
            for (i = 0; i < strlen(c); i++)
                c[i] = toupper(c[i]);

            if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0)
            {
                free(c);
                printf("*** %s Wins!!! ***", turn == 1 ? "Black" : "White");
                break;

            }
            free(c);
            free(move);
            continue;
        }
        if (strcmp(move, "D") == 0)
        {
            printf("%s offered a draw, accept?(yes/no) ", turn == -1 ? "Black" : "White");
            char* c = GetString();
            for (i = 0; i < strlen(c); i++)
                c[i] = toupper(c[i]);

            if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0)
            {
                free(c);
                printf("*** DRAW ***");
                break;
            }
            else
                printf("%s rejected the draw", turn == 1 ? "Black" : "White");
            Sleep(1000);
            free(c);
            free(move);
            continue;
        }

        else if (strcmp(move, "Z") == 0)
        {
            undo();
            if (mode == 1)
                undo();
            free(move);
            continue;
        }
        else if (strcmp(move, "Y") == 0)
        {
            redo();
            if (mode == 1)
                redo();
            free(move);
            continue;
        }
        else if (strcmp(move, "S") == 0)
        {
            save();
            free(move);
            continue;
        }
        else if (strcmp(move, "L") == 0)
        {
            printf("Current game will be lost, Proceed?(yes/no) ");
            char* c = GetString();
            for (i = 0; i < strlen(c); i++)
                c[i] = toupper(c[i]);

            if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0)
            {
                if (load())
                {
                    fclose(f);
                    f = fopen("undodata", "w+");
                }
            }
            free(move);
            free(c);
            continue;
        }
        else if (strcmp(move, "N") == 0)
        {
            printf("Do you want to save this game?(yes/no/cancel) ");
            char* c = GetString();
            for (i = 0; i < strlen(c); i++)
                c[i] = toupper(c[i]);

            if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0 || strcmp(c, "N") == 0 || strcmp(c, "NO") == 0)
            {
                if (strcmp(c, "YES") == 0 || strcmp(c, "Y") == 0)
                    save();
                fclose(f);
                //f = fopen("undodata", "w+");
                //newBoard();
                free(c);
                free(move);
                system("cls");
                goto newGame;
            }
            free(c);
            continue;
        }
        makeMoveHelper(move);
        free(move);
    }
    while (1);
    fclose(f);
    //fclose(run);
    return 0;
}

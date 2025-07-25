#include <iostream>
#include <algorithm>
#include <limits>
#include <ios>

int minimax(int,bool);

bool vsAi = false;
char board[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
bool draw = false;
char turn = 'X';

int evaluate()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            return (board[i][0] == 'O') ? 10 : -10; // row
        }

        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            return (board[0][i] == 'O') ? 10 : -10; // col
        }

        if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        {
            return (board[0][0] == 'O') ? 10 : -10; // left diagonal
        }

        if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        {
            return (board[0][2] == 'O') ? 10 : -10; // right diagonal
        }
    }

    return 0;
}

void bestMove()
{
    int bestVal = -1000;
    int bestRow = -1, bestCol = -1;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 'X' && board[i][j] != 'O')
            {
                char backup = board[i][j];
                board[i][j] = 'O';
                int moveVal = minimax(0, false);
                board[i][j] = backup;
                if (moveVal > bestVal)
                {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    board[bestRow][bestCol] = 'O';
    turn = 'X';
}

bool gameOver()
{
    for (int i = 0; i < 3; i++)
    {
        if ((board[i][0] == board[i][1] && board[i][1] == board[i][2]) ||
            (board[0][i] == board[1][i] && board[1][i] == board[2][i])) //row & col
            return false;
    }
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0])) //diagonal
        return false;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return true;

    draw = true;
    return false;
}

void display_board()
{
    system("cls");
    std::cout << "\n     T i c k   C r o s s   G a m e" << std::endl;
    std::cout << "\tPlayer1[X] \n\tPlayer2[O]\n\n";

    std::cout << "\t\t      |      |     \n";
    std::cout << "\t\t  " << board[0][0] << "   |  " << board[0][1] << "   |  " << board[0][2] << "  \n";
    std::cout << "\t\t______|______|______\n";
    std::cout << "\t\t      |      |     \n";
    std::cout << "\t\t  " << board[1][0] << "   |  " << board[1][1] << "   |  " << board[1][2] << "  \n";
    std::cout << "\t\t______|______|______\n";
    std::cout << "\t\t      |      |     \n";
    std::cout << "\t\t  " << board[2][0] << "   |  " << board[2][1] << "   |  " << board[2][2] << "  \n";
    std::cout << "\t\t      |      |      \n";
}

bool isValidMove(int choice, int &row, int &col)
{
    if (choice < 1 || choice > 9)
        return false;
    row = (choice - 1) / 3;
    col = (choice - 1) % 3;

    return board[row][col] != 'X' && board[row][col] != 'O';
}

void player_turn()
{
    int choice;

    if (turn == 'X')
    {
        std::cout << "\n\t Player1 [X] turn: ";
        std::cin >> choice;

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (choice < 1 || choice > 9 || board[row][col] == 'X' || board[row][col] == 'O')
        {
            std::cout << "Invalid move. Try again.\n";
            player_turn();
        }
        else
        {
            board[row][col] = 'X';
            turn = 'O';
        }
    }
    else
    {
        if (vsAi)
        {
            bestMove();
        }
        else
        {
            std::cout << "\n\t Player2 [O] turn: ";
            std::cin >> choice;

            int row = (choice - 1) / 3;
            int col = (choice - 1) % 3;

            if (choice < 1 || choice > 9 || board[row][col] == 'X' || board[row][col] == 'O')
            {
                std::cout << "Invalid move. Try again.\n";
                player_turn();
            }
            else
            {
                board[row][col] = 'O';
                turn = 'X';
            }
        }
    }
}

bool isMovesLeft()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return true;
    return false;
}

int minimax(int depth, bool isMax) //for ai not , for player 1 --> ai moves to a position and after that predicts user moves and with lowest user move value / highest ai move value it chooses the original move  
{ // false
    int score = evaluate();
    if (score == 10 || score == -10)
        return score;
    if (!isMovesLeft())
        return 0;

    if (isMax)
    {
        int best = -1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != 'X' && board[i][j] != 'O')
                {
                    char temp = board[i][j];
                    board[i][j] = 'O';
                    best = std::max(best, minimax(depth + 1, !isMax));
                    board[i][j] = temp;
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != 'X' && board[i][j] != 'O')
                {
                    char temp = board[i][j];
                    board[i][j] = 'X';
                    best = std::min(best, minimax(depth + 1, !isMax));
                    board[i][j] = temp;
                }
            }
        }
        return best;
    }
}

int main()
{
    int mode;
    std::cout << "\nSelect Mode: \n1. Player vs Player\n2. Player vs AI\nChoice: ";
    std::cin >> mode;

    vsAi = (mode == 2);

    while (gameOver())
    {
        display_board();
        player_turn();
    }

    display_board();
    if (draw)
        std::cout << "\nIt's a a draw" << std::endl;
    else
    {
        if (turn == 'X')
            std::cout << "\nPlayer2 [O] Wins! Congratulations!" << std::endl;
        else
            std::cout << "\nPlayer1 [X] Wins! Congratulations!" << std::endl;
    }

    return 0;
}

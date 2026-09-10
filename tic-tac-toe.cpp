#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

char board[3][3];

// Initialize board
void resetBoard() {
    char n = '1';

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = n++;
}

// Display board
void displayBoard() {
    cout << "\n\n";
    cout << "       TIC-TAC-TOE\n";
    cout << "   ┌───┬───┬───┐\n";

    for (int i = 0; i < 3; i++) {
        cout << "   │ ";
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << " │ ";

        cout << '\n';

        if (i < 2)
            cout << "   ├───┼───┼───┤\n";
    }

    cout << "   └───┴───┴───┘\n";
}

// Check winner
bool checkWinner(char player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player &&
            board[i][1] == player &&
            board[i][2] == player)
            return true;

        if (board[0][i] == player &&
            board[1][i] == player &&
            board[2][i] == player)
            return true;
    }

    return
        (board[0][0] == player &&
         board[1][1] == player &&
         board[2][2] == player) ||

        (board[0][2] == player &&
         board[1][1] == player &&
         board[2][0] == player);
}

// Check if board is full
bool isDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
                return false;

    return true;
}

// Make a move
void placeMove(int position, char player) {
    int row = (position - 1) / 3;
    int col = (position - 1) % 3;

    board[row][col] = player;
}

// Check whether move is available
bool available(int position) {
    int row = (position - 1) / 3;
    int col = (position - 1) % 3;

    return board[row][col] >= '1' &&
           board[row][col] <= '9';
}

// Player move
void playerMove() {
    int position;

    while (true) {
        cout << "\nYour move (1-9): ";

        if (!(cin >> position)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter a valid number.\n";
            continue;
        }

        if (position < 1 || position > 9) {
            cout << "Choose a position from 1 to 9.\n";
            continue;
        }

        if (!available(position)) {
            cout << "Position already occupied.\n";
            continue;
        }

        placeMove(position, 'X');
        break;
    }
}

// Find winning/blocking move
int findWinningMove(char player) {
    for (int position = 1; position <= 9; position++) {

        if (available(position)) {
            placeMove(position, player);

            if (checkWinner(player)) {
                placeMove(position, 'O');
                return position;
            }

            // Restore position
            int row = (position - 1) / 3;
            int col = (position - 1) % 3;
            board[row][col] = '0' + position;
        }
    }

    return -1;
}

// AI move
void aiMove() {

    // 1. Try to win
    int move = findWinningMove('O');

    if (move != -1) {
        cout << "\n🤖 AI chose position " << move << "!\n";
        return;
    }

    // 2. Block player
    move = findWinningMove('X');

    if (move != -1) {
        cout << "\n🤖 AI blocked your move at " << move << "!\n";
        return;
    }

    // 3. Take center
    if (available(5)) {
        placeMove(5, 'O');
        cout << "\n🤖 AI chose center!\n";
        return;
    }

    // 4. Take a corner
    vector<int> corners = {1, 3, 7, 9};
    random_shuffle(corners.begin(), corners.end());

    for (int position : corners) {
        if (available(position)) {
            placeMove(position, 'O');
            cout << "\n🤖 AI chose position "
                 << position << "!\n";
            return;
        }
    }

    // 5. Take any available position
    vector<int> moves;

    for (int i = 1; i <= 9; i++)
        if (available(i))
            moves.push_back(i);

    if (!moves.empty()) {
        move = moves[rand() % moves.size()];
        placeMove(move, 'O');

        cout << "\n🤖 AI chose position "
             << move << "!\n";
    }
}

// Play one round
void playGame() {
    resetBoard();

    cout << "\n";
    cout << "You are X  |  AI is O\n";
    cout << "Make your move using positions 1-9.\n";

    while (true) {

        // Player turn
        displayBoard();
        playerMove();

        if (checkWinner('X')) {
            displayBoard();
            cout << "\n🏆 CONGRATULATIONS! YOU WIN!\n";
            break;
        }

        if (isDraw()) {
            displayBoard();
            cout << "\n🤝 GAME DRAW!\n";
            break;
        }

        // AI turn
        aiMove();

        if (checkWinner('O')) {
            displayBoard();
            cout << "\n🤖 AI WINS! Better luck next time.\n";
            break;
        }

        if (isDraw()) {
            displayBoard();
            cout << "\n🤝 GAME DRAW!\n";
            break;
        }
    }
}

int main() {

    srand(static_cast<unsigned>(time(nullptr)));

    char replay;

    cout << "\n";
    cout << "╔══════════════════════════════════╗\n";
    cout << "║     🎮 TIC-TAC-TOE vs AI        ║\n";
    cout << "╠══════════════════════════════════╣\n";
    cout << "║  👤 You : X                     ║\n";
    cout << "║  🤖 AI  : O                     ║\n";
    cout << "╚══════════════════════════════════╝\n";

    do {
        playGame();

        cout << "\nPlay again? (Y/N): ";
        cin >> replay;

        replay = toupper(replay);

    } while (replay == 'Y');

    cout << "\nThanks for playing! 🎮\n";

    return 0;
}

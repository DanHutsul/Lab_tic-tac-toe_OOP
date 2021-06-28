#include <iostream>
#include <vector>
#include <algorithm>
//#include <random>
#include <string>
#define DIMENSIONS       3

bool isNumber(std::string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

class board {
public:
    board() {
        table.resize(DIMENSIONS);
        for(int i = 0; i < DIMENSIONS; i++)
            for (int j = 0; j < DIMENSIONS; j++)
                table[i].push_back(-1);
    }

    int get_value(int row, int col) {
        return table[row][col];
    }

    bool is_empty(int row, int col) {
        if (row >= table.size() || col >= table.size() || row < 0 || col < 0)
            return false;
        if (table[row][col] == -1) {
            return true;
        }
        return false;
    }

    int set(int row, int col, int symbol) {
        if (is_empty(row, col)) {
            table[row][col] = symbol;
            return true;
        }
        return false;
    }

    // -1 if not end, 0 if draw, other number to show the winner
    int end_state() {
        int temp_symbol;
        // Check rows
        for (int r = 0; r < DIMENSIONS; r++) {
            temp_symbol = table[r][0];
            if (temp_symbol == -1)
                break;
            for (int c = 1; c < DIMENSIONS; c++) {
                if (table[r][c] != temp_symbol)
                    break;
                if (c == DIMENSIONS-1) {
                    //std::cout << temp_symbol << std::endl;
                    return temp_symbol;
                }
            }
        }
        // Check columns
        for (int c = 0; c < DIMENSIONS; c++) {
            temp_symbol = table[0][c];
            if (temp_symbol == -1)
                break;
            for (int r = 1; r < DIMENSIONS; r++) {
                if (table[r][c] != temp_symbol)
                    break;
                if (r == DIMENSIONS-1) {
                    //std::cout << temp_symbol << std::endl;
                    return temp_symbol;
                }
            }
        }
        // Check diagonals
        // Left to Right
        temp_symbol = table[0][0];
        for (int i = 1; i < DIMENSIONS; i++) {
            if (temp_symbol == -1)
                break;
            if (table[i][i] != temp_symbol)
                break;
            if (i == DIMENSIONS-1) {
                //std::cout << temp_symbol << std::endl;
                return temp_symbol;
            }
        }
        // Right to left
        temp_symbol = table[0][DIMENSIONS-1];
        for (int i = 1; i < DIMENSIONS; i++) {
            if (temp_symbol == -1)
                break;
            if (table[i][DIMENSIONS-1-i] != temp_symbol)
                break;
            if (i == DIMENSIONS-1)
                return temp_symbol;
        }

        // Check if there are free squares
        for (int r = 0; r < DIMENSIONS; r++)
            for (int c = 0; c < DIMENSIONS; c++)
                if (table[r][c] == -1)
                    return -1;
        return 0;
    }
    board(board& b) {
        table = b.table;
    }

private:
    std::vector<std::vector<int>> table;
};

class ai {
public:
    virtual int choose_move(board &b, int symbol) = 0;
};

class rand_ai: public ai {
public:
    virtual int choose_move(board &b, int symbol) {
        int r = (rand() % (3));
        int c = (rand() % (3));
        while (!b.set(r, c, symbol)) {
            r = (rand() % (3));
            c = (rand() % (3));
        }
        return 0;
    }
};


class hard_ai: public ai {
private:
    int minimax(board &b, int symbol) {
        int move_row = -1;
        int score = -2;
        // Check for win state
        int end = b.end_state();
        if (end == symbol) {
            return -10;
        }
        // Check for lose state
        else if (end == symbol%2+1) {
            return 10;
        }
        // Check for draw state
        else if (end == 0) {
            return 0;
        }

        // If not ended - continue
        for (int i = 0; i < DIMENSIONS; i++) {
            for (int j = 0; j < DIMENSIONS; j++) {
                if (b.is_empty(i, j)) {
                    board temp_board = b;
                    temp_board.set(i, j, symbol);
                    int temp_score = -minimax(temp_board, symbol%2+1);
                    if (temp_score > score) {
                        score = temp_score;
                    }
                }
            }
        }
        return score;
    }
public:
    virtual int choose_move(board &b, int symbol) {
        int move_row, move_col;

        int score = -11;
        for (int i = 0; i < DIMENSIONS; i++) {
            for (int j = 0; j < DIMENSIONS; j++) {
                if (b.is_empty(i, j)) {
                    board temp_board = b;
                    temp_board.set(i, j, symbol);
                    int temp_score = -minimax(temp_board, symbol);

                    if (temp_score > score) {
                        score = temp_score;
                        move_row = i;
                        move_col = j;
                    }
                }
            }
        }
        b.set(move_row, move_col, symbol);
        return 0;
    }
};

class player {
private:
    int symbol;
    bool is_ai = false;
    ai* player_ai;
public:
    player (int in_symbol){
        symbol = in_symbol;
    }
    player(int in_symbol, ai &ai_type) {
        symbol = in_symbol;
        is_ai = true;
        player_ai = &ai_type;
    }

    int get_symbol() {
        return symbol;
    }

    bool is_player() {
        return !is_ai;
    }

    void move(board &b) {
        if (is_ai)
            player_ai->choose_move(b, symbol);
        else {
            // Insert prompt to ask coords
            // Continue until the correct move is given
            while (true) {
                std::string row, col;
                std::cout << "INPUT ROW:" << std::endl;
                std::cin >> row;
                std::cout << "INPUT COLUMN:" << std::endl;
                std::cin >> col;
                if (!isNumber(row) || !isNumber(col))
                    std::cout << "WRONG INPUT!" << std::endl;
                 else if (b.set(std::stoi(row)-1, std::stoi(col)-1, symbol))
                    return;
                else
                    std::cout << "WRONG INPUT!" << std::endl;
            }
        }
    }
};
class game_master {
private:
    int turn_count = 0;
    board b;
    char symbols[2] {'X', 'O'};
    char correct_in[3] {'p', 'r', 'h'};
    player players[2] {player(1), player(2)};
public:
    game_master() {
        b = board();
        for (int i = 0; i < 2; i++) {
            while (true) {
                std::cout << "Input settings for Player " << i+1 << ": " << std::endl;
                std::cout << "p - for player controlled | r - random move AI | h - imperfect minimax AI" << std::endl;
                std::string in;
                std::cin >> in;
                //std::cout << pl1 << std::endl;
                auto itr = std::find(correct_in, correct_in + sizeof(correct_in)/sizeof(correct_in[0]), in[0]);
                if (itr != std::end(correct_in)) {
                    int index = std::distance(correct_in, itr);
                    ai* new_ai;
                    switch (index) {
                        case 1:
                            new_ai = new rand_ai();
                            players[i] = player(i+1, *new_ai);
                            break;
                        case 2:
                            new_ai = new hard_ai();
                            players[i] = player(i+1, *new_ai);
                            break;
                        default:
                            players[i] = player(i+1);
                            break;
                    }
                    break;
                }
                std::cout << "Wrong input, try again" << std::endl;
            }
        }
        play_turn();
    }

    void play_turn() {
        draw_board();
        int state = b.end_state();
        switch (state) {
            case 0:
                std::cout << " -- DRAW -- " << std::endl;
                return;
            case -1:
                break;
            default:
                std::cout << " -- Player " << state << " wins! -- " << std::endl;
                return;
        }
        std::cout << " -- Player " << turn_count%2+1 << "'s turn -- " << std::endl;
        players[turn_count%2].move(b);
        turn_count++;
        play_turn();
    }

    void draw_board() {
        for (int r = 0; r < DIMENSIONS; r++) {
            for (int c = 0; c < DIMENSIONS; c++) {
                std::cout << "|";
                auto val = b.get_value(r, c);
                if (val > 0)
                    std::cout << symbols[val-1];
                else
                    std::cout << "_";
            }
            std::cout << "|" << std::endl;
        }
    }
};

int main() {
    game_master();
    return 0;
}

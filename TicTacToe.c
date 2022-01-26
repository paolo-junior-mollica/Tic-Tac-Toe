#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define AI 1
#define HUMAN (-1)
#define DRAW 0
#define N_CELLS 9

typedef struct tic_tac_toe {
    int *grid;
    int blank_cells;
    int current_player;
    int winner;
} TicTacToe;


int coin_toss();
void make_a_move(TicTacToe *game);
int MINIMAX_decision(TicTacToe game);
int human_decision(TicTacToe game);
int MIN_value(TicTacToe game);
int MAX_value(TicTacToe game);
TicTacToe result(TicTacToe game, int action);
void print_grid(TicTacToe game);
int check_winner(TicTacToe game);
bool terminal_test(TicTacToe game);

int main() {
    TicTacToe game;
    game.blank_cells = N_CELLS;
    game.winner = DRAW;
    game.grid = malloc(sizeof(int) * N_CELLS);
    for (int i = 0; i < N_CELLS; ++i)
        game.grid[i] = 0;
    printf("Welcome to Tic-Tac-Toe!\n");
    game.current_player = coin_toss();
    game.current_player == AI ? printf("AI starts!\n") : printf("You start!\n");
    while (game.blank_cells > 0 && game.winner == DRAW) {
        print_grid(game);
        make_a_move(&game);
        game.winner = check_winner(game);
        game.current_player *= -1;
    }
    print_grid(game);
    if (game.winner == AI)
        printf("You lose!\n");
    else if (game.winner == HUMAN)
        printf("You win!\n");
    else printf("Draw!\n");
    return EXIT_SUCCESS;
}

void make_a_move(TicTacToe* game) {
    if (game->current_player == AI) {
        int best_action = MINIMAX_decision(*game);
        game->grid[best_action] = AI;
    } else {
        int human_action = human_decision(*game);
        game->grid[human_action] = HUMAN;
    }
    game->blank_cells--;
}

int MINIMAX_decision(TicTacToe game) {
    int best_action = -1;
    int best_value = -2;    // -Inf
    for (int i = 0; i < N_CELLS; ++i) {
        if (game.grid[i] == 0) {
            int value = MIN_value(result(game, i));
            if (value > best_value) {
                best_value = value;
                best_action = i;
            }
        }
    }
    return best_action;
}

int MIN_value(TicTacToe game) {
    if (terminal_test(game))
        return check_winner(game);
    int best_value = 2;     // +Inf
    for (int i = 0; i < N_CELLS; ++i) {
        if (game.grid[i] == 0) {
            int value = MAX_value(result(game, i));
            if (value < best_value)
                best_value = value;
        }
    }
    return best_value;
}

int MAX_value(TicTacToe game) {
    if (terminal_test(game))
        return check_winner(game);
    int best_value = -2;    // -Inf
    for (int i = 0; i < N_CELLS; ++i) {
        if (game.grid[i] == 0) {
            int value = MIN_value(result(game, i));
            if (value > best_value)
                best_value = value;
        }
    }
    return best_value;
}

int human_decision(TicTacToe game) {
    int action = -1;
    if (game.blank_cells == 1) {    // Only choice
        while (++action < N_CELLS && game.grid[action] != 0);
    } else {
        do {
            printf("\nMake your move: choose a number between 1 and 9\n");
            scanf("%d", &action);
            printf("\n");
            action--;
        } while (action < 0 || action > 8 || game.grid[action] != 0);
    }
    return action;
}

// Transition model
TicTacToe result(TicTacToe game, int action) {
    TicTacToe simulated_game;
    simulated_game.winner = DRAW;
    simulated_game.blank_cells = game.blank_cells;
    simulated_game.current_player = game.current_player;
    simulated_game.grid = malloc(sizeof(int) * N_CELLS);
    for (int i = 0; i < N_CELLS; ++i)
        simulated_game.grid[i] = game.grid[i];
    simulated_game.grid[action] = simulated_game.current_player;
    simulated_game.blank_cells--;
    simulated_game.current_player *= -1;
    return simulated_game;
}

int coin_toss() {
    srand(time(0));
    printf("Coin toss: ");
    int first_player = (rand() % 2);
    if (first_player == AI)
        return AI;
    return HUMAN;
}

void print_grid(TicTacToe game) {
    printf("Current configuration:\n");
    for (int i = 0; i < N_CELLS; ++i) {
        switch (game.grid[i]) {
            case AI:
                printf(" O ");
                break;
            case 0:
                printf(" . ");
                break;
            case HUMAN:
                printf(" X ");
                break;
            default:
                fprintf(stderr, "Internal error");
                exit(1);
        }
        if (i == 2 || i == 5)
            printf("\n");
    }
    printf("\n\n");
}

// Utility function
int check_winner(TicTacToe game) {
    int wins[8][3] = {{0, 1, 2},
                      {3, 4, 5},
                      {6, 7, 8},
                      {0, 3, 6},
                      {1, 4, 7},
                      {2, 5, 8},
                      {0, 4, 8},
                      {2, 4, 6}};
    for (int i = 0; i < 8; ++i) {
        if (game.grid[wins[i][0]] != 0 &&
            game.grid[wins[i][0]] == game.grid[wins[i][1]] &&
            game.grid[wins[i][0]] == game.grid[wins[i][2]])
            return game.grid[wins[i][0]];
    }
    return DRAW;
}

// Terminal test
bool terminal_test(TicTacToe game) {
    return check_winner(game) != DRAW || game.blank_cells == 0;
}

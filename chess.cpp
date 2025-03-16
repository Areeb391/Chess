#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();
const int NEG_INF = numeric_limits<int>::min();

char board[8][8] = {
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

struct Move {
    int fromX, fromY, toX, toY;
};

vector<Move> generateMoves(bool isWhite) {
    vector<Move> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((isWhite && board[i][j] == 'P' && i > 0 && board[i - 1][j] == '.') ||
                (!isWhite && board[i][j] == 'p' && i < 7 && board[i + 1][j] == '.')) {
                moves.push_back({i, j, isWhite ? i - 1 : i + 1, j});
            }
        }
    }
    return moves;
}

int evaluateBoard() {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 'P') score += 10;
            else if (board[i][j] == 'p') score -= 10;
        }
    }
    return score;
}

int minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0) return evaluateBoard();
    
    vector<Move> moves = generateMoves(maximizingPlayer);
    if (moves.empty()) return evaluateBoard();

    if (maximizingPlayer) {
        int maxEval = NEG_INF;
        for (Move move : moves) {
            swap(board[move.fromX][move.fromY], board[move.toX][move.toY]);
            int eval = minimax(depth - 1, alpha, beta, false);
            swap(board[move.toX][move.toY], board[move.fromX][move.fromY]);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = INF;
        for (Move move : moves) {
            swap(board[move.fromX][move.fromY], board[move.toX][move.toY]);
            int eval = minimax(depth - 1, alpha, beta, true);
            swap(board[move.toX][move.toY], board[move.fromX][move.fromY]);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

Move findBestMove() {
    int bestValue = NEG_INF;
    Move bestMove;
    vector<Move> moves = generateMoves(false);
    for (Move move : moves) {
        swap(board[move.fromX][move.fromY], board[move.toX][move.toY]);
        int moveValue = minimax(3, NEG_INF, INF, true);
        swap(board[move.toX][move.toY], board[move.fromX][move.fromY]);
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    return bestMove;
}

void printBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    while (true) {
        printBoard();
        
        int fromX, fromY, toX, toY;
        cout << "Enter your move (fromX fromY toX toY): ";
        cin >> fromX >> fromY >> toX >> toY;

        if (board[fromX][fromY] == 'P' && board[toX][toY] == '.') {
            board[toX][toY] = 'P';
            board[fromX][fromY] = '.';
        }

        Move bestMove = findBestMove();
        board[bestMove.toX][bestMove.toY] = 'p';
        board[bestMove.fromX][bestMove.fromY] = '.';

        cout << "Computer moved from " << bestMove.fromX << "," << bestMove.fromY
             << " to " << bestMove.toX << "," << bestMove.toY << endl;
    }

    return 0;
}

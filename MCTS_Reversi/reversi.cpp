#include "reversi.hpp"
#include <iostream>

std::mutex mutex_valid_pos; //对valid_position加锁
Reversi::Reversi() { //
    score = 0;          //初始分数为0
    turn = BLACK_TURN;  //黑棋先走
    //初始化棋盘
    memset(board, 0, sizeof(int) * SIZE_BOARD * SIZE_BOARD);
    board[SIZE_BOARD/2][SIZE_BOARD/2-1] = BLACK_CHESS;
    board[SIZE_BOARD/2-1][SIZE_BOARD/2] = BLACK_CHESS;
    board[SIZE_BOARD/2-1][SIZE_BOARD/2-1] = WHITE_CHESS;
    board[SIZE_BOARD/2][SIZE_BOARD/2] = WHITE_CHESS;
    update_valid_positoin();
}
Reversi::Reversi(const Reversi &r){
    *this = r;
}
int Reversi::update_score(){ //计算当前局面分数
    int sum(0);
    for(int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            sum += board[i][j];
    score = sum;
    return sum;
}

void Reversi::set(std::string pos){
    action = pos;
    int i, j;
    get_position(pos, i, j);
    board[i][j] = turn;
    for (std::vector<int>::iterator it = valid_position[pos].begin(); it != valid_position[pos].end(); it++){
        int _i = i, _j = j;
        for (int step = 0; step < 8; step++){
            _i -= direction[*it][0];
            _j -= direction[*it][1];
            assert(_i >= 0 && _i < SIZE_BOARD && _j >= 0 && _j < SIZE_BOARD);
            if (board[_i][_j] == -turn) {
                board[_i][_j] = turn;
                //std::cout << "Reversi " << _i << " " <<  _j << "\n";
            }
            else break;
        }
    }
}

void Reversi::update_valid_positoin(){
    mutex_valid_pos.lock();
    update_chess_list();
    valid_position.clear();
    if (turn == BLACK_TURN){
        //std::cout << "UPDATE BLACK\n";
        for (int it = 0; it < white.size(); it++){ //遍历所有白棋
            int x, y;
            get_position(white[it], x, y);
            for (int dir = 0; dir < 8; dir++){ //遍历所有方向
                int _x = x + direction[dir][0];
                int _y = y + direction[dir][1];
                if (_x < 0 || _x >= 8 || y < 0 || y >= 8) continue; //出界下一个方向
                if (board[_x][_y] == 0 && legal_pos(_x, _y, dir)){ //空位且合法
                    std::string s = "";
                    position2s(s, _x, _y);
                    valid_position[s].push_back(dir);
                }
            }
        }
    } else if (turn == WHITE_TURN){
        //std::cout << "UPDATE WHITE\n";
        for (int it = 0; it < black.size(); it++){ 
            int x, y;
            get_position(black[it], x, y);
            for (int dir = 0; dir < 8; dir++){ 
                int _x = x + direction[dir][0];
                int _y = y + direction[dir][1];
                if (_x < 0 || _x >= 8 || y < 0 || y >= 8) continue; //出界下一个方向
                if (board[_x][_y] == 0 && legal_pos(_x, _y, dir)){ //空位且合法
                    std::string s = "";
                    position2s(s, _x, _y);
                    valid_position[s].push_back(dir);
                }
            }
        }
    }
    mutex_valid_pos.unlock();
    //=========================
    //debug
    //for (std::vector<std::string>::iterator it = black.begin(); it != black.end(); it++)
        //std::cout << "BLACK POS: " << *it << std::endl;
    //for (std::vector<std::string>::iterator it = white.begin(); it != white.end(); it++)
        //std::cout << "WHITE POS: " << *it << std::endl;
}
void Reversi::switch_player(){
    turn = -turn;
}
void Reversi::display(){
    //print head of board
    //show_valid_place();
    printf("TURN: %s \t SCORE:%d \t LAST POS: %s\n", turn == BLACK_TURN ? "BLACK X" : "WHITE O", score, action.c_str());
    printf("   "); for (int i = 0; i < SIZE_BOARD; i++) printf("%c  ", 'A'+i); printf("\n");
    for (int i = 0; i < SIZE_BOARD; i++){
        printf("%d  ", i);
        for (int j = 0; j < SIZE_BOARD; j++){
            printf("%c  ", board[i][j]==BLACK_CHESS ? 'X' : (board[i][j] == WHITE_CHESS ? 'O' : '-'));
        }
        printf("\n");
    }
}

void Reversi::update_chess_list(){
    black.clear();
    white.clear();
    for (int i = 0; i < SIZE_BOARD; i++){
        for (int j = 0; j < SIZE_BOARD; j++){
            if (board[i][j] == BLACK_CHESS){
                std::string s = "";
                position2s(s, i, j);
                black.push_back(s);
            } else if (board[i][j] == WHITE_CHESS){
                std::string s = "";
                position2s(s, i, j);
                white.push_back(s);
            }
        }
    }
}

bool Reversi::legal_pos(int _x, int _y, int dir){ //dir空白相对棋子的方向
    //std::string s=""; position2s(s, _x, _y); std::cout << " CHECK " << s << std::endl;
    if (_x < 0 || _x >= SIZE_BOARD || _y < 0 || _y >= SIZE_BOARD) return false;
    for (int step = 0; step < 8; step++){ //朝反方向走
        _x -= direction[dir][0];
        _y -= direction[dir][1];
        if (_x < 0 || _x >= 8 || _y < 0 || _y >= 8) continue;
        if (board[_x][_y] == 0) return false;
        if (board[_x][_y] == -turn) continue; //还是对方的棋，继续走
        if (board[_x][_y] == turn) {
            return true;
        }
    }
    return false;
}

void Reversi::show_valid_place(){
    std::cout << "========Valid Pos=========\n";
    for (std::map<std::string, std::vector<int> >::iterator it = valid_position.begin(); it != valid_position.end(); it++){
        std::cout << "Valid Pos: " << it->first;
        for (int i = 0; i < it->second.size(); i++) std::cout << " direction: " << it->second[i] << "\t";
        std::cout << std::endl;
    }
    std::cout << "==========================\n";
}
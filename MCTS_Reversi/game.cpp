#include <unistd.h>
#include <time.h>
#include <iostream>
#include "game.hpp"
#include "MCTS.hpp"
#define __DEBUG

std::string action = ""; //玩家走的
int iterators = 100;
int rounds = 100;
float c = 1 / sqrt(2); //超参数

Game::Game(){
    black_flag = true;
    white_flag = true;
}
Game::Game(const Game& g){
    *this = g;
}

void Game::one_round(std::string pos){
    assert(board.valid_position.find(pos) != board.valid_position.end());
    board.set(pos);
    board.update_score();
    board.switch_player(); //交换对手
}

int Game::simulate(){
    Game tmp(*this);
    while(!tmp.is_terminal()){
        //========debug=
        //std::cout<<"\n\n============================\n";
        //std::cout<<"+++++++++++++++\n";
        //std::cout<<"BLACK_FLAG: " << tmp.black_flag;
        //std::cout<<"\nWHITE_FLAG: " << tmp.white_flag;
        //std::cout<<"\n:Last Pos: " << tmp.action;
        //std::cout<<"\n+++++++++++++++\n";
        tmp.board.update_score();
        //tmp.board.display();
        //==============
        tmp.board.update_valid_positoin();
        int len = tmp.board.valid_position.size();
        if (len != 0){
            std::string pos;
            if (tmp.board.valid_position.find("A0") != tmp.board.valid_position.end()) pos = "A0";
            else if(tmp.board.valid_position.find("A7") != tmp.board.valid_position.end()) pos = "A7";
            else if(tmp.board.valid_position.find("H0") != tmp.board.valid_position.end()) pos = "H0";
            else if(tmp.board.valid_position.find("H7") != tmp.board.valid_position.end())  pos = "H7";
            else{
                int k = ((int)(drand48() * len) % len);//随机选一个位置
                int i = -1;
                for (std::map<std::string, std::vector<int> >::iterator it = tmp.board.valid_position.begin(); i < k; i++){
                    pos = it->first;
                }
            }
            if (tmp.board.turn == WHITE_TURN) tmp.white_flag = true;
            else tmp.black_flag = true;
            tmp.one_round(pos);
        } else{
            if (tmp.board.turn == WHITE_TURN) tmp.white_flag = false;
            else if (tmp.board.turn == BLACK_TURN) tmp.black_flag = false;
            tmp.board.switch_player();
        }
    }
    tmp.board.update_score();
    if (tmp.board.score > 0) tmp.board.score += 100;
    else if (tmp.board.score < 0) tmp.board.score -= 100;
    return tmp.board.score;
}

bool Game::is_terminal() const{
    if (white_flag || black_flag) return false;
    return true;
}

void Game::pvp(){
    while(!is_terminal()){
        std::cout << "==============ROUND=============\n";
        board.update_score();
        board.update_valid_positoin();
        board.display();
        //get input
        //MCTS find best
        std::string pos;
        do{
            std::cin >> pos;
        } while(board.valid_position.find(pos) == board.valid_position.end());
        one_round(pos);
    }
}

void Game::pve(){
    clock_t start, end;
    while(1){
        if (is_terminal()){
            sleep(2);
            board = Reversi();
        }
        board.update_score();
        if (!check_valid()){
            board.switch_player();
            continue;
        }
        std::string pos;
        //get input
        //MCTS find best
        if (board.turn == BLACK_TURN){ //黑棋 玩家走
            do{
                if (action != "") pos = action;
            } while(board.valid_position.find(pos) == board.valid_position.end());
            one_round(pos);
        } else { //白棋 电脑走
            start = clock();
            MCTS mcts(*this, iterators, c);
            end = clock();
            printf("decision time:%lf\n", (double)(end-start)/CLOCKS_PER_SEC);
            pos = mcts.get_best_choice();
            //std::cout << "BEST CHOICE:" << pos << std::endl;
            one_round(pos);
        }
    }
}

void Game::pve_debug(){
    clock_t start, end;
    int t = rounds, black_win(0);
    while(t){
        board.update_score();
        if (is_terminal()){
            t--;
            if (board.score > 0) black_win++;
            printf("-----------------------------==%d AI winning rate:%.3f\n", rounds - t, (float)(rounds - black_win - t) / (float)(rounds-t));
            sleep(1);
            board = Reversi();
        }

        if (!check_valid()){
            board.switch_player();
            continue;
        }
        std::string pos;
        if (board.turn == BLACK_TURN){ //黑棋 玩家走
            srand((unsigned int) time(NULL));
            int k = rand() % board.valid_position.size();
            std::map<std::string, std::vector<int> >::const_iterator it = board.valid_position.begin();
            while(k--) {it++;}
            pos = it->first;
            one_round(pos);
        } else { //白棋 电脑走
            start = clock();
            MCTS mcts(*this, iterators, c);
            end = clock();
#ifdef __DEBUG
            printf("\tdecision time:%lf\n", (double)(end-start)/CLOCKS_PER_SEC);
#endif
            pos = mcts.get_best_choice();
            one_round(pos);
        }
    }
    //printf("Winner: BLACK: %d\t WHITE: %d \t AI winning rate:%.3f\n", black_win, rounds - black_win, (float)(rounds - black_win) / (float)rounds);
}

bool Game::check_valid(){
    board.update_valid_positoin();
    if (board.valid_position.size() != 0) {
        if (board.turn == WHITE_TURN) {
            white_flag = true;
        }
        else if (board.turn == BLACK_TURN) {
            black_flag = true;
        }
        return true;

    } else{ //无合法落子位置
        if (board.turn == WHITE_TURN) white_flag = false;
        else if (board.turn == BLACK_TURN) black_flag = false;
        return false;
    }
}

#ifndef GAME_H
#define GAME_H
#include "reversi.hpp"

extern std::string action; //玩家合法动作，只对前端写
extern int iterators;//MCTS迭代次数
extern int rounds;  //游戏局数
extern float c;     //超参数

class Game{
public:
    Game();
    Game(const Game&);
    void one_round(std::string pos);
    void pvp();
    void pve();
    void pve_debug();
    bool is_terminal() const;
    bool check_valid();
    int simulate();
    Reversi board;
private:
    bool white_flag, black_flag;
};
#endif
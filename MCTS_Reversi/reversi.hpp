#ifndef REVERSI
#define REVERSI

#include <vector>
#include <map>
#include <string>
#include <mutex>

#define BLACK_TURN 1 
#define WHITE_TURN -1
#define BLACK_CHESS 1
#define WHITE_CHESS -1
#define SIZE_BOARD 8
#define get_position(c, x, y) { y = c[0] - 'A'; x = c[1]-'0'; assert(x < 8 && y < 8); }
#define position2s(c, x, y) { c += 'A' + y; c += '0' + x; }
#define check_and_push_2_valid_list(x, y) {\
    if (check_boundary(x, y) && board[x][y] == 0){\
        std::string tmp; tmp = y + 'A'; tmp += x + '0';\
        valid_position.push_back(tmp);\
    }\
}
extern std::mutex mutex_valid_pos; //对valid_position加锁

static const int direction[8][2] = { 
         0, -1, //0left
         0,  1, //1right
        -1,  0, //2up
         1,  0, //3down
        -1, -1, //4upleft
         1, -1, //5upright
        -1,  1, //6downleft
         1,  1}; //7downright

class Reversi{
public:
    Reversi(); //consturctor
    Reversi(const Reversi&);

    int score;
    int turn;
    std::map<std::string, std::vector<int> > valid_position; //棋子有效位置, 合法位置的反方向

    std::string action; //上一步如何到这一步
    int update_score();           //计算当前分数
    void set(std::string pos);   //放置棋子
    void switch_player();
    void display();
    void update_valid_positoin();   //更新有效落子位置
    void update_chess_list();
    void show_valid_place();
    std::vector<std::string> black, white; //落子位置
private:
    int board[SIZE_BOARD][SIZE_BOARD];
    //inline void check_boundary(const int&, const int&);
    bool legal_pos(int, int, int);
    std::vector<std::string> not_expand_list;
};

#endif
#ifndef MCTS_H
#define MCTS_H

#include <vector>
#include <string>
#include <map>
#include "game.hpp"

class Node{
public:
    //CONSTURCTOR & DISTRUCTOR
    //指定在before状态时，在pos下子后的状态
    Node() = default;
    Node(const Game &before);
    Node(const Game &before, std::string pos, Node *father);

    float UCB(int T, float C);       //T为总的探索次数, C为超参数
    int visit, score;      //访问数
    float expect;   //期望
    std::vector<std::string> not_expand_list;
    Game status;
    Node* father;
    std::vector<Node*> children;
};

class MCTS{
public:
    MCTS(Game status, int max_times, float C=1/sqrt(2));
    Node* select_policy(Node* v0); //以v0为初始节点选择一个节点
    Node* expansion(Node* root);
    Node* get_max_UCB_node(Node *root);
    int simulate_policy(Node *root); //模拟并返回终局得分
    void back_propagate(Node *begin, int score);
    void DFS_DEL(Node*); //深度优先删除

    std::string get_best_choice() const { return best_choice; };

private:
    int max_times; //最大搜索次数
    std::string best_choice;
    int T;  //总的探索次数
    Node* root;
    float C; //超参数
};
#endif

#include "MCTS.hpp"
#include "math.h"
#include <iostream>
#include <time.h>

Node::Node(const Game &before){
    this->status = before;
    visit = 0;
    score = 0;
    this->status.board.update_valid_positoin();
    Reversi *board = &status.board;
    for (std::map<std::string, std::vector<int> >::iterator it = board->valid_position.begin(); it != board->valid_position.end(); it++)
        not_expand_list.push_back(it->first);
}

Node::Node(const Game &before, std::string pos, Node* father){
    this->status = before;
    status.board.switch_player();
    status.board.update_valid_positoin();
    status.board.set(pos);
    this->father = father;
    visit = 0;
    score = 0;
    this->status.board.update_valid_positoin();
    Reversi *board = &status.board;
    for (std::map<std::string, std::vector<int> >::iterator it = board->valid_position.begin(); it != board->valid_position.end(); it++)
        not_expand_list.push_back(it->first);
}

MCTS::MCTS(Game status, int max_times, float C){
    this->C = C;
    T = 0;
    root = new Node(status);
    root->father = NULL;
    this->max_times = max_times;
    Node* max_ucb_node;

    while(max_times--){
        Node *select_node = select_policy(root); //
        assert(select_node != NULL);
        int terminal_score = simulate_policy(select_node);
        back_propagate(select_node, terminal_score);
        max_ucb_node = get_max_UCB_node(root);
    }
    best_choice = max_ucb_node->status.board.action;
}

Node* MCTS::select_policy(Node *root){
    Node *tmp = root;
    while(!tmp->status.is_terminal()){
        if (tmp->children.size() == 0){
            return expansion(tmp);
        } else if (!tmp->not_expand_list.empty()){ //未完全扩展
            return expansion(tmp);
        } else{
            if (tmp->not_expand_list.size() != 0)
                return expansion(tmp);
            else tmp = get_max_UCB_node(tmp);
        }
    }
    return tmp;
}

Node* MCTS::expansion(Node *root){
    if (root->status.board.valid_position.size() == 0) return root->father;
    if (root->not_expand_list.size() == 0) return root->father;

    int n = root->not_expand_list.size(); //未被扩展的节点数量
    srand((unsigned int) time(NULL));
    int k = rand() % n;
    std::string action = root->not_expand_list[k%n];
    root->not_expand_list.erase(root->not_expand_list.begin()+(k%n));
    Node *expand_node = new Node(root->status, action, root);
    root->children.push_back(expand_node);
    T++;

    return expand_node;
}

int MCTS::simulate_policy(Node *root){
    return root->status.simulate();
}

void MCTS::back_propagate(Node *root, int score){
    while(root != NULL){
        root->visit++;
        if (root->status.board.turn == BLACK_TURN) root->score += score; //黑棋加终局分数
        else root->score -= score; //白棋分数取反
        root = root->father;
    }
}

float Node::UCB(int T, float c){
    expect = visit ? score / visit : 0;
    float C = T != 0 ? c * sqrt(2 * log(visit)/ T) : 0;
    //float C = 1;
    //如果未被探索过UCB为无穷
    if (visit == 0) return std::numeric_limits<float>::max();
    return expect + C;
}

Node* MCTS::get_max_UCB_node(Node *root){
    Node *node_max;
    int max = -std::numeric_limits<int>::max();
    for (std::vector<Node*>::iterator it = root->children.begin(); it != root->children.end(); it++){
        float ucb = (*it)->UCB(this->T, this->C);
        if (max < ucb){
            node_max = *it;
            max = ucb;
        }
    }
    return node_max;
}

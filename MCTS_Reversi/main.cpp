#include <iostream>
#include <thread>
#include "game.hpp"
#include "MCTS.hpp"
#include "render.hpp"

#define DEBUG 0
#define PVE 1
#define PVP 2
using namespace std;

int main(int argc, char **argv){
    Game game;
    int model = PVE;    //默认人机模式

    for (int i = 1; i < argc; i++){ //处理数据
        if(!strcmp(argv[i], "-iterator")){
            i++; assert(i < argc);
            iterators = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-model")){
            i++; assert(i < argc);
            if (!strcmp(argv[i], "pve")){ //pvp
                model = PVE;
            } else if (!strcmp(argv[i], "debug")){ //debug
                model = DEBUG;
            } else if (!strcmp(argv[i], "pvp")){ //pvp
                model = PVP;
            } else{
                printf("Error model input\n");
                assert(0);
            }
        } else if (!strcmp(argv[i], "-round")){
            i++; assert(i < argc);
            rounds = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-c")){
            i++; assert(i < argc);
            c = atof(argv[i]);
        }
    }

    thread reversi;
    if (model == PVP){
        reversi = thread(&Game::pvp, &game);
    } else if (model == PVE){
        reversi = thread(&Game::pve, &game);
    } else if (model == DEBUG){
        reversi = thread (&Game::pve_debug, &game);
    }

    render(game);

    return 0;
}

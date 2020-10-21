#include <iostream>
#include "sys/ioctl.h"

#include "stdlib.h"

#include "config.h"
#include "internals.h"
#include "display.h"
#include "food.h"


void backgroundSetup( /* your code here */ ){
  /*
    your code here
  */
}

void add_snake( /* your code here */ ){
  /*
    your code here
  */
}

void remove_snake( /* your code here */ ){
  /*
    your code here
  */
}

void snake_movement( /* your code here */ ){
  /*
    your code here
  */
}

bool verifyBorder( /* your code here */ ){
  /*
    your code here
  */
}

void setupSnake( /* your code here */ ){
  /*
    your code here
  */
}

void update_snake_coordinates( /* your code here */ ){
  /*
    your code here
  */
}

void startGame(const int& lap, const int& nx, const int& ny, int& snl, int* snake, int* bg){
    char key;
    int dxdy[2] = {1,0};
    int food[2] = {0,0};

    createFood( bg, food, nx, ny );

    while( true ){
        internal::frameSleep(lap);
        if( internal::keyEvent() ){
            std::cin >> key; 
            snake_movement(key, dxdy);
        }
        backgroundClear();
        add_snake( snake, bg, snl, nx, ny );
        printFrame(nx, ny, bg);
        remove_snake(snake, bg, snl, nx, ny);
        bool out =  verifyBorder(snake, nx, ny);
        if( out == false){
            std::cerr << "" << std::endl;
            exit(1);
        }
        bool eat = eatFood(food, snake);
        if(eat){
            createFood(bg, food, nx, ny);
            snl++;
        }
        update_snake_coordinates(snake, snl, dxdy);

    }
}



int main(){
    const int nx = 50;
    const int ny = 25;
    const int lap=200;

    int snake_len = 3;
    int background[nx*ny] ;
    for( int i=0; i<nx*ny; i++){
        background[i] = -1;
    }
    int snake[2*SNAKE_LEN];

    backgroundSetup(nx, ny, background);
    setupSnake(snake, snake_len);


    startGame(lap, nx, ny, snake_len, snake, background);

}




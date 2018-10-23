#include "Player.h"
#include <iostream>

Player::Player(unsigned int columns, unsigned int rows)
  : gen(rd()) {
    this->columns = columns;
    this->rows = rows;

    this->reset();
}

Player::Player(unsigned int columns, unsigned int rows, unsigned int startX, unsigned int startY)
  : gen(rd()) {
    this->columns = columns;
    this->rows = rows;

    this->x = startX;
    this->y = startY;

}

void Player::reset(){
  std::uniform_int_distribution<> dis1(0, columns-1);
  std::uniform_int_distribution<> dis2(0, rows-1);

  unsigned int tempx = dis1(gen);
  unsigned int tempy = dis2(gen);

  while(true){
    // Don't spawn on top of target
    if(tempx == 9 && tempy == 3){
      tempx = dis1(gen);
      tempy = dis2(gen);

      //console.log('new numbers');
    } else {
      break;
    }
  }

  this->x = tempx;
  this->y = tempy;
}

void Player::reset(unsigned int startX, unsigned int startY){
  this->x = startX;
  this->y = startY;
}

void Player::makemove(unsigned int movenum){
  //std::cout << "Making move: " << movenum << std::endl;
  switch(movenum){
    case 0:
    //std::cout << "Right " << std::endl;
    this->dir(1,0);
    break;
    case 1:
    //std::cout << "Left " << std::endl;
    this->dir(-1,0);
    break;
    case 2:
    //std::cout << "Down " << std::endl;
    this->dir(0,1);
    break;
    case 3:
    //std::cout << "Up " << std::endl;
    this->dir(0,-1);
    break;
  }
}
unsigned int Player::getX(){
  return x;
}

unsigned int Player::getY(){
  return y;
}

void Player::dir(int dx, int dy){
  this->x += dx;
  this->y += dy;

  if(this->x > (int) columns-1){
    this->x = columns-1;
  } else if(this->x < 0){
    this->x = 0;
  }

  if(this->y > (int) rows-1){
    this->y = rows-1;
  } else if(this->y < 0){
    this->y = 0;
  }
}

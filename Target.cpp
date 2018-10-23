#include "Target.h"

Target::Target(unsigned int columns, unsigned int rows)
  : gen(rd()) {
  this->columns = columns;
  this->rows = rows;

  this->reset();
}

void Target::reset(){
  this->x = 9;
  this->y = 3;
}

void Target::move(){
  std::vector<unsigned int> moves;

  if(this->x < columns-1){
    moves.push_back(0);
  }

  if(this->x > 0){
    moves.push_back(1);
  }

  if(this->y < rows-1){
    moves.push_back(2);
  }

  if(this->y > 0){
    moves.push_back(3);
  }

  std::uniform_int_distribution<> dis(0, moves.size()-1);

  unsigned int r = dis(gen);

  unsigned int tmove = moves[r];

  // how moves are numbered
  if(tmove == 0) this->x++;
  else if(tmove==1) this->x--;
  else if(tmove==2) this->y++;
  else if(tmove==3) this->y--;
}

unsigned int Target::getX(){
  return this->x;
}

unsigned int Target::getY(){
  return this->y;
}

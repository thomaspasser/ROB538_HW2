#include <random>

#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
  Player(unsigned int columns, unsigned int rows);
  Player(unsigned int columns, unsigned int rows, unsigned int startX, unsigned int startY);
  void reset();
  void reset(unsigned int startX, unsigned int startY);
  void makemove(unsigned int movenum);
  unsigned int getX();
  unsigned int getY();

private:
  void dir(int dx, int dy);
  unsigned int rows, columns;
  int x, y;

  std::random_device rd;
  std::mt19937 gen;

};

#endif

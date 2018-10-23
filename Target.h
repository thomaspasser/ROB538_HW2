#include <random>

#ifndef TARGET_H
#define TARGET_H

class Target{
public:
  Target(unsigned int columns, unsigned int rows);
  void reset();
  void move();
  unsigned int getX();
  unsigned int getY();

private:
  unsigned int rows, columns;
  unsigned int x, y;

  std::random_device rd;
  std::mt19937 gen;

};

#endif

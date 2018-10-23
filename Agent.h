#include <cmath>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "Player.h"
#include "constants.h"

#ifndef AGENT_H
#define AGENT_H

template <unsigned int N, unsigned int M>
class Agent{
public:
  Agent(int initial_value);
  void attach(Player *p);
  void step(unsigned int tx, unsigned int ty, unsigned int it, bool greedy);
  void evaluate(unsigned int tx, unsigned int ty, double reward);
  void print(std::string filename);

private:
  unsigned int lastPlayerX, lastPlayerY, lastTargetX, lastTargetY, lastAction;
  double tau;//, alpha, gamma;
  double Q[N][M][N][M][4];
  Player *p;

  std::random_device rd;
  std::mt19937 gen;

};

#include "Agent.tcc"

#endif

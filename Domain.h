#ifndef DOMAIN_H
#define DOMAIN_H

#include <iomanip>
#include <algorithm>
#include <iterator>

#include "Agent.h"
#include "Player.h"
#include "Target.h"

#include "constants.h"

class Domain{
public:
  Domain();
  ~Domain();
  void run(const unsigned int totalIt, const unsigned int binSize, std::vector<unsigned int> *steps, std::vector<Agent<10,5>*>& a, bool train, bool globalReward = false, bool showVisualization = false);
  void reset();
  void visualize();
  void saveData(std::string fname_base);

private:
  bool isCollided(Player *thisp);
  void printPos(unsigned int x, unsigned int y);
  void printState();
  void showStats(unsigned int binSize);
  double distToTarget(Player* thisp);
  unsigned int it, thisSteps;
  std::vector<unsigned int> numCaught;
  std::vector<unsigned int> *steps;
  std::vector<std::vector<double> > distFrom;
  Target *t;
  std::vector<Player*> p;
  std::vector<Agent<10,5>*> a;
};

#endif

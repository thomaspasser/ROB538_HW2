#include <iostream>
#include "Domain.h"
#include "Agent.h"
#include "constants.h"

std::vector<unsigned int> stepsArray(totalIterations);

int main(){

  std::cout << "Running Q-learning for problem 1" << std::endl;

  Agent<10,5> a1(initialQ);
  std::vector<Agent<10,5>* > a;
  a.push_back(&a1);

  Domain domain;

  domain.run(totalIterations, totalIterations/100, &stepsArray, a, true);

  // save Q to json
  a1.print("agent1.json");

  // save training stats to csv
  std::ofstream csvfile;
  csvfile.open("training_stats_1.csv");
  std::copy(stepsArray.begin(), stepsArray.end(), std::ostream_iterator<double>(csvfile, "\n"));

  std::vector<unsigned int> stepsArray2(totalIterations*10);
  // Just run without training..
  domain.run(totalIterations*10, totalIterations*10, &stepsArray2, a, false, false, false);

  return 0;
}

#include <iostream>
#include "Domain.h"
#include "Agent.h"
#include "constants.h"

std::vector<unsigned int> stepsArray(totalIterations);

int main(){

  std::cout << "Running Q-learning for problem 3" << std::endl;

  Agent<10,5> a1(initialQ);
  Agent<10,5> a2(initialQ);

  std::vector<Agent<10,5>* > a;
  a.push_back(&a1);
  a.push_back(&a2);

  Domain domain;

  domain.run(totalIterations, totalIterations/100, &stepsArray, a, true, true);

  std::vector<unsigned int> stepsArray2(10*totalIterations);
  domain.run(totalIterations*10, totalIterations*10, &stepsArray2, a, false, true, false);

  domain.saveData("problem3_distdata");

  domain.run(10, 10, &stepsArray2, a, false, true, true);

  /*
  double avgFrom1 = std::accumulate(distFrom1.begin(), distFrom1.end(), 0.0)/distFrom1.size();
  double avgFrom2 = std::accumulate(distFrom2.begin(), distFrom2.end(), 0.0)/distFrom2.size();

  std::vector<double> diff1(distFrom1.size());
  std::transform(distFrom1.begin(), distFrom1.end(), diff1.begin(), [avgFrom1](double x) { return x - avgFrom1; });
  double sq_sum1 = std::inner_product(diff1.begin(), diff1.end(), diff1.begin(), 0.0);
  double distStdev1 = std::sqrt(sq_sum1 / distFrom1.size());

  std::vector<double> diff2(distFrom2.size());
  std::transform(distFrom2.begin(), distFrom2.end(), diff2.begin(), [avgFrom2](double x) { return x - avgFrom2; });
  double sq_sum2 = std::inner_product(diff2.begin(), diff2.end(), diff2.begin(), 0.0);
  double distStdev2 = std::sqrt(sq_sum2 / distFrom2.size());

  std::cout << "Average distances from: " << avgFrom1 << ", " << avgFrom2 << std::endl;
  std::cout << "Stdev: " << distStdev1 << ", " << distStdev2 << std::endl;

  double max1 = *std::max_element(distFrom1.begin(), distFrom1.end());
  double max2 = *std::max_element(distFrom2.begin(), distFrom2.end());

  double min1 = *std::min_element(distFrom1.begin(), distFrom1.end());
  double min2 = *std::min_element(distFrom2.begin(), distFrom2.end());
  std::cout << "Min: " << min1 << ", " << min2 << std::endl;
  std::cout << "Max: " << max1 << ", " << max2 << std::endl;
  */

  // save Q to json
  a1.print("agent3_1.json");
  a2.print("agent3_2.json");

  // save training stats to csv
  std::ofstream csvfile;
  csvfile.open("training_stats_3.csv");
  std::copy(stepsArray.begin(), stepsArray.end(), std::ostream_iterator<double>(csvfile, "\n"));

  return 0;
}

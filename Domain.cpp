#include "Domain.h"

Domain::Domain(){
  t = new Target(10,5);
}

Domain::~Domain(){
  for(unsigned int i=0; i<p.size(); i++){
    delete p[i];
  }
  delete t;
}

void Domain::reset(){
  //std::cout << "Caught in " << thisSteps << std::endl;

  (*steps)[it] = thisSteps;
  thisSteps = 0;
  it++;
  for(unsigned int i=0; i<p.size(); i++){
    if(i==0) p[i]->reset();
    else p[i]->reset(p[0]->getX(), p[0]->getY());
  }
  t->reset();
}

void Domain::run(const unsigned int totalIt, const unsigned int binSize, std::vector<unsigned int> *steps, std::vector<Agent<10,5>*>& a, bool train, bool globalReward, bool showVisualization){
  this->a = a;
  this->steps = steps;

  std::cout << "Running game with " << a.size() << " agent(s) for " << totalIt << " iterations." << std::endl;
  std::cout << "Learning: " << (train ? "true" : "false") << std::endl;
  std::cout << "Global reward: " << (globalReward ? "true" : "false") << std::endl;

  p.clear();
  distFrom.clear();
  numCaught.clear();

  for(unsigned int i=0; i < a.size(); i++){
    Player* thisp;
    if(i==0) thisp = new Player(10,5);
    else thisp = new Player(10,5, p[0]->getX(), p[0]->getY());

    a[i]->attach(thisp);
    p.push_back(thisp);
    numCaught.push_back(0);
    distFrom.push_back(std::vector<double>());
  }

  it = 0;
  thisSteps = 0;

  while(it < totalIt){
    // Calculate terminal output, average & stddev of last {binSize} iterations
    if(thisSteps==0 && it != 0 &&  it % binSize == 0){
      showStats(binSize);
    }

    thisSteps++;

    if(showVisualization) std::cout << "-------------------- Step: " << thisSteps << std::endl;

    std::vector<double> r(p.size());

    for(unsigned int i = 0; i < p.size(); i++){
      if(train) a[i]->step(t->getX(), t->getY(), it, false);
      else a[i]->step(t->getX(), t->getY(), it, true); // greedy action
    }

    std::vector<bool> caught(p.size(), false);
    // Player moves, collision is checked, if not, target moves and collision is checked again
    if(std::any_of(p.begin(), p.end(), [this](Player* thisp){return isCollided(thisp);})){

      std::transform(p.begin(), p.end(), caught.begin(), [this](Player* thisp){return isCollided(thisp);});
    } else {
      t->move();

      if(std::any_of(p.begin(), p.end(), [this](Player* thisp){return isCollided(thisp);})){
        std::transform(p.begin(), p.end(), caught.begin(), [this](Player* thisp){return isCollided(thisp);});
      }
    }

    if(showVisualization) visualize();

    std::transform(caught.begin(), caught.end(), r.begin(), [](bool v){return v ? 20 : -1;});

    if(showVisualization && globalReward){
      double rsum = std::accumulate(r.begin(), r.end(), 0.0);

      std::cout << "Global reward: " << rsum << std::endl;
    }

    if(train){
      if(globalReward){
        double rsum = std::accumulate(r.begin(), r.end(), 0.0);

        for(unsigned int i = 0; i < p.size(); i++){
          a[i]->evaluate(t->getX(), t->getY(), rsum);
        }
      } else {
        for(unsigned int i = 0; i < p.size(); i++){
          a[i]->evaluate(t->getX(), t->getY(), r[i]);
        }
      }
    }

    if(std::any_of(caught.begin(), caught.end(), [this](bool v) {return v;})){
      for(unsigned int i = 0; i < p.size(); i++){
        if(caught[i]) numCaught[i]++;
        else {
          // The target was caught, but not by this agent
          distFrom[i].push_back(distToTarget(p[i]));
        }
      }

      reset();
      if(showVisualization) {
        std::cout << "-------------------- Step: " << thisSteps << std::endl;
        visualize();
      }
    }

  }

  showStats(binSize);

  std::cout << "Run done, caught: ";
  for(unsigned int i = 0; i < p.size(); i++){
    std::cout << 100*numCaught[i] /(double)totalIt << "%,  ";
  }
  std::cout << std::endl;
}

void Domain::saveData(std::string fname_base){
  std::ofstream csvfile;

  for(unsigned int i = 0; i < p.size(); i++){
    csvfile.open(fname_base + std::to_string(i) + ".csv");
    csvfile << numCaught[i] << "\n";
    std::copy(distFrom[i].begin(), distFrom[i].end(), std::ostream_iterator<double>(csvfile, "\n"));
    csvfile.close();
  }
}

void Domain::showStats(unsigned int binSize){
  unsigned int thisdiv = it/binSize;
  std::cout << std::left << std::setw(4) << thisdiv << " == ";

  double avg = std::accumulate(steps->begin()+(thisdiv-1)*binSize, steps->begin()+thisdiv*binSize, 0.0)/(float)binSize;
  std::vector<double> diff(binSize);
  std::transform(steps->begin()+(thisdiv-1)*binSize, steps->begin()+thisdiv*binSize, diff.begin(), [avg](double x) { return x - avg; });
  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / (float)binSize);
  std::cout << "Average: " << std::setw(8) << avg << " | Stddev: " << stdev << std::endl;
}

void Domain::visualize(){

  for(unsigned int j = 0; j<5; j++){
    for(unsigned int i = 0; i<10; i++){

      unsigned int count = 0;
      for(unsigned int k = 0; k < p.size(); k++){
        if(p[k]->getX() == i && p[k]->getY() == j){
          count++;
          if(count > 1) break;
        }
      }

      if(t->getX() == i && t->getY() == j && std::any_of(p.begin(), p.end(), [this](Player* thisp){return isCollided(thisp);})){
        std::cout << " X ";
      } else if(count>1){
        std::cout << " M ";
      } else if(count == 1){
        for(unsigned int k = 0; k < p.size(); k++){
          if(p[k]->getX() == i && p[k]->getY() == j){
            std::cout << " " << k << " ";
          }
        }
      } else if(count == 0){
        if(t->getX() == i && t->getY() == j) std::cout << " T ";
        else std::cout << " - ";
      }
    }
    std::cout << std::endl;
  }

}


void Domain::printPos(unsigned int x, unsigned int y){
  std::cout << "(" << x << ", " << y << ")" << std::endl;
}

void Domain::printState(){
  std::cout << "Target: ";
  printPos(t->getX(), t->getY());

  for(unsigned int i = 0; i < p.size(); i++){
    std::cout << "Player: " << i << " ";
    printPos(p[i]->getX(), p[i]->getY());
  }
}

bool Domain::isCollided(Player *thisp){
  return thisp->getX() == t->getX() && thisp->getY() == t->getY();
}

double Domain::distToTarget(Player* thisp){
  return std::sqrt( std::pow((float)thisp->getX()-t->getX(),2) + std::pow((float)thisp->getY()-t->getY(),2) );
}

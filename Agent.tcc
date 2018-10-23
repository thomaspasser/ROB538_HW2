#include "Agent.h"

template <unsigned int N, unsigned int M>
Agent<N,M>::Agent(int initial_value)
: gen(rd()) {
  //this->alpha = 0.2;
  //this->gamma = 0.5;
  this->tau = 1000; // is changed in step()

  this->lastPlayerX = 0;
  this->lastPlayerY = 0;
  this->lastTargetX = 0;
  this->lastTargetY = 0;
  this->lastAction = 0;

  for(unsigned int i = 0; i<N; i++){
    for(unsigned int j = 0; j<M; j++){
      for(unsigned int l = 0; l<N; l++){
        for(unsigned int m = 0; m<M; m++){
          for(unsigned int k = 0; k<4; k++){
            this->Q[i][j][l][m][k] = initial_value;
          }
        }
      }
    }
  }

}

template <unsigned int N, unsigned int M>
void Agent<N,M>::attach(Player *p){
  this->p = p;
}

template <unsigned int N, unsigned int M>
void Agent<N,M>::step(unsigned int tx, unsigned int ty, unsigned int it, bool greedy){

  unsigned int action = 0;

  if(greedy){
    action = std::distance( this->Q[p->getX()][p->getY()][tx][ty], std::max_element(this->Q[p->getX()][p->getY()][tx][ty], this->Q[p->getX()][p->getY()][tx][ty] + 4) );

  } else {
    // Anneal temperature
    tau = 10.0*std::exp(-(float)it/100000.0);
    if(tau < MIN_TAU) tau = MIN_TAU; // weird stuff happens if it's too small
    //std::cout << "it: " << it <<  ", tau: " << tau << std::endl;

    // Softmax
    std::vector<double> probs;
    for(unsigned int k = 0; k<4; k++){
      probs.push_back(std::exp(this->Q[p->getX()][p->getY()][tx][ty][k]/tau));
    }

    // Draw from discrete distribution with given weights
    std::discrete_distribution<unsigned int> dis(probs.begin(), probs.end());
    action = dis(gen);
  }

  //std::cout << "Doing action: " << action << std::endl;

  // Save state at which the move was made
  lastPlayerX = p->getX();
  lastPlayerY = p->getY();
  lastTargetX = tx;
  lastTargetY = ty;

  p->makemove(action);
  lastAction = action;
}

template <unsigned int N, unsigned int M>
void Agent<N,M>::evaluate(unsigned int tx, unsigned int ty, double reward){
  // Update Q-value
  // Find max value of next action
  double maxQ = *std::max_element(this->Q[p->getX()][p->getY()][tx][ty], this->Q[p->getX()][p->getY()][tx][ty]+4);

  //std::cout << "MaxQ: " << maxQ << std::endl;

  //std::cout << "Reward: " << reward << std::endl;
  this->Q[lastPlayerX][lastPlayerY][lastTargetX][lastTargetY][lastAction] += ALPHA*(reward + GAMMA*maxQ - this->Q[lastPlayerX][lastPlayerY][lastTargetX][lastTargetY][lastAction]);
}


template <unsigned int N, unsigned int M>
void Agent<N,M>::print(std::string filename){
  std::ofstream jsonfile;
  jsonfile.open(filename);

  //Root element brackets
  jsonfile << "[" << std::endl;

  for(unsigned int i = 0; i<N; i++){
    jsonfile << "  [" << std::endl;

    for(unsigned int j = 0; j<M; j++){
      jsonfile << "    [" << std::endl;

      for(unsigned int l = 0; l<N; l++){
        jsonfile << "      [" << std::endl;

        for(unsigned int m = 0; m<M; m++){
          std::vector<double> v(std::begin(this->Q[i][j][l][m]), std::end(this->Q[i][j][l][m]));
          json j(v);
          jsonfile << "        " << j.dump(12);
          if(m != M-1) jsonfile << ",";
          jsonfile << std::endl;
        }
        jsonfile << "      ]";
        if(l != N-1) jsonfile << ",";
        jsonfile << std::endl;
      }
      jsonfile << "    ]";
      if(j != M-1) jsonfile << ",";
      jsonfile << std::endl;
    }
    jsonfile << "  ]";
    if(i != N-1) jsonfile << ",";
    jsonfile << std::endl;
  }

  jsonfile << "]" << std::endl;

  jsonfile.close();
}

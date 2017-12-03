#include <iomanip>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <map>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <sys/time.h>  
#include "TOP.h"
//Initialize S, T

/*
   struct cluster{
   cluster(): count(0) { }
   std::vector<double> center;
   unsigned int count;
   };

   struct vertex{
   std::vector<double> point;
   unsigned int best_cluster;
   double best_distance;
   };
*/

std::vector<vertex> Initialize_S(unsigned int num_points, unsigned int num_dimensions, unsigned int option, std::string filename)
{ 
  std::vector<vertex> S;
  if(option == 0){//randomly generated inputs
    for (unsigned int i=0; i < num_points; i++){
      vertex p;
      for (unsigned int d=0 ; d < num_dimensions; d++)
      { p.point.push_back( rand() % 10 ); }
      //normalize_vector(p.point);
      S.push_back(p);
#ifdef printtrue
      std::cout << "pid[" << i << "]= ("<< p.point << ")" <<std::endl;;
#endif
    }
  }
  //option1: readfromfile each line represents a vector
  else if(option == 1){
    std::ifstream ifs(filename.c_str());
    std::string s;
    for (unsigned int i=0; i < num_points; i++){
      vertex p;
      std::getline( ifs, s );
      std::istringstream iss(s);
      copy( std::istream_iterator<double>( iss ), std::istream_iterator<double>(),std::back_inserter(p.point));
      //normalize_vector(p.point);
      S.push_back(p);
#ifdef printtrue
      std::cout << "pid[" << i << "]= ("<< p.point << ")" <<std::endl;
#endif
    }
  }
  return S;
}


std::vector<cluster> Initialize_T(std::vector<vertex> S, unsigned int num_clusters)
{
  std::vector<cluster> T;
  //initialize Rep points to the first k vertexes.
  cluster p;
  for (unsigned int i = 0; i < num_clusters; i++){
    p.center = S[i].point;
    #ifdef printtrue
      std::cout << "center[" << i << "]= ("<< p.center << ")" <<std::endl;
    #endif
    
    T.push_back(p);
  }
  return T;
}

// T gets updated
bool TOP_update(std::vector<cluster> &T, std::vector<vertex> &S){

  bool changedFlag = false;

  unsigned int dimensions = S[0].point.size(); 
  unsigned int num_S = S.size();
  unsigned int num_T = T.size();

  std::vector<cluster> T_updated;
  T_updated.resize(num_T);

  std::vector<double> emptycenter;
  emptycenter.resize(dimensions);
  for(unsigned int i = 0; i < dimensions; i++){
    emptycenter[i] = 0;
  }

  for (unsigned int i = 0; i < num_T; ++i) {
    T_updated[i].center = emptycenter;
    T_updated[i].count = 0;
  }

  for (unsigned int i = 0; i < num_S; ++i) {
    plus_equal_vector(T_updated[S[i].best_cluster].center,S[i].point);
    T_updated[S[i].best_cluster].count  += 1; 
  }

  for (unsigned int i = 0; i < num_T; ++i) {
    // std::cout << "T["<<i<<"].count " <<T[i].count <<std::endl;
    double d = T_updated[i].count;
    if(d == 0){
      T_updated[i].center.clear(); continue; 
    }
    scale_vector(T_updated[i].center, 1.0/d);

    //check changedFlag
    if(T_updated[i].center != T[i].center){
      changedFlag = true;
    } 
    T[i].center = T_updated[i].center;
    T[i].count = T_updated[i].count;
  }

  return changedFlag;
}

int main(int argc, char** argv) {
  if(argc == 1)
  {
    std::cout<<"Usage: [argument]"<<std::endl;
    std::cout<<"Number of points "<<std::endl;
    std::cout<<"Dimenstions of the point "<<std::endl;
    std::cout<<"Randomly generately the graph or read from file? 0 or 1"<<std::endl;
    std::cout<<"filename"<<std::endl;
    std::cout<<"Number of clustes: k "<<std::endl;
    exit(1);
  }

  // distance definition
  TOP_defDistance("Euclidean"); 

  unsigned int num_points =  std::atoi(argv[1]);
  unsigned int num_dimensions =  std::atoi(argv[2]);
  unsigned int option =  std::atoi(argv[3]);
  std::string filename = argv[4];
  unsigned int num_clusters = std::atoi(argv[5]);

  std::vector<vertex> S = Initialize_S(num_points, num_dimensions, option, filename);
  std::vector<cluster> T = Initialize_T(S, num_clusters);

  bool changedFlag = 1;
  while (changedFlag){
    // find the closest target (a point in T) for each point in S
    changedFlag = TOP_findClosestTargets(1, S, T); 
    TOP_update(T, S); // T gets updated
  }
}

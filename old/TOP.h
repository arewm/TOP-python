/*
 * =====================================================================================
 *
 *       Filename:  TOP.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/17/2017 07:43:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yufei Ding, yding8@ncsu.edu
 *   Organization:  North Carolina State University
 *
 * =====================================================================================
 */

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

#ifndef TOP_H_INCLUDED
#define TOP_H_INCLUDED

//#define printtrue
#define infidouble  std::numeric_limits<double>::infinity()
typedef std::vector<double> Point;
typedef std::vector<unsigned int> IDs;

unsigned int MAX_C_ITERATION = 5;

struct cluster {
  cluster(): count(0),count_new_add(0),count_new_sub(0),label(0) { }
  std::vector<double> center;
  unsigned int count;
  unsigned int count_new_add;
  unsigned int count_new_sub;
  double distChange;
  unsigned int label;
};

struct vertex{
  std::vector<double> point;
  unsigned int best_cluster;
  unsigned int best_cluster_old;  //to guild whether we shoudl plus or substract
  bool changed;
  std::vector<double> lowerbounds;
  double upbound;
  bool   outofdata;
};

struct fineCat {
  fineCat(): catType(0),num_landmarks(0) { }
  unsigned int catType;
  unsigned int num_landmarks;
};

//declaraion of high-level TOP function
void TOP_defDistance(std::string defDist){
  std::cout << defDist << std::endl;
};

//declaraion of high-level TOP function
bool TOP_findClosestTargets(int k, std::vector<vertex> S, std::vector<cluster>T){
  std::cout << k << std::endl;
  return false;
}


// helper function to compute distance between points
double distance_Euclidean(const std::vector<double>& a, const std::vector<double>& b) {
  //ASSERT_EQ(a.size(), b.size());
  double total = 0;
  for (unsigned int i = 0;i < a.size(); ++i) {
    double d = a[i] - b[i];
    total += d * d;
  }
  return sqrt(total);
}


// helper function to add two vectors
std::vector<double>& plus_equal_vector(std::vector<double>& a, const std::vector<double>& b) {
  //ASSERT_EQ(a.size(), b.size());
  for (unsigned int i = 0;i < a.size(); ++i) {
    a[i] += b[i];
  }
  return a;
}

// helper function to subs two vectors
void subs_equal_vector(std::vector<double>& a,  const std::vector<double>& b) {
  //ASSERT_EQ(a.size(), b.size());
  for (unsigned int i = 0;i < a.size(); ++i) {
    a[i] -= b[i];
  }
}

// helper function to scale a vector vectors
void scale_vector(std::vector<double>& a, double d) {
  for (unsigned int i = 0;i < a.size(); ++i) {
    a[i] *= d;
  }
}

// Dump a point
std::ostream& operator << (std::ostream& os, Point& p){

  BOOST_FOREACH(Point::value_type d, p){ os << d << " "; }    
  return os;
}

// Dump a Set of Query IDs
std::ostream& operator << (std::ostream& os, IDs & sp){
  BOOST_FOREACH(IDs::value_type pid, sp){ os << pid << " " ;}
  return os;
};



/* still need implementation...
std::vector<int> configure(bool useTset, int cat, int L, int M, int Tsqrt, int Qsqrt){
  std::vector<int> conf;
  conf.resize(6);
  conf[0] = (int)useTset; conf[1] = cat; conf[2] = L; conf[3] = M; conf[4] = Tsqrt; conf[5] = Qsqrt; 
  return conf;
}
*/

//decide the max number of landmarks according to the budget and problem settigns.
int maxLandmarks(int Budget, int cat, int L, int M, int Tsqrt, int Qsqrt){
  //insert a formula here, find it in the paper.
  int MaxL = 100;
  return MaxL;
}

//still need implementation...
//query set Q, target set T, number of dimensions of the data space d, space budget Budget, category of the problme set.
fineCat detailedCategory(int Budget, int cat, std::vector<vertex> points, std::vector<cluster> clusters, bool distributedPlatform){
  int T = clusters.size();
  int Q = points.size();
  int d = points[0].point.size();

  int Qsqrt = sqrt(Q);
  int Tsqrt = sqrt(T);
  int L, M, nMax, spaceNeeds;
  bool useTset;
  if(cat == 1){
    //to use 1-level or 2-level landmarks;
    L = 1;
    if(distributedPlatform == true){
      L = 2;
    }
    //to use 1 or 2 landmarks as intermedidate points;
    M = 1;
    nMax = 1000;//maxLandmarks(Budget, cat, L, M, Tsqrt, Qsqrt);
    if(nMax < Qsqrt) M = 2; 
  }
  if(cat == 2 || cat == 3){
    //to decide whether Tset is to be used;
    useTset =  false;
    if(Tsqrt < 0.1 * Qsqrt){
      useTset = true;
    }
    //to select Tghost/Qghost or Tghost2L/Qghost2L;
    if(cat == 2){
      //estimateSpaceCost(Tghost, Tsqrt, Qsqrt, useTset);
      spaceNeeds = 1000;//estimateSpaceCost(Tsqrt, Tsqrt, Qsqrt, useTset);
    }
    else{
      //estimateSpaceCost(Qghost, Tsqrt, Qsqrt, useTset);
      spaceNeeds = 1000;//estimateSpaceCost(Qsqrt, Tsqrt, Qsqrt, useTset);
    }
    L = 1;
    if(spaceNeeds > Budget || d < 1000){
      L = 2;
    }
  }
  if(cat == 4 || cat == 5){
    //to select TQghost2M or TQghost2L2M
    spaceNeeds = 1000; //estimateSpaceCost(TQghost2M, Tsqrt, Qsqrt);
    L = 1;
    if(spaceNeeds > Budget || d < 1000){
      L = 2;
    }
  }
  
  // decide the fine landmark type and the setting the # of landmarks based on space budget
  // to be implementated...
  fineCat selectCat_Land;
  selectCat_Land.catType = 1;
  selectCat_Land.num_landmarks = T / 10;
  return selectCat_Land;
}

bool  getassignment(std::vector<vertex> &points, std::vector<cluster> &clusters)
{
  bool somechanged = false;
  double di, dbest;
  unsigned int cbest;
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();
  for (unsigned int i = 0; i < num_points;  ++i) {
    dbest = infidouble ;
    cbest = int(-1);
    for (unsigned int j = 0; j < num_clusters; ++j) {
      if(clusters[j].center.size()==0) continue;
      di = distance_Euclidean(points[i].point, clusters[j].center);
      if(dbest > di){
        dbest = di;
        cbest = j;
      }
    }
    points[i].upbound= cbest;
    if(cbest != points[i].best_cluster){
      points[i].best_cluster = cbest;
      somechanged = true;
    }
  }
  return somechanged;
}


/*
 * For the first iteration
 */
bool  getassignment_opt_first(std::vector<vertex> &points, std::vector<cluster> &clusters, unsigned int num_cclusters )
{
  bool somechanged = false;
  unsigned int distancecount = 0;
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();
  //std::cout << "num_points " << num_points << " num_clusters " << num_clusters <<std::endl;

  for (unsigned int curr = 0; curr < num_points;  ++curr) {

    unsigned int prev_asg = points[curr].best_cluster;
    points[curr].best_cluster_old = points[curr].best_cluster;

    double best_distance = infidouble;
    unsigned int best_cluster =  (int)(-1);

    points[curr].lowerbounds.resize(num_cclusters);
    for (unsigned int i = 0;i < num_cclusters; ++i) {
      points[curr].lowerbounds[i]=infidouble;
    }

    for (unsigned int i = 0;i < num_clusters; ++i) {
      if (clusters[i].center.size() > 0) {
        double d = 0.0;
        d = distance_Euclidean(points[curr].point, clusters[i].center);
        distancecount++;
        //std::cout << "From " << curr <<" to "<< i << "cluster's Distance " << d <<std::endl;
        if (d < best_distance) {
          if(best_cluster!=(int)(-1) )
            points[curr].lowerbounds[clusters[best_cluster].label]=best_distance;
          best_distance = d;
          best_cluster = i;
        }
        else{
          if(points[curr].lowerbounds[clusters[i].label] > d)
            points[curr].lowerbounds[clusters[i].label]= d;
        }
      }
    }
    points[curr].best_cluster = best_cluster;
    points[curr].upbound = best_distance;
    points[curr].outofdata = false;
    points[curr].changed = (prev_asg != points[curr].best_cluster);
    //std::cout << "points[" << curr << "].best_cluster = " << best_cluster << " changed " << points[curr].changed << std::endl;
    if(points[curr].changed){
      somechanged = true;
    }
  }
#ifdef printtrue
  std::cout << "distancecount " << distancecount << std::endl;
#endif
  return somechanged;
}

/*
   for later iterations
   */
bool  getassignment_opt_later(std::vector<vertex> &points, std::vector<cluster> &clusters,\
    unsigned int num_cclusters, std::vector<double> C_CLUSTERS_changemax)
{
  bool somechanged = false;
  unsigned int distancecount = 0;
  //unsigned int total = 0;
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();

  for( unsigned int curr = 0; curr < num_points; curr++){
    unsigned int prev_asg = points[curr].best_cluster;
    points[curr].best_cluster_old = points[curr].best_cluster;

    //step1: from the paper: update all lowerbound and upbound distance
    std::vector<double> templowerbounds;
    templowerbounds.resize(num_cclusters);
    double globallowerbound = infidouble;

    //later in the last step, we could remove templowerbound, and just use points[curr].lowerbounds[i] + C_CLUSTERS_changemax[i] to represent it
    for (unsigned int i = 0;i < num_cclusters; ++i) {
      templowerbounds[i] = points[curr].lowerbounds[i];
      points[curr].lowerbounds[i] = points[curr].lowerbounds[i] - C_CLUSTERS_changemax[i];
      if(globallowerbound > points[curr].lowerbounds[i]){
        globallowerbound = points[curr].lowerbounds[i];
      }
    }
    if(clusters[points[curr].best_cluster].distChange > 0){
      points[curr].upbound += clusters[points[curr].best_cluster].distChange;
      points[curr].outofdata = true;
    }

    //for counting global scope efficiency
    //points[curr].globalscope = 0;
    //points[curr].bothscope = 0;
    if(points[curr].upbound > globallowerbound){
      //this is the global scope filtering.

      //update upbound if necessary
      if(points[curr].outofdata) {
        double d = 0.0;
        d = distance_Euclidean(points[curr].point, clusters[points[curr].best_cluster].center);
        distancecount++;
        //@@@@@@@@@@@@@@@@@@@@@
        // points[curr].bothscope +=1;
        //@@@@@@@@@@@@@@@@@@@@@
        points[curr].upbound = d;
        points[curr].outofdata = false;
        if(points[curr].upbound <= globallowerbound) continue;
      }

      //first run, check whether each of the clusters of clusters need to be updated or not, as long as this second level global scope filtering can work, we will not do anything to that set, otherwise, we will try to find the best(you can regard it as the second best if we also considering the global best distance, and use it as the lowerbound of the whole set.)
      std::vector<bool> updatewholeornot;
      updatewholeornot.resize(num_cclusters);
      for (unsigned int i = 0;i < num_cclusters; ++i) {
        updatewholeornot[i] = false;
        if(points[curr].upbound > points[curr].lowerbounds[i]){
          updatewholeornot[i] =  true;
          points[curr].lowerbounds[i] =  infidouble;
          //total++;
        }
      }


      for (unsigned int i = 0;i < num_clusters; ++i) {
        if(i!=prev_asg && updatewholeornot[clusters[i].label]){
          if (clusters[i].center.size() == 0) {
            //std::cout << "cluster lost " << i << std::endl;
            continue;
          }
          else{
            if(points[curr].lowerbounds[clusters[i].label] > templowerbounds[clusters[i].label] - clusters[i].distChange){
              double di;
              di = distance_Euclidean(points[curr].point, clusters[i].center);
              distancecount++;
              //@@@@@@@@@@@@@@@@@@@@@
              //points[curr].bothscope +=1;
              //@@@@@@@@@@@@@@@@@@@@@
              if(di < points[curr].lowerbounds[clusters[i].label]){
                if(di < points[curr].upbound){
                  points[curr].lowerbounds[clusters[points[curr].best_cluster].label] = points[curr].upbound;
                  points[curr].upbound = di;
                  points[curr].outofdata = false;
                  points[curr].best_cluster = i; 
                }
                else{
                  points[curr].lowerbounds[clusters[i].label] = di;

                }
              }
            }
          }
        }
      }
      updatewholeornot.clear();
    }
    /*
       else{
       points[i].globalscope = 1;
       }
       */
    templowerbounds.clear(); 
    points[curr].changed = (prev_asg != points[curr].best_cluster);
    if(points[curr].changed){
      somechanged = true;
    }
  }
  //std::cout << "total cluster group to local filtering " << total << std::endl;
  #ifdef printtrue
    std::cout << "distancecount " << distancecount << std::endl;
  #endif
  
  return somechanged;
}
 
void updatecenter(std::vector<vertex> &points, std::vector<cluster> &clusters)
{
  unsigned int dimensions = points[0].point.size(); 
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();
  std::vector<double> emptycenter;
  emptycenter.resize(dimensions);
  for(unsigned int i = 0; i < dimensions; i++){
    emptycenter[i] = 0;
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    clusters[i].center = emptycenter;
    clusters[i].count = 0;
  }

  for (unsigned int i = 0; i < num_points;  ++i) {
    plus_equal_vector(clusters[points[i].best_cluster].center,points[i].point);
    clusters[points[i].best_cluster].count  +=1; 
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    //std::cout << "clusters["<<i<<"].count " <<clusters[i].count <<std::endl;
    double d = clusters[i].count;
    if(d==0){
      clusters[i].center.clear(); continue;
    }
    scale_vector(clusters[i].center, 1.0 /d);
  }
}

/*  
void updatecenter(std::vector<cluster> &clusters, std::vector<vertex> &points)
{
  unsigned int dimensions = points[0].point.size();
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();

  //step1: get the new clusters center by only handling points that changed their assignment
  std::vector<cluster> clusters_old = clusters;

  std::vector<double> emptycenter;
  emptycenter.resize(dimensions);
  for(unsigned int i = 0; i < dimensions; i++){
    emptycenter[i] = 0;
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    clusters[i].center = emptycenter;
    clusters[i].count = 0;
  }

  for (unsigned int i = 0; i < num_points;  ++i) {
    plus_equal_vector(clusters[points[i].best_cluster].center,points[i].point);
    clusters[points[i].best_cluster].count  +=1;
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    //std::cout << "clusters["<<i<<"].count " <<clusters[i].count <<std::endl;
    double d = clusters[i].count;
    scale_vector(clusters[i].center, 1.0 /d);
  }

  for (unsigned int i = 0;i < num_clusters; ++i) {
    double d = clusters[i].count;
    if (d > 0) {
      double d_update = distance_Euclidean(clusters[i].center,clusters_old[i].center);
      std::cout << "clusters["<<i<<"].distChange = " << d_update <<std::endl;
    }
    if(d == 0 && clusters_old[i].count >0){
      std::cout << "Cluster " << i << " lost" << std::endl;
      std::cout << "Cluster " << i << " lastrun" << clusters_old[i].count << std::endl;
      clusters[i].center.clear();
      clusters[i].count = 0;
      clusters[i].distChange = 0;
    }
  }
}
*/

void updatecenter_first(std::vector<vertex> &points, std::vector<cluster> &clusters, \
    unsigned int num_cclusters, std::vector<double>& C_CLUSTERS_changemax)
{

  unsigned int dimensions = points[0].point.size();
  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();

  //step1: get the new clusters center by only handling points that changed their assignment
  std::vector<cluster> clusters_old = clusters;

  std::vector<double> emptycenter;
  emptycenter.resize(dimensions);
  for(unsigned int i = 0; i < dimensions; i++){
    emptycenter[i] = 0;
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    clusters[i].center = emptycenter;
    clusters[i].count = 0;
  }

  for (unsigned int i = 0; i < num_points;  ++i) {
    plus_equal_vector(clusters[points[i].best_cluster].center,points[i].point);
    clusters[points[i].best_cluster].count  +=1;
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    //std::cout << "clusters["<<i<<"].count " <<clusters[i].count <<std::endl;
    double d = clusters[i].count;
    scale_vector(clusters[i].center, 1.0 /d);
  }

  //step 2, get the distance change(delta on different level):
  //maxdistChange = 0;
  for (unsigned int i = 0;i < num_cclusters; ++i) {
    C_CLUSTERS_changemax[i] = 0;
  }

  for (unsigned int i = 0;i < num_clusters; ++i) {
    double d = clusters[i].count;
    if (d > 0) {
      double d_update = distance_Euclidean(clusters[i].center,clusters_old[i].center);
      //std::cout << "clusters["<<i<<"].distChange = " << d_update <<std::endl;
      if(C_CLUSTERS_changemax[clusters[i].label] < d_update){
        C_CLUSTERS_changemax[clusters[i].label] = d_update;
      }
      clusters[i].distChange = d_update;
    }
    if(d == 0 && clusters_old[i].count >0){
      std::cout << "Cluster " << i << " lost" << std::endl;
      std::cout << "Cluster " << i << " lastrun" << clusters_old[i].count << std::endl;
      clusters[i].center.clear();
      clusters[i].count = 0;
      clusters[i].distChange = 0;
    }
  }
  /*
     for(unsigned int i = 0; i < num_cclusters; i++){
     std::cout << "C_CLUSTERS_changemax["<<i<<"] " <<C_CLUSTERS_changemax[i] <<std::endl;
     }*/

  //added because user-defined TOP_update
  //clusters = clusters_old;
}


void updatecenter_opt(std::vector<vertex> &points, std::vector<cluster> &clusters, \
    unsigned int num_cclusters, std::vector<double>& C_CLUSTERS_changemax)
{

  unsigned int num_points = points.size();
  unsigned int num_clusters = clusters.size();

  //step1: get the new clusters center by only handling points that changed their assignment
  std::vector<cluster> clusters_old = clusters;

  for (unsigned int i = 0; i < num_clusters; ++i) {
    scale_vector(clusters[i].center,clusters[i].count);
    clusters[i].count_new_add = 0;
    clusters[i].count_new_sub = 0;
  }

  for (unsigned int i = 0; i < num_points;  ++i) {
    if(points[i].changed){
      //  std::cout << "changed point " << i << " currbest " <<  points[i].best_cluster \
      //   << " oldbest " <<points[i].best_cluster_old <<std::endl;
      plus_equal_vector(clusters[points[i].best_cluster].center,points[i].point);
      clusters[points[i].best_cluster].count_new_add +=1;
      subs_equal_vector(clusters[points[i].best_cluster_old].center,points[i].point);
      clusters[points[i].best_cluster_old].count_new_sub  +=1;
    }
  }

  for (unsigned int i = 0; i < num_clusters; ++i) {
    double fdfd = clusters[i].count;
    clusters[i].count = clusters[i].count + clusters[i].count_new_add - clusters[i].count_new_sub;
    //std::cout << "OPT clusters["<<i<<"].count " <<clusters[i].count <<std::endl;
    double d = clusters[i].count;
    scale_vector(clusters[i].center, 1.0 /d);
    //  std::cout << "center[" << i << "]= ("<< clusters[i].center << ") = " <<" ("<< clusters_temp[i].center << ") "\
    << clusters[i].count << " " << clusters_temp[i].count \
      << " " << clusters[i].count << " " << clusters[i].count_new_add << " " << clusters[i].count_new_sub \
      <<std::endl;
  }


  //step 2, get the distance change(delta on different level):
  //maxdistChange = 0;
  for (unsigned int i = 0;i < num_cclusters; ++i) {
    C_CLUSTERS_changemax[i] = 0;
  }

  for (unsigned int i = 0;i < num_clusters; ++i) {
    double d = clusters[i].count;
    double d1 = clusters[i].count_new_add + clusters[i].count_new_sub;
    if (d > 0) {
      if(d1 > 0){
        double d_update = distance_Euclidean(clusters[i].center,clusters_old[i].center);
        //        std::cout << "clusters["<<i<<"].distChange = " << d_update <<std::endl;
        if(C_CLUSTERS_changemax[clusters[i].label] < d_update){
          C_CLUSTERS_changemax[clusters[i].label] = d_update;
        }
        clusters[i].distChange = d_update;
        //std::cout << "cluster["<<i<<"].distchange " <<d_update <<std::endl;
      }
      else{
        // cc.new_clusters[i].center = clusters[i].center;
        clusters[i].distChange = 0;
      }
    }
    if(d == 0 && clusters_old[i].count >0){
      std::cout << "Cluster " << i << " lost" << std::endl;
      std::cout << "Cluster " << i << " lastrun" << clusters_old[i].count << std::endl; 
      clusters[i].center.clear();
      clusters[i].count = 0;
      clusters[i].distChange = 0;
    }
    //else if(d1==0){
    //  clusters[i].distChange = 0;
    //}
  }
  /*
     for(unsigned int i = 0; i < num_cclusters; i++){
     std::cout << "C_CLUSTERS_changemax["<<i<<"] " <<C_CLUSTERS_changemax[i] <<std::endl;
     }*/
  
  //added because user-defined TOP_update
  //clusters = clusters_old;
}

void C_clusters(std::vector<cluster> &clusters, unsigned int num_cclusters) //target is to assign label for each cluster, there are kcc number of clusters of clusters.
{ 
  //changeformat to stardard case, where it is a mapping from  vertex to cluster.
  //  unsigned int iteration = 0;
  unsigned int num_clusters = clusters.size();

  std::vector<vertex> cvector;
  cvector.resize(num_clusters);
  for (unsigned int i = 0;i < num_clusters; ++i) {
    cvector[i].point = clusters[i].center;
  }

  std::vector<cluster> cclusters;
  cclusters.resize(num_cclusters);
  //it uses the first ckk centers as the centers for clusters of clusters as an initialization
  for (unsigned int i = 0;i < num_cclusters; ++i) {
    cclusters[i].center = clusters[i].center;
  }

  for(unsigned int i = 0; i < MAX_C_ITERATION; ++i ){
    getassignment(cvector, cclusters);
    //updatecenter(cclusters, cvector);
    updatecenter(cvector, cclusters);
  } 

  for (unsigned int i = 0;i < num_clusters; ++i) {
    std::cout << "cluster["<<i<<"].label" << cvector[i].best_cluster <<std::endl;
    clusters[i].label =  cvector[i].best_cluster;//i % index;//cvector[i].best_cluster;

  } 
}

void TOP_buildLandmark_bigCat_2_distanceType_Euclidean(int topk, int catType, int num_cclusters, std::vector<vertex>& points, std::vector<cluster>& clusters, std::vector<double>& C_CLUSTERS_changemax){
  
  if(catType == 1){
    if(topk == 1){
      int num_clusters = clusters.size();
      // int num_cclusters = C_CLUSTERS_changemax.size();
      //step2: get clusters of clusters done. Give label of each Cluster. assign .label = 0, 1, ..., t-1
      C_clusters(clusters,num_cclusters);
      //step3：
      //C_CLUSTERS_changemax.resize(num_cclusters);
    }
  }
}

bool TOP_findClosestTargets_bigCat_2_distanceType_Euclidean(int topk, int catType, int num_cclusters, int & iteration_count,\
    std::vector<vertex>& points, std::vector<cluster>& clusters, std::vector<double> &C_CLUSTERS_changemax){
  

  int num_clusters = clusters.size();
  int num_points = points.size();
  int dimensions = points[0].point.size();
 
  //fineCat selectCat_Land = detailedCategory(1000, 2, points, clusters, 0);
  //int catType = selectCat_Land.catType;
  //int num_cclusters = selectCat_Land.num_landmarks;
  
  //std::cout << "catType " << catType << " num_cclusters " << num_cclusters  << std::endl;
  // int num_cclusters = num_clusters/10;

  bool somechanged = true;

  if(catType == 1){
    if(topk == 1){
      if(iteration_count == 0){
        somechanged = getassignment_opt_first(points, clusters, num_cclusters);
        updatecenter_first(points, clusters, num_cclusters, C_CLUSTERS_changemax);
        //in the first run, although we can not apply our optimization, we still need to get three kinds of delta of cluster center mover.
        iteration_count++;
      }
      else{
        somechanged = getassignment_opt_later(points, clusters, num_cclusters, C_CLUSTERS_changemax);
        updatecenter_opt(points, clusters, num_cclusters, C_CLUSTERS_changemax);
        iteration_count++;
      }
    }
  }
  return somechanged;
}

/*
void TOP_update_kmeans_centers_bigCat_2_smallCat_1_distanceType_Euclidean_top_1(int iteration_count,\
    std::vector<vertex>& points, std::vector<cluster>& clusters, std::vector<double> &C_CLUSTERS_changemax){
  bool somechanged = true;

  int num_clusters = clusters.size();
  int num_cclusters = num_clusters/10;
  
  if(iteration_count == 0){
    somechanged = getassignment_opt_first(points, clusters, num_cclusters);
    updatecenter_first(points, clusters, num_cclusters, C_CLUSTERS_changemax);
    //in the first run, although we can not apply our optimization, we still need to get three kinds of delta of cluster center mover.
    iteration_count++;
  }
  else{
    somechanged = getassignment_opt_later(points, clusters, num_cclusters, C_CLUSTERS_changemax);
    updatecenter_opt(points, clusters, num_cclusters, C_CLUSTERS_changemax);
    iteration_count++;
  }
}
*/


#endif // TOP_H_INCLUDED

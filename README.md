# TOP-python

## API
Structs:
- cluster
- vertex
- fineCat

High-level TOP functions:
- void TOP_defDistance(String)
- bool TOP_findClosestTargets(int, vector<vertex>, vector<cluster>)

Helper functions:
- double distance_Euclidean(vector<double>, vector<double>)
- vector<double> plus_equal_vector(vector<double>, vector<double>)
- void subs_equal_vector(vector<double>, vector<double>)
- void scale_vector(vector<double>, double)
- fineCat detailedCategory(int, int, vector<vertex>, vector<cluster>, bool)
- bool getAssignment(vector<vertex>, vector<cluster>)
- bool getAssignment_opt_first(vector<vertex>, vector<cluster>, unsigned int)
- bool getAssignment_opt_later(vector<vertex>, vector<cluster>, unsigned int, vector<double>)
- void updateCenter(vector<vertex>, vector<cluster>)
- void updateCenter_first(vector<vertex>, vector<cluster>, unsigned int, vector<double>)
- void updateCenter_opt(vector<vertex>, vector<cluster>, unsigned int, vector<double>)
- void C_clusters(vector<cluster>, unsigned int)
- void TOP_buildLandmark_bigCat_2_distanceType_Euclidean(int, int, int, vector<vertex>, vector<cluster>, vector<double>)
- bool TOP_findClosestTargets_bigCat_2_distanceType_Euclidean(int, int, int, int, vector<vertex>, vector<cluster>, vector<double>)

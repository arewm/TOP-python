# TOP-python

## Team Members
- [Brian Clee](https://github.com/cleebp)
- [Andrew McNamara](https://github.com/arewm)
- [Esha Sherma]

## API
Structs:
- `cluster`
- `vertex`
- `fineCat`

High-level TOP functions:
- `void TOP_defDistance(String)`
- `bool TOP_findClosestTargets(int, vector<vertex>, vector<cluster>)`

Helper functions:
- `double distance_Euclidean(vector<double>, vector<double>)`
- `vector<double> plus_equal_vector(vector<double>, vector<double>)`
- `void subs_equal_vector(vector<double>, vector<double>)`
- `void scale_vector(vector<double>, double)`
- Todo: `fineCat detailedCategory(int, int, vector<vertex>, vector<cluster>, bool)`
- Todo: `bool getAssignment(vector<vertex>, vector<cluster>)`
- Todo: `bool getAssignment_opt_first(vector<vertex>, vector<cluster>, unsigned int)`
- Todo: `bool getAssignment_opt_later(vector<vertex>, vector<cluster>, unsigned int, vector<double>)`
- Todo: `void updateCenter(vector<vertex>, vector<cluster>)`
- Todo: `void updateCenter_first(vector<vertex>, vector<cluster>, unsigned int, vector<double>)`
- Todo: `void updateCenter_opt(vector<vertex>, vector<cluster>, unsigned int, vector<double>)`
- Todo: `void C_clusters(vector<cluster>, unsigned int)`
- Todo: `void TOP_buildLandmark_bigCat_2_distanceType_Euclidean(int, int, int, vector<vertex>, vector<cluster>, vector<double>)`
- Todo: `bool TOP_findClosestTargets_bigCat_2_distanceType_Euclidean(int, int, int, int, vector<vertex>, vector<cluster>, vector<double>)`

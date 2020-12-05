# K-Means-EDA
Implementation of K-means algorithm using C++ and SFML.
# Important Functions
* void pick_random_centroids(vector<DataPoint> &dataset, vector<DataPoint> &centroids, size_t k)
* double get_distance(const DataPoint A, const DataPoint B)
* bool eval_centroids(vector<DataPoint> &centroids)
* void k_means_clustering(vector<DataPoint> &dataset, size_t k)
* void get_data(vector<DataPoint> &dataset, bool random_gen = true)

# Data set:
![Conjunto de datos sin agrupar](/assets/asset_1_no_cluster.png?raw=true "Visualizador sin agrupación de datos")
# Example with k = 5:
![Conjunto de datos agrupados](/assets/asset_2_ok_cluster.png?raw=true "Agrupación por k-means")

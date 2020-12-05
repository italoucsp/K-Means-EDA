#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;
using namespace sf;

bool show_clusters = true;

const int W = 800, H = 800;

const int n_clusters = 13;

Color clusters[n_clusters] = { Color(0,0,0),
                      Color(255,0,0),
                      Color(0,255,0),
                      Color(0,0,255),
                      Color(255, 153, 51),
                      Color(204, 51, 255),
                      Color(0, 204, 102),
                      Color(102, 153, 0),
                      Color(255, 255, 0),
                      Color(153, 51, 0),
                      Color(102, 102, 153),
                      Color(255, 51, 153),
                      Color(0, 255, 255), };

struct DataPoint {
  long double longitude, latitude;
  size_t cluster_id;
  bool clustered = false;
  vector<DataPoint*> cluster_family;
  CircleShape point;
  DataPoint() { latitude = longitude = cluster_id = 0; }
  DataPoint(long double lon, long double lat) : latitude(lat), longitude(lon) {
    point.setRadius(0.5f);
    point.setOrigin(0.5f, 0.5f);
    point.setPosition(longitude, latitude);
    cluster_id = 0;
  }
  bool operator==(const DataPoint &other) const {
    return (longitude == other.longitude && latitude == other.latitude);
  }
  bool operator!=(const DataPoint &other) const {
    return !(*this == other);
  }
  DataPoint operator+(const DataPoint p2) {
    DataPoint p;
    p.longitude = longitude + p2.longitude; p.latitude = latitude + p2.latitude;
    return p;
  }
  DataPoint operator/(const int div) {
    DataPoint p;
    p.longitude = longitude / div; p.latitude = latitude / div;
    return p;
  }
  void show(RenderWindow *win) {
    point.setFillColor((show_clusters) ? clusters[cluster_id] : clusters[0]);
    win->draw(point);
  }
};

int Random_Number(int f_n, int s_n) {
  if (f_n > s_n) {
    int temp = f_n;
    f_n = s_n;
    s_n = temp;
  }
  return (f_n != s_n) ? (rand() % (s_n - f_n + 1)) + f_n : f_n;
}

class PerlinNoise2D {
  vector<int> p;
public:
  PerlinNoise2D();
  PerlinNoise2D(unsigned int seed);
  double noise(double x, double y, double z);
private:
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};

PerlinNoise2D::PerlinNoise2D() {

  p = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
    8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
    35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
    134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
    55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
    18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
    250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
    189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
    43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
    97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
    107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

  p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise2D::PerlinNoise2D(unsigned int seed) {
  p.resize(256);
  iota(p.begin(), p.end(), 0);
  default_random_engine engine(seed);
  shuffle(p.begin(), p.end(), engine);
  p.insert(p.end(), p.begin(), p.end());

}

double PerlinNoise2D::fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
double PerlinNoise2D::lerp(double t, double a, double b) { return a + t * (b - a); }
double PerlinNoise2D::grad(int hash, double x, double y, double z) {
  int h = hash & 15;
  double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoise2D::noise(double x, double y, double z) {
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;
  int Z = (int)floor(z) & 255;

  x -= floor(x);
  y -= floor(y);
  z -= floor(z);

  double u = fade(x);
  double v = fade(y);
  double w = fade(z);

  int A = p[X] + Y;
  int AA = p[A] + Z;
  int AB = p[A + 1] + Z;
  int B = p[X + 1] + Y;
  int BA = p[B] + Z;
  int BB = p[B + 1] + Z;

  double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)), lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
  return (res + 1.0) / 2.0;
}

void pick_random_centroids(vector<DataPoint> &dataset, vector<DataPoint> &centroids, size_t k) {
  for (size_t i = 1; i <= k; ++i) {
    size_t new_centroid = rand() % dataset.size();
    bool already = false;
    for (DataPoint &c : centroids) {
      if (dataset[new_centroid] == c) {
        already = true;
        break;
      }
    }
    dataset[new_centroid].cluster_id = i % n_clusters;
    if (!already) centroids.push_back(dataset[new_centroid]);
  }
}

double get_distance(const DataPoint A, const DataPoint B) {
  return sqrt(pow(A.latitude - B.latitude, 2) + pow(A.longitude - B.longitude, 2));
}

bool eval_centroids(vector<DataPoint> &centroids) {
  bool ok = true;
  for (DataPoint &centroid : centroids) {
    ok = ok && !centroid.clustered;
  }
  return ok;
}

void copy_centroids(vector<DataPoint> &centroids, vector<DataPoint> &vec_copy) {
  vec_copy.resize(centroids.size());
  for (size_t i = 0; i < centroids.size(); ++i) {
    vec_copy[i].clustered = centroids[i].clustered;
    vec_copy[i].longitude = centroids[i].longitude;
    vec_copy[i].latitude = centroids[i].latitude;
    vec_copy[i].cluster_id = centroids[i].cluster_id;
    vec_copy[i].point = centroids[i].point;
  }
}

void k_means_clustering(vector<DataPoint> &dataset, size_t k) {
  vector<DataPoint> centroids;
  pick_random_centroids(dataset, centroids, k);
  while (eval_centroids(centroids)) {
    vector<DataPoint> clusters_media;
    copy_centroids(centroids, clusters_media);
    vector<int> single_cluster_counter(k, 0);
    for (DataPoint &data : dataset) {
      if (data.clustered == false) {
        double min_dist = DBL_MAX;
        size_t cluster_id = 0;
        for (DataPoint &centroid : centroids) {
          if (centroid.clustered == false) {
            double current_min = min_dist;
            min_dist = min(min_dist, get_distance(data, centroid));
            if (current_min != min_dist) {
              cluster_id = centroid.cluster_id;
            }
          }
        }
        if (cluster_id != 0) {
          data.cluster_id = cluster_id;
          ++single_cluster_counter[cluster_id - 1];
          clusters_media[cluster_id - 1] = clusters_media[cluster_id - 1] + data;
          centroids[cluster_id - 1].cluster_family.push_back(&data);
        }
      }
    }
    vector<DataPoint> temp_centroids;
    copy_centroids(centroids, temp_centroids);
    for (size_t i = 0; i < k; ++i) {
      if (centroids[i].clustered == false) {
        centroids[i] = clusters_media[i] / single_cluster_counter[i];
        if (centroids[i] == temp_centroids[i]) {
          for (DataPoint* &cf : centroids[i].cluster_family) {
            cf->clustered = true;
          }
          centroids[i].clustered = true;
        }
        centroids[i].cluster_family.clear();
      }
    }
  }
}

void get_data(vector<DataPoint> &dataset, bool random_gen = true) {
  if (random_gen) {
    srand(time(NULL));
    unsigned int X; cout << "Limit X: "; cin >> X;
    unsigned int Y; cout << "Limit Y: "; cin >> Y;
    PerlinNoise2D noise_generator(Random_Number(-75812165, 7515128));
    PerlinNoise2D z_noise(Random_Number(-75812165, 7515128));
    for (int i = 0; i < X; ++i) {
      for (int j = 0; j < Y; ++j) {
        double x = (double)j / ((double)X);
        double y = (double)i / ((double)Y);
        double n = noise_generator.noise(10 * x, 10 * y, 0.8);
        n = n - floor(n);
        double rnoise = floor(255 * n);
        if (Random_Number(120,255) < rnoise) {
          dataset.push_back(DataPoint(i, j));
        }
      }
    }
    cout << "Dataset created.\n";
  }
  else {
    string path; cout << "Name of the file: "; cin >> path;
    cout << "Dataset loaded.\n";
  }
}
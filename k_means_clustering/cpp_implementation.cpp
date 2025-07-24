#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
typedef std::vector<std::vector<double>> vector_set;

#define DEBUG_PRINT_VECS(x) \
    for (const auto& v: x) { \
        std::cout << "("; \
        for (const auto& i : v) { \
            std::cout << i << " "; \
        } \
        std::cout << "\b)" << std::endl; \
    } \

#define DEBUG_PRINT_SINGLE_VEC(x) \
    std::cout << "("; \
    for (const auto& i : x) { \
        std::cout << i << " "; \
    } \
    std::cout << "\b)" << std::endl; \

class KMeansEngine {
private:
    int k, max_iterations, dim;
    bool termination_flag;
    std::unique_ptr<vector_set> data;
    std::unique_ptr<vector_set> centroids = std::make_unique<vector_set>();
    std::unique_ptr<std::vector<double>> data_centroid_map = std::make_unique<std::vector<double>>(data->size(), 0.0);

    // Helper functions for initialising centroids

    const inline std::vector<double> stepSize(const std::vector<double>& x, const std::vector<double>& y, int partitions) {
        std::vector<double> result(dim);
        for (int i = 0; i < dim; i++) {
            result[i] = (y[i] - x[i])/(double)partitions;
        }
        return result;
    }

    const inline std::vector<double> axplusby(const std::vector<double>& x, const std::vector<double>& y, double a, double b) {
        std::vector<double> result(dim);
        for (int i=0; i<dim; i++) {
            result[i] = (a*x[i])+(b*y[i]);
        }
        return result;
    }

    vector_set getMinMax() {
        vector_set min_max = {data->at(0), data->at(0)};
        for (auto vec = data->begin(); vec!=data->end(); vec++) {
            for (int i=0; i<dim; i++) {
                min_max[0][i] = (*vec)[i]<min_max[0][i]?(*vec)[i]:min_max[0][i];
                min_max[1][i] = (*vec)[i]>min_max[1][i]?(*vec)[i]:min_max[1][i];
            }
        }
        return min_max;
    }

    void initCentroids(const vector_set& min_max) {
        int num_partitions = k - 1;
        std::vector<double> step_size = stepSize(min_max[0], min_max[1], num_partitions);
        for (int i=0; i<k; i++) {
            centroids->push_back(axplusby(min_max[0], step_size, 1, i));
        }
    }

    // Helper functions for clustering algorithm

    double L2(const std::vector<double>& x, const std::vector<double>& y) {
        double running_sum;
        for (int i=0; i<dim; i++) {
            running_sum += pow(x[i]-y[i], 2);
        }
        return running_sum;
    }

    int findNearestCentroid(const std::vector<double>& datapoint) {
        double L2_lower_bound = L2(datapoint, centroids->at(0));
        int nearest_centroid = 0;
        double current_L2;
        for (int i=0; i<k; i++) {
            current_L2 = L2(datapoint, centroids->at(i));
            if (current_L2 < L2_lower_bound) {
                L2_lower_bound = current_L2;
                nearest_centroid = i;
            }
        }
        return nearest_centroid;
    }

    void updateCentroidMap() {
        int i=0;
        std::vector<double> current_map(data->size(), 0.0);
        for (auto vec=data->begin(); vec!=data->end(); vec++) {
            current_map[i] = findNearestCentroid(*vec);
            i++;
        }
        if (current_map != *data_centroid_map) {
            *data_centroid_map = current_map;
        } else {
            termination_flag = true;
        }
    }

    std::vector<double> averageVector(vector_set& presumed_cluster) {
        double divisor = presumed_cluster.size();
        std::vector<double> average_container(dim, 0.0);
        for (auto vec=presumed_cluster.begin(); vec!=presumed_cluster.end(); vec++) {
            average_container = axplusby(average_container, *vec, (double)(1/divisor), (double)(1/divisor));
        }
        return average_container;
    }

    void updateGivenCentroid(int centroid_num) {
        // Assumes data_centroid_reference has been updated
        vector_set cluster;
        for (int i=0; i<data->size(); i++) {
            if (data_centroid_map->at(i) == centroid_num) {
                cluster.push_back(data->at(i));
            }
        }
        (*centroids)[centroid_num] = averageVector(cluster);
    }

    void updateCentroids() {
        for (int centroid=0; centroid<k; centroid++) {
            updateGivenCentroid(centroid);
        }
    }

public:
    KMeansEngine(vector_set dataset, int k, int max_iterations): k(k), max_iterations(max_iterations), data(std::make_unique<vector_set>(dataset)) {
        dim = dataset[0].size();
        termination_flag = false;
        vector_set data_bounds = getMinMax();
        initCentroids(data_bounds);
    }

    void runClustering() {
        for (int i=0; i<max_iterations; i++) {
            updateCentroidMap();
            if (termination_flag == true) { break; }
            else { updateCentroids(); }
        }
    }

    std::vector<double> getClusterMap() { return (*data_centroid_map); }
    vector_set getCentroids() { return (*centroids); }
};

int main() {
    std::cout << "K-Means Demonstration" << std::endl;
    vector_set test_data = {
        {1.0, 2.0, 3.0},
        {-1.0, 3.0, 7.0},
        {4.0, -10.0, 1.0}
    };
    KMeansEngine kmeans(test_data, 2, 100);
    kmeans.runClustering();
    std::vector<double> final_clusters = kmeans.getClusterMap();
    return 0;
}
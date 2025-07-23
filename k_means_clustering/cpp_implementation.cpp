#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

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
    int k;
    int max_iterations;
    bool terminated;
    std::unique_ptr<std::vector<std::vector<double>>> data;
    std::unique_ptr<std::vector<std::vector<double>>> centroids;
    std::unique_ptr<std::vector<int>> centroid_reference;
    
    // These functions help in initialising centroids

    inline std::vector<double> minMaxDimension(int i) {
        std::vector<double> minMax = {data->at(0).at(i), data->at(0).at(i)};
        for (int j=0; j<data->size(); j++) {
            minMax[0] = (data->at(j).at(i) < minMax[0])?data->at(j).at(i):minMax[0];
            minMax[1] = (data->at(j).at(i) > minMax[0])?data->at(j).at(i):minMax[1];
        }
        return minMax;
    }

    inline std::vector<double> stepSize(const std::vector<double>& max_vector, const std::vector<double>& min_vector, int partitions) {
        std::vector<double> stepVec;
        for (int i = 0; i < max_vector.size(); i++) {
            stepVec.push_back((max_vector[i] - min_vector[i]) / (double)partitions);
        }
        return stepVec;
    }

    std::vector<double> xplusay(const std::vector<double>& x, const std::vector<double>& y, const int a) {
        std::vector<double> finalVec;
        for (int i = 0; i < x.size(); i++) {
            finalVec.push_back(x[i]+((double)a*y[i]));
        }
        return finalVec;
    }

    void initializeCentroids(int num_centroids) {
        std::vector<double> minVector, maxVector;
        std::vector<double> current_min_max;
        for (int elem=0; elem<data->at(0).size(); elem++) {
            current_min_max = minMaxDimension(elem);
            minVector.push_back(current_min_max[0]);
            maxVector.push_back(current_min_max[1]);
        }
        std::vector<double> step_size = stepSize(maxVector, minVector, num_centroids-1);
        std::vector<double> current_centroid;
        std::vector<std::vector<double>> centroid_set;
        for (int centroid_count=0; centroid_count<num_centroids; centroid_count++) {
            current_centroid = xplusay(minVector, step_size, centroid_count);
            centroid_set.push_back(current_centroid);
        }
        centroids = std::move(std::make_unique<std::vector<std::vector<double>>>(centroid_set));
    }

    // Helper functions for clustering algorithm

    inline double L2(std::vector<double>& x, std::vector<double>& y) {
        double running_sum = 0;
        for (int i=0; i<x.size(); i++) {
            running_sum += pow(x[i]-y[i], 2);
        }
        return running_sum;
    }

    int nearestCentroid(std::vector<double>& data_point) {
        int nearest = 0;
        double current_max = 0;
        double running_distance;
        for (int c=0; c<centroids->size(); c++) {
            running_distance = L2(centroids->at(c), data_point);
            if (running_distance > current_max) {
                nearest = c;
                current_max = running_distance;
            }
        }
        return nearest;
    }

    void updateReferenceVector() {
        std::vector<int> reference_vector;
        for (int i=0; i<data->size(); i++) {
            reference_vector.push_back(nearestCentroid(data->at(i)));
        }
        if (reference_vector == *centroid_reference) {
            terminated = true;
        } else {
            centroid_reference = std::move(std::make_unique<std::vector<int>>(reference_vector));
        }
    }

    void updateCentroids() {
        std::vector<double> vector_sum(data->at(0).size(), 0.0);
        std::vector<std::vector<double>> new_centroids;
        int vector_count = 0;
        for (int c=0; c<k; c++) {
            for (int centroid: *centroid_reference) {
            if (centroid == c) {
                vector_sum = xplusay(vector_sum, data->at(c), 1);
                vector_count++;
            }
            }
            for (int i=0; i<vector_sum.size(); i++) { vector_sum[i] /= (double)vector_count; }
            new_centroids.push_back(vector_sum);
            std::fill(vector_sum.begin(), vector_sum.end(), 0.0);
            vector_count = 0;
        }
        centroids = std::move(std::make_unique<std::vector<std::vector<double>>>(vector_sum));
    }

public:
    KMeansEngine(int k, int max_iterations, std::vector<std::vector<double>>& sample_data) : k(k), max_iterations(max_iterations) {
        data = std::make_unique<std::vector<std::vector<double>>>(sample_data);
        centroid_reference = nullptr;
        terminated = false;
        initializeCentroids(k);
    }

    std::vector<std::vector<double>> getCentroids() {
        return *centroids;
    }

    std::vector<int> getClassification() {
        return *centroid_reference;
    }

    void runClustering() {
        for (int iteration=0; iteration<max_iterations; iteration++) {
            updateReferenceVector();
            if (terminated == true) { break; }
            updateCentroids();
        }
    }
};

int main() {
    std::cout << "K-Means Demonstration" << std::endl;
    std::vector<std::vector<double>> test_data = {
        {1.0, 2.0, 3.0},
        {-1.0, 3.0, 7.0},
        {4.0, -10.0, 1.0}
    };
    KMeansEngine kmeans(3, 100, test_data);
    kmeans.runClustering();
    std::vector<int> result = kmeans.getClassification();
    DEBUG_PRINT_SINGLE_VEC(result);
    return 0;
}
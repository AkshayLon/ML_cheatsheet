#include <iostream>
#include <vector>
#include <memory>

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
    std::unique_ptr<std::vector<std::vector<double>>> data;
    std::unique_ptr<std::vector<std::vector<double>>> centroids; 

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
        for (int centroid_count=0; centroid_count<num_centroids; centroid_count++) {
            DEBUG_PRINT_SINGLE_VEC(xplusay(minVector, step_size, centroid_count));
            // centroids->push_back(xplusay(minVector, step_size, centroid_count));
        }
    }

public:
    KMeansEngine(int k, int max_iterations, std::vector<std::vector<double>>& sample_data) : k(k), max_iterations(max_iterations) {
        data = std::make_unique<std::vector<std::vector<double>>>(sample_data);
        initializeCentroids(k);
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
    return 0;
}
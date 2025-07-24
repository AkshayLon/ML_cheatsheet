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
    std::unique_ptr<vector_set> data;
    std::unique_ptr<vector_set> centroids = std::make_unique<vector_set>();

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

public:
    KMeansEngine(vector_set dataset, int k, int max_iterations): k(k), max_iterations(max_iterations), data(std::make_unique<vector_set>(dataset)) {
        dim = dataset.size();
        vector_set data_bounds = getMinMax();
        initCentroids(data_bounds);
    }
};

int main() {
    std::cout << "K-Means Demonstration" << std::endl;
    vector_set test_data = {
        {1.0, 2.0, 3.0},
        {-1.0, 3.0, 7.0},
        {4.0, -10.0, 1.0}
    };
    KMeansEngine kmeans(test_data, 2, 100);
    return 0;
}
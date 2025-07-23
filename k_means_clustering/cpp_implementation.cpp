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
    int k;
    int max_iterations;
    std::unique_ptr<vector_set> data;
    std::unique_ptr<vector_set> centroids;

public:
    KMeansEngine(vector_set dataset, int k, int max_iterations): k(k), max_iterations(max_iterations), data(std::make_unique<vector_set>(dataset)) {
        centroids = std::make_unique<vector_set>(nullptr);
    }
};

int main() {
    std::cout << "K-Means Demonstration" << std::endl;
    vector_set test_data = {
        {1.0, 2.0, 3.0},
        {-1.0, 3.0, 7.0},
        {4.0, -10.0, 1.0}
    };
    return 0;
}
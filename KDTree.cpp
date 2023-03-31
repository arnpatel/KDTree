#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// Define a struct to represent a point in k-dimensional space
struct Point {
    std::vector<double> coords; // Coordinates of the point
    int id; // ID of the point
};

// Define a struct to represent a node in the KDTree
struct Node {
    Point point; // Point associated with the node
    Node* left; // Pointer to the left child
    Node* right; // Pointer to the right child
};

// Define a function to calculate the distance between two points
double distance(Point p1, Point p2) {
    double dist = 0.0;
    for (int i = 0; i < p1.coords.size(); i++) {
        dist += pow(p1.coords[i] - p2.coords[i], 2);
    }
    return sqrt(dist);
}

// Define a function to compare points based on a given dimension
bool comparePoints(const Point& p1, const Point& p2, int dim) {
    return p1.coords[dim] < p2.coords[dim];
}

// Define a function to build the KDTree recursively
Node* buildKDTree(std::vector<Point>& points, int start, int end, int dim) {
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) / 2;
    std::nth_element(points.begin() + start, points.begin() + mid, points.begin() + end + 1, [&](const Point& p1, const Point& p2) {
        return comparePoints(p1, p2, dim);
    });
    Node* node = new Node;
    node->point = points[mid];
    node->left = buildKDTree(points, start, mid - 1, (dim + 1) % points[0].coords.size());
    node->right = buildKDTree(points, mid + 1, end, (dim + 1) % points[0].coords.size());
    return node;
}

// Define a function to search the KDTree for nearest neighbors
void searchKDTree(Node* node, Point query, std::vector<Point>& neighbors, int k) {
    if (node == nullptr) {
        return;
    }
    double dist = distance(node->point, query);
    if (neighbors.size() < k || dist < distance(neighbors.back(), query)) {
        if (neighbors.size() == k) {
            neighbors.pop_back();
        }
        neighbors.push_back(node->point);
        std::sort(neighbors.begin(), neighbors.end(), [&](const Point& p1, const Point& p2) {
            return distance(p1, query) < distance(p2, query);
        });
    }
    if (neighbors.size() < k || query.coords[node->point.coords.size() % node->point.coords.size()] - node->point.coords[node->point.coords.size() % node->point.coords.size()] <= neighbors.back().coords[node->point.coords.size() % node->point.coords.size()]) {
        searchKDTree(node->left, query, neighbors, k);
    }
    if (neighbors.size() < k || node->point.coords[node->point.coords.size() % node->point.coords.size()] - query.coords[node->point.coords.size() % node->point.coords.size()] <= neighbors.back().coords[node->point.coords.size() % node->point.coords.size()]) {
        searchKDTree(node->right, query, neighbors, k);
    }
}

int main() {
    // Create a vector of points
    std::vector<Point> points = {
        {{1.0, 2.0, 3.0}, 0},
        {{4.0, 5.0, 6.0}, 1},
        {{7.0, 8.0, 9.0}, 2},
        {{2.0, 3.0, 1.0}, 3},
        {{5.0, 6.0, 4.0}, 4},
        {{8.0, 9.0, 7.0}, 5}
    };
    
    // Build the KDTree from the vector of points
    Node* root = buildKDTree(points, 0, points.size() - 1, 0);

    // Search for the k nearest neighbors of a query point
    Point query = {{3.0, 4.0, 5.0}, -1};
    int k = 2;
    std::vector<Point> neighbors;
    searchKDTree(root, query, neighbors, k);

    // Print the IDs of the k nearest neighbors
    std::cout << "The IDs of the " << k << " nearest neighbors of the query point are: ";
    for (auto& neighbor : neighbors) {
        std::cout << neighbor.id << " ";
    }
    std::cout << std::endl;

    // Free the memory allocated for the KDTree
    delete root;

    return 0;
}
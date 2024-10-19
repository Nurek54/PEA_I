#include "BruteForceTSP.h"
#include "SaveToCSV.h"
#include "TSPUtilities.h"
#include <chrono>
#include <iostream>
#include <climits>

// Funkcja do generowania permutacji
bool next_permutation(std::vector<int>& cities) {
    int n = cities.size();
    int i = n - 2;

    // Znajdź pierwszy element, który jest mniejszy od swojego sąsiada
    while (i >= 0 && cities[i] >= cities[i + 1]) {
        i--;
    }

    if (i < 0) {
        return false;  // Osiągnięto ostatnią permutację
    }

    // Znajdź element większy od cities[i] po prawej stronie
    int j = n - 1;
    while (cities[j] <= cities[i]) {
        j--;
    }

    // Zamień cities[i] z cities[j]
    std::swap(cities[i], cities[j]);

    // Odwróć kolejność elementów po pozycji i
    int k = i + 1;
    int l = n - 1;
    while (k < l) {
        std::swap(cities[k], cities[l]);
        k++;
        l--;
    }

    return true;
}

// Funkcja brute force do znajdowania najkrótszej trasy
std::pair<std::vector<int>, int> tsp_bruteforce(const TSPInstance& instance) {
    std::vector<std::vector<int>> distances = instance.getDistances();
    int num_cities = instance.getCityCount();

    std::vector<int> cities(num_cities);
    for (int i = 0; i < num_cities; ++i) {
        cities[i] = i;
    }

    int min_cost = INT_MAX;
    std::vector<int> best_path;

    auto start = std::chrono::high_resolution_clock::now();

    do {
        int current_cost = Utilities::calculate_cost(cities, distances);
        if (current_cost < min_cost) {
            min_cost = current_cost;
            best_path = cities;
        }
    } while (next_permutation(cities));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> seconds = end - start;

    SaveToCSV save("BruteForceResults.csv");
    save.saveResults("BruteForce", seconds, {}, {}, best_path, min_cost);

    return {best_path, min_cost};
}

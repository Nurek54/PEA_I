#include "NearestNeighborTSP.h"
#include <limits>
#include <chrono>  // Do mierzenia czasu
#include <fstream>  // Do zapisu pliku CSV
#include <iostream> // Do wyświetlania wyników
#include <iomanip>  // Do precyzyjnego formatowania liczb

int calculate_cost(const std::vector<int>& path, const std::vector<std::vector<int>>& distances);

std::pair<std::vector<int>, int> tsp_nearest_neighbor(const TSPInstance& instance) {
    std::vector<std::vector<int>> distances = instance.getDistances();
    int num_cities = instance.getCityCount();

    std::vector<int> tour;
    std::vector<bool> visited(num_cities, false);

    int currentCity = 0;
    tour.push_back(currentCity);
    visited[currentCity] = true;

    auto start = std::chrono::high_resolution_clock::now();  // Start pomiaru czasu

    for (int i = 1; i < num_cities; ++i) {
        int nearestCity = -1;
        int minCost = std::numeric_limits<int>::max();

        for (int j = 0; j < num_cities; ++j) {
            if (!visited[j] && distances[currentCity][j] < minCost) {
                nearestCity = j;
                minCost = distances[currentCity][j];
            }
        }

        currentCity = nearestCity;
        tour.push_back(currentCity);
        visited[currentCity] = true;
    }

    auto end = std::chrono::high_resolution_clock::now();  // Koniec pomiaru czasu

    // Obliczamy czasy w różnych jednostkach
    std::chrono::duration<double> seconds = end - start;
    std::chrono::duration<double, std::milli> milliseconds = end - start;
    std::chrono::duration<double, std::nano> nanoseconds = end - start;

    // Obliczamy koszt trasy (poza pomiarem czasu)
    int totalCost = calculate_cost(tour, distances);

    // Wyświetlanie wyników na konsoli
    std::cout << "Algorytm Najbliższych Sąsiadów\n";
    std::cout << "Czas wykonania:\n";
    std::cout << "  W sekundach: " << seconds.count() << " s\n";
    std::cout << "  W milisekundach: " << milliseconds.count() << " ms\n";
    std::cout << "  W nanosekundach: " << nanoseconds.count() << " ns\n";

    std::cout << "Najlepsza trasa: ";
    for (int city : tour) {
        std::cout << city << " ";
    }
    std::cout << "\nKoszt: " << totalCost << "\n";

    // Zapis wyników do pliku CSV
    std::ofstream csvFile("results.csv", std::ios::app);  // Tryb dodawania do pliku
    if (csvFile.is_open()) {
        // Sprawdzenie, czy plik jest nowy, i jeśli tak, dodanie nagłówka
        static bool isFileNew = true;
        if (isFileNew) {
            csvFile << "Algorithm | Seconds | Milliseconds | Nanoseconds | Path | Cost\n";  // Nagłówek CSV
            isFileNew = false;
        }

        csvFile << std::fixed << std::setprecision(6)  // Ustaw precyzję na 6 miejsc po przecinku
                << "NearestNeighbor" << " | "
                << seconds.count() << " | "
                << milliseconds.count() << " | "
                << nanoseconds.count() << " | ";

        // Zapisz trasę oddzielając miasta " | "
        for (size_t i = 0; i < tour.size(); ++i) {
            csvFile << tour[i];
            if (i != tour.size() - 1) {
                csvFile << " | ";  // Oddziel miasta " | "
            }
        }

        csvFile << " | " << totalCost << "\n";  // Zakończ linię pliku z kosztem trasy
        csvFile.close();
        std::cout << "Wyniki zapisano do pliku: results.csv\n";
    } else {
        std::cerr << "Nie można otworzyć pliku: results.csv\n";
    }

    return {tour, totalCost};
}

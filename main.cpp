#include <iostream>
#include <vector>
#include <algorithm>
#include "metro.h"

int main() {
    int station_number, line_number;
    std::cin >> station_number >> line_number;
    std::vector<station> stations(station_number + 1);
    for (int i = 1; i <= station_number; ++i) {
        stations[i] = i;
    }
    std::vector<edge> edges;
    for (int i = 0; i < line_number; ++i) {
        int amount, frequency;
        std::cin >> amount >> frequency;
        for (int j = 0; j < amount; ++j) {
            int from, to, time;
            std::cin >> from >> to >> time;
            edges.emplace_back(edge(from,to,time,frequency));
        }
    }
    Metro metro(stations, edges);
    int requests_number;
    std::cin >> requests_number;
    for (int i = 0; i < requests_number; ++i) {
        int from, to, entry_time;
        std::cin >> from >> to >> entry_time;
        auto res =  metro.build_path(from, to,entry_time);
        for (int j = 0; j < res.size(); ++j) {
            std::cout << res[j] << " ";
        }
    }
}
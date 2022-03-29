#include <string>
#include <set>
#include <algorithm>
#include <utility>

struct station;
struct metro_line;
struct metro_line {
    //station start_station, finish_station;
    std::string m_name;
    int frequency;
};

struct station {
    std::string m_name;
    //metro_line* line_name;
    int id;

    station() = default;

    station(int id) : id(id) {}

    station(std::string name, int id) : m_name(std::move(name)), id(id) {}

    station(const station &other) = default;

    bool operator==(const station &other) const {
        return id == other.id;
    }

    bool operator!=(const station &other) const {
        return !(*this == other);
    }
    operator int() {
        return id;
    }
};

struct edge {
    station m_to, m_from;
    int m_time;
    int frequency;

    edge() = default;

    edge(const station &to, const station &from, int time, int frequency) : m_to(to), m_from(from), m_time(time),
                                                                            frequency(frequency) {}
};

class Metro {
    std::vector<station> m_stations; //id - позиция в этом списке??
    std::vector<std::vector<edge>> m_edges;
    const int MAX_TIME = 10'000;
public:
    Metro(std::vector<station> stations, const std::vector<edge> &edges) : m_stations(std::move(stations)) {
        m_edges.resize(edges.size() + 1);
        for (const auto &item : edges) {
            m_edges[item.m_to.id].emplace_back(edge(item.m_from, item.m_to, item.m_time, item.frequency));
            m_edges[item.m_from.id].emplace_back(edge(item.m_to, item.m_from, item.m_time, item.frequency));
        }
    }

    std::vector<station> build_path(const station &from, const station &to, int entry_time) {
        auto res = dijkstra(from, entry_time);
        res.first[to.id]; // мин длина.
        auto parent = res.second;
        int i = to.id;
        std::vector<station> path;
        path.push_back(to);
        while (parent[i] != from) {
            path.push_back(m_stations[parent[i].id]);
            i = parent[i].id;
        }
        path.push_back(from);
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<station> build_reading_path() {
        //to do
    };
private:
    std::pair<std::vector<long long>, std::vector<station>> dijkstra(const station &start, int entry_time) {
        //учитывать станции с нулевым переходом???
        std::vector<long long> dist(m_edges.size(), MAX_TIME);
        std::vector<bool> used(m_edges.size(), false);
        std::vector<int> time_delay(m_stations.size(), MAX_TIME);
        std::vector<station> parent(m_stations.size(), -1);
        dist[start.id] = 0;
        time_delay[start.id] = entry_time;
        std::set<std::pair<int, int>> queue;
        queue.insert({0, start.id});
        while (!queue.empty()) {
            auto front = *queue.begin();
            queue.erase(*queue.begin());
            for (auto &edge : m_edges[front.second]) {
                //сколько ждать до след. поезда
                int time_delay_t = edge.frequency ?
                                   (edge.frequency - (time_delay[edge.m_from.id] % edge.frequency)) % edge.frequency
                                                  : 0;
                if (dist[edge.m_to.id] > dist[edge.m_from.id] + edge.m_time + time_delay_t) {
                    if (queue.count({dist[edge.m_to.id], edge.m_to.id})) {
                        queue.erase({dist[edge.m_to.id], edge.m_to.id});
                    }
                    dist[edge.m_to.id] = dist[edge.m_from.id] + edge.m_time + time_delay_t;
                    time_delay[edge.m_to.id] = time_delay[edge.m_from.id] + time_delay_t + edge.m_time;
                    parent[edge.m_to.id] = edge.m_from;
                    if (!used[edge.m_to.id]) queue.insert({dist[edge.m_to.id], edge.m_to.id});
                }
            }
        }
        return std::make_pair(dist, parent);
    }

};
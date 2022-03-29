#include <string>
#include <set>
#include <algorithm>
#include <utility>


struct station {

    int id;

    station() = default;

    station(int id) : id(id) {}

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
    int m_frequency;
    int sum_line1, sum_line2;

    edge() = default;

    edge(const station &from, const station &to, int time, int frequency) : m_from(from), m_to(to), m_time(time),
                                                                            m_frequency(frequency) {}

    edge(const station &from, const station &to, int time, int frequency, int line) : m_from(from), m_to(to),
                                                                                      m_time(time),
                                                                                      m_frequency(frequency),
                                                                                      sum_line1(line) {}
};

class Metro {
    std::vector<station> m_stations;
    std::vector<std::vector<edge>> m_edges;
    const int MAX_TIME = 10'000;
public:
    using path = std::pair<long long, std::vector<station>>;

    Metro(std::vector<station> stations, const std::vector<edge> &edges) : m_stations(std::move(stations)) {
        m_edges.resize(m_stations.size() + 1);
        for (const auto &item : edges) {
            m_edges[item.m_from.id].emplace_back(
                    edge(item.m_from, item.m_to, item.m_time, item.m_frequency, item.sum_line1));
            m_edges[item.m_to.id].emplace_back(
                    edge(item.m_to, item.m_from, item.m_time, item.m_frequency, item.sum_line2));
        }
    }

    path build_path(const station &from, const station &to, int entry_time) {
        auto res = dijkstra(from, to, entry_time);
        auto parent = res.second;
        int i = to.id;
        std::vector<station> result_path;
        result_path.push_back(to);
        while (parent[i] != from) {
            result_path.push_back(m_stations[parent[i].id]);
            i = parent[i].id;
        }
        result_path.push_back(from);
        std::reverse(result_path.begin(), result_path.end());
        return std::make_pair(res.first, result_path);
    }

    std::vector<station> build_reading_path() {
        //to do
    }

private:
    path dijkstra(const station &start, const station &finish, int entry_time) {
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
                int time_delay_t = 0;
                if (time_delay[edge.m_from.id] < edge.sum_line1) {
                    time_delay_t += edge.sum_line1 - time_delay[edge.m_from.id];
                } else {
                    time_delay_t = edge.m_frequency ?
                                   (edge.m_frequency - (time_delay[edge.m_from.id] % edge.m_frequency)) %
                                   edge.m_frequency
                                                    : 0;
                }
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
        return std::make_pair(dist[finish.id], parent);
    }

};

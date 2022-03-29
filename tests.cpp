#include "metro.h"
#include <gtest/gtest.h>
TEST(simple_underground, SIMPLE) {
std::vector<station> stations = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}};
std::vector<edge> edges = {{1,4,10,5}, {2,3,2,7},
                           {3,4,2,7}, {4,6,4,7},
                           {4,5,5,17}, {5,7,3,5},
                           {6,7,3,7}};
Metro metro(stations, edges);
auto res =  metro.build_path(station(1), station(7),0);
    EXPECT_EQ(res.size(), 4);
    EXPECT_EQ(res[0], 1);
    EXPECT_EQ(res[1], 4);
    EXPECT_EQ(res[2], 6);
    EXPECT_EQ(res[3], 7);
}
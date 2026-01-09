#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

struct ZoneData {
    string zoneID;
    long long totalTrips;
};

struct HourlySlot {
    string zoneID;
    int hour;
    long long count;
};

struct ZoneRanking {
    bool operator()(const ZoneData& a, const ZoneData& b) const {
        if (a.totalTrips != b.totalTrips) return a.totalTrips > b.totalTrips;
        return a.zoneID < b.zoneID;
    }
};

struct SlotRanking {
    bool operator()(const HourlySlot& a, const HourlySlot& b) const {
        if (a.count != b.count) return a.count > b.count;
        if (a.zoneID != b.zoneID) return a.zoneID < b.zoneID;
        return a.hour < b.hour;
    }
};

class CityTripProcessor {
private:
    struct TripStatistics {
        long long grandTotal = 0;
        long long hourlyDistribution[24] = {0};
    };

    unordered_map<string, TripStatistics> storage;

    int extractHour(const string& dt);

public:
    void loadFromStream(istream& input);
    void displayTopResults();
};

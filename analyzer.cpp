#include "analyzer.h"

int CityTripProcessor::extractHour(const string& dt) {
    size_t spacePos = dt.find(' ');
    if (spacePos == string::npos) spacePos = dt.find('T');
    if (spacePos == string::npos || spacePos + 2 >= dt.size()) return -1;

    try {
        return stoi(dt.substr(spacePos + 1, 2));
    } catch (...) { return -1; }
}

void CityTripProcessor::loadFromStream(istream& input) {
    ios::sync_with_stdio(false);
    input.tie(NULL);

    storage.reserve(250000);
    string row;
    bool isHeader = true;

    while (getline(input, row)) {
        if (row.empty()) continue;

        vector<string> tokens;
        size_t start = 0, end;
        while ((end = row.find(',', start)) != string::npos) {
            tokens.push_back(row.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(row.substr(start));

        if (tokens.size() < 6 || tokens[0] == "TripID") continue;

        string pickupZone = tokens[1];
        int hour = extractHour(tokens[3]);

        if (hour >= 0 && hour <= 23) {
            auto& entry = storage[pickupZone];
            entry.grandTotal++;
            entry.hourlyDistribution[hour]++;
        }
    }
}

void CityTripProcessor::displayTopResults() {

    priority_queue<ZoneData, vector<ZoneData>, ZoneRanking> zoneHeap;
    for (const auto& pair : storage) {
        ZoneData current = {pair.first, pair.second.grandTotal};
        zoneHeap.push(current);
        if (zoneHeap.size() > 10) zoneHeap.pop();
    }

    vector<ZoneData> topZones;
    while (!zoneHeap.empty()) {
        topZones.push_back(zoneHeap.top());
        zoneHeap.pop();
    }
    sort(topZones.begin(), topZones.end(), ZoneRanking());

    cout << "TOP_ZONES" << endl;
    for (const auto& z : topZones) cout << z.zoneID << "," << z.totalTrips << endl;

    priority_queue<HourlySlot, vector<HourlySlot>, SlotRanking> slotHeap;
    for (const auto& pair : storage) {
        for (int h = 0; h < 24; h++) {
            if (pair.second.hourlyDistribution[h] > 0) {
                HourlySlot s = {pair.first, h, pair.second.hourlyDistribution[h]};
                slotHeap.push(s);
                if (slotHeap.size() > 10) slotHeap.pop();
            }
        }
    }

    vector<HourlySlot> topSlots;
    while (!slotHeap.empty()) {
        topSlots.push_back(slotHeap.top());
        slotHeap.pop();
    }
    sort(topSlots.begin(), topSlots.end(), SlotRanking());

    cout << "TOP_SLOTS" << endl;
    for (const auto& s : topSlots) cout << s.zoneID << "," << s.hour << "," << s.count << endl;
}

int main() {
    CityTripProcessor analyzer;
    analyzer.loadFromStream(cin);
    analyzer.displayTopResults();
    return 0;
}

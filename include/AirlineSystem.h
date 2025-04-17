#ifndef AIRLINESYSTEM_H
#define AIRLINESYSTEM_H

#pragma once

#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

constexpr double EARTH_RADIUS = 3958.8; // Miles

// Utility function prototypes (implemented in Utils.cpp)
int convert_to_minutes(int hhmm);
string format_time(int minutes);

// Structs
struct airport {
    string code, state, city, name;
    double latitude, longitude;
    airport() = default;
    airport(string Code, string State, string City, string Name, double Lat, double Lon);
};

struct flight {
    string destination, source, airline, flight_number;
    int departure_time, duration, arrival_time; // minutes since midnight
    double cost, miles;
    flight(string Destination, string Source, string Airline, string Flight_number,
           int Departure_time, int Duration, int Arrival_time, double Cost, double Miles);
};

// Data loading function prototypes
unordered_map<string, airport> read_airports(string filename);
unordered_map<string, vector<flight>> read_flights(string filename);

// Class declaration
class airline_system {
private:
    unordered_map<string, airport> airports;
    unordered_map<string, vector<flight>> flights;

    double to_radians(double degrees);
    double haversine_distance(const airport &a1, const airport &a2);
    double heuristic_cost(string current, string goal);
    double heuristic_time(string current, string goal);

public:
    airline_system(string airport_file, string flight_file);
    
    void display_airport(string code);
    void airports_in_state(string state);
    void flights_leaving_from_source(string source_code);
    void flights_arriving_at_destination(string destination_code);
    void flights_from_source_to_destination(string source_code, string destination_code);
    void reconstruct_path(unordered_map<string, string> came_from, string destination);
    double cheapest_route(string source, string destination);
    int shortest_route_time(string source, string destination);
    int shortest_route_distance(string source, string destination);
    int fewest_hops(string source, string destination);
    void printPath(vector<string>& path);
    void reachable_destinations_num_flights(string current, int flights_left, vector<string>& path, vector<string>& seen_destinations);
    void destinations_under_cost(string source, double cost_left, vector<string> &path, vector<string> &seen_destinations);
    void destinations_within_time_helper(string current, int current_time, int cutoff, vector<string> &path, vector<string> &seen_destinations);
    void destinations_within_time(string source, int start_time, int allowed_duration);
    vector<string> bfs_path(string start, string goal);
    void fewest_flights_through_middle(string source, string middle, string destination);
    vector<string> reconstruct_path_vector(unordered_map<string, string> &came_from, string destination);
    int shortest_route_with_start(string source, string destination, int start_time, unordered_map<string, string> &came_from);
    int earliest_arrival(string source, string destination, int start_time);
    void earliest_arrival_through_middle(string source, string middle, string destination, int start_time);
    pair<double, vector<string>> cheapest_route_leg(string source, string destination);
    void cheapest_trip_through_middle(string source, string middle, string destination);
    bool direct_flight(string source, string destination);
    bool is_reachable(string source, string destination);
    void prim_mst_subgraph(vector<string>& nodes, double& totalWeight, vector<pair<string, string>>& treeEdges);
};

#endif // AIRLINESYSTEM_H

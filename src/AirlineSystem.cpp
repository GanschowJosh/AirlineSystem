#include "AirlineSystem.h"
#include "Utils.h"

// --- Struct Constructors ---
airport::airport(string Code, string State, string City, string Name, double Lat, double Lon)
    : code(Code), state(State), city(City), name(Name), latitude(Lat), longitude(Lon) {}

flight::flight(string Destination, string Source, string Airline, string Flight_number, int Departure_time, int Duration,
               int Arrival_time, double Cost, double Miles)
    : destination(Destination), source(Source), airline(Airline), flight_number(Flight_number), departure_time(Departure_time), duration(Duration), arrival_time(Arrival_time), cost(Cost), miles(Miles) {}

// --- Data Loading Functions ---
unordered_map<string, airport> read_airports(string filename)
{
    ifstream file(filename);
    string line;
    unordered_map<string, airport> airports;
    while (getline(file, line))
    {
        stringstream ss(line);
        string code, name, state, city;
        double lat, lon;
        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, city, ',');
        lat = 0;
        lon = 0;
        //ss >> lat;
        //ss.ignore();
        //ss >> lon;
        airports[code] = airport(code, state, city, name, lat, lon);
    }
    return airports;
}

unordered_map<string, vector<flight>> read_flights(string filename)
{
    ifstream file(filename);
    string line;
    unordered_map<string, vector<flight>> flights;
    while (getline(file, line))
    {
        stringstream ss(line);
        string source, destination, airline, flight_number;
        int raw_departure_time, raw_arrival_time;
        double cost, miles;
        ss >> source >> destination >> raw_departure_time >> raw_arrival_time >> cost >> miles >> airline >> flight_number;
        int departure_time = convert_to_minutes(raw_departure_time);
        int arrival_time = convert_to_minutes(raw_arrival_time);
        int duration = (arrival_time < departure_time) ? (1440 - departure_time + arrival_time) : (arrival_time - departure_time);
        flights[source].push_back(
            flight(destination, source, airline, flight_number, departure_time, duration, arrival_time, cost, miles));
    }
    return flights;
}

// --- Member Function Implementations ---

// Private utility functions

double airline_system::to_radians(double degrees)
{
    return degrees * M_PI / 180.0;
}

double airline_system::haversine_distance(const airport &a1, const airport &a2)
{
    double lat1 = to_radians(a1.latitude);
    double lon1 = to_radians(a1.longitude);
    double lat2 = to_radians(a2.latitude);
    double lon2 = to_radians(a2.longitude);
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

double airline_system::heuristic_cost(string current, string goal)
{
    // if (airports.find(current) == airports.end() || airports.find(goal) ==
    // airports.end())
    //     return 0;
    // return haversine_distance(airports[current], airports[goal]) * 0.2;
    return 0; // literally just uniform cost search now lol
}

double airline_system::heuristic_time(string current, string goal)
{
    // if (airports.find(current) == airports.end() || airports.find(goal) ==
    // airports.end())
    //     return 0;
    // return haversine_distance(airports[current], airports[goal]) / 500.0;
    return 0;
}

// Public functions

airline_system::airline_system(string airport_file, string flight_file)
{
    airports = read_airports(airport_file);
    flights = read_flights(flight_file);
}

void airline_system::display_airport(string code)
{
    if (airports.find(code) == airports.end())
    {
        cout << "Airport not found" << endl;
        return;
    }
    auto &a = airports[code];
    cout << "Airport code: " << a.code << endl;
    cout << "Airport name: " << a.name << endl;
    cout << "City: " << a.city << endl;
    cout << "State: " << a.state << endl;
    cout << "Latitude: " << a.latitude << endl;
    cout << "Longitude: " << a.longitude << endl;
}

void airline_system::airports_in_state(string state)
{
    int count = 0;
    for (const auto &a : airports)
    {
        if (a.second.state == state)
        {
            cout << a.second.name << " (" << a.second.code << ")" << endl;
            count++;
        }
    }
    cout << "Total airports in " << state << ": " << count << endl;
}

void airline_system::flights_leaving_from_source(string source_code)
{
    int count = 0;
    for (const auto &f : flights[source_code])
    {
        cout << "Destination: " << f.destination << endl;
        cout << "Airline: " << f.airline << endl;
        cout << "Flight number: " << f.flight_number << endl;
        cout << "Departure time: " << format_time(f.departure_time) << endl;
        cout << "Duration: " << f.duration << " minutes" << endl;
        cout << "Arrival time: " << format_time(f.arrival_time) << endl;
        cout << "Cost: $" << f.cost << endl;
        cout << "Miles: " << f.miles << endl
             << endl;
        count++;
    }
    cout << "Total flights leaving from " << airports[source_code].name << ": " << count << endl;
}

void airline_system::flights_arriving_at_destination(string destination_code)
{
    int count = 0;
    for (const auto &entry : flights)
    {
        for (const auto &f : entry.second)
        {
            if (f.destination == destination_code)
            {
                cout << "Source: " << f.source << endl;
                cout << "Airline: " << f.airline << endl;
                cout << "Flight number: " << f.flight_number << endl;
                cout << "Departure time: " << format_time(f.departure_time) << endl;
                cout << "Duration: " << f.duration << " minutes" << endl;
                cout << "Arrival time: " << format_time(f.arrival_time) << endl;
                cout << "Cost: $" << f.cost << endl;
                cout << "Miles: " << f.miles << endl
                     << endl;
                count++;
            }
        }
    }
    cout << "Total flights arriving at " << airports[destination_code].name << ": " << count << endl;
}

void airline_system::flights_from_source_to_destination(string source_code, string destination_code)
{
    int count = 0;
    for (const auto &f : flights[source_code])
    {
        if (f.destination == destination_code)
        {
            cout << "Airline: " << f.airline << endl;
            cout << "Flight number: " << f.flight_number << endl;
            cout << "Departure time: " << format_time(f.departure_time) << endl;
            cout << "Duration: " << f.duration << " minutes" << endl;
            cout << "Arrival time: " << format_time(f.arrival_time) << endl;
            cout << "Cost: $" << f.cost << endl;
            cout << "Miles: " << f.miles << endl
                 << endl;
            count++;
        }
    }
    cout << "Total flights from " << airports[source_code].name << " to " << airports[destination_code].name << ": " << count << endl;
}

void airline_system::reconstruct_path(unordered_map<string, string> came_from, string destination)
{
    vector<string> path;
    for (string cur = destination; cur != ""; cur = came_from[cur])
    {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    cout << "Optimal Path: ";
    for (int i = 0; i < path.size() - 1; i++)
    {
        cout << path[i] << " -> ";
    }
    cout << path.back() << endl;
}

double airline_system::cheapest_route(string source, string destination)
{
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    unordered_map<string, double> cost_so_far;
    unordered_map<string, string> came_from;

    pq.push({0, source});
    cost_so_far[source] = 0;
    came_from[source] = "";

    while (!pq.empty())
    {
        auto [cost, current] = pq.top();
        pq.pop();
        if (current == destination)
        {
            reconstruct_path(came_from, destination);
            return cost;
        }

        for (auto &f : flights[current])
        {
            double new_cost = cost_so_far[current] + f.cost;
            if (!cost_so_far.count(f.destination) || new_cost < cost_so_far[f.destination])
            {
                cost_so_far[f.destination] = new_cost;
                came_from[f.destination] = current;
                double priority = new_cost + heuristic_cost(f.destination, destination);
                pq.push({priority, f.destination});
            }
        }
    }
    return -1; // No path found
}

// Modified shortest_route_time to use minutes since midnight.
int airline_system::shortest_route_time(string source, string destination)
{
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    unordered_map<string, int> time_so_far;
    unordered_map<string, string> came_from;

    pq.push({0, source});
    time_so_far[source] = 0;
    came_from[source] = "";

    while (!pq.empty())
    {
        auto [current_time, current] = pq.top();
        pq.pop();
        if (current == destination)
        {
            reconstruct_path(came_from, destination);
            return current_time;
        }

        for (auto &f : flights[current])
        {
            // Compute waiting time based on current time modulo 1440 minutes.
            int current_mod = current_time % 1440;
            int wait_time =
                (f.departure_time >= current_mod) ? (f.departure_time - current_mod) : (1440 - current_mod + f.departure_time);
            int new_time = current_time + wait_time + f.duration;
            if (!time_so_far.count(f.destination) || new_time < time_so_far[f.destination])
            {
                time_so_far[f.destination] = new_time;
                came_from[f.destination] = current;
                int priority = new_time + heuristic_time(f.destination, destination);
                pq.push({priority, f.destination});
            }
        }
    }
    return -1; // No path found
}

//finds shortest route distance using uniform cost search
int airline_system::shortest_route_distance(string source, string destination) {
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    unordered_map<string, double> distance_so_far;
    unordered_map<string, string> came_from;

    pq.push({0, source});
    distance_so_far[source] = 0;
    came_from[source] = "";

    while (!pq.empty())
    {
        auto [distance, current] = pq.top();
        pq.pop();
        if (current == destination)
        {
            reconstruct_path(came_from, destination);
            return distance;
        }

        for (auto &f : flights[current])
        {
            double new_distance = distance_so_far[current] + f.miles;
            if (!distance_so_far.count(f.destination) || new_distance < distance_so_far[f.destination])
            {
                distance_so_far[f.destination] = new_distance;
                came_from[f.destination] = current;
                double priority = new_distance;
                pq.push({priority, f.destination});
            }
        }
    }
    return -1; //no path
}

int airline_system::fewest_hops(string source, string destination)
{
    //bfs in dis 
    queue<pair<string, int>> q;
    unordered_map<string, bool> visited;
    q.push({source, 0});
    visited[source] = true;

    while (!q.empty())
    {
        auto [current, hops] = q.front();
        q.pop();
        if (current == destination)
        {
            return hops;
        }
        for (const auto &f : flights[current])
        {
            if (!visited[f.destination])
            {
                visited[f.destination] = true;
                q.push({f.destination, hops + 1});
            }
        }
    }
    return -1; // No path found
}

// new methods (assigned 3/27):
// From source
// a) list destinations reachable with at most F flights
// b) list destinations for under M total fare
// c) list destinations for within H hours (including time waiting for next
// flight if you arrived too late in the day)

// with source and traveling to destination
// starting trip from source airport at Time (may need to add waiting time for
// initial flight here) must stop through Middle a) fewest flights (BFS from
// source to middle, then middle to destination) b) earliest arrival c) cheapest
// trip IDEA: A* from source to middle, then from middle to destination

// helper function to print path
void airline_system::printPath(vector<string> &path)
{
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i];
        if (i < path.size() - 1)
        {
            cout << " -> ";
        }
    }
    cout << endl;
}

// list all destinations reachable with at most F flights
void airline_system::reachable_destinations_num_flights(string current, int flights_left, vector<string> &path,
                                                        vector<string> &seen_destinations)
{
    for (auto &f : flights[current])
    {
        if (flights_left == 0)
        {
            return;
        }
        // avoid cycles
        if (find(path.begin(), path.end(), f.destination) != path.end())
            continue;
        path.push_back(f.destination);
        if (find(seen_destinations.begin(), seen_destinations.end(), f.destination) == seen_destinations.end())
        {
            printPath(path);
            seen_destinations.push_back(f.destination);
        }
        reachable_destinations_num_flights(f.destination, flights_left - 1, path, seen_destinations);
        path.pop_back();
    }
}

// list all destinations for under M total fare
void airline_system::destinations_under_cost(string source, double cost_left, vector<string> &path,
                                             vector<string> &seen_destinations)
{
    for (auto &f : flights[source])
    {
        if (f.cost > cost_left)
        {
            continue;
        }
        // avoid cycles
        if (find(path.begin(), path.end(), f.destination) != path.end())
            continue;
        path.push_back(f.destination);
        if (find(seen_destinations.begin(), seen_destinations.end(), f.destination) == seen_destinations.end())
        {
            printPath(path);
            seen_destinations.push_back(f.destination);
        }
        destinations_under_cost(f.destination, cost_left - f.cost, path, seen_destinations);
        path.pop_back();
    }
}

// helper for recursive DFS with a given start time and allowed duration. 'cutoff' is the maximum allowed absolute time (in minutes since midnight) you can arrive by.
void airline_system::destinations_within_time_helper(string current, int current_time, int cutoff, vector<string> &path,
                                                     vector<string> &seen_destinations)
{
    for (auto &f : flights[current])
    {
        int current_mod = current_time % 1440;
        int wait_time = (f.departure_time >= current_mod) ? (f.departure_time - current_mod) : (1440 - current_mod + f.departure_time);
        int new_time = current_time + wait_time + f.duration;

        if (new_time <= cutoff)
        {
            // Avoid cycles
            if (find(path.begin(), path.end(), f.destination) != path.end())
                continue;
            path.push_back(f.destination);
            if (find(seen_destinations.begin(), seen_destinations.end(), f.destination) == seen_destinations.end())
            {
                printPath(path);
                seen_destinations.push_back(f.destination);
            }
            destinations_within_time_helper(f.destination, new_time, cutoff, path, seen_destinations);
            path.pop_back();
        }
    }
}

// accounts for a given start time and allowed duration
void airline_system::destinations_within_time(string source, int start_time, int allowed_duration)
{
    vector<string> path;
    path.push_back(source);
    vector<string> seen_destinations;
    int cutoff = start_time + allowed_duration;
    destinations_within_time_helper(source, start_time, cutoff, path, seen_destinations);
}

// helper function for BFS returns the shortest path
vector<string> airline_system::bfs_path(string start, string goal)
{
    unordered_map<string, string> came_from;
    queue<string> q;
    q.push(start);
    came_from[start] = "";

    while (!q.empty())
    {
        string current = q.front();
        q.pop();
        if (current == goal)
        {
            vector<string> path;
            for (string cur = goal; cur != ""; cur = came_from[cur])
                path.push_back(cur);
            reverse(path.begin(), path.end());
            return path;
        }
        for (auto &f : flights[current])
        {
            if (came_from.find(f.destination) == came_from.end())
            {
                came_from[f.destination] = current;
                q.push(f.destination);
            }
        }
    }
    return vector<string>(); // empty vector if no path
}

// fewest flights stopping through middle
void airline_system::fewest_flights_through_middle(string source, string middle, string destination)
{
    vector<string> path1 = bfs_path(source, middle);
    vector<string> path2 = bfs_path(middle, destination);

    if (path1.empty() || path2.empty())
    {
        cout << "No path found through middle" << endl;
        return;
    }

    // combin the paths
    vector<string> full_path = path1;
    for (int i = 1; i < path2.size(); i++)
    {
        full_path.push_back(path2[i]);
    }

    printPath(full_path);
}

// New helper function to reconstruct the path as a vector from the came_from map.
vector<string> airline_system::reconstruct_path_vector(unordered_map<string, string> &came_from, string destination)
{
    vector<string> path;
    for (string cur = destination; cur != ""; cur = came_from[cur])
        path.push_back(cur);
    reverse(path.begin(), path.end());
    return path;
}

// adapted shortest_route_time but now it's technically Dijkstra? accounts for start time
int airline_system::shortest_route_with_start(string source, string destination, int start_time,
                                              unordered_map<string, string> &came_from)
{
    // stores pairs: (actual arrival time, airport)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    unordered_map<string, int> time_so_far;

    pq.push({start_time, source});
    time_so_far[source] = start_time;
    came_from[source] = "";

    while (!pq.empty())
    {
        auto [current_time, current] = pq.top();
        pq.pop();
        if (current == destination) {
            return current_time;
        }

        for (auto &f : flights[current])
        {
            // Compute waiting time based on current time modulo 1440 minutes.
            int current_mod = current_time % 1440;
            int wait_time =
                (f.departure_time >= current_mod) ? (f.departure_time - current_mod) : (1440 - current_mod + f.departure_time);
            int new_time = current_time + wait_time + f.duration;

            if (!time_so_far.count(f.destination) || new_time < time_so_far[f.destination])
            {
                time_so_far[f.destination] = new_time;
                came_from[f.destination] = current;
                pq.push({new_time, f.destination});
            }
        }
    }
    return -1; // No path found.
}

int airline_system::earliest_arrival(string source, string destination, int start_time)
{
    // Priority queue holds pairs: (arrival time, airport)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    unordered_map<string, int> time_so_far;
    unordered_map<string, string> came_from;

    pq.push({start_time, source});
    time_so_far[source] = start_time;
    came_from[source] = "";

    while (!pq.empty())
    {
        auto [current_time, current] = pq.top();
        pq.pop();

        if (current == destination)
        {
            // Reconstruct and print the path
            vector<string> full_path = reconstruct_path_vector(came_from, destination);
            cout << "Earliest arrival route: ";
            for (size_t i = 0; i < full_path.size(); i++)
            {
                cout << full_path[i];
                if (i < full_path.size() - 1)
                    cout << " -> ";
            }
            cout << endl;
            return current_time;
        }

        // Explore outgoing flights from the current airport.
        for (auto &f : flights[current])
        {
            int current_mod = current_time % 1440;
            int wait_time = (f.departure_time >= current_mod)
                                ? (f.departure_time - current_mod)
                                : (1440 - current_mod + f.departure_time);
            int new_time = current_time + wait_time + f.duration;

            if (!time_so_far.count(f.destination) || new_time < time_so_far[f.destination])
            {
                time_so_far[f.destination] = new_time;
                came_from[f.destination] = current;
                pq.push({new_time, f.destination});
            }
        }
    }
    // No path found
    return -1;
}


// finds earliest arrival time from source to destination throug middle
void airline_system::earliest_arrival_through_middle(string source, string middle, string destination, int start_time)
{
    unordered_map<string, string> came_from_leg1;
    int arrival_middle = shortest_route_with_start(source, middle, start_time, came_from_leg1);
    if (arrival_middle == -1)
    {
        cout << "No path found from " << source << " to " << middle << endl;
        return;
    }

    unordered_map<string, string> came_from_leg2;
    int arrival_dest = shortest_route_with_start(middle, destination, arrival_middle, came_from_leg2);
    if (arrival_dest == -1)
    {
        cout << "No path found from " << middle << " to " << destination << endl;
        return;
    }

    vector<string> path_leg1 = reconstruct_path_vector(came_from_leg1, middle);
    vector<string> path_leg2 = reconstruct_path_vector(came_from_leg2, destination);

    vector<string> full_path = path_leg1;
    full_path.insert(full_path.end(), path_leg2.begin() + 1, path_leg2.end());

    cout << "Earliest arrival route: ";
    for (size_t i = 0; i < full_path.size(); i++)
    {
        cout << full_path[i];
        if (i < full_path.size() - 1)
            cout << " -> ";
    }
    cout << endl;
    cout << "Earliest arrival time: " << format_time(arrival_dest) << endl;
}

// pair containing the total cost and the corresponding path from source to destination.
pair<double, vector<string>> airline_system::cheapest_route_leg(string source, string destination)
{
    // Priority queue: (priority, airport), where priority = accumulated cost + heuristic.
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    unordered_map<string, double> cost_so_far;
    unordered_map<string, string> came_from;

    cost_so_far[source] = 0;
    came_from[source] = "";
    pq.push({0, source});

    while (!pq.empty())
    {
        auto [priority, current] = pq.top();
        pq.pop();
        if (current == destination)
        {
            vector<string> path = reconstruct_path_vector(came_from, destination);
            return {cost_so_far[destination], path};
        }
        for (auto &f : flights[current])
        {
            double new_cost = cost_so_far[current] + f.cost;
            if (!cost_so_far.count(f.destination) || new_cost < cost_so_far[f.destination])
            {
                cost_so_far[f.destination] = new_cost;
                came_from[f.destination] = current;
                double new_priority = new_cost + heuristic_cost(f.destination, destination);
                pq.push({new_priority, f.destination});
            }
        }
    }
    return {-1, vector<string>()}; // No path found.
}

// finds the cheapest trip from source to destination through middle
void airline_system::cheapest_trip_through_middle(string source, string middle, string destination)
{
    auto [cost1, path1] = cheapest_route_leg(source, middle);
    if (cost1 < 0)
    {
        cout << "No path found from " << source << " to " << middle << endl;
        return;
    }

    auto [cost2, path2] = cheapest_route_leg(middle, destination);
    if (cost2 < 0)
    {
        cout << "No path found from " << middle << " to " << destination << endl;
        return;
    }

    double total_cost = cost1 + cost2;
    vector<string> full_path = path1;
    // Remove duplicate entry for the middle airport.
    full_path.insert(full_path.end(), path2.begin() + 1, path2.end());

    printPath(full_path);
    cout << "Total cost: $" << total_cost << endl;
}

bool airline_system::direct_flight(string source, string destination)
{
    for (auto &f : flights[source])
    {
        if (f.destination == destination)
        {
            return true;
        }
    }
    return false;
}

bool airline_system::is_reachable(string source, string destination)
{
    return fewest_hops(source, destination) != -1;
}

void airline_system::prim_mst_subgraph(vector<string>& nodes, double& totalWeight, vector<pair<string, string>>& treeEdges) {
    int n = nodes.size();

    unordered_map<string, int> idx;

    for(int i = 0; i < n; i++) idx[nodes[i]] = i;

    //building subgraph
    vector<vector<pair<int, double>>> adj(n);
    for(int i = 0; i < n; i++) {
        auto it = flights.find(nodes[i]);
        if(it == flights.end()) continue; //not concerning current subgraph
        for (auto &f : it->second) {
            auto jt = idx.find(f.destination);
            if(jt != idx.end()) {
                adj[i].push_back({jt->second, f.cost});
            }
        }
    }

    set inMST;
    inMST += 0; //start at first node
    totalWeight = 0;

    while(inMST.size() < n) {
        double bestW = numeric_limits<double>::infinity();
        int bu=-1, bv=-1;
        //look for cheapest edge crossing the "cut" (one edge in and one edge not in current tree)
        for(int u = 0; u < n; u++) {
            if (!(u^inMST)) continue; //u not in MST
            for(auto &pr : adj[u]) {
                int v = pr.first;
                double w = pr.second;
                if(!(v^inMST) && w < bestW) {
                    bestW = w;
                    bu = u;
                    bv = v;
                }
            }
        }
        if(bu < 0) {  //disconnected
            cout << "Subset not fully connected" << endl;
            break;
        } 
        totalWeight += bestW;
        inMST += bu;
        inMST += bv;
        treeEdges.emplace_back(nodes[bu], nodes[bv]);
    }

    // //decode into string-pairs
    // for(int code : chosenEdges.data) {
    //     int u = code / n, v = code % n;
    //     treeEdges.emplace_back(nodes[u], nodes[v]);
    // }
}
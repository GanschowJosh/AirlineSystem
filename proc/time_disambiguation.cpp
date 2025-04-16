// Notes:
//  Airport CSV and Flights CSV are provided
//  Flights are shown for one day and they repeat every day the same way

#include <bits/stdc++.h>

using namespace std;

constexpr double EARTH_RADIUS = 3958.8; // Miles

// Converts HHMM (e.g., 800 or 1130) to minutes since midnight.
int convert_to_minutes(int hhmm)
{
    int hours = hhmm / 100;
    int minutes = hhmm % 100;
    return hours * 60 + minutes;
}

// Formats minutes since midnight into "hh:mm" string.
string format_time(int minutes)
{
    int h = minutes / 60;
    int m = minutes % 60;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", h, m);
    return string(buffer);
}

struct airport
{
    string code, state, city, name;
    double latitude, longitude;
    airport() = default;
    airport(string Code, string State, string City, string Name, double Lat, double Lon)
        : code(Code), state(State), city(City), name(Name), latitude(Lat), longitude(Lon) {}
};

struct flight
{
    string destination, source, airline, flight_number;
    int departure_time, duration, arrival_time; // times in minutes since midnight
    double cost, miles;
    flight(string Destination, string Source, string Airline, string Flight_number,
           int Departure_time, int Duration, int Arrival_time, double Cost, double Miles)
        : destination(Destination), source(Source), airline(Airline), flight_number(Flight_number),
          departure_time(Departure_time), duration(Duration), arrival_time(Arrival_time), cost(Cost), miles(Miles) {}
};

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
        ss >> lat;
        ss.ignore();
        ss >> lon;
        airports[code] = airport(code, state, city, name, lat, lon);
    }
    return airports;
}

unordered_map<string, vector<flight>> read_flights(string filename)
{
    // Format: space separated: source destination departure_time(hhmm) arrival_time(hhmm) cost miles airline flight_number
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

        // Convert HHMM values to minutes since midnight.
        int departure_time = convert_to_minutes(raw_departure_time);
        int arrival_time = convert_to_minutes(raw_arrival_time);

        int duration;
        // Calculate duration, accounting for flights that go past midnight.
        if (arrival_time < departure_time)
        {
            duration = 1440 - departure_time + arrival_time;
        }
        else
        {
            duration = arrival_time - departure_time;
        }
        flights[source].push_back(flight(destination, source, airline, flight_number, departure_time, duration, arrival_time, cost, miles));
    }
    return flights;
}

class airline_system
{
private:
    unordered_map<string, airport> airports;
    unordered_map<string, vector<flight>> flights;

    double to_radians(double degrees)
    {
        return degrees * M_PI / 180.0;
    }

    double haversine_distance(const airport &a1, const airport &a2)
    {
        double lat1 = to_radians(a1.latitude);
        double lon1 = to_radians(a1.longitude);
        double lat2 = to_radians(a2.latitude);
        double lon2 = to_radians(a2.longitude);

        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;
        double a = sin(dlat / 2) * sin(dlat / 2) +
                   cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return EARTH_RADIUS * c;
    }

    double heuristic_cost(string current, string goal)
    {
        if (airports.find(current) == airports.end() || airports.find(goal) == airports.end())
            return 0;
        return haversine_distance(airports[current], airports[goal]) * 0.2;
    }

    double heuristic_time(string current, string goal)
    {
        if (airports.find(current) == airports.end() || airports.find(goal) == airports.end())
            return 0;
        return haversine_distance(airports[current], airports[goal]) / 500.0;
    }

public:
    airline_system(string airport_file, string flight_file)
    {
        airports = read_airports(airport_file);
        flights = read_flights(flight_file);
    }

    void display_airport(string code)
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

    void airports_in_state(string state)
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

    void flights_leaving_from_source(string source_code)
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

    void flights_arriving_at_destination(string destination_code)
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

    void flights_from_source_to_destination(string source_code, string destination_code)
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

    void reconstruct_path(unordered_map<string, string> came_from, string destination)
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

    double cheapest_route(string source, string destination)
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
    int shortest_route_time(string source, string destination)
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
                int wait_time = (f.departure_time >= current_mod)
                                    ? (f.departure_time - current_mod)
                                    : (1440 - current_mod + f.departure_time);
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

    int fewest_hops(string source, string destination)
    {
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
    // c) list destinations for within H hours (including time waiting for next flight if you arrived too late in the day)

    // with source and traveling to destination
    // starting trip from source airport at Time (may need to add waiting time for initial flight here)
    // must stop through Middle
    // a) fewest flights (BFS from source to middle, then middle to destination)
    // b) earliest arrival
    // c) cheapest trip
    // IDEA: A* from source to middle, then from middle to destination

    // helper function to print path
    void printPath(vector<string> &path)
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
    void reachable_destinations_num_flights(string current, int flights_left, vector<string> &path, vector<string> &seen_destinations)
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
    void destinations_under_cost(string source, double cost_left, vector<string> &path, vector<string> &seen_destinations)
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

    // helper for recursive DFS with a given start time and allowed duration.
    //'cutoff' is the maximum allowed absolute time (in minutes since midnight) you can arrive by.
    void destinations_within_time_helper(string current, int current_time, int cutoff, vector<string> &path, vector<string> &seen_destinations)
    {
        for (auto &f : flights[current])
        {
            int current_mod = current_time % 1440;
            int wait_time = (f.departure_time >= current_mod)
                                ? (f.departure_time - current_mod)
                                : (1440 - current_mod + f.departure_time);
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
    void destinations_within_time(string source, int start_time, int allowed_duration)
    {
        vector<string> path;
        path.push_back(source);
        vector<string> seen_destinations;
        int cutoff = start_time + allowed_duration;
        destinations_within_time_helper(source, start_time, cutoff, path, seen_destinations);
    }

    // helper function for BFS returns the shortest path
    vector<string> bfs_path(string start, string goal)
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
    void fewest_flights_through_middle(string source, string middle, string destination)
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
    vector<string> reconstruct_path_vector(unordered_map<string, string> &came_from, string destination)
    {
        vector<string> path;
        for (string cur = destination; cur != ""; cur = came_from[cur])
            path.push_back(cur);
        reverse(path.begin(), path.end());
        return path;
    }

    // adapted shortest_route_time but now it's technically Dijkstra? accounts for start time
    int shortest_route_with_start(string source, string destination, int start_time, unordered_map<string, string> &came_from)
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
            if (current == destination)
                return current_time;

            for (auto &f : flights[current])
            {
                // Compute waiting time based on current time modulo 1440 minutes.
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
        return -1; // No path found.
    }

    // finds earliest arrival time from source to destination throug middle
    void earliest_arrival_through_middle(string source, string middle, string destination, int start_time)
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
    pair<double, vector<string>> cheapest_route_leg(string source, string destination)
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
    void cheapest_trip_through_middle(string source, string middle, string destination)
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

    bool direct_flight(string source, string destination)
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

    bool is_reachable(string source, string destination)
    {
        return fewest_hops(source, destination) != -1;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <airport_file> <flight_file>" << endl;
        return 1;
    }
    airline_system as(argv[1], argv[2]);

    while (true)
    {
        cout << "1. Display airport details" << endl;
        cout << "2. Airports in a state" << endl;
        cout << "3. Flights leaving from source" << endl;
        cout << "4. Flights arriving at destination" << endl;
        cout << "5. Flights from source to destination" << endl;
        cout << "6. Cheapest route from source to destination" << endl;
        cout << "7. Shortest route from source to destination" << endl;
        cout << "8. Fewest number of hops from source to destination" << endl;
        cout << "9. Is there a direct flight from source to destination" << endl;
        cout << "10. Destinations within time from source" << endl;
        cout << "11. Fewest flights through a middle airport" << endl;
        cout << "12. Earliest arrival through a middle airport" << endl;
        cout << "13. Cheapest trip through a middle airport" << endl;
        cout << "14. List destinations reachable with at most F flights" << endl;
        cout << "15. List destinations for under M total fare" << endl;
        cout << "16. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

#ifdef _WIN32
        system("cls"); // Clear screen on Windows
#else
        system("clear"); // Clear screen on Linux and macOS
#endif

        if (choice == 16)
        {
            break;
        }
        string source, destination, middle;
        if (choice == 1)
        {
            cout << "Enter airport code: ";
            cin >> source;
            as.display_airport(source);
        }
        else if (choice == 2)
        {
            cout << "Enter state: ";
            cin >> source;
            as.airports_in_state(source);
        }
        else if (choice == 3)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            as.flights_leaving_from_source(source);
        }
        else if (choice == 4)
        {
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.flights_arriving_at_destination(destination);
        }
        else if (choice == 5)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.flights_from_source_to_destination(source, destination);
        }
        else if (choice == 6)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            auto cost = as.cheapest_route(source, destination);
            if (cost == -1)
            {
                cout << "No path found" << endl;
            }
            else
            {
                cout << "Cheapest route from " << source << " to " << destination << ": $" << cost << endl;
            }
        }
        else if (choice == 7)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            auto total_time = as.shortest_route_time(source, destination);
            if (total_time == -1)
            {
                cout << "No path found" << endl;
            }
            else
            {
                cout << "Shortest route from " << source << " to " << destination << ": "
                     << total_time << " minutes" << endl;
            }
        }
        else if (choice == 8)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            auto hops = as.fewest_hops(source, destination);
            if (hops != -1)
                cout << "Fewest number of hops from " << source << " to " << destination << ": " << hops << endl;
            else
                cout << "Not possible to get from " << source << " to " << destination << endl;
        }
        else if (choice == 9)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            if (as.direct_flight(source, destination))
                cout << "There is a direct flight from " << source << " to " << destination << endl;
            else
                cout << "There is no direct flight from " << source << " to " << destination << endl;
        }
        else if (choice == 10)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            int time_limit;
            cout << "Enter available time (in minutes): ";
            cin >> time_limit;
            cout << "Enter initial start time (HHMM): ";
            int start_time_input;
            cin >> start_time_input;
            int start_time = convert_to_minutes(start_time_input);
            as.destinations_within_time(source, start_time, time_limit);
        }
        else if (choice == 11)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter middle airport code: ";
            cin >> middle;
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.fewest_flights_through_middle(source, middle, destination);
        }
        else if (choice == 12)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter middle airport code: ";
            cin >> middle;
            cout << "Enter destination airport code: ";
            cin >> destination;
            int start_time_input;
            cout << "Enter start time (HHMM): ";
            cin >> start_time_input;
            int start_time = convert_to_minutes(start_time_input);
            as.earliest_arrival_through_middle(source, middle, destination, start_time);
        }
        else if (choice == 13)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter middle airport code: ";
            cin >> middle;
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.cheapest_trip_through_middle(source, middle, destination);
        }
        else if (choice == 14)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            int max_flights;
            cout << "Enter maximum number of flights: ";
            cin >> max_flights;
            vector<string> path = {source};
            vector<string> seen_destinations;
            as.reachable_destinations_num_flights(source, max_flights, path, seen_destinations);
        }
        else if (choice == 15)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            double max_cost;
            cout << "Enter maximum total fare: ";
            cin >> max_cost;
            vector<string> path = {source};
            vector<string> seen_destinations;
            as.destinations_under_cost(source, max_cost, path, seen_destinations);
        }

        cout << "\n==================================================\n"
             << endl;
    }

    return 0;
}
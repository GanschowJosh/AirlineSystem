#include "AirlineSystem.h"
#include "Utils.h"
#include <cstdlib>

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
        cout << "7. Shortest route time from source to destination" << endl;
        cout << "8. Shortest route distance from source to destination" << endl;
        cout << "9. Fewest number of hops from source to destination" << endl;
        cout << "10. Is there a direct flight from source to destination" << endl;
        cout << "11. Destinations within time from source" << endl;
        cout << "12. Earliest arrival from source to destination" << endl;  // New option
        cout << "13. Fewest flights through a middle airport" << endl;
        cout << "14. Earliest arrival through a middle airport" << endl;
        cout << "15. Cheapest trip through a middle airport" << endl;
        cout << "16. List destinations reachable with at most F flights" << endl;
        cout << "17. List destinations for under M total fare" << endl;
        cout << "18. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

#ifdef _WIN32
        system("cls"); // Clear screen on Windows
#else
        system("clear"); // Clear screen on Linux and macOS
#endif

        if (choice == 18)
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
                cout << "Shortest route time from " << source << " to " << destination << ": "
                     << total_time << " minutes" << endl;
            }
        }
        else if (choice == 8)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            auto distance = as.shortest_route_distance(source, destination);
            if (distance == -1)
            {
                cout << "No path found" << endl;
            }
            else
            {
                cout << "Shortest route distance from " << source << " to " << destination << ": "
                     << distance << " miles" << endl;
            }
        }
        else if (choice == 9)
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
        else if (choice == 10)
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
        else if (choice == 11)
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
        else if (choice == 12)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter destination airport code: ";
            cin >> destination;
            cout << "Enter start time (HHMM): ";
            int start_time_input;
            cin >> start_time_input;
            int start_time = convert_to_minutes(start_time_input);
            int arrival = as.earliest_arrival(source, destination, start_time);
            if (arrival != -1)
                cout << "Earliest arrival time: " << format_time(arrival) << endl;
            else
                cout << "No path found" << endl;
        }
        else if (choice == 13)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter middle airport code: ";
            cin >> middle;
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.fewest_flights_through_middle(source, middle, destination);
        }
        else if (choice == 14)
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
        else if (choice == 15)
        {
            cout << "Enter source airport code: ";
            cin >> source;
            cout << "Enter middle airport code: ";
            cin >> middle;
            cout << "Enter destination airport code: ";
            cin >> destination;
            as.cheapest_trip_through_middle(source, middle, destination);
        }
        else if (choice == 16)
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
        else if (choice == 17)
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

        cout << "\n==================================================\n" << endl;
    }

    return 0;
}

# Airline System

A C++ command-line application to explore airports and flights data, compute optimal travel routes, and query various metrics such as cost, time, and hops between airports. Created for my CSC310 (Advanced Data Structures) class.

## Features

- Load airport and flight data from CSV files
- Display airport details (code, name, city, state, coordinates)
- List all airports in a given state
- Show flights leaving from or arriving at a given airport
- Query flights between two airports
- Compute cheapest routes using A* with geographic heuristics
- Compute shortest-time routes (including connection waits)
- Find routes with fewest hops (BFS)
- List reachable destinations within time, cost, or number of flights
- Multi-leg queries: fewest flights, earliest arrival, and cheapest trip through a middle airport

## Repository Structure

```plaintext
YourProject/
├── include/            # Header files
│   ├── AirlineSystem.h
│   └── Utils.h
├── src/                # Source files
│   ├── AirlineSystem.cpp
│   ├── Utils.cpp
│   └── main.cpp
├── Makefile            # Build using Make
├── CMakeLists.txt      # Build using CMake
└── README.md           # This file
```

## Prerequisites

- A C++ compiler supporting C++11 (e.g., `g++`, `clang++`, or Visual Studio)
- CMake ≥ 3.10 (optional)
- Make (optional)
- Git (for cloning the repo)

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/GanschowJosh/AirlineSystem.git
cd AirlineSystem
```

### Build with Make

```bash
make
```

Resulting binary: `bin/airline_app` (or `.bin\airline_app.exe` on Windows)

### Build with CMake

```bash
mkdir build && cd build
cmake -S .. -B .
cmake --build . --config Release
```

On Linux/Mac: binary at `build/airline_app`  
On Windows (Visual Studio): `build\Debug\airline_app.exe` or `build\Release\airline_app.exe`

## Usage

```bash
./bin/airline_app <airport_csv> <flight_csv>
```

Example:  
```bash
./bin/airline_app data/airports.csv data/flights.csv
```

Once launched, follow the on-screen menu to select queries and input airport codes or parameters.

## Contributing

1. Fork the repo
2. Create a feature branch (`git checkout -b feature/YourFeature`)
3. Commit your changes (`git commit -m "Add feature"`)
4. Push to your branch (`git push origin feature/YourFeature`)
5. Open a Pull Request

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.


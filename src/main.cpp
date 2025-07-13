#include <systemc.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "noc.h"
#include "routing_algorithms.h"

// Function to run a simulation with specified parameters
void run_simulation(int mesh_size, const std::string& routing_algorithm, 
                   int packet_injection_rate, int simulation_time) {
    // Create NoC
    NoC noc("network_on_chip", mesh_size, mesh_size, 
            routing_algorithm, packet_injection_rate, simulation_time);
    
    // Start simulation
    sc_start(simulation_time + 100, SC_NS);
    
    // End simulation
    sc_stop();
}

int main(int argc, char* argv[]) {
    // Seed random number generator
    std::srand(std::time(nullptr));
    
    // Default parameters
    int mesh_size = 4;  // 4x4 mesh by default
    std::string routing_algorithm = "XY";  // XY routing by default
    int packet_injection_rate = 10;  // 10% injection rate by default
    int simulation_time = 1000;  // 1000 cycles by default
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-size" && i + 1 < argc) {
            mesh_size = std::atoi(argv[++i]);
        } else if (arg == "-routing" && i + 1 < argc) {
            routing_algorithm = argv[++i];
        } else if (arg == "-rate" && i + 1 < argc) {
            packet_injection_rate = std::atoi(argv[++i]);
        } else if (arg == "-time" && i + 1 < argc) {
            simulation_time = std::atoi(argv[++i]);
        } else if (arg == "-help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -size SIZE        Set mesh size (default: 4)" << std::endl;
            std::cout << "  -routing ALGO     Set routing algorithm: XY, WEST_FIRST (default: XY)" << std::endl;
            std::cout << "  -rate RATE        Set packet injection rate in percent (default: 10)" << std::endl;
            std::cout << "  -time TIME        Set simulation time in cycles (default: 1000)" << std::endl;
            std::cout << "  -help, -h         Show this help message" << std::endl;
            return 0;
        }
    }
    
    // Validate parameters
    if (mesh_size < 2 || mesh_size > 16) {
        std::cout << "Error: Mesh size must be between 2 and 16" << std::endl;
        return 1;
    }
    
    if (routing_algorithm != "XY" && routing_algorithm != "WEST_FIRST") {
        std::cout << "Error: Unknown routing algorithm. Supported: XY, WEST_FIRST" << std::endl;
        return 1;
    }
    
    if (packet_injection_rate < 1 || packet_injection_rate > 100) {
        std::cout << "Error: Packet injection rate must be between 1 and 100" << std::endl;
        return 1;
    }
    
    if (simulation_time < 100) {
        std::cout << "Error: Simulation time must be at least 100 cycles" << std::endl;
        return 1;
    }
    
    // Run simulation with XY routing
    std::cout << "Running simulation with " << routing_algorithm << " routing algorithm..." << std::endl;
    run_simulation(mesh_size, routing_algorithm, packet_injection_rate, simulation_time);
    
    // If we're using XY routing, also run with West-First for comparison
    if (routing_algorithm == "XY") {
        std::cout << "\n\nRunning simulation with WEST_FIRST routing algorithm for comparison..." << std::endl;
        run_simulation(mesh_size, "WEST_FIRST", packet_injection_rate, simulation_time);
    }
    
    return 0;
}
#ifndef ROUTING_ALGORITHMS_H
#define ROUTING_ALGORITHMS_H

#include <systemc.h>
#include <vector>
#include <utility>
#include <cstdlib>
#include <climits>

// Direction enumeration for mesh topology
enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    LOCAL = 4,
    NONE = -1
};

// Base class for routing algorithms
class RoutingAlgorithm {
public:
    virtual ~RoutingAlgorithm() {}
    
    // Calculate output port based on current position and destination
    virtual Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const = 0;
    
    // Get algorithm name
    virtual std::string getName() const = 0;
};

// XY Routing Algorithm (Dimension-Ordered Routing)
// First routes in X dimension, then in Y dimension
class XYRoutingAlgorithm : public RoutingAlgorithm {
public:
    Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const override {
        
        // First check if we're at destination
        if (current_x == dest_x && current_y == dest_y) {
            return LOCAL;
        }
        
        // First move in X direction until we match destination's X
        if (current_x < dest_x) {
            return EAST;
        } else if (current_x > dest_x) {
            return WEST;
        }
        
        // Then move in Y direction
        if (current_y < dest_y) {
            return SOUTH;
        } else if (current_y > dest_y) {
            return NORTH;
        }
        
        return NONE; // Should never happen
    }
    
    std::string getName() const override {
        return "XY Routing";
    }
};

// Adaptive West-First Routing Algorithm
// Packets can only turn west at the first hop, after that they must go minimally
class WestFirstRoutingAlgorithm : public RoutingAlgorithm {
public:
    Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const override {
        
        // First check if we're at destination
        if (current_x == dest_x && current_y == dest_y) {
            return LOCAL;
        }
        
        // A vector of possible directions
        std::vector<Direction> possible_dirs;
        
        // If destination is to the west, we must go west first
        if (current_x > dest_x) {
            return WEST;
        }
        
        // Otherwise, we can choose adaptively among remaining directions
        if (current_x < dest_x) {
            possible_dirs.push_back(EAST);
        }
        if (current_y < dest_y) {
            possible_dirs.push_back(SOUTH);
        }
        if (current_y > dest_y) {
            possible_dirs.push_back(NORTH);
        }
        
        // If there are possible directions, choose one (randomly in this case)
        if (!possible_dirs.empty()) {
            // In a real implementation, we'd choose based on congestion
            // For simulation, we'll just pick the first option
            return possible_dirs[0];
        }
        
        return NONE; // Should never happen
    }
    
    std::string getName() const override {
        return "West-First Routing";
    }
};

// Factory function to create routing algorithms
inline RoutingAlgorithm* createRoutingAlgorithm(const std::string& algorithm_name) {
    if (algorithm_name == "XY") {
        return new XYRoutingAlgorithm();
    } else if (algorithm_name == "WEST_FIRST") {
        return new WestFirstRoutingAlgorithm();
    }
    
    // Default to XY routing
    std::cout << "Warning: Unknown routing algorithm. Using XY routing." << std::endl;
    return new XYRoutingAlgorithm();
}

#endif // ROUTING_ALGORITHMS_H
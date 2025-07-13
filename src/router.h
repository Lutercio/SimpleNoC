#ifndef ROUTER_H
#define ROUTER_H

#include <systemc.h>
#include <vector>
#include <queue>
#include <memory>
#include "packet.h"
#include "routing_algorithms.h"

// Router for Network-on-Chip
class Router : public sc_module {
public:
    // Ports
    sc_in<bool> clk;
    
    // Input ports from channels (NORTH, EAST, SOUTH, WEST, LOCAL)
    sc_in<Packet> in_packets[5];
    sc_in<bool> in_valids[5];
    sc_out<bool> in_readys[5];
    
    // Output ports to channels (NORTH, EAST, SOUTH, WEST, LOCAL)
    sc_out<Packet> out_packets[5];
    sc_out<bool> out_valids[5];
    sc_in<bool> out_readys[5];

    // Constructor
    Router(sc_module_name name, int x, int y, int mesh_size_x, int mesh_size_y, 
           RoutingAlgorithm* routing_algorithm) : 
        sc_module(name),
        x_(x),
        y_(y),
        mesh_size_x_(mesh_size_x),
        mesh_size_y_(mesh_size_y),
        routing_algorithm_(routing_algorithm) {
        
        // Initialize input buffers and output status
        input_buffers.resize(5);
        output_reserved.resize(5, false);
        
        // Register processes
        SC_METHOD(process_inputs);
        sensitive << clk.pos();
        
        SC_METHOD(process_routing);
        sensitive << clk.pos();
    }

    // Process to handle inputs from channels
    void process_inputs() {
        // Process each input port
        for (int i = 0; i < 5; i++) {
            // If there's a valid input and we're ready to receive
            if (in_valids[i].read() && input_buffers[i].size() < BUFFER_SIZE) {
                // Read packet from input
                Packet packet = in_packets[i].read();
                
                // Add router position to path
                packet.addToPath(y_ * mesh_size_x_ + x_);
                
                // Increment hops counter
                packet.incrementHops();
                
                // Add to input buffer
                input_buffers[i].push(packet);
            }
            
            // Signal if we're ready to receive more packets
            in_readys[i].write(input_buffers[i].size() < BUFFER_SIZE);
        }
    }

    // Process to handle routing of packets
    void process_routing() {
        // Reset output valid signals
        for (int i = 0; i < 5; i++) {
            if (!output_reserved[i]) {
                out_valids[i].write(false);
            }
        }
        
        // Process packets in input buffers
        for (int input_port = 0; input_port < 5; input_port++) {
            if (!input_buffers[input_port].empty()) {
                // Get packet from buffer
                Packet packet = input_buffers[input_port].front();
                
                // Convert destination ID to x,y coordinates
                int dest_x = packet.getDstId() % mesh_size_x_;
                int dest_y = packet.getDstId() / mesh_size_x_;
                
                // Determine output port using routing algorithm
                Direction output_dir = routing_algorithm_->calculateOutputPort(
                    x_, y_, dest_x, dest_y);
                
                int output_port = static_cast<int>(output_dir);
                
                // If output is available and ready to receive
                if (output_port != NONE && !output_reserved[output_port] && out_readys[output_port].read()) {
                    // Forward the packet
                    out_packets[output_port].write(packet);
                    out_valids[output_port].write(true);
                    output_reserved[output_port] = true;
                    
                    // Remove packet from input buffer
                    input_buffers[input_port].pop();
                }
            }
        }
    }

    // Get router coordinates
    int getX() const { return x_; }
    int getY() const { return y_; }
    
    // Get node ID
    int getNodeId() const { return y_ * mesh_size_x_ + x_; }
    
    // Get routing algorithm name
    std::string getRoutingAlgorithmName() const {
        return routing_algorithm_->getName();
    }

private:
    int x_, y_;                          // Router coordinates in mesh
    int mesh_size_x_, mesh_size_y_;      // Mesh dimensions
    std::unique_ptr<RoutingAlgorithm> routing_algorithm_;  // Routing algorithm
    
    static const int BUFFER_SIZE = 4;    // Input buffer size
    std::vector<std::queue<Packet>> input_buffers;  // Buffers for each input port
    std::vector<bool> output_reserved;   // Whether output ports are currently in use
};

#endif // ROUTER_H
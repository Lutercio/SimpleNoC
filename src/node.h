#ifndef NODE_H
#define NODE_H

#include <systemc.h>
#include <queue>
#include <vector>
#include <random>
#include "packet.h"

// Node for Network-on-Chip
class Node : public sc_module {
public:
    // Ports
    sc_in<bool> clk;
    
    // Local port connection to router
    sc_out<Packet> out_packet;  // Output to router
    sc_out<bool> out_valid;     // Output valid signal
    sc_in<bool> out_ready;      // Router ready signal
    
    sc_in<Packet> in_packet;    // Input from router
    sc_in<bool> in_valid;       // Router valid signal
    sc_out<bool> in_ready;      // Input ready signal

    // Constructor
    Node(sc_module_name name, int id, int total_nodes, int packet_injection_rate = 10,
         int simulation_time = 1000) : 
        sc_module(name),
        id_(id),
        total_nodes_(total_nodes),
        packet_injection_rate_(packet_injection_rate),
        simulation_time_(simulation_time),
        packets_sent_(0),
        packets_received_(0),
        current_time_(0),
        rng_(std::random_device()()) {
        
        // Register processes
        SC_METHOD(process_send);
        sensitive << clk.pos();
        
        SC_METHOD(process_receive);
        sensitive << clk.pos();
    }

    // Process to send packets
    void process_send() {
        // Update time
        current_time_++;
        
        // Check if we're ready to send and router can receive
        if (out_ready.read()) {
            // Attempt to send a new packet based on injection rate
            if (generate_packet()) {
                // Create a new packet
                int dest_id = generate_destination();
                int payload = rand() % 1000;
                Packet packet(id_, dest_id, Packet::DATA, payload, current_time_);
                
                // Send packet
                out_packet.write(packet);
                out_valid.write(true);
                packets_sent_++;
                
                std::cout << "Node " << id_ << " sending packet to " << dest_id 
                          << " (payload: " << payload << ") at time " << current_time_ << std::endl;
            } else {
                // No packet to send this cycle
                out_valid.write(false);
            }
        }
    }

    // Process to receive packets
    void process_receive() {
        // Always ready to receive
        in_ready.write(true);
        
        // If there's a valid packet coming in, receive it
        if (in_valid.read()) {
            // Read packet
            Packet packet = in_packet.read();
            packets_received_++;
            
            // Calculate latency
            int latency = packet.getLatency(current_time_);
            int hops = packet.getHops();
            
            // Record statistics
            total_latency_ += latency;
            total_hops_ += hops;
            
            // Print packet information
            std::cout << "Node " << id_ << " received " << packet 
                      << " at time " << current_time_
                      << " (Latency: " << latency 
                      << ", Hops: " << hops << ")" << std::endl;
        }
    }

    // Generate a new packet based on injection rate
    bool generate_packet() {
        // Don't generate packets after simulation end time
        if (current_time_ >= simulation_time_) {
            return false;
        }
        
        // Random chance based on injection rate (percentage)
        std::uniform_int_distribution<> dist(1, 100);
        return dist(rng_) <= packet_injection_rate_;
    }

    // Generate a random destination node ID that's not this node
    int generate_destination() {
        std::uniform_int_distribution<> dist(0, total_nodes_ - 1);
        int dest;
        do {
            dest = dist(rng_);
        } while (dest == id_);
        return dest;
    }

    // Get node statistics
    int getId() const { return id_; }
    int getPacketsSent() const { return packets_sent_; }
    int getPacketsReceived() const { return packets_received_; }
    
    // Get average latency and hops
    double getAverageLatency() const {
        return packets_received_ > 0 ? static_cast<double>(total_latency_) / packets_received_ : 0.0;
    }
    
    double getAverageHops() const {
        return packets_received_ > 0 ? static_cast<double>(total_hops_) / packets_received_ : 0.0;
    }

private:
    int id_;                     // Node ID
    int total_nodes_;            // Total number of nodes in the network
    int packet_injection_rate_;  // Percentage chance of packet generation per cycle
    int simulation_time_;        // Maximum simulation time
    int packets_sent_;           // Number of packets sent
    int packets_received_;       // Number of packets received
    int current_time_;           // Current simulation time
    int total_latency_ = 0;      // Sum of packet latencies
    int total_hops_ = 0;         // Sum of packet hops
    
    std::mt19937 rng_;           // Random number generator
};

#endif // NODE_H
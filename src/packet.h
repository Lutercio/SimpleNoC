#ifndef PACKET_H
#define PACKET_H

#include <systemc.h>
#include <string>
#include <iostream>

// Packet class definition for NoC communication
class Packet {
public:
    // Packet types
    enum PacketType {
        DATA,
        CONTROL
    };

    // Constructor
    Packet(int src_id = -1, int dst_id = -1, PacketType type = DATA, 
           int payload = 0, int timestamp = 0) : 
        src_id_(src_id),
        dst_id_(dst_id),
        type_(type),
        payload_(payload),
        timestamp_(timestamp),
        hops_(0),
        creation_time_(timestamp) {}

    // Copy constructor
    Packet(const Packet& other) :
        src_id_(other.src_id_),
        dst_id_(other.dst_id_),
        type_(other.type_),
        payload_(other.payload_),
        timestamp_(other.timestamp_),
        hops_(other.hops_),
        creation_time_(other.creation_time_),
        route_path_(other.route_path_) {}

    // Getters
    int getSrcId() const { return src_id_; }
    int getDstId() const { return dst_id_; }
    PacketType getType() const { return type_; }
    int getPayload() const { return payload_; }
    int getTimestamp() const { return timestamp_; }
    int getHops() const { return hops_; }
    int getCreationTime() const { return creation_time_; }
    std::string getRoutePath() const { return route_path_; }

    // Setters
    void setSrcId(int src_id) { src_id_ = src_id; }
    void setDstId(int dst_id) { dst_id_ = dst_id; }
    void setType(PacketType type) { type_ = type; }
    void setPayload(int payload) { payload_ = payload; }
    void setTimestamp(int timestamp) { timestamp_ = timestamp; }

    // Increment hop count when packet passes through a router
    void incrementHops() { hops_++; }

    // Add node ID to route path
    void addToPath(int node_id) {
        if (!route_path_.empty()) {
            route_path_ += " -> ";
        }
        route_path_ += std::to_string(node_id);
    }

    // Calculate latency
    int getLatency(int current_time) const {
        return current_time - creation_time_;
    }

    // Print packet information
    friend std::ostream& operator<<(std::ostream& os, const Packet& packet) {
        os << "Packet[" << packet.src_id_ << "->" << packet.dst_id_ 
           << ", Type: " << (packet.type_ == DATA ? "DATA" : "CONTROL") 
           << ", Payload: " << packet.payload_
           << ", Hops: " << packet.hops_
           << ", Path: " << packet.route_path_ << "]";
        return os;
    }

    // Operators for SystemC compatibility
    bool operator==(const Packet& other) const {
        return src_id_ == other.src_id_ && dst_id_ == other.dst_id_ && 
               payload_ == other.payload_ && timestamp_ == other.timestamp_;
    }

    bool operator!=(const Packet& other) const {
        return !(*this == other);
    }

    // Assignment operator
    Packet& operator=(const Packet& other) {
        if (this != &other) {
            src_id_ = other.src_id_;
            dst_id_ = other.dst_id_;
            type_ = other.type_;
            payload_ = other.payload_;
            timestamp_ = other.timestamp_;
            hops_ = other.hops_;
            creation_time_ = other.creation_time_;
            route_path_ = other.route_path_;
        }
        return *this;
    }

private:
    int src_id_;          // Source node ID
    int dst_id_;          // Destination node ID
    PacketType type_;     // Packet type (data or control)
    int payload_;         // Packet payload
    int timestamp_;       // Current timestamp
    int hops_;            // Number of hops traveled
    int creation_time_;   // Time when packet was created
    std::string route_path_; // String representing the path taken
};

// SystemC trace function for Packet class
inline void sc_trace(sc_core::sc_trace_file* tf, const Packet& packet, const std::string& name) {
    sc_trace(tf, packet.getSrcId(), name + "_src_id");
    sc_trace(tf, packet.getDstId(), name + "_dst_id");
    sc_trace(tf, packet.getPayload(), name + "_payload");
    sc_trace(tf, packet.getHops(), name + "_hops");
}

#endif // PACKET_H
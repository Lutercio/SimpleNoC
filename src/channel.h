#ifndef CHANNEL_H
#define CHANNEL_H

#include <systemc.h>
#include "packet.h"

// Channel class for NoC communication
class Channel : public sc_module {
public:
    // Input and output ports
    sc_in<bool> clk;
    sc_in<Packet> in_packet;    // Input packet
    sc_in<bool> in_valid;       // Input valid signal
    sc_out<bool> in_ready;      // Input ready signal
    
    sc_out<Packet> out_packet;  // Output packet
    sc_out<bool> out_valid;     // Output valid signal
    sc_in<bool> out_ready;      // Output ready signal

    // Channel state
    bool busy;
    Packet buffer;
    int delay;
    int delay_counter;

    // Constructor
    Channel(sc_module_name name, int transmission_delay = 1) : 
        sc_module(name),
        busy(false),
        delay(transmission_delay),
        delay_counter(0) {
        
        // Register processes
        SC_METHOD(process);
        sensitive << clk.pos();
    }

    // Process method
    void process() {
        // Always signal if we're ready to receive
        in_ready.write(!busy);
        
        // If not busy and there's a valid input, accept it
        if (!busy && in_valid.read()) {
            buffer = in_packet.read();
            busy = true;
            delay_counter = delay;
        }
        
        // If busy, count down delay
        if (busy) {
            if (delay_counter > 0) {
                delay_counter--;
            }
            
            // If delay is done and output is ready, send the packet
            if (delay_counter == 0 && out_ready.read()) {
                out_packet.write(buffer);
                out_valid.write(true);
                busy = false;
            }
        } else {
            out_valid.write(false);
        }
    }
};

#endif // CHANNEL_H
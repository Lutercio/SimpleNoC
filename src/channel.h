#ifndef CHANNEL_H
#define CHANNEL_H

#include <systemc.h>
#include "packet.h"

// Classe Channel para comunicação NoC
class Channel : public sc_module {
public:
    // Portas de entrada e saída
    sc_in<bool> clk;
    sc_in<Packet> in_packet;    // Pacote de entrada
    sc_in<bool> in_valid;       // Sinal de validade da entrada
    sc_out<bool> in_ready;      // Sinal de pronto da entrada
    
    sc_out<Packet> out_packet;  // Pacote de saída
    sc_out<bool> out_valid;     // Sinal de validade da saída
    sc_in<bool> out_ready;      // Sinal de pronto da saída

    // Estado do canal
    bool busy;
    Packet buffer;
    int delay;
    int delay_counter;

    // Construtor
    Channel(sc_module_name name, int transmission_delay = 1) : 
        sc_module(name),
        busy(false),
        delay(transmission_delay),
        delay_counter(0) {
        
        // Registrar processos
        SC_METHOD(process);
        sensitive << clk.pos();
    }

    // Método de processo
    void process() {
        // Sempre sinalizar se estamos prontos para receber
        in_ready.write(!busy);
        
        // Se não estamos ocupados e há uma entrada válida, aceitar
        if (!busy && in_valid.read()) {
            buffer = in_packet.read();
            busy = true;
            delay_counter = delay;
        }
        
        // Se ocupado, decrementar contador de atraso
        if (busy) {
            if (delay_counter > 0) {
                delay_counter--;
            }
            
            // Se o atraso terminou e a saída está pronta, enviar o pacote
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
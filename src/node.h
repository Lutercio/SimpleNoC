#ifndef NODE_H
#define NODE_H

#include <systemc.h>
#include <queue>
#include <vector>
#include <random>
#include "packet.h"

// Nó para Network-on-Chip
class Node : public sc_module {
public:
    // Portas
    sc_in<bool> clk;
    
    // Conexão da porta local para o roteador
    sc_out<Packet> out_packet;  // Saída para o roteador
    sc_out<bool> out_valid;     // Sinal de validade da saída
    sc_in<bool> out_ready;      // Sinal de pronto do roteador
    
    sc_in<Packet> in_packet;    // Entrada do roteador
    sc_in<bool> in_valid;       // Sinal de validade do roteador
    sc_out<bool> in_ready;      // Sinal de pronto da entrada

    // Construtor
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
        
        // Registrar processos
        SC_METHOD(process_send);
        sensitive << clk.pos();
        
        SC_METHOD(process_receive);
        sensitive << clk.pos();
    }

    // Processo para enviar pacotes
    void process_send() {
        // Atualizar tempo
        current_time_++;
        
        // Verificar se estamos prontos para enviar e o roteador pode receber
        if (out_ready.read()) {
            // Tentar enviar um novo pacote baseado na taxa de injeção
            if (generate_packet()) {
                // Criar um novo pacote
                int dest_id = generate_destination();
                int payload = rand() % 1000;
                Packet packet(id_, dest_id, Packet::DATA, payload, current_time_);
                
                // Enviar pacote
                out_packet.write(packet);
                out_valid.write(true);
                packets_sent_++;
                
                std::cout << "Node " << id_ << " sending packet to " << dest_id 
                          << " (payload: " << payload << ") at time " << current_time_ << std::endl;
            } else {
                // Nenhum pacote para enviar neste ciclo
                out_valid.write(false);
            }
        }
    }

    // Processo para receber pacotes
    void process_receive() {
        // Sempre pronto para receber
        in_ready.write(true);
        
        // Se há um pacote válido chegando, recebê-lo
        if (in_valid.read()) {
            // Ler pacote
            Packet packet = in_packet.read();
            packets_received_++;
            
            // Calcular latência
            int latency = packet.getLatency(current_time_);
            int hops = packet.getHops();
            
            // Registrar estatísticas
            total_latency_ += latency;
            total_hops_ += hops;
            
            // Imprimir informações do pacote
            std::cout << "Node " << id_ << " received " << packet 
                      << " at time " << current_time_
                      << " (Latency: " << latency 
                      << ", Hops: " << hops << ")" << std::endl;
        }
    }

    // Gerar um novo pacote baseado na taxa de injeção
    bool generate_packet() {
        // Não gerar pacotes após o tempo final da simulação
        if (current_time_ >= simulation_time_) {
            return false;
        }
        
        // Chance aleatória baseada na taxa de injeção (porcentagem)
        std::uniform_int_distribution<> dist(1, 100);
        return dist(rng_) <= packet_injection_rate_;
    }

    // Gerar um ID de nó de destino aleatório que não seja este nó
    int generate_destination() {
        std::uniform_int_distribution<> dist(0, total_nodes_ - 1);
        int dest;
        do {
            dest = dist(rng_);
        } while (dest == id_);
        return dest;
    }

    // Obter estatísticas do nó
    int getId() const { return id_; }
    int getPacketsSent() const { return packets_sent_; }
    int getPacketsReceived() const { return packets_received_; }
    
    // Obter latência média e saltos médios
    double getAverageLatency() const {
        return packets_received_ > 0 ? static_cast<double>(total_latency_) / packets_received_ : 0.0;
    }
    
    double getAverageHops() const {
        return packets_received_ > 0 ? static_cast<double>(total_hops_) / packets_received_ : 0.0;
    }

private:
    int id_;                     // ID do nó
    int total_nodes_;            // Número total de nós na rede
    int packet_injection_rate_;  // Porcentagem de chance de geração de pacote por ciclo
    int simulation_time_;        // Tempo máximo de simulação
    int packets_sent_;           // Número de pacotes enviados
    int packets_received_;       // Número de pacotes recebidos
    int current_time_;           // Tempo atual da simulação
    int total_latency_ = 0;      // Soma das latências dos pacotes
    int total_hops_ = 0;         // Soma dos saltos dos pacotes
    
    std::mt19937 rng_;           // Gerador de números aleatórios
};

#endif // NODE_H
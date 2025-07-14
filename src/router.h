#ifndef ROUTER_H
#define ROUTER_H

#include <systemc.h>
#include <vector>
#include <queue>
#include <memory>
#include "packet.h"
#include "routing_algorithms.h"

// Router para Network-on-Chip
class Router : public sc_module {
public:
    // Portas
    sc_in<bool> clk;
    
    // Portas de entrada dos canais (NORTE, LESTE, SUL, OESTE, LOCAL)
    sc_in<Packet> in_packets[5];
    sc_in<bool> in_valids[5];
    sc_out<bool> in_readys[5];
    
    // Portas de saída para os canais (NORTE, LESTE, SUL, OESTE, LOCAL)
    sc_out<Packet> out_packets[5];
    sc_out<bool> out_valids[5];
    sc_in<bool> out_readys[5];

    // Construtor
    Router(sc_module_name name, int x, int y, int mesh_size_x, int mesh_size_y, 
           RoutingAlgorithm* routing_algorithm) : 
        sc_module(name),
        x_(x),
        y_(y),
        mesh_size_x_(mesh_size_x),
        mesh_size_y_(mesh_size_y),
        routing_algorithm_(routing_algorithm) {
        
        // Inicializar buffers de entrada e status de saída
        input_buffers.resize(5);
        output_reserved.resize(5, false);
        
        // Registrar processos
        SC_METHOD(process_inputs);
        sensitive << clk.pos();
        
        SC_METHOD(process_routing);
        sensitive << clk.pos();
    }

    // Processo para tratar entradas dos canais
    void process_inputs() {
        // Processar cada porta de entrada
        for (int i = 0; i < 5; i++) {
            // Se há entrada válida e estamos prontos para receber
            if (in_valids[i].read() && input_buffers[i].size() < BUFFER_SIZE) {
                // Ler pacote da entrada
                Packet packet = in_packets[i].read();
                
                // Adicionar posição do router ao caminho
                packet.addToPath(y_ * mesh_size_x_ + x_);
                
                // Incrementar contador de saltos
                packet.incrementHops();
                
                // Adicionar ao buffer de entrada
                input_buffers[i].push(packet);
            }
            
            // Sinalizar se estamos prontos para receber mais pacotes
            in_readys[i].write(input_buffers[i].size() < BUFFER_SIZE);
        }
    }

    // Processo para tratar roteamento de pacotes
    void process_routing() {
        // Primeiro, liberar reservas se as saídas foram aceitas
        for (int i = 0; i < 5; i++) {
            if (output_reserved[i] && out_readys[i].read()) {
                output_reserved[i] = false; // LIBERAR A RESERVA!
            }
        }
        
        // Resetar sinais de validade de saída
        for (int i = 0; i < 5; i++) {
            if (!output_reserved[i]) {
                out_valids[i].write(false);
            }
        }
        
        // Processar pacotes nos buffers de entrada
        for (int input_port = 0; input_port < 5; input_port++) {
            if (!input_buffers[input_port].empty()) {
                // Obter pacote do buffer
                Packet packet = input_buffers[input_port].front();
                
                // Converter ID de destino para coordenadas x,y
                int dest_x = packet.getDstId() % mesh_size_x_;
                int dest_y = packet.getDstId() / mesh_size_x_;
                
                // Determinar porta de saída usando algoritmo de roteamento
                Direction output_dir = routing_algorithm_->calculateOutputPort(
                    x_, y_, dest_x, dest_y);
                
                int output_port = static_cast<int>(output_dir);
                
                // Se a saída está disponível e pronta para receber
                if (output_port != NONE && !output_reserved[output_port] && out_readys[output_port].read()) {
                    // Encaminhar o pacote
                    out_packets[output_port].write(packet);
                    out_valids[output_port].write(true);
                    output_reserved[output_port] = true;
                    
                    // Remover pacote do buffer de entrada
                    input_buffers[input_port].pop();
                }
            }
        }
    }

    // Obter coordenadas do router
    int getX() const { return x_; }
    int getY() const { return y_; }
    
    // Obter ID do nó
    int getNodeId() const { return y_ * mesh_size_x_ + x_; }
    
    // Obter nome do algoritmo de roteamento
    std::string getRoutingAlgorithmName() const {
        return routing_algorithm_->getName();
    }

private:
    int x_, y_;                          // Coordenadas do router na malha
    int mesh_size_x_, mesh_size_y_;      // Dimensões da malha
    std::unique_ptr<RoutingAlgorithm> routing_algorithm_;  // Algoritmo de roteamento
    
    static const int BUFFER_SIZE = 4;    // Tamanho do buffer de entrada
    std::vector<std::queue<Packet>> input_buffers;  // Buffers para cada porta de entrada
    std::vector<bool> output_reserved;   // Se as portas de saída estão em uso atualmente
};

#endif // ROUTER_H
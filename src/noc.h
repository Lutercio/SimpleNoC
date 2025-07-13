#ifndef NOC_H
#define NOC_H

#include <systemc.h>
#include <vector>
#include <memory>
#include <string>
#include "node.h"
#include "router.h"
#include "channel.h"
#include "routing_algorithms.h"

// Classe Network-on-Chip
class NoC : public sc_module {
public:
    // Construtor
    NoC(sc_module_name name, int mesh_size_x, int mesh_size_y, 
        const std::string& routing_algorithm_name,
        int packet_injection_rate = 10, int simulation_time = 1000) : 
        sc_module(name),
        mesh_size_x_(mesh_size_x),
        mesh_size_y_(mesh_size_y),
        packet_injection_rate_(packet_injection_rate),
        simulation_time_(simulation_time),
        routing_algorithm_name_(routing_algorithm_name) {
        
        // Criar clock
        clk = new sc_clock("clk", sc_time(1, SC_NS));
        
        // Criar routers e nós
        create_network();
        
        // Registrar processo
        SC_THREAD(run_simulation);
        sensitive << *clk;
    }
    
    // Destrutor
    ~NoC() {
        delete clk;
    }

    // Executar simulação
    void run_simulation() {
        std::cout << "Iniciando simulação NoC..." << std::endl;
        std::cout << "Tamanho da malha: " << mesh_size_x_ << "x" << mesh_size_y_ << std::endl;
        std::cout << "Algoritmo de roteamento: " << routing_algorithm_name_ << std::endl;
        std::cout << "Taxa de injeção de pacotes: " << packet_injection_rate_ << "%" << std::endl;
        std::cout << "Tempo de simulação: " << simulation_time_ << " ciclos" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
        
        // Aguardar simulação completar
        wait(simulation_time_ + 100, SC_NS);
        
        // Imprimir estatísticas
        print_statistics();
        
        // Terminar simulação
        sc_stop();
    }

    // Imprimir estatísticas da simulação
    void print_statistics() {
        std::cout << "\n-------- Simulation Statistics --------" << std::endl;
        std::cout << "Routing Algorithm: " << routing_algorithm_name_ << std::endl;
        
        int total_packets_sent = 0;
        int total_packets_received = 0;
        double total_latency = 0;
        double total_hops = 0;
        
        for (auto& node : nodes_) {
            int packets_sent = node->getPacketsSent();
            int packets_received = node->getPacketsReceived();
            double avg_latency = node->getAverageLatency();
            double avg_hops = node->getAverageHops();
            
            std::cout << "Node " << node->getId() << ":"
                      << " Sent=" << packets_sent
                      << ", Received=" << packets_received
                      << ", Avg Latency=" << avg_latency
                      << ", Avg Hops=" << avg_hops << std::endl;
                      
            total_packets_sent += packets_sent;
            total_packets_received += packets_received;
            total_latency += avg_latency * packets_received;
            total_hops += avg_hops * packets_received;
        }
        
        double network_avg_latency = 0;
        double network_avg_hops = 0;
        
        if (total_packets_received > 0) {
            network_avg_latency = total_latency / total_packets_received;
            network_avg_hops = total_hops / total_packets_received;
        }
        
        std::cout << "Network Summary:"
                  << " Total Sent=" << total_packets_sent
                  << ", Total Received=" << total_packets_received
                  << ", Avg Latency=" << network_avg_latency
                  << ", Avg Hops=" << network_avg_hops << std::endl;
    }

private:
    int mesh_size_x_, mesh_size_y_;           // Dimensões da malha
    int packet_injection_rate_;               // Taxa de injeção (porcentagem)
    int simulation_time_;                     // Tempo de simulação em ciclos
    std::string routing_algorithm_name_;      // Nome do algoritmo de roteamento
    
    sc_clock* clk;                            // Clock do sistema
    
    std::vector<std::unique_ptr<Node>> nodes_;               // Nós da rede
    std::vector<std::unique_ptr<Router>> routers_;           // Routers da rede
    std::vector<std::unique_ptr<Channel>> channels_;         // Canais da rede
    std::vector<std::unique_ptr<RoutingAlgorithm>> routing_algorithms_;  // Algoritmos de roteamento
    
    // Sinais para conectar componentes
    std::vector<std::unique_ptr<sc_signal<Packet>>> packet_signals_;
    std::vector<std::unique_ptr<sc_signal<bool>>> valid_signals_;
    std::vector<std::unique_ptr<sc_signal<bool>>> ready_signals_;
    
    // Criar a topologia da rede
    void create_network() {
        // Criar nós e routers
        for (int y = 0; y < mesh_size_y_; y++) {
            for (int x = 0; x < mesh_size_x_; x++) {
                int node_id = y * mesh_size_x_ + x;
                
                // Criar algoritmo de roteamento para este router
                RoutingAlgorithm* routing_algorithm = createRoutingAlgorithm(routing_algorithm_name_);
                routing_algorithms_.push_back(std::unique_ptr<RoutingAlgorithm>(routing_algorithm));
                
                // Criar router
                std::string router_name = "router_" + std::to_string(node_id);
                Router* router = new Router(router_name.c_str(), x, y, mesh_size_x_, mesh_size_y_, routing_algorithm);
                router->clk(*clk);
                routers_.push_back(std::unique_ptr<Router>(router));
                
                // Criar nó
                std::string node_name = "node_" + std::to_string(node_id);
                Node* node = new Node(node_name.c_str(), node_id, mesh_size_x_ * mesh_size_y_, 
                                     packet_injection_rate_, simulation_time_);
                node->clk(*clk);
                nodes_.push_back(std::unique_ptr<Node>(node));
            }
        }
        
        // Criar canais e conectar componentes
        for (int y = 0; y < mesh_size_y_; y++) {
            for (int x = 0; x < mesh_size_x_; x++) {
                int node_id = y * mesh_size_x_ + x;
                Router* router = routers_[node_id].get();
                Node* node = nodes_[node_id].get();
                
                // Conectar nó à porta local do router usando sinais
                // Criar sinais para comunicação nó-router
                auto node_to_router_packet = std::make_unique<sc_signal<Packet>>(("sig_node_to_router_packet_" + std::to_string(node_id)).c_str());
                auto node_to_router_valid = std::make_unique<sc_signal<bool>>(("sig_node_to_router_valid_" + std::to_string(node_id)).c_str());
                auto node_to_router_ready = std::make_unique<sc_signal<bool>>(("sig_node_to_router_ready_" + std::to_string(node_id)).c_str());
                
                auto router_to_node_packet = std::make_unique<sc_signal<Packet>>(("sig_router_to_node_packet_" + std::to_string(node_id)).c_str());
                auto router_to_node_valid = std::make_unique<sc_signal<bool>>(("sig_router_to_node_valid_" + std::to_string(node_id)).c_str());
                auto router_to_node_ready = std::make_unique<sc_signal<bool>>(("sig_router_to_node_ready_" + std::to_string(node_id)).c_str());
                
                // Conectar saídas do nó aos sinais
                node->out_packet.bind(*node_to_router_packet);
                node->out_valid.bind(*node_to_router_valid);
                node->out_ready.bind(*node_to_router_ready);
                
                // Conectar entradas LOCAL do router aos sinais
                router->in_packets[LOCAL].bind(*node_to_router_packet);
                router->in_valids[LOCAL].bind(*node_to_router_valid);
                router->in_readys[LOCAL].bind(*node_to_router_ready);
                
                // Conectar saídas LOCAL do router aos sinais
                router->out_packets[LOCAL].bind(*router_to_node_packet);
                router->out_valids[LOCAL].bind(*router_to_node_valid);
                router->out_readys[LOCAL].bind(*router_to_node_ready);
                
                // Conectar entradas do nó aos sinais
                node->in_packet.bind(*router_to_node_packet);
                node->in_valid.bind(*router_to_node_valid);
                node->in_ready.bind(*router_to_node_ready);
                
                // Armazenar sinais
                packet_signals_.push_back(std::move(node_to_router_packet));
                valid_signals_.push_back(std::move(node_to_router_valid));
                ready_signals_.push_back(std::move(node_to_router_ready));
                packet_signals_.push_back(std::move(router_to_node_packet));
                valid_signals_.push_back(std::move(router_to_node_valid));
                ready_signals_.push_back(std::move(router_to_node_ready));
                
                // Conectar router aos routers vizinhos
                connect_routers(x, y);
                
                // Conectar portas não utilizadas a sinais dummy
                connect_unused_ports(router, x, y);
            }
        }
    }
    
    // Conectar um router aos seus vizinhos
    void connect_routers(int x, int y) {
        int router_id = y * mesh_size_x_ + x;
        Router* router = routers_[router_id].get();
        
        // Conectar ao vizinho NORTE
        if (y > 0) {
            int north_id = (y - 1) * mesh_size_x_ + x;
            Router* north_router = routers_[north_id].get();
            
            // Criar sinais para comunicação router-para-norte
            auto router_to_north_packet = std::make_unique<sc_signal<Packet>>(("sig_r" + std::to_string(router_id) + "_to_n" + std::to_string(north_id) + "_packet").c_str());
            auto router_to_north_valid = std::make_unique<sc_signal<bool>>(("sig_r" + std::to_string(router_id) + "_to_n" + std::to_string(north_id) + "_valid").c_str());
            auto router_to_north_ready = std::make_unique<sc_signal<bool>>(("sig_r" + std::to_string(router_id) + "_to_n" + std::to_string(north_id) + "_ready").c_str());
            
            auto north_to_router_packet = std::make_unique<sc_signal<Packet>>(("sig_n" + std::to_string(north_id) + "_to_r" + std::to_string(router_id) + "_packet").c_str());
            auto north_to_router_valid = std::make_unique<sc_signal<bool>>(("sig_n" + std::to_string(north_id) + "_to_r" + std::to_string(router_id) + "_valid").c_str());
            auto north_to_router_ready = std::make_unique<sc_signal<bool>>(("sig_n" + std::to_string(north_id) + "_to_r" + std::to_string(router_id) + "_ready").c_str());
            
            // Conectar saídas NORTE do router às entradas SUL do router norte
            router->out_packets[NORTH].bind(*router_to_north_packet);
            router->out_valids[NORTH].bind(*router_to_north_valid);
            router->out_readys[NORTH].bind(*router_to_north_ready);
            
            north_router->in_packets[SOUTH].bind(*router_to_north_packet);
            north_router->in_valids[SOUTH].bind(*router_to_north_valid);
            north_router->in_readys[SOUTH].bind(*router_to_north_ready);
            
            // Conectar saídas SUL do router norte às entradas NORTE do router
            north_router->out_packets[SOUTH].bind(*north_to_router_packet);
            north_router->out_valids[SOUTH].bind(*north_to_router_valid);
            north_router->out_readys[SOUTH].bind(*north_to_router_ready);
            
            router->in_packets[NORTH].bind(*north_to_router_packet);
            router->in_valids[NORTH].bind(*north_to_router_valid);
            router->in_readys[NORTH].bind(*north_to_router_ready);
            
            // Armazenar sinais
            packet_signals_.push_back(std::move(router_to_north_packet));
            valid_signals_.push_back(std::move(router_to_north_valid));
            ready_signals_.push_back(std::move(router_to_north_ready));
            packet_signals_.push_back(std::move(north_to_router_packet));
            valid_signals_.push_back(std::move(north_to_router_valid));
            ready_signals_.push_back(std::move(north_to_router_ready));
        }
        
        // Conectar ao vizinho LESTE
        if (x < mesh_size_x_ - 1) {
            int east_id = y * mesh_size_x_ + (x + 1);
            Router* east_router = routers_[east_id].get();
            
            // Criar sinais para comunicação router-para-leste
            auto router_to_east_packet = std::make_unique<sc_signal<Packet>>(("sig_r" + std::to_string(router_id) + "_to_e" + std::to_string(east_id) + "_packet").c_str());
            auto router_to_east_valid = std::make_unique<sc_signal<bool>>(("sig_r" + std::to_string(router_id) + "_to_e" + std::to_string(east_id) + "_valid").c_str());
            auto router_to_east_ready = std::make_unique<sc_signal<bool>>(("sig_r" + std::to_string(router_id) + "_to_e" + std::to_string(east_id) + "_ready").c_str());
            
            auto east_to_router_packet = std::make_unique<sc_signal<Packet>>(("sig_e" + std::to_string(east_id) + "_to_r" + std::to_string(router_id) + "_packet").c_str());
            auto east_to_router_valid = std::make_unique<sc_signal<bool>>(("sig_e" + std::to_string(east_id) + "_to_r" + std::to_string(router_id) + "_valid").c_str());
            auto east_to_router_ready = std::make_unique<sc_signal<bool>>(("sig_e" + std::to_string(east_id) + "_to_r" + std::to_string(router_id) + "_ready").c_str());
            
            // Conectar saídas LESTE do router às entradas OESTE do router leste
            router->out_packets[EAST].bind(*router_to_east_packet);
            router->out_valids[EAST].bind(*router_to_east_valid);
            router->out_readys[EAST].bind(*router_to_east_ready);
            
            east_router->in_packets[WEST].bind(*router_to_east_packet);
            east_router->in_valids[WEST].bind(*router_to_east_valid);
            east_router->in_readys[WEST].bind(*router_to_east_ready);
            
            // Conectar saídas OESTE do router leste às entradas LESTE do router
            east_router->out_packets[WEST].bind(*east_to_router_packet);
            east_router->out_valids[WEST].bind(*east_to_router_valid);
            east_router->out_readys[WEST].bind(*east_to_router_ready);
            
            router->in_packets[EAST].bind(*east_to_router_packet);
            router->in_valids[EAST].bind(*east_to_router_valid);
            router->in_readys[EAST].bind(*east_to_router_ready);
            
            // Armazenar sinais
            packet_signals_.push_back(std::move(router_to_east_packet));
            valid_signals_.push_back(std::move(router_to_east_valid));
            ready_signals_.push_back(std::move(router_to_east_ready));
            packet_signals_.push_back(std::move(east_to_router_packet));
            valid_signals_.push_back(std::move(east_to_router_valid));
            ready_signals_.push_back(std::move(east_to_router_ready));
        }
    }
    
    // Conectar portas não utilizadas a sinais dummy
    void connect_unused_ports(Router* router, int x, int y) {
        int router_id = y * mesh_size_x_ + x;
        
        // Conectar portas NORTE não utilizadas
        if (y == 0) {
            auto dummy_north_in_packet = std::make_unique<sc_signal<Packet>>(("dummy_north_in_packet_" + std::to_string(router_id)).c_str());
            auto dummy_north_in_valid = std::make_unique<sc_signal<bool>>(("dummy_north_in_valid_" + std::to_string(router_id)).c_str());
            auto dummy_north_in_ready = std::make_unique<sc_signal<bool>>(("dummy_north_in_ready_" + std::to_string(router_id)).c_str());
            auto dummy_north_out_packet = std::make_unique<sc_signal<Packet>>(("dummy_north_out_packet_" + std::to_string(router_id)).c_str());
            auto dummy_north_out_valid = std::make_unique<sc_signal<bool>>(("dummy_north_out_valid_" + std::to_string(router_id)).c_str());
            auto dummy_north_out_ready = std::make_unique<sc_signal<bool>>(("dummy_north_out_ready_" + std::to_string(router_id)).c_str());
            
            router->in_packets[NORTH].bind(*dummy_north_in_packet);
            router->in_valids[NORTH].bind(*dummy_north_in_valid);
            router->in_readys[NORTH].bind(*dummy_north_in_ready);
            router->out_packets[NORTH].bind(*dummy_north_out_packet);
            router->out_valids[NORTH].bind(*dummy_north_out_valid);
            router->out_readys[NORTH].bind(*dummy_north_out_ready);
            
            packet_signals_.push_back(std::move(dummy_north_in_packet));
            valid_signals_.push_back(std::move(dummy_north_in_valid));
            ready_signals_.push_back(std::move(dummy_north_in_ready));
            packet_signals_.push_back(std::move(dummy_north_out_packet));
            valid_signals_.push_back(std::move(dummy_north_out_valid));
            ready_signals_.push_back(std::move(dummy_north_out_ready));
        }
        
        // Conectar portas SUL não utilizadas
        if (y == mesh_size_y_ - 1) {
            auto dummy_south_in_packet = std::make_unique<sc_signal<Packet>>(("dummy_south_in_packet_" + std::to_string(router_id)).c_str());
            auto dummy_south_in_valid = std::make_unique<sc_signal<bool>>(("dummy_south_in_valid_" + std::to_string(router_id)).c_str());
            auto dummy_south_in_ready = std::make_unique<sc_signal<bool>>(("dummy_south_in_ready_" + std::to_string(router_id)).c_str());
            auto dummy_south_out_packet = std::make_unique<sc_signal<Packet>>(("dummy_south_out_packet_" + std::to_string(router_id)).c_str());
            auto dummy_south_out_valid = std::make_unique<sc_signal<bool>>(("dummy_south_out_valid_" + std::to_string(router_id)).c_str());
            auto dummy_south_out_ready = std::make_unique<sc_signal<bool>>(("dummy_south_out_ready_" + std::to_string(router_id)).c_str());
            
            router->in_packets[SOUTH].bind(*dummy_south_in_packet);
            router->in_valids[SOUTH].bind(*dummy_south_in_valid);
            router->in_readys[SOUTH].bind(*dummy_south_in_ready);
            router->out_packets[SOUTH].bind(*dummy_south_out_packet);
            router->out_valids[SOUTH].bind(*dummy_south_out_valid);
            router->out_readys[SOUTH].bind(*dummy_south_out_ready);
            
            packet_signals_.push_back(std::move(dummy_south_in_packet));
            valid_signals_.push_back(std::move(dummy_south_in_valid));
            ready_signals_.push_back(std::move(dummy_south_in_ready));
            packet_signals_.push_back(std::move(dummy_south_out_packet));
            valid_signals_.push_back(std::move(dummy_south_out_valid));
            ready_signals_.push_back(std::move(dummy_south_out_ready));
        }
        
        // Conectar portas OESTE não utilizadas
        if (x == 0) {
            auto dummy_west_in_packet = std::make_unique<sc_signal<Packet>>(("dummy_west_in_packet_" + std::to_string(router_id)).c_str());
            auto dummy_west_in_valid = std::make_unique<sc_signal<bool>>(("dummy_west_in_valid_" + std::to_string(router_id)).c_str());
            auto dummy_west_in_ready = std::make_unique<sc_signal<bool>>(("dummy_west_in_ready_" + std::to_string(router_id)).c_str());
            auto dummy_west_out_packet = std::make_unique<sc_signal<Packet>>(("dummy_west_out_packet_" + std::to_string(router_id)).c_str());
            auto dummy_west_out_valid = std::make_unique<sc_signal<bool>>(("dummy_west_out_valid_" + std::to_string(router_id)).c_str());
            auto dummy_west_out_ready = std::make_unique<sc_signal<bool>>(("dummy_west_out_ready_" + std::to_string(router_id)).c_str());
            
            router->in_packets[WEST].bind(*dummy_west_in_packet);
            router->in_valids[WEST].bind(*dummy_west_in_valid);
            router->in_readys[WEST].bind(*dummy_west_in_ready);
            router->out_packets[WEST].bind(*dummy_west_out_packet);
            router->out_valids[WEST].bind(*dummy_west_out_valid);
            router->out_readys[WEST].bind(*dummy_west_out_ready);
            
            packet_signals_.push_back(std::move(dummy_west_in_packet));
            valid_signals_.push_back(std::move(dummy_west_in_valid));
            ready_signals_.push_back(std::move(dummy_west_in_ready));
            packet_signals_.push_back(std::move(dummy_west_out_packet));
            valid_signals_.push_back(std::move(dummy_west_out_valid));
            ready_signals_.push_back(std::move(dummy_west_out_ready));
        }
        
        // Conectar portas LESTE não utilizadas
        if (x == mesh_size_x_ - 1) {
            auto dummy_east_in_packet = std::make_unique<sc_signal<Packet>>(("dummy_east_in_packet_" + std::to_string(router_id)).c_str());
            auto dummy_east_in_valid = std::make_unique<sc_signal<bool>>(("dummy_east_in_valid_" + std::to_string(router_id)).c_str());
            auto dummy_east_in_ready = std::make_unique<sc_signal<bool>>(("dummy_east_in_ready_" + std::to_string(router_id)).c_str());
            auto dummy_east_out_packet = std::make_unique<sc_signal<Packet>>(("dummy_east_out_packet_" + std::to_string(router_id)).c_str());
            auto dummy_east_out_valid = std::make_unique<sc_signal<bool>>(("dummy_east_out_valid_" + std::to_string(router_id)).c_str());
            auto dummy_east_out_ready = std::make_unique<sc_signal<bool>>(("dummy_east_out_ready_" + std::to_string(router_id)).c_str());
            
            router->in_packets[EAST].bind(*dummy_east_in_packet);
            router->in_valids[EAST].bind(*dummy_east_in_valid);
            router->in_readys[EAST].bind(*dummy_east_in_ready);
            router->out_packets[EAST].bind(*dummy_east_out_packet);
            router->out_valids[EAST].bind(*dummy_east_out_valid);
            router->out_readys[EAST].bind(*dummy_east_out_ready);
            
            packet_signals_.push_back(std::move(dummy_east_in_packet));
            valid_signals_.push_back(std::move(dummy_east_in_valid));
            ready_signals_.push_back(std::move(dummy_east_in_ready));
            packet_signals_.push_back(std::move(dummy_east_out_packet));
            valid_signals_.push_back(std::move(dummy_east_out_valid));
            ready_signals_.push_back(std::move(dummy_east_out_ready));
        }
    }
};

#endif // NOC_H
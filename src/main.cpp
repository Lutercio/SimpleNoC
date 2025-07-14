#include <systemc.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "noc.h"
#include "routing_algorithms.h"

// Função para executar uma simulação com parâmetros especificados
void run_simulation(int mesh_size, const std::string& routing_algorithm, 
                   int packet_injection_rate, int simulation_time) {
    // Criar NoC
    NoC noc("network_on_chip", mesh_size, mesh_size, 
            routing_algorithm, packet_injection_rate, simulation_time);
    
    // Iniciar simulação e aguardar completar
    sc_start();
}

int main(int argc, char* argv[]) {
    // Inicializar gerador de números aleatórios
    std::srand(std::time(nullptr));
    
    // Parâmetros padrão
    int mesh_size = 4;  // malha 4x4 por padrão
    std::string routing_algorithm = "XY";  // roteamento XY por padrão
    int packet_injection_rate = 10;  // taxa de injeção 10% por padrão
    int simulation_time = 1000;  // 1000 ciclos por padrão
    
    // Analisar argumentos da linha de comando
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
            std::cout << "Uso: " << argv[0] << " [opções]" << std::endl;
            std::cout << "Opções:" << std::endl;
            std::cout << "  -size TAMANHO     Define o tamanho da malha (padrão: 4)" << std::endl;
            std::cout << "  -routing ALGO     Define o algoritmo de roteamento: XY, WEST_FIRST (padrão: XY)" << std::endl;
            std::cout << "  -rate TAXA        Define a taxa de injeção de pacotes em porcentagem (padrão: 10)" << std::endl;
            std::cout << "  -time TEMPO       Define o tempo de simulação em ciclos (padrão: 1000)" << std::endl;
            std::cout << "  -help, -h         Mostra esta mensagem de ajuda" << std::endl;
            return 0;
        }
    }
    
    // Validar parâmetros
    if (mesh_size < 2 || mesh_size > 16) {
        std::cout << "Erro: Tamanho da malha deve estar entre 2 e 16" << std::endl;
        return 1;
    }
    
    if (routing_algorithm != "XY" && routing_algorithm != "WEST_FIRST") {
        std::cout << "Erro: Algoritmo de roteamento desconhecido. Suportados: XY, WEST_FIRST" << std::endl;
        return 1;
    }
    
    if (packet_injection_rate < 1 || packet_injection_rate > 100) {
        std::cout << "Erro: Taxa de injeção de pacotes deve estar entre 1 e 100" << std::endl;
        return 1;
    }
    
    if (simulation_time < 100) {
        std::cout << "Erro: Tempo de simulação deve ser pelo menos 100 ciclos" << std::endl;
        return 1;
    }
    
    // Executar simulação com o algoritmo especificado
    std::cout << "Executando simulação com algoritmo de roteamento " << routing_algorithm << "..." << std::endl;
    run_simulation(mesh_size, routing_algorithm, packet_injection_rate, simulation_time);
    
    return 0;
}
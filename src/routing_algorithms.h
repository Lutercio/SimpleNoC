#ifndef ROUTING_ALGORITHMS_H
#define ROUTING_ALGORITHMS_H

#include <systemc.h>
#include <vector>
#include <utility>
#include <cstdlib>
#include <climits>

// Enumeração de direções para topologia de malha
enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    LOCAL = 4,
    NONE = -1
};

// Classe base para algoritmos de roteamento
class RoutingAlgorithm {
public:
    virtual ~RoutingAlgorithm() {}
    
    // Calcular porta de saída baseada na posição atual e destino
    virtual Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const = 0;
    
    // Obter nome do algoritmo
    virtual std::string getName() const = 0;
};

// Algoritmo de Roteamento XY (Roteamento Ordenado por Dimensão)
// Primeiro roteia na dimensão X, depois na dimensão Y
class XYRoutingAlgorithm : public RoutingAlgorithm {
public:
    Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const override {
        
        // Primeiro verificar se estamos no destino
        if (current_x == dest_x && current_y == dest_y) {
            return LOCAL;
        }
        
        // Primeiro mover na direção X até combinar com o X do destino
        if (current_x < dest_x) {
            return EAST;
        } else if (current_x > dest_x) {
            return WEST;
        }
        
        // Então mover na direção Y
        if (current_y < dest_y) {
            return SOUTH;
        } else if (current_y > dest_y) {
            return NORTH;
        }
        
        return NONE; // Nunca deveria acontecer
    }
    
    std::string getName() const override {
        return "XY Routing";
    }
};

// Algoritmo de Roteamento Adaptativo West-First
// Pacotes só podem virar para oeste no primeiro salto, depois devem seguir minimamente
class WestFirstRoutingAlgorithm : public RoutingAlgorithm {
public:
    Direction calculateOutputPort(
        int current_x, int current_y,
        int dest_x, int dest_y) const override {
        
        // Primeiro verificar se estamos no destino
        if (current_x == dest_x && current_y == dest_y) {
            return LOCAL;
        }
        
        // Um vetor de direções possíveis
        std::vector<Direction> possible_dirs;
        
        // Se o destino está a oeste, devemos ir para oeste primeiro
        if (current_x > dest_x) {
            return WEST;
        }
        
        // Caso contrário, podemos escolher adaptativamente entre as direções restantes
        if (current_x < dest_x) {
            possible_dirs.push_back(EAST);
        }
        if (current_y < dest_y) {
            possible_dirs.push_back(SOUTH);
        }
        if (current_y > dest_y) {
            possible_dirs.push_back(NORTH);
        }
        
        // Se há direções possíveis, escolher uma (aleatoriamente neste caso)
        if (!possible_dirs.empty()) {
            // Em uma implementação real, escolheríamos baseado no congestionamento
            // Para simulação, vamos apenas escolher a primeira opção
            return possible_dirs[0];
        }
        
        return NONE; // Nunca deveria acontecer
    }
    
    std::string getName() const override {
        return "West-First Routing";
    }
};

// Função de fábrica para criar algoritmos de roteamento
inline RoutingAlgorithm* createRoutingAlgorithm(const std::string& algorithm_name) {
    if (algorithm_name == "XY") {
        return new XYRoutingAlgorithm();
    } else if (algorithm_name == "WEST_FIRST") {
        return new WestFirstRoutingAlgorithm();
    }
    
    // Padrão para roteamento XY
    std::cout << "Aviso: Algoritmo de roteamento desconhecido. Usando roteamento XY." << std::endl;
    return new XYRoutingAlgorithm();
}

#endif // ROUTING_ALGORITHMS_H
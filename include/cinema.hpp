#pragma once

#include <string>
#include <vector>


struct Cinema{
private:
    std::string id;
    std::string nome_do_cinema;
    int x;
    int y;
    double preco_ingresso;
    std::vector<std::string> filmes_em_exibicao;

public:
    Cinema (std::string id, std::string nome_do_cinema, int x, int y,
        double preco_ingresso, std::vector<std::string> filmes_em_exibicao){
        this->id = id;
        this->nome_do_cinema = nome_do_cinema;
        this->x = x;
        this->y = y;
        this->preco_ingresso = preco_ingresso;
        this->filmes_em_exibicao = filmes_em_exibicao;
    }
};

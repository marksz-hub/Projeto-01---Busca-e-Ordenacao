#pragma once

#include <string>
#include <vector>

using namespace std;


struct Cinemas{
private:
    string id;
    string nome_do_cinema;
    int x;
    int y;
    double preco_ingresso;
    vector <string> filmes_em_exibicao;

public:
    Cinemas (string id, string nome_do_cinema, int x, int y,
        double preco_ingresso, vector <string> filmes_em_exibicao){
        this->id = id;
        this->nome_do_cinema = nome_do_cinema;
        this->x = x;
        this->y = y;
        this->preco_ingresso = preco_ingresso;
        this->filmes_em_exibicao = filmes_em_exibicao;
    }
}

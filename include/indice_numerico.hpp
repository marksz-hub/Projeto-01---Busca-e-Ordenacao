/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Arvore para indices numericos
 *
 * INICIO:
 *   2026-14-10
 */
	 
#pragma once

#include "filme.hpp"
#include <vector>

struct NoIndice {
		unsigned valor;
		std::vector<unsigned> filmes;
		NoIndice *esq;
		NoIndice *dir;

		NoIndice(unsigned valor, unsigned indiceFilme) {
			this->valor = valor;
			filmes.push_back(indiceFilme);
			esq = nullptr;
			dir = nullptr;
		}
};

inline void inserir(NoIndice *&raiz, unsigned valor, unsigned indiceFilme) {
	if(raiz == nullptr) {
		raiz = new NoIndice(valor, indiceFilme);
		return;
	}

	if(valor < raiz->valor) {
		return inserir(raiz->esq, valor, indiceFilme);
	}

	else if(valor > raiz->valor) {
		return inserir(raiz->dir, valor, indiceFilme);
	}

	else {
		return raiz->filmes.push_back(indiceFilme);
	}
}


inline void buscarIntervalo(NoIndice *raiz, unsigned minimo, unsigned maximo, std::vector<unsigned> &resultado) {
    if (raiz == nullptr) {
        return;
    }

    if (raiz->valor > minimo) {
        buscarIntervalo(raiz->esq, minimo, maximo, resultado);
    }

    if (raiz->valor >= minimo && raiz->valor <= maximo) {
        for (unsigned i = 0; i < raiz->filmes.size(); i++) {
            resultado.push_back(raiz->filmes[i]);
        }
    }

    if (raiz->valor < maximo) {
        buscarIntervalo(raiz->dir, minimo, maximo, resultado);
    }
}


inline void destruirArvore(NoIndice *&raiz) {
    if (raiz == nullptr) {
        return;
    }

    destruirArvore(raiz->esq);
    destruirArvore(raiz->dir);

    delete raiz;
    raiz = nullptr;
}


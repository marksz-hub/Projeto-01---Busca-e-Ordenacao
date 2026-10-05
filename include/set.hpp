/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Estrutura de dados: Conjunto
 *
 * INICIO:
 *   2026-09-15
 */

#pragma once

#include <algorithm>
#include <vector>

struct Conjunto {
private:
	unsigned k;

public:
	bool ordenado;
	std::vector<unsigned> v;

	Conjunto()
		: k { 0 }
		, ordenado { false } { }

	Conjunto(unsigned n)
		: k { 0 }
		, ordenado { false }
		, v(n, 0) { }

	unsigned tamanho(void) const {
		return this->v.size();
	}

	void add(unsigned v) {
		if( this->tamanho() > 1 && this->v[this->tamanho() - 2] == v ) {
			return;
		}

		this->v.push_back(v);
		this->k = std::max(v, this->k);
		this->ordenado = false;
	}

	unsigned get(unsigned i) {
		return this->v[i];
	}

	void ordenar(void) {
		if( this->ordenado ) {
			return;
		}

		std::vector<unsigned> contagem(k + 1, 0);
		std::vector<unsigned> ordenado(v.size());
		unsigned i, j;

		for( i = 0; i < v.size(); ++i ) {
			j = v[i];
			contagem[j] = contagem[j] + 1;
		}

		for( i = 1; i <= k; ++i ) {
			contagem[i] = contagem[i] + contagem[i - 1];
		}

		for( i = v.size(); i > 0; --i ) {
			j = v[i - 1];
			contagem[j] = contagem[j] - 1;
			ordenado[contagem[j]] = v[i - 1];
		}

		v = ordenado;
	}

	/* Obtém a intersecção de dois conjuntos
	 * C = A & B
	 */
	Conjunto interseccao(Conjunto &lhs, Conjunto &rhs) const {
		Conjunto novo;
		unsigned i = 0, j = 0;

		lhs.ordenar();
		rhs.ordenar();

		while( i < lhs.tamanho() && j < rhs.tamanho() ) {
			const unsigned x = lhs.get(i);
			const unsigned y = rhs.get(j);
			if( x == y ) {
				novo.add(x);
			} else if( x < y ) {
				++i;
			} else {
				++j;
			}
		}

		novo.ordenado = true;
		return novo;
	}

	/* Obtém a união entre dois conjuntos
	 * C = A | B
	 */
	Conjunto uniao(Conjunto &lhs, Conjunto &rhs) const {
		Conjunto novo;
		unsigned i = 0, j = 0;

		lhs.ordenar();
		rhs.ordenar();

		while( i < lhs.tamanho() && j < rhs.tamanho() ) {
			const unsigned x = lhs.get(i);
			const unsigned y = rhs.get(j);

			if( x == y ) {
				++i;
				++j;

				novo.add(x);
			} else if( x < y ) {
				++i;
				novo.add(x);
			} else {
				++j;
			}
		}

		novo.ordenado = true;
		return novo;
	}

	Conjunto operator&(Conjunto &rhs) {
		return this->interseccao(*this, rhs);
	}

	Conjunto operator|(Conjunto &rhs) {
		return this->uniao(*this, rhs);
	}
};

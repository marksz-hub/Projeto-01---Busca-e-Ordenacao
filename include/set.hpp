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
#include <cstdint>

struct Conjunto {
private:
	std::vector<uint64_t> v;

public:
	Conjunto(unsigned n)
		: v((n + 64 - 1) / 64) { }

	unsigned tamanho(void) const {
		return this->v.size();
	}

	unsigned getChunk(unsigned i) const {
		return this->v[i];
	}

	void setChunk(unsigned i, uint64_t valor) {
		this->v[i] = valor;
	}

	/* Obtém a união entre dois conjuntos
	 * C = A & B
	 */
	Conjunto uniao(const Conjunto &lhs, const Conjunto &rhs) const {
		const unsigned tamanho = std::min(lhs.tamanho(), rhs.tamanho());
		Conjunto novo { tamanho };

		for( unsigned i = 0; i < tamanho; ++i ) {
			novo.setChunk(i, lhs.getChunk(i) & rhs.getChunk(i));
		}

		return novo;
	}

	/* Obtém a intersecção entre dois conjuntos
	 * C = A | B
	 */
	Conjunto interseccao(const Conjunto &lhs, const Conjunto &rhs) const {
		const unsigned tamanho = std::min(lhs.tamanho(), rhs.tamanho());
		Conjunto novo { tamanho };

		for( unsigned i = 0; i < tamanho; ++i ) {
			novo.setChunk(i, lhs.getChunk(i) | rhs.getChunk(i));
		}

		return novo;
	}

	/* Obtém a diferença entre dois conjuntos
	 * C = A - B
	 */
	Conjunto diferenca(const Conjunto &lhs, const Conjunto &rhs) const {
		const unsigned tamanho = std::min(lhs.tamanho(), rhs.tamanho());
		Conjunto novo { tamanho };

		for( unsigned i = 0; i < tamanho; ++i ) {
			novo.setChunk(i, lhs.getChunk(i) - rhs.getChunk(i));
		}

		return novo;
	}

	Conjunto operator&(const Conjunto &rhs) const {
		return this->uniao(*this, rhs);
	}

	Conjunto operator|(const Conjunto &rhs) const {
		return this->interseccao(*this, rhs);
	}

	Conjunto operator-(const Conjunto &rhs) const {
		return this->diferenca(*this, rhs);
	}
};

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
#include <iostream>
#include <vector>
#include <cstdint>

struct Conjunto {
private:
	const unsigned n;
	std::vector<uint64_t> v;

public:
	Conjunto(unsigned n)
		: n { n }
		, v((n + 64 - 1) / 64, 0) { }

	unsigned qtd(void) const {
		return n;
	}

	unsigned tamanho(void) const {
		return this->v.size();
	}

	unsigned getChunk(const unsigned i) const {
		return this->v[i];
	}

	void setChunk(const unsigned i, const uint64_t v) {
		this->v[i] = v;
	}

	uint64_t getBit(const uint64_t i) const {
		const uint64_t shift = i % 64;
		return this->v[i >> 6] & (1UL << shift);
	}

	void setBit(uint64_t i) {
		const uint64_t shift = i % 64;
		this->v[i >> 6] |= (1UL << shift);
	}

	void clearBit(uint64_t i) {
		const uint64_t shift = i % 64;
		this->v[i >> 6] &= ~(1UL << shift);
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

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
	const unsigned n;
	unsigned bitsSetados;
	std::vector<uint64_t> v;

public:
	Conjunto(unsigned n)
		: n { n }
		, bitsSetados { 0 }
		, v((n + 64 - 1) / 64, 0) { }

	unsigned qtd(void) const {
		return n;
	}

	unsigned tamanho(void) const {
		return this->v.size();
	}

	unsigned setados() const {
		return this->bitsSetados;
	}

	uint64_t getChunk(const uint64_t i) const {
		return this->v[i];
	}

	void setChunk(const uint64_t i, const uint64_t v) {
		this->v[i] = v;

		uint64_t n = v;
		while( n ) {
			n &= (n - 1);
			++this->bitsSetados;
		}
	}

	uint64_t getBit(const uint64_t i) const {
		const uint64_t shift = i % 64;
		return this->v[i >> 6] & (1UL << shift);
	}

	void setBit(uint64_t i) {
		const uint64_t shift = i % 64;
		if( !getBit(i) ) {
			++this->bitsSetados;
		}

		this->v[i >> 6] |= (1UL << shift);
	}

	void clearBit(uint64_t i) {
		const uint64_t shift = i % 64;
		if( getBit(i) ) {
			--this->bitsSetados;
		}

		this->v[i >> 6] &= ~(1UL << shift);
	}

	/* Obtém a intersecção de dois conjuntos
	 * C = A & B
	 */
	Conjunto interseccao(const Conjunto &lhs, const Conjunto &rhs) const {
		const unsigned n = std::min(lhs.qtd(), rhs.qtd());
		Conjunto novo { n };

		for( unsigned i = 0; i < novo.tamanho(); ++i ) {
			novo.setChunk(i, lhs.getChunk(i) & rhs.getChunk(i));
		}

		return novo;
	}

	/* Obtém a união entre dois conjuntos
	 * C = A | B
	 */
	Conjunto uniao(const Conjunto &lhs, const Conjunto &rhs) const {
		const unsigned n = std::min(lhs.qtd(), rhs.qtd());
		Conjunto novo { n };

		for( unsigned i = 0; i < novo.tamanho(); ++i ) {
			novo.setChunk(i, lhs.getChunk(i) | rhs.getChunk(i));
		}

		return novo;
	}

	/* Obtém a negação de um conjunto
	 * C = A - B
	 */
	Conjunto negacao(const Conjunto &c) const {
		Conjunto novo { c.tamanho() };

		for( unsigned i = 0; i < c.tamanho(); ++i ) {
			novo.setChunk(i, ~c.getChunk(i));
		}

		return novo;
	}

	Conjunto operator&(const Conjunto &rhs) const {
		return this->interseccao(*this, rhs);
	}

	Conjunto operator|(const Conjunto &rhs) const {
		return this->uniao(*this, rhs);
	}

	Conjunto operator~(void) const {
		return this->negacao(*this);
	}
};

/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Estrutura de dados: Hash-map
 *
 * INÍCIO:
 *   2026-09-26
 */

#pragma once

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "util.hpp"

/* Hashmap
 *
 * Por simplicidade, o tipo das chaves (key, K) é sempre string, mas o tipo do
 * valor armazendo é genérico.
 */
template <typename V> struct Hashmap {
private:
	/* Nó no hasmap */
	struct HashmapNo {
		std::string k;
		V v;
		std::unique_ptr<HashmapNo> prox;
	};

	const uint64_t TAMANHO = 64;

	std::vector<std::unique_ptr<HashmapNo>> v;

public:
	/* Inicializa um hashmap vazio */
	Hashmap()
		: v(TAMANHO) { }

	/* Inicializa usando uma lista de pares KV */
	Hashmap(std::initializer_list<std::pair<std::string, V>> list)
		: v(TAMANHO) {
		for( const std::pair<std::string, V> &kv : list ) {
			inserir(kv.first, kv.second);
		}
	}

	~Hashmap() {
		for( std::unique_ptr<HashmapNo> &v : this->v ) {
			while( v ) {
				v = std::move(v->prox);
			}
		}
	}

	/* Inseri um novo par KV no hashmap */
	void inserir(const std::string k, const V v) {
		uint64_t i;
		std::unique_ptr<HashmapNo> no
			= std::unique_ptr<HashmapNo>(new HashmapNo { k, v, nullptr });

		i = computarHash(k) & this->TAMANHO;
		if( !this->v[i] ) {
			no->prox = std::move(this->v[i]);
		}

		this->v[i] = std::move(no);
	}

	/* Remove o par KV associado à chave K do hashmap
	 *
	 * Retorna se a operação for bem sucedida (algo foi removido)
	 */
	bool remover(const std::string k) {
		uint64_t i;

		i = computarHash(k) & this->TAMANHO;
		std::unique_ptr<HashmapNo> *prox;
		std::unique_ptr<HashmapNo> no = std::move(this->v[i]);

		while( no ) {
			prox = &no;
			if( no->k == k ) {
				*prox = no->prox;
				delete no;

				return true;
			}

			no = std::move(no->prox);
		}

		return false;
	}

	/* Busca o valor associado à chave K no hashmap */
	bool buscar(const std::string k, V *v) {
		uint64_t i;

		i = computarHash(k) & this->TAMANHO;
		std::unique_ptr<HashmapNo> no = std::move(this->v[i]);

		while( no ) {
			if( no->k == k ) {
				*v = no->v;
				return true;
			}

			no = std::move(no->prox);
		}

		return false;
	}
};

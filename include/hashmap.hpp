/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Estrutura de dados: Hash-map
 *
 * INÍCIO:
 *   2026-09-26
 */

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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

	const uint64_t FNV1A_OFFSET_BASIS = 14695981039346656037U;
	const uint64_t FNV1A_PRIME = 1099511628211U;

	std::vector<std::unique_ptr<HashmapNo>> v;

	/* Algoritmo FNV-1a
	 *
	 * Relativamente rápido (veloz o suficiente para nosso caso) e muito
	 * simples de se implementar
	 */
	uint64_t computarHash(const std::string &str) {
		uint64_t h = FNV1A_OFFSET_BASIS;
		for( char c : str ) {
			h ^= c;
			h *= FNV1A_PRIME;
		}

		return h & TAMANHO;
	}

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

		i = this->computarHash(k);
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

		i = this->computarHash(k);
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

		i = this->computarHash(k);
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

/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Banco de Dados de filmes e cinemas
 *
 * INICIO:
 *   2026-09-15
 */

#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

#include "cinema.hpp"
#include "consulta.hpp"
#include "filme.hpp"
#include "leitor.hpp"
#include "set.hpp"
#include "indice_numerico.hpp"

struct BancoDeDados {
private:
	/* Tipo para as funções de ordenação da função quicksort */
	typedef bool(ord_t)(Filme &a, Filme &b);

	/* Tipo para as funções de ordenação da função countingsort */
	typedef unsigned(key_t)(Filme &f);

	const std::string CAMINHO_FILMES = "../data/filmes.csv";
	const std::string CAMINHO_CINEMAS = "../data/cinemas.csv";

	const unsigned FILMES_POR_PAGINA = 50;
	const unsigned CINEMAS_POR_PAGINA = 50;

	Consulta cache;
	std::vector<Filme> filmes;
	std::vector<Filme *> filmes_cache;

	Quadtree quadtree;

	unsigned ano_inicial_max;
	unsigned ano_final_max;
	unsigned duracao_max;

	unsigned cinema_x_min, cinema_y_min;
	unsigned cinema_x_max, cinema_y_max;

	std::vector<unsigned> ind_titulo_primario;
	std::vector<unsigned> ind_titulo_original;
	std::vector<unsigned> ind_ano_inicial;
	std::vector<unsigned> ind_ano_final;
	std::vector<unsigned> ind_duracao;

	NoIndice *arvore_duracao = nullptr;
	NoIndice *arvore_ano_inicial = nullptr;

	std::vector<std::vector<unsigned>> ind_tipo;
	std::vector<std::vector<unsigned>> ind_genero;
	std::vector<unsigned> ind_preco;

	std::vector<std::vector<unsigned>> ind_tipo_cinema;
	std::vector<std::vector<unsigned>> ind_genero_cinema;

	Consulta cache_cinema;
	std::vector<Cinema> cinemas;
	std::vector<Cinema *> cinemas_cache;

	Conjunto ind_filmes_nao_adultos;
	Conjunto ind_filmes_adultos;

	std::vector<Filme> lerFilmes(std::string caminho) {
		std::ifstream arq(caminho);
		if( !arq.is_open() ) {
			std::cerr << "nao foi possivel abrir o arquivo: '" << caminho
					  << "'!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		LeitorFilme leitor { caminho };
		std::vector<Filme> filmes = leitor.ler();

		this->ano_inicial_max = leitor.ano_inicial_max;
		this->ano_final_max = leitor.ano_final_max;
		this->duracao_max = leitor.duracao_max;

		this->ind_filmes_nao_adultos = leitor.ind_adulto[1];
		this->ind_filmes_adultos = leitor.ind_adulto[1];

		return filmes;
	}

	std::vector<Cinema> lerCinemas(std::string caminho) {
		/* Abre o arquivo de cinemas para leitura */
		std::ifstream arq(caminho);
		if( !arq.is_open() ) {
			std::cerr << "nao foi possivel abrir o arquivo: '" << caminho
					  << "'!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		LeitorCinema leitor { caminho };
		std::vector<Cinema> cinemas = leitor.ler();

		this->cinema_x_min = leitor.x_min;
		this->cinema_y_min = leitor.y_min;

		this->cinema_x_max = leitor.x_max;
		this->cinema_y_max = leitor.y_max;

		return cinemas;
	}

	void construirIndicesFilmes(void) {
		std::cout << "  * Indice: titulo primario... " << std::flush;
		this->ind_titulo_primario = this->criarIndiceTitulo();
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: titulo original... " << std::flush;
		this->ind_titulo_original = this->criarIndiceTitulo();
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: ano inicial... " << std::flush;
		this->ind_ano_inicial
			= this->criarIndiceNumero(ano_inicial_max, obterAnoInicial);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: ano final... " << std::flush;
		this->ind_ano_final
			= this->criarIndiceNumero(ano_final_max, obterAnoFinal);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Arvore: ano inicial... " << std::flush;
		criarArvoreAnoInicial();
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: duracao... " << std::flush;
		this->ind_duracao = this->criarIndiceNumero(duracao_max, obterDuracao);
		criarArvoreDuracao();
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: tipo... " << std::endl;
		this->criarIndiceTipo();
		std::cout << "    * OK!" << std::endl;

		std::cout << "  * Indice: genero... " << std::endl;
		this->criarIndiceGenero();
		std::cout << "    * OK!" << std::endl;

		std::cout << "  * Indice: preco... " << std::endl;
		this->criarIndicePreco();
		std::cout << "    * OK!" << std::endl;
	}

	/* Cria o índice de título */
	std::vector<unsigned> criarIndiceTitulo(void) {
		std::vector<unsigned> indices(this->filmes.size());
		std::iota(std::begin(indices), std::end(indices), 0);

		quicksort(indices, 0, indices.size() - 1);

		return indices;
	}

	/* Cria o índice de gêneros */
	void criarIndiceGenero(void) {
		for( unsigned i = 0; i < filmes.size(); i++ ) {
			for( const std::string &genero : filmes[i].generos ) {
				unsigned pos = 0;

				while( pos < generos.size() && generos[pos] != genero ) {
					pos++;
				}

				if( pos == generos.size() ) {
					generos.push_back(genero);
					ind_genero.push_back(std::vector<unsigned>());
				}

				ind_genero[pos].push_back(i);
			}
		}

		for( unsigned i = 0; i < this->generos.size(); ++i ) {
			std::cout << "    * " << this->generos[i] << ": "
					  << this->ind_genero[i].size() << " filmes" << std::endl;
		}
	}

	/* Cria o índice de preço */
	void criarIndicePreco(void) {
		this->ind_preco.resize(this->cinemas.size());
		std::iota(std::begin(this->ind_preco), std::end(this->ind_preco), 0);

		quicksort_preco(this->ind_preco, 0, this->ind_preco.size() - 1);
	}

	void criarIndiceTipo(void) {
		for( unsigned i = 0; i < this->filmes.size(); i++ ) {
			unsigned pos = 0;

			/* Percorre o vetor de filmes enquanto o tipo do filme não for
			   correspondente ao tipo do vetor de tipos, E enquanto não chegar
				ao final do vetor de tipos*/
			while( pos < this->tipos.size()
				&& this->tipos[pos] != this->filmes[i].tipo ) {
				pos++;
			}

			/* Se o tipo ainda não existir, adicionamos o novo tipo
			   e criamos o vetor correspondente */
			if( pos == this->tipos.size() ) {
				this->tipos.push_back(this->filmes[i].tipo);
				this->ind_tipo.push_back(std::vector<unsigned>());
			}

			/* Colocamos o índice no vetor de índices do tipo correspondente*/
			this->ind_tipo[pos].push_back(i);
		}

		for( unsigned i = 0; i < this->tipos.size(); ++i ) {
			std::cout << "    * " << this->tipos[i] << ": "
					  << this->ind_tipo[i].size() << " filmes" << std::endl;
		}
	}

	/* Cria um índice baseado em uma propriedade duma classe */
	std::vector<unsigned> criarIndiceNumero(unsigned k, key_t f) {
		std::vector<unsigned> indices(this->filmes.size(), 0);
		std::iota(std::begin(indices), std::end(indices), 0);

		countingsort(indices, k, f);

		return indices;
	}

	void criarArvoreDuracao() {
		destruirArvore(arvore_duracao);

		for( unsigned i = 0; i < filmes.size(); i++ ) {
			if( filmes[i].duracao > 0 ) {
				inserir(arvore_duracao, filmes[i].duracao, i);
			}
		}
	}

	void criarArvoreAnoInicial() {
		destruirArvore(arvore_ano_inicial);

		for( unsigned i = 0; i < filmes.size(); ++i ) {
			const unsigned ano = filmes[i].ano_inicial;

			// Ano zero representa dado ausente.
			if( ano == 0 ) {
				continue;
			}

			inserir(arvore_ano_inicial, ano, i);
		}
	}

	/* Ordena um vetor de inteiros utilizando o algoritmo Quicksort
	 *
	 * Este algoritmo é usado para ordenar os títulos, já que são, em sua
	 * maioria, distintos, e essencialmente aleatórios (bom caso-de-uso para
	 * o Quicksort)
	 */
	void quicksort(std::vector<unsigned> &v, int baixo, int alto) {
		if( baixo >= 0 && alto >= 0 && baixo < alto ) {
			unsigned p = partition(v, baixo, alto);
			quicksort(v, baixo, p);
			quicksort(v, p + 1, alto);
		}
	}

	/* Particiona o array (para o Quicksort) */
	int partition(std::vector<unsigned> &v, int baixo, int alto) {
		unsigned pivo;
		pivo = v[baixo];

		int i = baixo - 1;
		int j = alto + 1;

		while( true ) {
			do {
				++i;
			} while( this->filmes[v[i]].titulo_original
				< this->filmes[pivo].titulo_original );

			do {
				--j;
			} while( this->filmes[v[j]].titulo_original
				> this->filmes[pivo].titulo_original );

			if( i >= j ) {
				return j;
			}

			std::swap(v[i], v[j]);
		}
	}

	/* Ordena um vetor de inteiros utilizando o algoritmo Quicksort
	 * Variação para ordenar preços de ingressos
	 */
	void quicksort_preco(std::vector<unsigned> &v, int baixo, int alto) {
		if( baixo >= 0 && alto >= 0 && baixo < alto ) {
			unsigned p = partition_preco(v, baixo, alto);
			quicksort_preco(v, baixo, p);
			quicksort_preco(v, p + 1, alto);
		}
	}

	/* Particiona o array por preço (para o Quicksort) */
	int partition_preco(std::vector<unsigned> &v, int baixo, int alto) {
		unsigned pivo;
		pivo = v[baixo];

		int i = baixo - 1;
		int j = alto + 1;

		while( true ) {
			do {
				++i;
			} while( this->cinemas[v[i]].preco_ingresso
				< this->cinemas[pivo].preco_ingresso );

			do {
				--j;
			} while( this->cinemas[v[j]].preco_ingresso
				> this->cinemas[pivo].preco_ingresso );

			if( i >= j ) {
				return j;
			}

			std::swap(v[i], v[j]);
		}
	}

	/* Ordena um vetor de inteiros utilizando o algoritmo Counting Sort
	 *
	 * Este algoritmo é usado para ordenar os anos de lançamento e durações, já
	 * que são números inteiros positivos e que se repetem com frequência
	 * (excelente caso-de-uso para o Counting Sort...)
	 */
	void countingsort(std::vector<unsigned> &v, unsigned k, key_t f) {
		std::vector<unsigned> contagem(k + 1, 0);
		std::vector<unsigned> ordenado(v.size());
		unsigned i, j;

		for( i = 0; i < v.size(); ++i ) {
			j = f(this->filmes[v[i]]);
			contagem[j] = contagem[j] + 1;
		}

		for( i = 1; i <= k; ++i ) {
			contagem[i] = contagem[i] + contagem[i - 1];
		}

		for( i = v.size(); i > 0; --i ) {
			j = f(this->filmes[v[i - 1]]);
			contagem[j] = contagem[j] - 1;
			ordenado[contagem[j]] = v[i - 1];
		}

		v = ordenado;
	}

	void construirIndicesCinemas() {
		std::cout << "  * Indice: tipo... " << std::endl;
		this->criarIndiceTipoCinema();
		std::cout << "    * OK!" << std::endl;

		std::cout << "  * Indice: genero... " << std::endl;
		this->criarIndiceGeneroCinema();
		std::cout << "    * OK!" << std::endl;

		std::cout << "  * Indice: localizacao... " << std::flush;
		this->criarIndiceLocalizacaoCinema();
		std::cout << "OK!" << std::endl;
	}

	unsigned encontrarFilme(const std::string &id) const {
		unsigned esq = 0, dir = filmes.size() - 1, m;

		while( esq < dir ) {
			m = esq + ((dir - esq) / 2);

			if( filmes[m].id < id ) {
				esq = m + 1;
			} else {
				dir = m - 1;
			}
		}

		while( filmes[esq].id < id ) {
			++esq;
		}

		return esq;
	}

	void criarIndiceTipoCinema(void) {
		this->ind_tipo_cinema.resize(this->tipos.size());

		for( unsigned i = 0; i < this->cinemas.size(); i++ ) {
			for( const std::string &idFilme : cinemas[i].filmes_em_exibicao ) {
				unsigned indiceFilme = this->encontrarFilme(idFilme);
				const std::string &tipo = this->filmes[indiceFilme].tipo;

				unsigned pos = 0;
				while( pos < this->tipos.size() && this->tipos[pos] != tipo ) {
					pos++;
				}

				if( pos == this->tipos.size() ) {
					continue;
				}

				bool jaExiste = false;
				for( unsigned cinema : this->ind_tipo_cinema[pos] ) {
					if( cinema == i ) {
						jaExiste = true;
						break;
					}
				}

				if( !jaExiste ) {
					this->ind_tipo_cinema[pos].push_back(i);
				}
			}
		}
		for( unsigned i = 0; i < this->tipos.size(); ++i ) {
			std::cout << "    * " << this->tipos[i] << ": "
					  << this->ind_tipo_cinema[i].size() << " cinemas"
					  << std::endl;
		}
	}

	void criarIndiceGeneroCinema(void) {
		this->ind_genero_cinema.resize(this->generos.size());

		for( unsigned i = 0; i < this->cinemas.size(); i++ ) {
			for( const std::string &idFilme :
				this->cinemas[i].filmes_em_exibicao ) {
				int indiceFilme = this->encontrarFilme(idFilme);
				if( indiceFilme == -1 ) {
					continue;
				}

				for( const std::string &genero :
					this->filmes[indiceFilme].generos ) {
					unsigned pos = 0;

					while( pos < this->generos.size()
						&& this->generos[pos] != genero ) {
						pos++;
					}

					if( pos == this->generos.size() ) {
						continue;
					}

					bool jaExiste = false;
					for( unsigned cinema : this->ind_genero_cinema[pos] ) {
						if( cinema == i ) {
							jaExiste = true;
							break;
						}
					}

					if( !jaExiste ) {
						this->ind_genero_cinema[pos].push_back(i);
					}
				}
			}
		}
		for( unsigned i = 0; i < this->generos.size(); ++i ) {
			std::cout << "    * " << this->generos[i] << ": "
					  << this->ind_genero_cinema[i].size() << " cinemas"
					  << std::endl;
		}
	}

	void criarIndiceLocalizacaoCinema(void) {
		this->cinema_x_max -= this->cinema_x_min;
		this->cinema_y_max -= this->cinema_y_min;

		this->quadtree.atualizarLimites(this->cinema_x_max, this->cinema_y_max);

		for( unsigned i = 0; i < this->cinemas.size(); ++i ) {
			Cinema &c = this->cinemas[i];
			c.x -= this->cinema_x_min;
			c.y -= this->cinema_y_min;

			this->quadtree.inserir(c.x, c.y, i);
		}
	}

	static unsigned obterAnoInicial(Filme &f) {
		return f.ano_inicial;
	}

	static unsigned obterAnoFinal(Filme &f) {
		return f.ano_final;
	}

	static unsigned obterDuracao(Filme &f) {
		return f.duracao;
	}

	Conjunto consultarFilme(const ConsultaNo *consulta) const {
		switch( consulta->tipo ) {
		case TipoConsulta::TITULO:
			return this->consultarFilmePorTitulo(consulta->getStr());
		case TipoConsulta::ADULTO:
			if( consulta->isAdulto() ) {
				return this->ind_filmes_adultos;
			}
			return this->ind_filmes_nao_adultos;
		case TipoConsulta::ANO:
			return this->consultarFilmePorIntervaloDeAnos(
				consulta->getNum(), consulta->getNum());
		case TipoConsulta::ANO_FAIXA_INICIAL:
			return this->consultarFilmePorIntervaloDeAnos(
				consulta->getNum(), this->ano_inicial_max);
		case TipoConsulta::ANO_FAIXA_FINAL:
			return this->consultarFilmePorIntervaloDeAnos(
				1, consulta->getNum());
		case TipoConsulta::ANO_INTERVALO:
			return this->consultarFilmePorIntervaloDeAnos(
				consulta->getAnoMinimo(), consulta->getAnoMaximo());
		case TipoConsulta::DURACAO_INTERVALO: {
			std::vector<unsigned> indices;
			buscarIntervalo(this->arvore_duracao, consulta->getDuracaoMinima(),
				consulta->getDuracaoMaxima(), indices);
			Conjunto resultado;
			for( unsigned i = 0; i < indices.size(); i++ ) {
				resultado.add(indices[i]);
			}
			return resultado;
		}
		case TipoConsulta::PRECO:
			break;
		case TipoConsulta::GENERO:
			return this->consultarFilmePorGenero(consulta->getStr());
		case TipoConsulta::TIPO:
			return this->consultarFilmePorTipo(consulta->getStr());
		case TipoConsulta::LOCALIZACAO:
			break;
		case TipoConsulta::OP_E: {
			Conjunto a = this->consultarFilme(consulta->getEsq());
			Conjunto b = this->consultarFilme(consulta->getDir());
			return a & b;
		}
		case TipoConsulta::OP_OU: {
			Conjunto a = this->consultarFilme(consulta->getEsq());
			Conjunto b = this->consultarFilme(consulta->getDir());
			return a | b;
		}
		case TipoConsulta::OP_NAO: {
			Conjunto a;
			Conjunto b = this->consultarFilme(consulta->getDir());

			a.preencher(this->filmes.size());
			return a - b;
		}
		}

		std::cerr << "consulta invalidississima" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	bool subExiste(
		const std::string &needle, const std::string &haystack) const {
		unsigned i, j;
		if( haystack.size() < needle.size() ) {
			return false;
		}

		for( i = 0; i < haystack.size(); ++i ) {
			if( haystack[i] != needle[0] ) {
				continue;
			}

			bool ok = true;
			unsigned k = i + 1;
			for( j = 1; k < haystack.size() && j < needle.size(); ++k, ++j ) {
				if( haystack[k] != needle[j] ) {
					ok = false;
					break;
				}
			}

			if( ok ) {
				return true;
			}
		}

		return false;
	}

	Conjunto consultarFilmePorTitulo(const std::string &titulo) const {
		Conjunto c;
		unsigned esq = 0, dir = this->filmes.size() - 1;

		/* Busca binária pelo título */
		while( esq < dir ) {
			const unsigned m = (esq + dir) / 2;
			const std::string &valor
				= this->filmes[this->ind_titulo_original[m]].titulo_original;

			if( valor < titulo ) {
				esq = m + 1;
			} else {
				dir = m;
			}
		}

		/* Se cairmos nesse caso, não encontramos nenhum filme...
		 * Sai da função
		 */
		if( esq >= this->filmes.size()
			|| !this->subExiste(titulo,
				this->filmes[this->ind_titulo_original[esq]]
					.titulo_original) ) {
			/* TODO: alguma forma de dizer 'ei, não achamos nada.'
			 * Ou talvez só retornar o set vazio já seja suficiente...
			 */
			return c;
		}

		/* Se cairmos nesse caso, encontramos!
		 *
		 * Como podem existir vários filmes com o mesmo título. Aquela busca
		 * binária foi feita para encontrar o filme que aparece primeiro no
		 * índice. Por isso que temos esse loop aqui, pra ir adicionando todos
		 * os filmes com título igual, avançando filme por filme:
		 *
		 * Exemplo:
		 *
		 * BUSCA: "AFTERMATH"
		 *
		 * [ ... , "A", "AFTERMATH", "AFTERMATH", "AFTERMATH", "B", ... ]
		 *                   │                         │
		 *                   busca binária             │
		 *                   encontra essa             │
		 *                   posição                   │
		 *                                             │
		 *                                             ...mas temos que
		 *                                             percorrer até aqui
		 *                                             para pegar todos os
		 *                                             filmes cujo nome bate
		 *                                             com a busca
		 *
		 * */
		unsigned ok = esq;
		do {
			c.add(this->ind_titulo_original[ok++]);
		} while( ok < this->filmes.size()
			&& this->subExiste(titulo,
				this->filmes[this->ind_titulo_original[ok]].titulo_original) );

		return c;
	}

	Conjunto consultarFilmePorGenero(const std::string &genero) const {
		Conjunto c;

		unsigned pos = 0;

		/* Percorre o vetor de tipos até encontrar o gênero procurado */
		while( pos < this->generos.size() && this->generos[pos] != genero ) {
			pos++;
		}

		/* Caso o gênero não exista, retorna o conjunto c vazio */
		if( pos == this->generos.size() ) {
			return c;
		}

		/* "Setamos" todos os índices do vetor de índices do gênero procurado */
		for( unsigned indice : this->ind_genero[pos] ) {
			c.add(indice);
		}

		return c;
	}

	Conjunto consultarFilmePorTipo(const std::string &tipo) const {
		Conjunto c; // cria o conjunto vazio

		unsigned pos = 0;

		/* Percorre o vetor de tipos até encontrar o tipo procurado */
		while( pos < this->tipos.size() && this->tipos[pos] != tipo ) {
			pos++;
		}

		/* Caso o tipo não exista, retorna o conjunto c vazio */
		if( pos == this->tipos.size() ) {
			return c;
		}

		/* "Setamos" todos os índices do vetor de índices do tipo procurado */
		for( unsigned indice : this->ind_tipo[pos] ) {
			c.add(indice);
		}

		return c;
	};

	Conjunto consultarCinema(const ConsultaNo *consulta) const {
		switch( consulta->tipo ) {
		case TipoConsulta::GENERO:
			return this->consultarCinemaPorGenero(consulta->getStr());
		case TipoConsulta::TIPO:
			return this->consultarCinemaPorTipo(consulta->getStr());
		case TipoConsulta::PRECO:
			return this->consultarCinemaPorPreco(consulta->getPreco());
		case TipoConsulta::LOCALIZACAO:
			return this->consultarCinemaPorLocalizacao(consulta->getLoc());
		case TipoConsulta::OP_E: {
			Conjunto a = this->consultarCinema(consulta->getEsq());
			Conjunto b = this->consultarCinema(consulta->getDir());
			return a & b;
		}
		case TipoConsulta::OP_OU: {
			Conjunto a = this->consultarCinema(consulta->getEsq());
			Conjunto b = this->consultarCinema(consulta->getDir());
			return a | b;
		}
		case TipoConsulta::OP_NAO: {
			Conjunto a;
			Conjunto b = this->consultarCinema(consulta->getDir());

			a.preencher(this->filmes.size());
			return a - b;
		}

		default:
			break;
		}

		std::cerr << "consulta de cinema invalida" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	/* Busca cinemas que possuem filmes de um gênero específico em cartaz */
	Conjunto consultarCinemaPorGenero(const std::string &genero) const {
		Conjunto c;

		unsigned pos = 0;

		while( pos < this->generos.size() && this->generos[pos] != genero ) {
			++pos;
		}

		if( pos == this->generos.size() ) {
			return c;
		}

		for( unsigned indice : this->ind_genero_cinema[pos] ) {
			c.add(indice);
		}

		return c;
	}

	/* Busca cinemas que possuem filmes de um tipo específico em cartaz */
	Conjunto consultarCinemaPorTipo(const std::string &tipo) const {
		Conjunto c; // cria o conjunto vazio

		unsigned pos = 0;

		/* Percorre o vetor de tipos até encontrar o tipo procurado */
		while( pos < this->tipos.size() && this->tipos[pos] != tipo ) {
			pos++;
		}

		/* Caso o tipo não exista, retorna o conjunto c vazio */
		if( pos == this->tipos.size() ) {
			return c;
		}

		/* "Setamos" todos os índices do vetor de índices do tipo procurado */
		for( unsigned indice : this->ind_tipo_cinema[pos] ) {
			c.add(indice);
		}

		return c;
	}

	/* Busca cinemas que possuem ingressos abaixo de um valor */
	Conjunto consultarCinemaPorPreco(double preco) const {
		Conjunto c;
		unsigned esq = 0, dir = this->cinemas.size() - 1;

		/* Busca binária pelo título */
		while( esq < dir ) {
			const unsigned m = (esq + dir) / 2;
			const double valor
				= this->cinemas[this->ind_preco[m]].preco_ingresso;

			if( valor > preco ) {
				dir = m;
			} else {
				esq = m + 1;
			}
		}

		dir = std::min(dir, (unsigned)this->cinemas.size() - 1);
		for( unsigned i = 0; i < dir; ++i ) {
			c.add(this->ind_preco[i]);
		}

		return c;
	}

	/* Busca cinemas que possuem filmes proximos específico em cartaz */
	Conjunto consultarCinemaPorLocalizacao(const Loc loc) const {
		Conjunto c;

		const std::vector<unsigned> pontos = this->quadtree.buscar(loc);
		for( const unsigned ind : pontos ) {
			c.add(ind);
		}

		return c;
	}

	Conjunto consultarIndiceNumerico(const std::vector<unsigned> &indice,
		unsigned minimo, unsigned maximo, unsigned Filme::*campo) const {
		Conjunto resultado;

		if( minimo > maximo || indice.empty() ) {
			return resultado;
		}

		// Encontra a primeira posição cujo valor seja >= minimo.
		unsigned esq = 0;
		unsigned dir = indice.size();

		while( esq < dir ) {
			unsigned meio = esq + (dir - esq) / 2;
			unsigned valor = filmes[indice[meio]].*campo;

			if( valor < minimo ) {
				esq = meio + 1;
			} else {
				dir = meio;
			}
		}

		// Adiciona os filmes até ultrapassar maximo.
		for( unsigned i = esq; i < indice.size(); ++i ) {
			unsigned valor = filmes[indice[i]].*campo;

			if( valor > maximo ) {
				break;
			}

			// Ano e duração iguais a zero representam dados ausentes.
			if( valor != 0 ) {
				resultado.add(indice[i]);
			}
		}

		return resultado;
	}

	std::vector<unsigned> consultarDuracaoIntervalo(
		unsigned minimo, unsigned maximo) {
		std::vector<unsigned> resultado;

		if( minimo > maximo ) {
			return resultado;
		}

		buscarIntervalo(arvore_duracao, minimo, maximo, resultado);

		return resultado;
	}

	Conjunto consultarFilmePorIntervaloDeAnos(
		unsigned minimo, unsigned maximo) const {
		Conjunto resultado;

		if( minimo == 0 || minimo > maximo ) {
			return resultado;
		}

		std::vector<unsigned> indices;

		buscarIntervalo(arvore_ano_inicial, minimo, maximo, indices);

		for( unsigned indice : indices ) {
			resultado.add(indice);
		}

		return resultado;
	}

public:
	unsigned getDuracaoMaxima() const {
		return duracao_max;
	}

	std::vector<std::string> tipos;
	std::vector<std::string> generos;

	BancoDeDados()
		: quadtree(4) {
		srand(time(NULL));
	}

	/* Lê os dados dos filmes e cinemas */
	void lerDados(void) {
		std::cout << "# Lendo filmes ... " << std::flush;
		this->filmes = this->lerFilmes(this->CAMINHO_FILMES);
		std::cout << "OK!" << std::endl;

		std::cout << "# Lendo cinemas ... " << std::flush;
		this->cinemas = this->lerCinemas(this->CAMINHO_CINEMAS);
		std::cout << "OK!" << std::endl;

		std::cout << "# Construindo indices de filmes ... " << std::endl;
		this->construirIndicesFilmes();

		std::cout << "# Construindo indices de cinemas ... " << std::endl;
		this->construirIndicesCinemas();
	}

	void printConsulta(ConsultaNo *no) const {
		switch( no->tipo ) {
		case TipoConsulta::TITULO:
			std::cout << "(TITULO: " << no->getStr() << ")" << std::endl;
			break;
		case TipoConsulta::ADULTO:
			std::cout << "(ADULTO: " << no->isAdulto() << ")" << std::endl;
			break;
		case TipoConsulta::ANO:
			std::cout << "(ANO >= " << no->getNum() << ")" << std::endl;
			break;
		case TipoConsulta::ANO_FAIXA_INICIAL:
			std::cout << "(ANO INICIAL >= " << no->getNum() << ")" << std::endl;
			break;
		case TipoConsulta::ANO_FAIXA_FINAL:
			std::cout << "(ANO FINAL >= " << no->getNum() << ")" << std::endl;
			break;
		case TipoConsulta::ANO_INTERVALO:
			std::cout << "(ANO: " << no->getAnoMinimo() << "-"
					  << no->getAnoMaximo() << ")" << std::endl;
			break;
		case TipoConsulta::DURACAO_INTERVALO:
			std::cout << "(DURACAO: " << no->getDuracaoMinima() << "-"
					  << no->getDuracaoMaxima() << ")" << std::endl;
			break;
		case TipoConsulta::GENERO:
			std::cout << "(GENERO: " << no->getStr() << ")" << std::endl;
			break;
		case TipoConsulta::TIPO:
			std::cout << "(TIPO: " << no->getStr() << ")" << std::endl;
			break;
		case TipoConsulta::PRECO:
			std::cout << "(PRECO: " << no->getPreco() << ")" << std::endl;
			break;
		case TipoConsulta::LOCALIZACAO: {
			const Loc &loc = no->getLoc();
			std::cout << "(TIPO: " << loc.x << " " << loc.y << " " << loc.raio
					  << ")" << std::endl;
		} break;
		case TipoConsulta::OP_E:
			std::cout << "(E:" << std::endl;

			std::cout << "  ";
			this->printConsulta(no->getEsq());
			std::cout << "  ";
			this->printConsulta(no->getDir());

			std::cout << ")" << std::endl;
			break;
		case TipoConsulta::OP_OU:
			std::cout << "(OU:" << std::endl;

			std::cout << "  ";
			this->printConsulta(no->getEsq());
			std::cout << "  ";
			this->printConsulta(no->getDir());

			std::cout << ")" << std::endl;
			break;
		case TipoConsulta::OP_NAO:
			std::cout << "(NAO:" << std::endl;
			std::cout << "  ";
			this->printConsulta(no->getDir());

			std::cout << ")" << std::endl;
			break;
		}
	}

	/* Faz uma consulta por filmes */
	std::vector<Filme *> consultarFilme(
		const Consulta &consulta, unsigned pagina, unsigned &total) {
		std::vector<Filme *> resultado { FILMES_POR_PAGINA };
		unsigned limite, i, j;

		this->printConsulta(consulta.raiz);
		if( !this->cache || this->cache != consulta ) {
			this->cache = consulta;

			const Conjunto conj = this->consultarFilme(consulta.raiz);

			total = conj.tamanho();
			this->filmes_cache.resize(total);

			j = 0;
			for( unsigned x : conj.v ) {
				this->filmes_cache[j++] = &this->filmes[x];
			}
		} else {
			total = this->filmes_cache.size();
		}

		i = pagina * this->FILMES_POR_PAGINA;
		limite = std::min(i + this->FILMES_POR_PAGINA, total);
		for( j = 0; i < limite; ++i, ++j ) {
			resultado[j] = this->filmes_cache[i];
		}

		resultado.resize(j);
		return resultado;
	}

	unsigned getFilmesPorPagina(void) const {
		return this->FILMES_POR_PAGINA;
	}

	std::vector<Cinema *> consultarCinema(
		const Consulta &consulta, unsigned pagina, unsigned &total) {

		std::vector<Cinema *> resultado { CINEMAS_POR_PAGINA };

		unsigned limite, i, j;

		if( !this->cache_cinema || this->cache_cinema != consulta ) {
			this->cache_cinema = consulta;

			const Conjunto conj = this->consultarCinema(consulta.raiz);

			total = conj.tamanho();
			this->cinemas_cache.resize(total);

			j = 0;
			for( unsigned x : conj.v ) {
				this->cinemas_cache[j++] = &this->cinemas[x];
			}
		} else {
			total = this->cinemas_cache.size();
		}

		i = pagina * this->CINEMAS_POR_PAGINA;
		limite = std::min(i + this->CINEMAS_POR_PAGINA, total);

		for( j = 0; i < limite; ++i, ++j ) {
			resultado[j] = this->cinemas_cache[i];
		}

		resultado.resize(j);

		return resultado;
	}

	unsigned getCinemasPorPagina(void) const {
		return this->CINEMAS_POR_PAGINA;
	}

	/* Desenha o mapa de cinemas */
	void desenharMapaDeCinemas(unsigned &x, unsigned &y, unsigned &zoom) const {
		const unsigned SZ = 32;
		std::vector<std::vector<char>> mapa(SZ, std::vector<char>(SZ, '.'));

		const unsigned W = this->cinema_x_max >> zoom;
		const unsigned H = this->cinema_y_max >> zoom;
		const unsigned X = x * W;
		const unsigned Y = y * H;

		Rect r = Rect { X, Y, W, H };
		std::vector<unsigned> pontos = this->quadtree.buscar(r);

		for( const unsigned ind : pontos ) {
			const Cinema &c = this->cinemas[ind];
			const unsigned x = c.x;
			const unsigned y = c.y;

			const unsigned ix = ((double)(SZ - 1) / W) * (x - X);
			const unsigned iy = ((double)(SZ - 1) / H) * (y - Y);
			mapa[iy][ix] = '*';
		}

		/* Nosso "cursor" */
		mapa[SZ / 2][SZ / 2] = 'O';
		for( std::vector<char> &v : mapa ) {
			for( char c : v ) {
				std::cout << c << " ";
			}
			std::cout << std::endl;
		}

		std::cout << "- - -" << std::endl;
		std::cout << "X: " << X << " | Y: " << Y << " | Zoom: " << (zoom + 1)
				  << "x (de 8x)" << std::endl;
		std::cout << "Recorte: (" << X << ", " << Y << ") .. (" << X + W << ", "
				  << Y + H << ")" << std::endl;
	}

	~BancoDeDados() {
		destruirArvore(arvore_duracao);
		destruirArvore(arvore_ano_inicial);
	}
};

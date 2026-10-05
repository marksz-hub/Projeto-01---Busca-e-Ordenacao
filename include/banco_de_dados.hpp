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

struct BancoDeDados {
private:
	/* Tipo para as funções de ordenação da função quicksort */
	typedef bool(ord_t)(Filme &a, Filme &b);

	/* Tipo para as funções de ordenação da função countingsort */
	typedef unsigned(key_t)(Filme &f);

	const std::string CAMINHO_FILMES = "../data/filmes.csv";
	const std::string CAMINHO_CINEMAS = "../data/cinemas.csv";

	const unsigned FILMES_POR_PAGINA = 50;

	Consulta cache;
	std::vector<Filme> filmes;
	std::vector<Filme *> filmes_cache;

	unsigned ano_inicial_max;
	unsigned ano_final_max;
	unsigned duracao_max;

	std::vector<unsigned> ind_titulo_primario;
	std::vector<unsigned> ind_titulo_original;
	std::vector<unsigned> ind_ano_inicial;
	std::vector<unsigned> ind_ano_final;
	std::vector<unsigned> ind_duracao;
	std::vector<std::vector<unsigned>> ind_tipo;
	std::vector<std::vector<unsigned>> ind_genero;

	std::vector<Cinema> cinemas, cinemas_cache;

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

		return cinemas;
	}

	void construirIndicesFilmes(void) {
		std::cout << "  * Indice: titulo primario... " << std::flush;
		this->ind_titulo_primario
			= this->criarIndiceTitulo(compararPorTituloPrimario);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: titulo original... " << std::flush;
		this->ind_titulo_original
			= this->criarIndiceTitulo(compararPorTituloOriginal);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: ano inicial... " << std::flush;
		this->ind_ano_inicial
			= this->criarIndiceNumero(ano_inicial_max, obterAnoInicial);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: ano final... " << std::flush;
		this->ind_ano_final
			= this->criarIndiceNumero(ano_final_max, obterAnoFinal);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: duracao... " << std::flush;
		this->ind_duracao = this->criarIndiceNumero(duracao_max, obterDuracao);
		std::cout << "OK!" << std::endl;

		std::cout << "  * Indice: tipo... " << std::endl;
		this->criarIndiceTipo();
		std::cout << "    * OK!" << std::endl;

		std::cout << "  * Indice: genero... " << std::endl;
		this->criarIndiceGenero();
		std::cout << "    * OK!" << std::endl;
	}

	/* Cria um índice baseado em uma propriedade duma classe */
	std::vector<unsigned> criarIndiceTitulo(ord_t f) {
		std::vector<unsigned> indices(this->filmes.size(), 0);
		std::iota(std::begin(indices), std::end(indices), 0);

		quicksort(indices, 0, indices.size() - 1, f);

		return indices;
	}

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

	/* Ordena um vetor de inteiros utilizando o algoritmo Quicksort
	 *
	 * Este algoritmo é usado para ordenar os títulos, já que são, em sua
	 * maioria, distintos, e essencialmente aleatórios (bom caso-de-uso para
	 * o Quicksort)
	 */
	void quicksort(std::vector<unsigned> &v, int baixo, int alto, ord_t f) {
		if( baixo >= alto || baixo < 0 ) {
			return;
		}

		unsigned p = partition(v, baixo, alto, f);

		quicksort(v, baixo, p - 1, f);
		quicksort(v, p + 1, alto, f);
	}

	/* Particiona o array (para o Quicksort) */
	int partition(std::vector<unsigned> &v, int baixo, int alto, ord_t f) {
		unsigned pivo;
		int i, j;

		/* TODO: utilizar outro esquema de escolha de pivô mais eficiente */
		pivo = v[alto];

		i = baixo;
		for( j = baixo; j < alto - 1; ++j ) {
			if( f(this->filmes[v[j]], this->filmes[pivo]) ) {
				std::swap(v[i], v[j]);
				++i;
			}
		}

		std::swap(v[i], v[alto]);
		return i;
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
		std::cout << "	Indice: tipo... " << std::endl;
		this->criarIndiceTipoCinema();
		std::cout << " 	OK!" << std::endl;

		std::cout << "	Indice: genero... " << std::endl;
		this->criarIndiceGeneroCinema();
		std::cout << " 	OK!" << std::endl;
	}

	int encontrarFilme(const std::string &id) const {
		for(unsigned i = 0; i < filmes.size(); i++) {
			if(filmes[i].id == id) {
				return i;
			}
		}
		return -1;
	}
	
	void criarIndiceTipoCinema(void) {
		this->ind_tipo_cinema.resize(this->tipos.size());

		for(unsigned i = 0; i < this->cinemas.size(); i++) {

			for(const std::string &idFilme : cinemas[i].filmes_em_exibicao) {

				int indiceFilme = this->encontrarFilme(idFilme);

				if(indiceFilme == -1) {
					continue;
				}

				const std::string &tipo = this->filmes[indiceFilme].tipo;

				unsigned pos = 0;
				while(pos < this->tipos.size() && this->tipos[pos] != tipo) {
					pos++;
				}

				if(pos == this->tipos.size()){
					continue;	
				}

				bool jaExiste = false;

				for(unsigned cinema : this->ind_tipo_cinema[pos]) {
					if(cinema == i) {
						jaExiste = true;
						break;
					}
				}

				if(!jaExiste) {
					this->ind_tipo_cinema[pos].push_back(i);
				}
			}
		}
		for( unsigned i = 0; i < this->tipos.size(); ++i ) {
			std::cout << "    * " << this->tipos[i] << ": "
				  	  << this->ind_tipo_cinema[i].size()
				  	  << " cinemas" << std::endl;
		}
	}

	void criarIndiceGeneroCinema(void) {
		this->ind_genero_cinema.resize(this->generos.size());

		for(unsigned i = 0; i < this->cinemas.size(); i++) {

			for(const std::string &idFilme : this->cinemas[i].filmes_em_exibicao) {
				int indiceFilme = this->encontrarFilme(idFilme);

				if( indiceFilme == -1 ) {
					continue;
				}

				for(const std::string &genero : this->filmes[indiceFilme].generos) {
					unsigned pos = 0;

					while(pos < this->generos.size() && this->generos[pos] != genero) {
						pos++;
					}

					if(pos == this->generos.size()) {
						continue;
					}

					bool jaExiste = false;
					for(unsigned cinema : this->ind_genero_cinema[pos]) {
						if(cinema == i) {
							jaExiste = true;
							break;
						}
					}

					if(!jaExiste) {
						this->ind_genero_cinema[pos].push_back(i);
					}
				}
			}
		}
		for( unsigned i = 0; i < this->generos.size(); ++i ) {
		std::cout << "    * " << this->generos[i] << ": "
				  << this->ind_genero_cinema[i].size()
				  << " cinemas" << std::endl;
		}
	}




	static bool compararPorTituloPrimario(Filme &a, Filme &b) {
		return a.titulo_primario < b.titulo_primario;
	}

	static bool compararPorTituloOriginal(Filme &a, Filme &b) {
		return a.titulo_original < b.titulo_original;
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
			break;
		case TipoConsulta::ANO:
			break;
		case TipoConsulta::ANO_FAIXA_INICIAL:
			break;
		case TipoConsulta::ANO_FAIXA_FINAL:
			break;
		case TipoConsulta::DURACAO:
			break;
		case TipoConsulta::GENERO:
			return this->consultarFilmePorGenero(consulta->getStr());
		case TipoConsulta::TIPO:
			return this->consultarFilmePorTipo(consulta->getStr());
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
			Conjunto c = this->consultarFilme(consulta->getEsq());
			return ~c;
		}
		}

		std::cerr << "consulta invalidississima" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	Conjunto consultarFilmePorTitulo(const std::string &titulo) const {
		Conjunto c(this->filmes.size());
		unsigned l = 0, r = this->filmes.size() - 1;

		/* Busca binária pelo título */
		while( l < r ) {
			const unsigned m = (l + r) / 2;
			const std::string &valor
				= this->filmes[this->ind_titulo_original[m]].titulo_original;

			if( valor < titulo ) {
				l = m + 1;
			} else {
				r = m;
			}
		}

		/* Se cairmos nesse caso, não encontramos nenhum filme...
		 * Sai da função
		 */
		if( l >= this->filmes.size()
			|| this->filmes[this->ind_titulo_original[l]].titulo_original
				!= titulo ) {
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
		unsigned ok = l;
		do {
			c.setBit(this->ind_titulo_original[ok++]);
		} while( ok < this->filmes.size()
			&& this->filmes[this->ind_titulo_original[ok]].titulo_original
				<= titulo );

		return c;
	}

	Conjunto consultarFilmePorGenero(const std::string &genero) const {
		Conjunto c(this->filmes.size());

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
			c.setBit(indice);
		}

		return c;
	}

	Conjunto consultarFilmePorTipo(const std::string &tipo) const {
		Conjunto c(this->filmes.size()); // cria o conjunto vazio

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
			c.setBit(indice);
		}

		return c;
	};




	Conjunto consultarCinema(const ConsultaNo *consulta) const {
		switch(consulta->tipo) {
		case TipoConsulta::GENERO:
			return this->consultarCinemaPorGenero(consulta->getStr());
		case TipoConsulta::TIPO:
			return this->consultarCinemaPorTipo(consulta->getStr());

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
			Conjunto c = this->consultarCinema(consulta->getEsq());
			return ~c;
		}

		default:
			break;
		}

		std::cerr << "consulta de cinema invalida" << std::endl;
    	std::exit(EXIT_FAILURE);
	}

	Conjunto consultarCinemaPorGenero(const std::string &genero) const {
		Conjunto c(this->cinemas.size());

		unsigned pos = 0;

		while( pos < this->generos.size()
			&& this->generos[pos] != genero ) {
			++pos;
		}

		if( pos == this->generos.size() ) {
			return c;
		}

		for( unsigned indice : this->ind_genero_cinema[pos] ) {
			c.setBit(indice);
		}

		return c;
	}


	Conjunto consultarCinemaPorTipo(const std::string &tipo) const {
		Conjunto c(this->cinemas.size()); // cria o conjunto vazio

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
			c.setBit(indice);
		}

		return c;
	}





public:
	std::vector<std::string> tipos;
	std::vector<std::string> generos;

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
			std::cout << "(ANO:    " << no->getNum() << ")" << std::endl;
			break;
		case TipoConsulta::ANO_FAIXA_INICIAL:
			break;
		case TipoConsulta::ANO_FAIXA_FINAL:
			break;
		case TipoConsulta::DURACAO:
			break;
		case TipoConsulta::GENERO:
			std::cout << "(GENERO: " << no->getStr() << ")" << std::endl;
			break;
		case TipoConsulta::TIPO:
			std::cout << "(TIPO: " << no->getStr() << ")" << std::endl;
			break;
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
			break;
		}
	}

	/* Faz uma consulta por filmes */
	std::vector<Filme *> consultarFilme(
		const Consulta &consulta, unsigned pagina, unsigned &total) {
		std::vector<Filme *> resultado { FILMES_POR_PAGINA };
		unsigned limite, i, j;

		if( !this->cache || this->cache != consulta ) {
			this->cache = consulta;

			const Conjunto conj = this->consultarFilme(consulta.raiz);

			total = conj.setados();
			this->filmes_cache.resize(total);

			for( i = j = 0; j < total; ++i ) {
				if( conj.getBit(i) ) {
					this->filmes_cache[j++] = &this->filmes[i];
				}
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

			total = conj.setados();

			this->cinemas_cache.resize(total);

			for( i = j = 0; j < total; ++i ) {
				if( conj.getBit(i) ) {
					this->cinemas_cache[j++] = &this->cinemas[i];
				}
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
};

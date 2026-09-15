/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Banco de Dados de filmes e cinemas
 *
 * INICIO:
 *   2026-09-15
 */

#pragma once

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "cinema.hpp"
#include "filme.hpp"

struct BancoDeDados {
private:
	/* Tipo para as funções de ordenação da função quicksort */
	typedef bool(ord_t)(Filme &a, Filme &b);

	const std::string CAMINHO_FILMES = "../data/filmes.csv";
	const std::string CAMINHO_CINEMAS = "../data/cinemas.csv";

	std::vector<Filme> filmes;
	std::vector<unsigned> ind_titulos;

	std::vector<Cinema> cinemas;

	std::vector<Filme> lerFilmes(std::string caminho) {
		std::ifstream arq(caminho);
		if( !arq.is_open() ) {
			std::cerr << "nao foi possivel abrir o arquivo: '" << caminho
					  << "'!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		std::vector<Filme> filmes;

		std::string id;
		std::string tipo;
		std::string titulo_primario;
		std::string titulo_original;
		std::string generos;

		std::string buf;
		int ano_inicial;
		int ano_final;
		bool adulto;
		int duracao;

		/* Pula a primeira linha */
		std::getline(arq, buf);

		while( !arq.eof() ) {
			std::getline(arq, id, '\t');
			std::getline(arq, tipo, '\t');
			std::getline(arq, titulo_primario, '\t');
			std::getline(arq, titulo_original, '\t');

			std::getline(arq, buf, '\t');
			adulto = (std::stoi(buf) == 1);

			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				ano_inicial = -1;
			} else {
				ano_inicial = std::stoi(buf);
			}

			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				ano_final = -1;
			} else {
				ano_final = std::stoi(buf);
			}

			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				duracao = -1;
			} else {
				duracao = std::stoi(buf);
			}

			std::getline(arq, generos);

			Filme filme = Filme(id, tipo, titulo_primario, titulo_original,
				ano_inicial, ano_final, adulto, duracao, generos);
			filmes.push_back(filme);
		}

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

		std::vector<Cinema> cinemas;
		std::string linha;

		/* Pula a primeira linha */
		std::getline(arq, linha);

		while( std::getline(arq, linha) ) {

			/* Variáveis que armazenam os dados de um cinema */
			std::string id;
			std::string nome_do_cinema;
			int x;
			int y;
			double preco_ingresso;
			std::vector<std::string> filmes_em_exibicao;

			/* Usa a linha como um fluxo para separar os campos por vírgula */
			std::string buf;
			std::stringstream ss(linha);

			std::getline(ss, id, ',');

			std::getline(ss, nome_do_cinema, ',');
			removerEspacoInicial(nome_do_cinema);

			/* Lê as coordenadas e converte seus valores de string para inteiro
			 */
			std::getline(ss, buf, ',');
			x = std::stoi(buf);
			std::getline(ss, buf, ',');
			y = std::stoi(buf);

			/* Lê o preço e converte seu valor de string para double */
			std::getline(ss, buf, ',');
			preco_ingresso = std::stod(buf);

			/* Os campos restantes da linha correspondem aos filmes em exibição,
			 * com a quantidade de filmes (campos) podendo variar entre os
			 * cinemas
			 */
			while( std::getline(ss, buf, ',') ) {
				removerEspacoInicial(buf);
				filmes_em_exibicao.push_back(buf);
			}

			/* Cria o objeto cinema com os dados lidos da linha */
			Cinema cinema = Cinema(
				id, nome_do_cinema, x, y, preco_ingresso, filmes_em_exibicao);
			/* Adiciona o cinema ao vetor */
			cinemas.push_back(cinema);
		}

		return cinemas;
	}

	/* Remove o espaco que aparece no início dos campos após a separação por
	 * vírgula
	 */
	void removerEspacoInicial(std::string &texto) {
		if( !texto.empty() && texto[0] == ' ' ) {
			texto.erase(0, 1);
		}
	}

	/* Cria um índice baseado em uma propriedade duma classe */
	std::vector<unsigned> criarIndice(ord_t f) {
		std::vector<unsigned> indices(this->filmes.size(), 0);
		std::iota(std::begin(indices), std::end(indices), 0);

		quicksort(indices, 0, indices.size() - 1, f);

		return indices;
	}

	/* Ordena um vetor de inteiros utiliando o algoritmo Quicksort */
	void quicksort(std::vector<unsigned> &v, int baixo, int alto, ord_t f) {
		if( baixo >= alto || baixo < 0 ) {
			return;
		}

		unsigned p = partition(v, baixo, alto, f);

		quicksort(v, baixo, p - 1, f);
		quicksort(v, p + 1, alto, f);
	}

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

	static bool compararPorTituloPrimario(Filme &a, Filme &b) {
		return a.titulo_primario <= b.titulo_primario;
	}

	static bool compararPorAnoInicial(Filme &a, Filme &b) {
		return a.ano_inicial <= b.ano_inicial;
	}

public:
	/* Lê os dados dos filmes e cinemas */
	void lerDados(void) {
		this->filmes = this->lerFilmes(this->CAMINHO_FILMES);
		this->ind_titulos = this->criarIndice(compararPorTituloPrimario);

		this->cinemas = this->lerCinemas(this->CAMINHO_CINEMAS);
	}
};

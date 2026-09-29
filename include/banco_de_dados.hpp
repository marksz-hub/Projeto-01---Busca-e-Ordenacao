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
#include <string>
#include <vector>

#include "cinema.hpp"
#include "filme.hpp"
#include "leitor.hpp"

struct BancoDeDados {
private:
	/* Tipo para as funções de ordenação da função quicksort */
	typedef bool(ord_t)(Filme &a, Filme &b);

	/* Tipo para as funções de ordenação da função countingsort */
	typedef unsigned(key_t)(Filme &f);

	const std::string CAMINHO_FILMES = "../data/filmes.csv";
	const std::string CAMINHO_CINEMAS = "../data/cinemas.csv";

	std::vector<Filme> filmes;

	unsigned ano_inicial_max;
	unsigned ano_final_max;
	unsigned duracao_max;

	std::vector<unsigned> ind_titulo_primario;
	std::vector<unsigned> ind_titulo_original;
	std::vector<unsigned> ind_ano_inicial;
	std::vector<unsigned> ind_ano_final;
	std::vector<unsigned> ind_duracao;

	std::vector<Cinema> cinemas;

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
		this->ind_titulo_primario
			= this->criarIndiceTitulo(compararPorTituloPrimario);
		this->ind_titulo_original
			= this->criarIndiceTitulo(compararPorTituloOriginal);
		this->ind_ano_inicial
			= this->criarIndiceNumero(ano_inicial_max, obterAnoInicial);
		this->ind_ano_final
			= this->criarIndiceNumero(ano_final_max, obterAnoFinal);
		this->ind_duracao = this->criarIndiceNumero(duracao_max, obterDuracao);
	}

	/* Cria um índice baseado em uma propriedade duma classe */
	std::vector<unsigned> criarIndiceTitulo(ord_t f) {
		std::vector<unsigned> indices(this->filmes.size(), 0);
		std::iota(std::begin(indices), std::end(indices), 0);

		quicksort(indices, 0, indices.size() - 1, f);

		return indices;
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

	static bool compararPorTituloPrimario(Filme &a, Filme &b) {
		return a.titulo_primario <= b.titulo_primario;
	}

	static bool compararPorTituloOriginal(Filme &a, Filme &b) {
		return a.titulo_original <= b.titulo_original;
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

public:
	/* Lê os dados dos filmes e cinemas */
	void lerDados(void) {
		this->filmes = this->lerFilmes(this->CAMINHO_FILMES);
		this->construirIndicesFilmes();

		this->cinemas = this->lerCinemas(this->CAMINHO_CINEMAS);
	}
};

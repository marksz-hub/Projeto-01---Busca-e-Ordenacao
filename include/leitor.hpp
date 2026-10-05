/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Leitores de filmes & cinemas
 *
 * INÍCIO:
 *   2026-09-01
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "cinema.hpp"
#include "filme.hpp"
#include "set.hpp"

class Leitor {
protected:
	/* Remove o espaco que aparece no início dos campos após a separação por
	 * vírgula
	 */
	static void removerEspacoInicial(std::string &texto) {
		if( !texto.empty() && texto[0] == ' ' ) {
			texto.erase(0, 1);
		}
	}
};

class LeitorFilme : Leitor {
private:
	std::string caminho;

public:
	unsigned ano_inicial_max = 0;
	unsigned ano_final_max = 0;
	unsigned duracao_max = 0;

	std::vector<Conjunto> ind_adulto;

	LeitorFilme(std::string caminho)
		: caminho { caminho }
		, ind_adulto(2) { }

	std::vector<Filme> ler(void) {
		std::ifstream arq(this->caminho);
		if( !arq.is_open() ) {
			std::cerr << "nao foi possivel abrir o arquivo: '" << this->caminho
					  << "'!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		std::vector<Filme> filmes;

		/* Pula a primeira linha */
		arq.ignore(1024, '\n');
		while( arq.peek() != EOF ) {
			Filme filme;
			std::string buf, generos;

			std::getline(arq, buf, '\t');
			filme.id = buf.substr(2);

			std::getline(arq, filme.tipo, '\t');
			std::getline(arq, filme.titulo_primario, '\t');
			std::getline(arq, filme.titulo_original, '\t');

			std::getline(arq, buf, '\t');
			filme.adulto = buf == "1";
			this->ind_adulto[filme.adulto].add(filmes.size());

			/* Lê o ano inicial do filme */
			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				filme.ano_inicial = 0;
			} else {
				filme.ano_inicial = std::stoi(buf);
				ano_inicial_max = std::max(ano_inicial_max, filme.ano_inicial);
			}

			/* Lê o ano final do filme */
			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				filme.ano_final = 0;
			} else {
				filme.ano_final = std::stoi(buf);
				ano_final_max = std::max(ano_final_max, filme.ano_final);
			}

			/* Lê a duração do filme */
			std::getline(arq, buf, '\t');
			if( buf == "\\N" ) {
				filme.duracao = 0;
			} else {
				filme.duracao = std::stoi(buf);
				duracao_max = std::max(duracao_max, filme.duracao);
			}

			/* Lê os gêneros */
			std::getline(arq, generos);
			std::istringstream ss { generos };
			while( std::getline(ss, buf, ',') ) {
				/* Gênero "nulo" */
				if( buf == "\\N" ) {
					continue;
				}

				filme.generos.push_back(buf);
			}

			filmes.push_back(filme);
		}

		return filmes;
	}
};

class LeitorCinema : Leitor {
private:
	std::string caminho;

public:
	LeitorCinema(std::string caminho) {
		this->caminho = caminho;
	};

	std::vector<Cinema> ler(void) {
		std::ifstream arq(this->caminho);
		if( !arq.is_open() ) {
			std::cerr << "nao foi possivel abrir o arquivo: '" << this->caminho
					  << "'!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		std::vector<Cinema> cinemas;

		/* Pula a primeira linha */
		arq.ignore(1024, '\n');
		while( arq.peek() != EOF ) {
			Cinema cinema;
			std::string buf, filmes;

			std::getline(arq, buf, ',');
			cinema.id = buf.substr(2);

			std::getline(arq, buf, ',');
			removerEspacoInicial(buf);
			cinema.nome_do_cinema = buf;

			/* Lê as coordenadas e converte seus valores de string para inteiro
			 */
			std::getline(arq, buf, ',');
			cinema.x = std::stoi(buf);
			std::getline(arq, buf, ',');
			cinema.y = std::stoi(buf);

			/* Lê o preço e converte seu valor de string para double */
			std::getline(arq, buf, ',');
			cinema.preco_ingresso = std::stod(buf);

			/* Os campos restantes da linha correspondem aos filmes em exibição,
			 * com a quantidade de filmes (campos) podendo variar entre os
			 * cinemas
			 */
			std::getline(arq, filmes);
			std::istringstream ss { filmes };
			while( std::getline(ss, buf, ',') ) {
				removerEspacoInicial(buf);
				cinema.filmes_em_exibicao.push_back(buf.substr(2));
			}

			cinemas.push_back(cinema);
		}

		return cinemas;
	}
};

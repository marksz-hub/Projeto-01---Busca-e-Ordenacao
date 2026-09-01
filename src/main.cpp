/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Ponto de entrada
 *
 * INÍCIO:
 *   2026-09-01
 *
 * Marcus Vinicius <mvff@aluno.ifnmg.edu.br>
 * Pedro Buitrago <pbns1@aluno.ifnmg.edu.br>
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "filme.hpp"

std::vector<Filme> ler_filmes(std::string caminho);

int main() {
	std::vector<Filme> filmes = ler_filmes("../data/filmes.csv");

	return EXIT_SUCCESS;
}

std::vector<Filme> ler_filmes(std::string caminho) {
	std::ifstream arq(caminho);
	if( !arq.is_open() ) {
		std::cerr << "nao foi possivel abrir o arquivo: '" << caminho << "'!"
				  << std::endl;
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

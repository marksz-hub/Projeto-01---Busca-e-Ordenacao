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
#include <sstream>

#include "filme.hpp"
#include "cinema.hpp"

std::vector<Filme> ler_filmes(std::string caminho);
std::vector<Cinema> ler_cinemas(std::string caminho);

int main() {
	std::vector<Filme> filmes = ler_filmes("../data/filmes.csv");
	std::vector<Cinema> cinemas = ler_cinemas("../data/cinemas.csv");

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






/* LEITURA DO ARQUIVO DE CINEMAS */


void removerEspacoInicial(std::string &texto);


std::vector<Cinema> ler_cinemas(std::string caminho){
	/* Abre o arquivo de cinemas para leitura */
    std::ifstream arq(caminho);
    if ( !arq.is_open() ){
        std::cerr << "nao foi possivel abrir o arquivo: '" << caminho << "'!"
			 	  << std::endl;
		std::exit(EXIT_FAILURE);
    }


	std::vector<Cinema> cinemas;
	std::string linha;

	/* Pula a primeira linha */
	std::getline(arq, linha); 

	while (std::getline(arq, linha)){

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

		/* Lê as coordenadas e converte seus valores de string para inteiro */
		std::getline(ss, buf, ',');
		x = std::stoi(buf);
		std::getline(ss, buf, ',');
		y = std::stoi(buf);

		/* Lê o preço e converte seu valor de string para double */
		std::getline(ss, buf, ',');
		preco_ingresso = std::stod(buf);

		/* Os campos restantes da linha correspondem aos filmes em exibição,
		com a quantidade de filmes (campos) podendo variar entre os cinemas */
		while (std::getline(ss, buf, ',')) {
			removerEspacoInicial(buf);
			filmes_em_exibicao.push_back(buf);
		}

		/* Cria o objeto cinema com os dados lidos da linha */
		Cinema cinema = Cinema(id, nome_do_cinema, x, y, 
			preco_ingresso, filmes_em_exibicao);
		/* Adiciona o cinema ao vetor */
		cinemas.push_back(cinema);
	}

	return cinemas;
}
	


/* Remove o espaco que aparece no início dos campos
   após a separação por vírgula  */
void removerEspacoInicial(std::string &texto){
	if (!texto.empty()  &&  texto[0] == ' '){
		texto.erase(0, 1);
	}
}

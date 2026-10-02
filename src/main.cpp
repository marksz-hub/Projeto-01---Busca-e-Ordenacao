/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Ponto de entrada
 *
 * INÍCIO:
 *   2026-09-01
 */

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#include "banco_de_dados.hpp"
#include "consulta.hpp"
#include "filme.hpp"

void cmdBuscarFilme(BancoDeDados &bd);
bool cmdBuscarFilmeAdicionarFiltro(Consulta &consulta);

void cmdBuscarCinema(BancoDeDados &bd);

int main() {
	BancoDeDados bd {};

	std::cout << "- - - - - - - - - - - - - - -" << std::endl;
	std::cout << "  BUSCA de FILMES e CINEMAS" << std::endl;
	std::cout << "- - - - - - - - - - - - - - -" << std::endl;
	std::cout << std::endl;

	std::cout << "# Carregando dados..." << std::endl;
	bd.lerDados();
	std::cout << "# Pronto!" << std::endl;
	std::cout << std::endl;

	bool rodando = true;
	while( rodando ) {
		unsigned cmd;

		std::cout << "# COMANDOS" << std::endl;
		std::cout << "  1 Buscar filme" << std::endl;
		std::cout << "  2 Buscar cinema" << std::endl;
		std::cout << "  0 Sair" << std::endl;
		std::cout << "  > " << std::flush;

		std::cin >> cmd;

		std::cout << std::endl;
		switch( cmd ) {
		case 1:
			cmdBuscarFilme(bd);
			break;
		case 2:
			cmdBuscarCinema(bd);
			break;
		case 0:
			rodando = false;
			break;
		default:
			std::cout << "Comando invalido, tente novamente:" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

void cmdBuscarFilme(BancoDeDados &bd) {
	Consulta consulta;

	bool rodando = true;
	while( rodando ) {
		if( !cmdBuscarFilmeAdicionarFiltro(consulta) ) {
			std::cout << "# BUSCA CANCELADA!" << std::endl;
			return;
		}

		unsigned cmd;
		std::cout << "# OPERADOR" << std::endl;
		std::cout << "  1 E ..... (AND)" << std::endl;
		std::cout << "  2 OU .... (OR)" << std::endl;
		std::cout << "  3 NAO ... (NOT)" << std::endl;
		std::cout << "  0 Finalizar filtro e pesquisar" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> cmd;

		switch( cmd ) {
		case 1:
			consulta.adicionarOperadorE();
			break;
		case 2:
			consulta.adicionarOperadorOu();
			break;
		case 3:
			consulta.adicionarOperadorNao();
			break;
		case 0:
			rodando = false;
			break;
		}
	}

	if( consulta.raiz == nullptr ) {
		std::cout << "# BUSCA VAZIA! Cancelando..." << std::endl;
		return;
	}

	for( const Filme &f : bd.consultarFilme(consulta) ) {
		std::cout << f << std::endl;
	}

	std::cout << std::endl;
}

bool cmdBuscarFilmeAdicionarFiltro(Consulta &consulta) {
	unsigned cmd;

	std::cout << "# ADICIONANDO FILTROS" << std::endl;
	std::cout << "  1 Adicionar filtro 'titulo'" << std::endl;
	std::cout << "  2 Adicionar filtro 'adulto'" << std::endl;
	std::cout << "  3 Adicionar filtro 'ano'" << std::endl;
	std::cout << "  4 Adicionar filtro 'a partir do ano...'" << std::endl;
	std::cout << "  5 Adicionar filtro 'ate o ano...'" << std::endl;
	std::cout << "  6 Adicionar filtro 'duracao'" << std::endl;
	std::cout << "  7 Adicionar filtro 'genero'" << std::endl;
	std::cout << "	8 Adicionar filtro 'tipo'" << std::endl;
	std::cout << "  0 Cancelar" << std::endl;
	std::cout << "  > " << std::flush;
	std::cin >> cmd;

	switch( cmd ) {
	case 1: {
		std::string titulo;
		std::cout << "+ FILTRAR por 'TITULO' (digite titulo)" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, titulo);

		consulta.adicionarBuscaPorTitulo(titulo);
	} break;
	case 2: {
		unsigned adulto;
		std::cout << "+ FILTRAR por 'ADULTO' (digite 1 ou 0)" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> adulto;

		consulta.adicionarBuscaPorAdulto(adulto == 1);
	} break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7: {
		std::string genero;
		std::cout << "+ FILTRAR por 'GENERO' (digite genero)" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, genero);

		consulta.adicionarBuscaPorGenero(genero);
	} break;
	case 8: {
		std::string tipo;
		std::cout << "+ FILTRAR por 'TIPO' (digite tipo)" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, tipo);

		consulta.adicionarBuscaPorTipo(tipo);
	} break;
	case 0:
		return false;
	}

	return true;
}

void cmdBuscarCinema(BancoDeDados &bd) {
	(void)bd;
}

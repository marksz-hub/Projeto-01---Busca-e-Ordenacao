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
#include <vector>

#include "banco_de_dados.hpp"
#include "consulta.hpp"
#include "filme.hpp"

void cmdBuscarFilme(BancoDeDados &bd);
bool cmdBuscarFilmeAdicionarFiltro(BancoDeDados &bd, Consulta &consulta);
void cmdPaginarConsultaFilme(BancoDeDados &bd, const Consulta &consulta);

void cmdBuscarCinema(BancoDeDados &bd);
bool cmdBuscarCinemaAdicionarFiltro(const BancoDeDados &bd, Consulta &consulta);
void cmdPaginarConsultaCinema(BancoDeDados &bd, const Consulta &consulta);

void imprimirGeneros(const BancoDeDados &bd);
void imprimirTipos(const BancoDeDados &bd);

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
		if( !cmdBuscarFilmeAdicionarFiltro(bd, consulta) ) {
			std::cout << "# BUSCA CANCELADA!" << std::endl;
			return;
		}

		unsigned cmd;
		std::cout << "# OPERADOR" << std::endl;
		std::cout << "  1 E ..... (AND)" << std::endl;
		std::cout << "  2 OU .... (OR)" << std::endl;
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
		case 0:
			rodando = false;
			break;
		}
	}

	if( consulta.raiz == nullptr ) {
		std::cout << "# BUSCA VAZIA! Cancelando..." << std::endl;
		return;
	}

	std::cout << std::endl;
	cmdPaginarConsultaFilme(bd, consulta);
}

bool cmdBuscarFilmeAdicionarFiltro(BancoDeDados &bd, Consulta &consulta) {
	bool negar = false;
	while( true ) {
		unsigned cmd;

		if( negar ) {
			std::cout << "# NEGANDO FILTRO" << std::endl;
		} else {
			std::cout << "# ADICIONANDO FILTROS" << std::endl;
		}
		std::cout << "  1 Adicionar filtro 'titulo'" << std::endl;
		std::cout << "  2 Adicionar filtro 'adulto'" << std::endl;
		std::cout << "  3 Adicionar filtro 'ano'" << std::endl;
		std::cout << "  4 Adicionar filtro 'a partir do ano...'" << std::endl;
		std::cout << "  5 Adicionar filtro 'ate o ano...'" << std::endl;
		std::cout << "  6 Adicionar filtro 'duracao'" << std::endl;
		std::cout << "  7 Adicionar filtro 'genero'" << std::endl;
		std::cout << "  8 Adicionar filtro 'tipo'" << std::endl;
		if( negar ) {
			std::cout << "  9 Filtro normal (nao negar)" << std::endl;
		} else {
			std::cout << "  9 Negar filtro (NAO)" << std::endl;
		}
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

			consulta.adicionarBuscaPorTitulo(titulo, negar);
		} break;
		case 2: {
			unsigned adulto;
			std::cout << "+ FILTRAR por 'ADULTO' (digite 1 ou 0)" << std::endl;
			std::cout << "  > " << std::flush;
			std::cin >> adulto;

			consulta.adicionarBuscaPorAdulto(adulto == 1, negar);
		} break;
		case 3:
			std::cout << "NAO IMPLEMENTADO" << std::endl;
			continue;
		case 4:
			std::cout << "NAO IMPLEMENTADO" << std::endl;
			continue;
		case 5:
			std::cout << "NAO IMPLEMENTADO" << std::endl;
			continue;
		case 6:
			std::cout << "NAO IMPLEMENTADO" << std::endl;
			continue;
		case 7: {
			std::string genero;

			std::cout << "+ FILTRAR por 'GENERO' (digite genero)" << std::endl;
			imprimirGeneros(bd);
			std::cout << std::endl;
			std::cout << "  > " << std::flush;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, genero);

			consulta.adicionarBuscaPorGenero(genero, negar);
		} break;
		case 8: {
			std::string tipo;

			std::cout << "+ FILTRAR por 'TIPO' (digite tipo)" << std::endl;
			imprimirTipos(bd);

			std::cout << std::endl;
			std::cout << "  > " << std::flush;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, tipo);

			consulta.adicionarBuscaPorTipo(tipo, negar);
		} break;
		case 9:
			negar = not negar;
			continue;
		case 0:
			return false;
		default:
			std::cout << "Comando invalido, tente novamente:" << std::endl;
			continue;
		}

		return true;
	}
}

void cmdPaginarConsultaFilme(BancoDeDados &bd, const Consulta &consulta) {
	unsigned pagina = 0, total;

	bool rodando = true;
	while( rodando ) {
		unsigned cmd;

		std::vector<Filme *> filmes
			= bd.consultarFilme(consulta, pagina, total);
		for( const Filme *f : filmes ) {
			std::cout << *f << std::endl;
		}

		if( total <= 50 ) {
			std::cout << std::endl;
			return;
		}

		const unsigned maxPaginas = total / bd.getFilmesPorPagina();

		std::cout << std::endl;
		std::cout << "PAGINA: " << (pagina + 1) << " de " << maxPaginas
				  << " | TOTAL: " << total << std::endl;

		std::cout << "# ACAO" << std::endl;
		std::cout << "  1 Proxima pagina" << std::endl;
		std::cout << "  2 Pagina anterior" << std::endl;
		std::cout << "  0 Encerrar" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> cmd;

		switch( cmd ) {
		case 1:
			if( pagina < maxPaginas - 1 ) {
				++pagina;
			}
			break;
		case 2:
			if( pagina > 0 ) {
				--pagina;
			}
			break;
		case 0:
			rodando = false;
			break;
		}
	}

	std::cout << std::endl;
}

void cmdBuscarCinema(BancoDeDados &bd) {
	Consulta consulta;

	bool rodando = true;
	while( rodando ) {
		if( !cmdBuscarCinemaAdicionarFiltro(bd, consulta) ) {
			std::cout << "# BUSCA CANCELADA!" << std::endl;
			return;
		}

		unsigned cmd;
		std::cout << "# OPERADOR" << std::endl;
		std::cout << "  1 E ..... (AND)" << std::endl;
		std::cout << "  2 OU .... (OR)" << std::endl;
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
		case 0:
			rodando = false;
			break;
		}
	}

	if( consulta.raiz == nullptr ) {
		std::cout << "# BUSCA VAZIA! Cancelando..." << std::endl;
		return;
	}

	std::cout << std::endl;
	cmdPaginarConsultaCinema(bd, consulta);
}

bool cmdBuscarCinemaAdicionarFiltro(
	const BancoDeDados &bd, Consulta &consulta) {
	bool negar = false;
	while( true ) {
		unsigned cmd;

		if( negar ) {
			std::cout << "# NEGANDO FILTRO" << std::endl;
		} else {
			std::cout << "# ADICIONANDO FILTROS" << std::endl;
		}
		std::cout << "  1 Adicionar filtro 'genero'" << std::endl;
		std::cout << "  2 Adicionar filtro 'tipo'" << std::endl;
		if( negar ) {
			std::cout << "  9 Filtro normal (nao negar)" << std::endl;
		} else {
			std::cout << "  9 Negar filtro (NAO)" << std::endl;
		}
		std::cout << "  0 Cancelar" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> cmd;

		switch( cmd ) {
		case 1: {
			std::string genero;
			std::cout << "+ FILTRAR por 'GENERO' (digite genero)" << std::endl;
			imprimirGeneros(bd);
			std::cout << std::endl;
			std::cout << "  > " << std::flush;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, genero);

			consulta.adicionarBuscaPorGenero(genero, negar);
		} break;
		case 2: {
			std::string tipo;

			std::cout << "+ FILTRAR por 'TIPO' (digite tipo)" << std::endl;
			imprimirTipos(bd);

			std::cout << std::endl;
			std::cout << "  > " << std::flush;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, tipo);

			consulta.adicionarBuscaPorTipo(tipo, negar);
		} break;
		case 9:
			negar = not negar;
			continue;
		case 0:
			return false;
		default:
			std::cout << "Comando invalido, tente novamente:" << std::endl;
			continue;
		}

		return true;
	}
}

void cmdPaginarConsultaCinema(BancoDeDados &bd, const Consulta &consulta) {
	unsigned pagina = 0, total;

	bool rodando = true;
	while( rodando ) {
		unsigned cmd;

		std::vector<Cinema *> cinemas
			= bd.consultarCinema(consulta, pagina, total);
		for( const Cinema *c : cinemas ) {
			std::cout << *c << std::endl;
		}

		if( total <= 50 ) {
			std::cout << std::endl;
			return;
		}

		const unsigned maxPaginas = total / bd.getCinemasPorPagina();

		std::cout << std::endl;
		std::cout << "PAGINA: " << (pagina + 1) << " de " << maxPaginas
				  << " | TOTAL: " << total << std::endl;

		std::cout << "# ACAO" << std::endl;
		std::cout << "  1 Proxima pagina" << std::endl;
		std::cout << "  2 Pagina anterior" << std::endl;
		std::cout << "  0 Encerrar" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> cmd;

		switch( cmd ) {
		case 1:
			if( pagina < maxPaginas - 1 ) {
				++pagina;
			}
			break;
		case 2:
			if( pagina > 0 ) {
				--pagina;
			}
			break;
		case 0:
			rodando = false;
			break;
		}
	}

	std::cout << std::endl;
}

void imprimirGeneros(const BancoDeDados &bd) {
	unsigned i;

	std::cout << "GENEROS:\n  " << std::flush;
	for( i = 1; i <= bd.generos.size(); ++i ) {
		std::cout << bd.generos[i - 1];
		if( i % 6 == 0 ) {
			std::cout << std::endl;
		}

		std::cout << "  " << std::flush;
	}
}

void imprimirTipos(const BancoDeDados &bd) {
	unsigned i;

	std::cout << "TIPOS:\n  " << std::flush;
	for( i = 0; i < bd.tipos.size(); ++i ) {
		std::cout << bd.tipos[i];
		if( i < bd.tipos.size() - 1 ) {
			std::cout << "  " << std::flush;
		}
	}
}

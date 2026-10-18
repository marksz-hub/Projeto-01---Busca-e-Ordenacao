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

void cmdDesenharMapaDeCinemas(const BancoDeDados &bd);

void limparTela(void);

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
		std::cout << "  3 Mapa de cinemas" << std::endl;
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
		case 3:
			cmdDesenharMapaDeCinemas(bd);
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
		std::cout << "  3 Adicionar filtro 'ano de lancamento'" << std::endl;
		std::cout << "  4 Adicionar filtro 'duracao'" << std::endl;
		std::cout << "  5 Adicionar filtro 'genero'" << std::endl;
		std::cout << "  6 Adicionar filtro 'tipo'" << std::endl;
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
		case 3: {
		    unsigned opcaoAno;
		    std::cout << "+ FILTRAR por 'ANO DE LANCAMENTO'" << std::endl;
		    std::cout << "  1 Ano especifico de lancamento (filmes ou series lancados em um ano especifico)" << std::endl;
		    std::cout << "  2 Intervalo de anos de lancamento (filmes ou series lancados em um certo intervalo de anos)" << std::endl;
			std::cout << "	3 Ano minimo de lancamento (filmes ou series lancados a partir de certo ano)" << std::endl;
			std::cout << "	4 Ano maximo de lancamento (filmes ou series lancada ate certo ano)" << std::endl;
		    std::cout << "  0 Voltar" << std::endl;
		    std::cout << "  > " << std::flush;
		    std::cin >> opcaoAno;
		
		    switch (opcaoAno) {
		    case 1: {
		        unsigned ano;
		
		        std::cout << "+ FILTRAR por 'ANO ESPECIFICO'" << std::endl;
		        std::cout << "  Ano de lancamento > " << std::flush;
		        std::cin >> ano;
		
		        if (ano == 0) {
		            std::cout << "Ano invalido!" << std::endl;
		            return true;
		        }
		
		        consulta.adicionarBuscaPorIntervaloAno(ano, ano, negar);
		    } break;
		    case 2: {
		        unsigned minimo, maximo;
		
		        do {
		            std::cout << "+ FILTRAR por 'INTERVALO DE ANOS'" << std::endl;
		            std::cout << "  De: " << std::flush;
		            std::cin >> minimo;
		            std::cout << "  Ate: " << std::flush;
		            std::cin >> maximo;
		
		            if (minimo == 0 || minimo > maximo) {
		                std::cout << "Intervalo invalido! "
		                          << "O ano inicial deve ser positivo "
		                          << "e menor ou igual ao ano final."
		                          << std::endl;
		            }
		        } while (minimo == 0 || minimo > maximo);
		
		        consulta.adicionarBuscaPorIntervaloAno(minimo, maximo, negar);
		    } break;
			case 3: {
				unsigned minimo;
				do{
					std::cout << "+ FILTRAR por 'ANO MINIMO DE LANCAMENTO'" <<std::endl;
					std::cout << " De: > " << std::flush;
					std::cin >> minimo;

					if(minimo == 0 || minimo > bd.getAnoInicialMaximo()) {
						std::cout << "Ano invalido! "
								  << "O ano minimo de lancamento deve ser positivo "
								  << "e menor ou igual a " 
								  << bd.getAnoInicialMaximo()
								  << std::endl;
					}

				} while(minimo == 0 || minimo > bd.getAnoInicialMaximo());
		        consulta.adicionarBuscaPorIntervaloAno(minimo, bd.getAnoInicialMaximo(), negar);
			} break;
			case 4: {
				unsigned maximo;
				do{
					std::cout << "+ FILTRAR por 'ANO MAXIMO DE LANCAMENTO'" <<std::endl;
					std::cout << " Ate: > " << std::flush;
					std::cin >> maximo;

					if(maximo == 0 || maximo > bd.getAnoInicialMaximo()) {
						std::cout << "Ano invalido! "
								  << "O ano maximo de lancamento deve ser positivo "
								  << "e menor ou igual a " 
								  << bd.getAnoInicialMaximo()
								  << std::endl;
					}		

				} while(maximo == 0 || maximo > bd.getAnoInicialMaximo());
		        consulta.adicionarBuscaPorIntervaloAno(1, maximo, negar);
			} break;
		
		    case 0:
		        continue;
		
		    default:
		        std::cout << "Opcao invalida!" << std::endl;
		        continue;
		    }
		} break;
			
		case 4: {
			unsigned opcaoDuracao;
	
			std::cout << "+ FILTRAR por 'DURACAO'" << std::endl;
			std::cout << "  1 Duracao minima" << std::endl;
			std::cout << "  2 Duracao maxima" << std::endl;
			std::cout << "	3 Duracao exata" << std::endl;
			std::cout << "	4 Intervalo de duracao" << std::endl;
			std::cout << "  0 Cancelar busca" << std::endl;
			std::cout << "  > " << std::flush;
	
			std::cin >> opcaoDuracao;
	
			switch (opcaoDuracao) {
			case 1: {
				unsigned minimo;
				std::cout << "+ FILTRAR por 'DURACAO MINIMA' (em minutos)" <<std::endl;
				std::cout << " > " << std::flush;
				std::cin >> minimo;
				consulta.adicionarBuscaPorIntervaloDuracao(minimo, bd.getDuracaoMaxima(), negar);
			} break;
			case 2: {
				unsigned maximo;
				std::cout << "+ FILTRAR por 'DURACAO MAXIMA' (em minutos)" <<std::endl;
				std::cout << " > " << std::flush;
				std::cin >> maximo;
				consulta.adicionarBuscaPorIntervaloDuracao(1, maximo, negar);
			} break;
			case 3: {
				unsigned duracao;
				std::cout << "+ FILTRAR por 'DURACAO EXATA' (em minutos)" <<std::endl;
				std::cout << " Duracao minima > " << std::flush;
				std::cin >> duracao;
				consulta.adicionarBuscaPorIntervaloDuracao(duracao, duracao, negar);
			} break;
			case 4: {
				unsigned minimo, maximo;
				do {
					std::cout << "+ FILTRAR por 'INTERVALO DE DURACAO'(minimo e maximo em minutos)" << std::endl;
					std::cout << " Duracao minima > " << std::flush;
					std::cin >> minimo;
					std::cout << " Duracao maxima > " << std::flush;
					std::cin >> maximo;
					if(minimo > maximo) {
						std::cout << "Intervalo invalido (o minimo deve ser menor ou igual ao maximo)." << std::endl;
					} 
				} while(minimo > maximo);
				consulta.adicionarBuscaPorIntervaloDuracao(minimo, maximo, negar);
			} break;
			case 0: 
				return false;
			default: 
				std::cout << "Opcao invalida!" << std::endl;
				return true;	
		} break;
			
		case 5: {
			std::string genero;

			std::cout << "+ FILTRAR por 'GENERO' (digite genero)" << std::endl;
			imprimirGeneros(bd);
			std::cout << std::endl;
			std::cout << "  > " << std::flush;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, genero);

			consulta.adicionarBuscaPorGenero(genero, negar);
		} break;
		case 6: {
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
		std::cout << "  3 Adicionar filtro 'preco'" << std::endl;
		std::cout << "  4 Adicionar filtro 'localizacao'" << std::endl;
		std::cout << "	5 Adicionar filtro 'ano de lancamento'" << std::endl;
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
		case 3: {
			double preco;

			std::cout << "+ FILTRAR por 'PRECO' (digite preco maximo)"
					  << std::endl;
			std::cout << "> R$" << std::flush;
			std::cin >> preco;

			consulta.adicionarBuscaPorPreco(preco, negar);
		} break;
		case 4: {
			Loc loc;

			std::cout << "+ FILTRAR por 'LOCALIZACAO'" << std::endl;
			std::cout << "  1 Coordenada X: " << std::endl;
			std::cin >> loc.x;
			std::cout << "  2 Coordenada Y: " << std::endl;
			std::cin >> loc.y;
			std::cout << "  3 Raio da busca: " << std::endl;
			std::cin >> loc.raio;

			consulta.adicionarBuscaPorLocalizacao(loc, negar);

		} break;
		case 5: {
			unsigned opcaoAno;
		    std::cout << "+ FILTRAR por 'ANO DE LANCAMENTO'" << std::endl;
		    std::cout << "  1 Ano especifico de lancamento (filmes ou series lancados em um ano especifico)" << std::endl;
		    std::cout << "  2 Intervalo de anos de lancamento (filmes ou series lancados em um certo intervalo de anos)" << std::endl;
			std::cout << "	3 Ano minimo de lancamento (filmes ou series lancados a partir de certo ano)" << std::endl;
			std::cout << "	4 Ano maximo de lancamento (filmes ou series lancada ate certo ano)" << std::endl;
		    std::cout << "  0 Voltar" << std::endl;
		    std::cout << "  > " << std::flush;
		    std::cin >> opcaoAno;
		
		    switch (opcaoAno) {
			case 1: {
		        unsigned ano;
		
		        std::cout << "+ FILTRAR por 'ANO ESPECIFICO'" << std::endl;
		        std::cout << "  Ano de lancamento > " << std::flush;
		        std::cin >> ano;
		
		        if (ano == 0) {
		            std::cout << "Ano invalido!" << std::endl;
		            return true;
		        }
		
		        consulta.adicionarBuscaPorIntervaloAno(ano, ano, negar);
		    } break;
		    case 2: {
		        unsigned minimo, maximo;
		
		        do {
		            std::cout << "+ FILTRAR por 'INTERVALO DE ANOS'" << std::endl;
		            std::cout << "  De: " << std::flush;
		            std::cin >> minimo;
		            std::cout << "  Ate: " << std::flush;
		            std::cin >> maximo;
		
		            if (minimo == 0 || minimo > maximo) {
		                std::cout << "Intervalo invalido! "
		                          << "O ano inicial deve ser positivo "
		                          << "e menor ou igual ao ano final."
		                          << std::endl;
		            }
		        } while (minimo == 0 || minimo > maximo);
		
		        consulta.adicionarBuscaPorIntervaloAno(minimo, maximo, negar);
		    } break;
			case 3: {
				unsigned minimo;
				do{
					std::cout << "+ FILTRAR por 'ANO MINIMO DE LANCAMENTO'" <<std::endl;
					std::cout << " De: > " << std::flush;
					std::cin >> minimo;

					if(minimo == 0 || minimo > bd.getAnoInicialMaximo()) {
						std::cout << "Ano invalido! "
								  << "O ano minimo de lancamento deve ser positivo "
								  << "e menor ou igual a "
								  << bd.getAnoInicialMaximo()
								  << std::endl;
					}

				} while(minimo == 0 || minimo > bd.getAnoInicialMaximo());
		        consulta.adicionarBuscaPorIntervaloAno(minimo, bd.getAnoInicialMaximo(), negar);
			} break;
			case 4: {
				unsigned maximo;
				do{
					std::cout << "+ FILTRAR por 'ANO MAXIMO DE LANCAMENTO'" <<std::endl;
					std::cout << " Ate: > " << std::flush;
					std::cin >> maximo;

					if(maximo == 0 || maximo > bd.getAnoInicialMaximo()) {
						std::cout << "Ano invalido! "
								  << "O ano maximo de lancamento deve ser positivo "
								  << "e menor ou igual a " 
								  << bd.getAnoInicialMaximo()
								  << std::endl;
					}		

				} while(maximo == 0 || maximo > bd.getAnoInicialMaximo());
		        consulta.adicionarBuscaPorIntervaloAno(1, maximo, negar);
			} break;
		
		    case 0:
		        continue;
		
		    default:
		        std::cout << "Opcao invalida!" << std::endl;
		        continue;
		    }
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

void cmdDesenharMapaDeCinemas(const BancoDeDados &bd) {
	unsigned x = 0, y = 0, zoom = 0;

	bool rodando = true;
	while( rodando ) {
		unsigned cmd;

		bd.desenharMapaDeCinemas(x, y, zoom);

		std::cout << "# NAVEGACAO:" << std::endl;
		std::cout << "  1 Mover pra esquerda" << std::endl;
		std::cout << "  2 Mover pra direita" << std::endl;
		std::cout << "  3 Mover pra cima" << std::endl;
		std::cout << "  4 Mover pra baixo" << std::endl;
		std::cout << "  5 Mais zoom" << std::endl;
		std::cout << "  6 Menos zoom" << std::endl;
		std::cout << "  0 Sair" << std::endl;
		std::cout << "  > " << std::flush;
		std::cin >> cmd;

		switch( cmd ) {
		case 1:
			if( x > 0 ) {
				--x;
			}
			break;
		case 2:
			if( x < zoom ) {
				++x;
			}
			break;
		case 3:
			if( y > 0 ) {
				--y;
			}
			break;
		case 4:
			if( y < zoom ) {
				++y;
			}
			break;
		case 5:
			if( zoom < 7 ) {
				++zoom;
			}
			break;
		case 6:
			if( zoom > 0 ) {
				--zoom;
				x = std::min(x, zoom - 1);
				y = std::min(y, zoom - 1);
			}
			break;
		case 0:
			rodando = false;
			break;
		}
	}

	std::cout << std::endl;
}

void limparTela(void) {
	std::cout << "\x1B[2J\x1B[H";
}

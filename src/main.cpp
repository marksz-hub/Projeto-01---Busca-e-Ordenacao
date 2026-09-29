/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Ponto de entrada
 *
 * INÍCIO:
 *   2026-09-01
 */

#include <cstdlib>
#include <ostream>

#include "banco_de_dados.hpp"
#include "consulta.hpp"
#include "filme.hpp"

BancoDeDados bd {};

int main() {
	bd.lerDados();

	Consulta consulta;
	consulta.raiz = new ConsultaNo("Aftermath");

	for( const Filme &f : bd.consultarFilme(consulta) ) {
		std::cout << f.tipo << " " << f.titulo_original << std::endl;
	}

	return EXIT_SUCCESS;
}

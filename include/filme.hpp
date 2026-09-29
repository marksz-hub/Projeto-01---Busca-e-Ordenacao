/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Filmes
 *
 * INÍCIO:
 *   2026-09-01
 */

#pragma once

#include <string>
#include <vector>

struct Filme {
	std::string id;
	std::string tipo;
	std::string titulo_primario;
	std::string titulo_original;
	std::string pesquisavel;
	bool adulto;
	unsigned ano_inicial;
	unsigned ano_final;
	unsigned duracao;
	std::vector<std::string> generos;
};

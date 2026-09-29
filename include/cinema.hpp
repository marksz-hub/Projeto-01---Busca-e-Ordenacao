/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Cinema
 *
 * INÍCIO:
 *   2026-09-01
 */

#pragma once

#include <string>
#include <vector>

struct Cinema {
	std::string id;
	std::string nome_do_cinema;
	int x;
	int y;
	double preco_ingresso;
	std::vector<std::string> filmes_em_exibicao;
};

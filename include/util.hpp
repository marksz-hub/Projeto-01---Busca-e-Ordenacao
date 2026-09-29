/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Utilidades
 *
 * INICIO:
 *   2026-09-29
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <locale>
#include <string>

#define FNV1A_OFFSET_BASIS 14695981039346656037U
#define FNV1A_PRIME 1099511628211U

/* Algoritmo FNV-1a
 *
 * Relativamente rápido (veloz o suficiente para nosso caso) e muito
 * simples de se implementar
 */
inline uint64_t computarHash(const std::string &str) {
	uint64_t h = FNV1A_OFFSET_BASIS;
	for( char c : str ) {
		h ^= c;
		h *= FNV1A_PRIME;
	}

	return h;
}

/* Torna uma string "pesquisável" */
inline void criarPesquisavel(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
		return std::tolower(c, std::locale("pt_BR.utf8"));
	});
}

/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Benchmarking
 *
 * INÍCIO:
 *   2026-10-19
 */

#include <cstddef>
#include <cstdlib>
#include <ctime>

#include <benchmark/benchmark.h>
#include <string>

#include "banco_de_dados.hpp"
#include "consulta.hpp"

static void BM_LerDados(benchmark::State &state) {
	for( auto _ : state ) {
		BancoDeDados bd {};
		bd.lerDados();
	}
}

static void BM_BuscaPorTitulo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	Consulta c;
	c.adicionarBuscaPorTitulo("Aftermath", false);

	for( auto _ : state ) {
		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorTituloAleatorio(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	const std::string CHARS
		= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;

		std::string titulo;
		titulo.resize(8);

		srand(time(NULL));
		for( unsigned i = 0; i < 8; i++ ) {
			titulo[i] = CHARS[rand() % CHARS.length()];
		}

		c.adicionarBuscaPorTitulo(titulo, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

BENCHMARK(BM_LerDados);
BENCHMARK(BM_BuscaPorTitulo);
BENCHMARK(BM_BuscaPorTituloAleatorio);

BENCHMARK_MAIN();

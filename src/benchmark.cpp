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
#include <utility>
#include <vector>

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

static void BM_BuscaPorClassificacaoIndicativaAdulto(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	Consulta c;
	c.adicionarBuscaPorAdulto(true, false);

	for( auto _ : state ) {
		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorClassificacaoIndicativaNaoAdulto(
	benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	Consulta c;
	c.adicionarBuscaPorAdulto(false, false);

	for( auto _ : state ) {
		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorClassificacaoIndicativaAleatoria(
	benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		c.adicionarBuscaPorAdulto(rand() % 2, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorAnoEspecifico(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));
	for( auto _ : state ) {
		Consulta c;
		unsigned ano = (rand() % 1026) + 1900;
		c.adicionarBuscaPorIntervaloAno(ano, ano, false);
	}
}

static void BM_BuscaPorAnoIntervalo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));
	for( auto _ : state ) {
		Consulta c;
		unsigned ano_inicial = (rand() % 1026) + 1900;
		unsigned ano_final = (rand() % 1026) + 1900;
		if( ano_final < ano_inicial ) {
			std::swap(ano_inicial, ano_final);
		} else if( ano_inicial == ano_final ) {
			ano_final += rand() % 10;
		}

		c.adicionarBuscaPorIntervaloAno(ano_inicial, ano_final, false);
	}
}

static void BM_BuscaPorAnoMinimo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		unsigned ano = (rand() % 1026) + 1900;
		c.adicionarBuscaPorIntervaloAno(ano, 2026, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorAnoMaximo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		unsigned ano = (rand() % 1026) + 1900;
		c.adicionarBuscaPorIntervaloAno(1, ano, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorDuracaoEspecifico(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));
	for( auto _ : state ) {
		Consulta c;
		unsigned duracao = (rand() % 51420);
		c.adicionarBuscaPorIntervaloDuracao(duracao, duracao, false);
	}
}

static void BM_BuscaPorDuracaoIntervalo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));
	for( auto _ : state ) {
		Consulta c;
		unsigned duracao_inicial = (rand() % 51420);
		unsigned duracao_final = (rand() % 51420);
		if( duracao_final < duracao_inicial ) {
			std::swap(duracao_inicial, duracao_final);
		} else if( duracao_inicial == duracao_final ) {
			duracao_final += rand() % 10;
		}

		std::cout << duracao_inicial << " " << duracao_final << std::endl;
		c.adicionarBuscaPorIntervaloDuracao(
			duracao_inicial, duracao_final, false);
	}
}

static void BM_BuscaPorDuracaoMinimo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		unsigned duracao = (rand() % 51420);
		c.adicionarBuscaPorIntervaloDuracao(duracao, 51420, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorDuracaoMaximo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		unsigned duracao = (rand() % 51420);
		c.adicionarBuscaPorIntervaloDuracao(0, duracao, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorGenero(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	const std::vector<std::string> GENEROS = {
		"Western",
		"Mystery",
		"Sport",
		"Game-Show",
		"Drama",
		"History",
		"Crime",
		"Sci-Fi",
		"Family",
		"Musical",
		"News",
		"Fantasy",
		"Reality-TV",
		"Horror",
		"Action",
		"Comedy",
		"Documentary",
		"Music",
		"War",
		"Adventure",
		"Thriller",
		"Short",
		"Adult",
		"Romance",
		"Talk-Show",
		"Animation",
		"Biography",
	};

	for( auto _ : state ) {
		Consulta c;
		const std::string &GENERO = GENEROS[rand() % GENEROS.size()];
		c.adicionarBuscaPorGenero(GENERO, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaPorTipo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	const std::vector<std::string> TIPOS = {
		"tvMiniSeries",
		"movie",
		"tvSeries",
		"tvMovie",
		"tvShort",
		"tvEpisode",
		"videoGame",
		"short",
		"video",
		"tvSpecial",
	};

	for( auto _ : state ) {
		Consulta c;
		const std::string &TIPO = TIPOS[rand() % TIPOS.size()];
		c.adicionarBuscaPorTipo(TIPO, false);

		unsigned total;
		bd.consultarFilme(c, 0, total);
	}
}

static void BM_BuscaCinemaPorGenero(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	const std::vector<std::string> GENEROS = {
		"Western",
		"Mystery",
		"Sport",
		"Game-Show",
		"Drama",
		"History",
		"Crime",
		"Sci-Fi",
		"Family",
		"Musical",
		"News",
		"Fantasy",
		"Reality-TV",
		"Horror",
		"Action",
		"Comedy",
		"Documentary",
		"Music",
		"War",
		"Adventure",
		"Thriller",
		"Short",
		"Adult",
		"Romance",
		"Talk-Show",
		"Animation",
		"Biography",
	};

	for( auto _ : state ) {
		Consulta c;
		const std::string &GENERO = GENEROS[rand() % GENEROS.size()];
		c.adicionarBuscaPorGenero(GENERO, false);

		unsigned total;
		bd.consultarCinema(c, 0, total);
	}
}

static void BM_BuscaCinemaPorTipo(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	const std::vector<std::string> TIPOS = {
		"tvMiniSeries",
		"movie",
		"tvSeries",
		"tvMovie",
		"tvShort",
		"tvEpisode",
		"videoGame",
		"short",
		"video",
		"tvSpecial",
	};

	for( auto _ : state ) {
		Consulta c;
		const std::string &TIPO = TIPOS[rand() % TIPOS.size()];
		c.adicionarBuscaPorTipo(TIPO, false);

		unsigned total;
		bd.consultarCinema(c, 0, total);
	}
}

static void BM_BuscaCinemaPorPreco(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		const double preco = (double)((rand() % 15) + 5);
		c.adicionarBuscaPorPreco(preco, false);

		unsigned total;
		bd.consultarCinema(c, 0, total);
	}
}

static void BM_BuscaCinemaPorLocalizacao(benchmark::State &state) {
	BancoDeDados bd {};
	bd.lerDados();

	srand(time(NULL));

	for( auto _ : state ) {
		Consulta c;
		Loc loc = {
			(unsigned)(rand() % 400000),
			(unsigned)(rand() % 400000),
			(unsigned)(rand() % 400000 + 1),
		};
		c.adicionarBuscaPorLocalizacao(loc, false);

		unsigned total;
		bd.consultarCinema(c, 0, total);
	}
}

BENCHMARK(BM_LerDados);

BENCHMARK(BM_BuscaPorTitulo);
BENCHMARK(BM_BuscaPorTituloAleatorio);

BENCHMARK(BM_BuscaPorClassificacaoIndicativaAdulto);
BENCHMARK(BM_BuscaPorClassificacaoIndicativaNaoAdulto);
BENCHMARK(BM_BuscaPorClassificacaoIndicativaAleatoria);

BENCHMARK(BM_BuscaPorAnoEspecifico);
BENCHMARK(BM_BuscaPorAnoIntervalo);
BENCHMARK(BM_BuscaPorAnoMinimo);
BENCHMARK(BM_BuscaPorAnoMaximo);

BENCHMARK(BM_BuscaPorDuracaoEspecifico);
BENCHMARK(BM_BuscaPorDuracaoIntervalo);
BENCHMARK(BM_BuscaPorDuracaoMinimo);
BENCHMARK(BM_BuscaPorDuracaoMaximo);

BENCHMARK(BM_BuscaPorGenero);
BENCHMARK(BM_BuscaPorTipo);

BENCHMARK(BM_BuscaCinemaPorGenero);
BENCHMARK(BM_BuscaCinemaPorTipo);
BENCHMARK(BM_BuscaCinemaPorPreco);
BENCHMARK(BM_BuscaCinemaPorLocalizacao);

BENCHMARK_MAIN();

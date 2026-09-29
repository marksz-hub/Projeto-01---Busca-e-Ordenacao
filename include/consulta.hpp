/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Consulta no banco de dados, armazenada como uma AST
 *
 * INICIO:
 *   2026-09-29
 */

#pragma once

#include <cstring>
#include <string>

enum struct TipoConsulta {
	TITULO,
	ADULTO,
	ANO,
	ANO_FAIXA_INICIAL,
	ANO_FAIXA_FINAL,
	DURACAO,
	GENERO,
	OP_E,
	OP_OU,
	OP_NAO,
};

struct ConsultaNo;

union ConsultaDado {
	const char *titulo; /* Para: TipoConsulta::TITULO */
	bool adulto; /* Para: TipoConsulta::ADULTO */
	unsigned ano; /* Para: TipoConsulta::ANO */
	unsigned ano_inicial; /* Para: TipoConsulta::ANO_FAIXA_INICIAL */
	unsigned ano_final; /* Para: TipoConsulta::ANO_FAIXA_FINAL */
	const char *genero; /* Para: TipoConsulta::GENERO */

	/* Para: TipoConsulta::OP_* */
	struct {
		ConsultaNo *esq;
		ConsultaNo *dir;
	} filho;
};

/* 'tagged union' com a consulta */
struct ConsultaNo {
	/* Tipo de consulta */
	TipoConsulta tipo;
	std::string stringificada;

	/* Dado relevante (depende do tipo) */
	ConsultaDado dado;

	ConsultaNo(std::string titulo) {
		this->tipo = TipoConsulta::TITULO;
		this->dado.titulo = titulo.c_str();
	}

	std::string stringificar(void) {
		switch( this->tipo ) {
		case TipoConsulta::OP_E:
			return "&:" + this->dado.filho.esq->stringificar()
				+ " &:" + this->dado.filho.dir->stringificar();
		case TipoConsulta::OP_OU:
			return "|:" + this->dado.filho.esq->stringificar()
				+ " |:" + this->dado.filho.dir->stringificar();
		case TipoConsulta::OP_NAO:
			return "~:" + this->dado.filho.esq->stringificar()
				+ " ~:" + this->dado.filho.dir->stringificar();
		default:
			return this->stringificada;
		}
	}
};

/* A consulta em si */
struct Consulta {
	ConsultaNo *raiz;
	std::string stringificada;

	void stringificar(void) {
		this->stringificada = raiz->stringificar();
	}

	bool operator==(const Consulta &rhs) const {
		return this->stringificada == rhs.stringificada;
	}
};

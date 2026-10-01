/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Consulta no banco de dados, armazenada como uma AST
 *
 * INICIO:
 *   2026-09-29
 */

#pragma once

#include <cassert>
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
	/* Para: TipoConsulta::TITULO e TipoConsulta::GENERO */
	char *str;

	/* Para: TipoConsulta::ADULTO */
	bool adulto;

	/* Para:
	 * - TipoConsulta::ANO,
	 * - TipoConsulta::ANO_FAIXA_INICIAL,
	 * - TipoConsulta::ANO_FAIXA_FINAL, e
	 * - TipoConsulta::DURACAO
	 */
	unsigned num;

	/* Para: TipoConsulta::OP_* */
	struct {
		ConsultaNo *esq;
		ConsultaNo *dir;
	} filho;
};

/* 'tagged union' com a consulta */
struct ConsultaNo {
private:
	/* Dado relevante (depende do tipo) */
	ConsultaDado dado;
	std::string stringificada;

public:
	/* Tipo de consulta */
	TipoConsulta tipo;

	ConsultaNo(TipoConsulta tipo) {
		this->tipo = tipo;
	}

	ConsultaNo(TipoConsulta tipo, std::string str) {
		const char *c_str = str.c_str();
		const std::string::size_type n = str.size() + 1;

		this->tipo = tipo;
		this->dado.str = new char[n];
		std::memcpy(this->dado.str, c_str, n);
	}

	ConsultaNo(TipoConsulta tipo, bool adulto) {
		this->tipo = tipo;
		this->dado.adulto = adulto;
	}

	ConsultaNo(TipoConsulta tipo, unsigned num) {
		this->tipo = tipo;
		this->dado.num = num;
	}

	~ConsultaNo(void) {
		switch( tipo ) {
		case TipoConsulta::TITULO:
		case TipoConsulta::GENERO:
			delete[] this->dado.str;
			break;
		case TipoConsulta::OP_E:
		case TipoConsulta::OP_OU:
			delete this->getEsq();
			/* fallthrough */
		case TipoConsulta::OP_NAO:
			delete this->getDir();
			break;
		default:
			break;
		}
	}

	std::string getStr(void) const {
		assert(this->tipo == TipoConsulta::TITULO
			|| this->tipo == TipoConsulta::GENERO);
		return this->dado.str;
	}

	bool isAdulto(void) const {
		assert(this->tipo == TipoConsulta::ADULTO);
		return this->dado.adulto;
	}

	unsigned getNum(void) const {
		assert(tipo == TipoConsulta::ANO
			|| tipo == TipoConsulta::ANO_FAIXA_INICIAL
			|| tipo == TipoConsulta::ANO_FAIXA_FINAL || TipoConsulta::DURACAO);
		return this->dado.num;
	}

	ConsultaNo *getEsq(void) const {
		return this->dado.filho.esq;
	}

	ConsultaNo *getDir(void) const {
		return this->dado.filho.dir;
	}

	void setEsq(ConsultaNo *no) {
		this->dado.filho.esq = no;
	}

	void setDir(ConsultaNo *no) {
		this->dado.filho.dir = no;
	}

	void adicionarNo(ConsultaNo *no) {
		if( this->getDir() ) {
			this->getDir()->adicionarNo(no);
			return;
		}

		this->setDir(no);
	}

	std::string stringificar(void) const {
		switch( this->tipo ) {
		case TipoConsulta::OP_E:
			return "&:" + this->getEsq()->stringificar()
				+ " &:" + this->getDir()->stringificar();
		case TipoConsulta::OP_OU:
			return "|:" + this->getEsq()->stringificar()
				+ " |:" + this->getDir()->stringificar();
		case TipoConsulta::OP_NAO:
			return "~:" + this->getEsq()->stringificar()
				+ " ~:" + this->getDir()->stringificar();
		default:
			return this->stringificada;
		}
	}
};

/* A consulta em si */
struct Consulta {
private:
	std::string stringificada;

	void adicionarNo(ConsultaNo *no) {
		if( raiz == nullptr ) {
			raiz = no;
		} else {
			raiz->adicionarNo(no);
		}
	}

	void adicionarOp(ConsultaNo *op) {
		assert(raiz != nullptr);

		ConsultaNo *pai, *filho;

		pai = this->raiz;
		filho = pai->getDir();

		if( !filho ) {
			op->setEsq(pai);
			this->raiz = op;
			return;
		}

		while( filho->getDir() ) {
			pai = pai->getDir();
			filho = filho->getDir();
		}

		op->setEsq(filho);
		pai->setDir(op);
	}

public:
	ConsultaNo *raiz;

	Consulta()
		: raiz { nullptr } { }

	void adicionarBuscaPorTitulo(const std::string titulo) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::TITULO, titulo);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorAdulto(const bool adulto) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ADULTO, adulto);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorAno(const unsigned ano) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO, ano);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorFaixaAnoInicial(const unsigned ano) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO_FAIXA_INICIAL, ano);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorFaixaAnoFinal(const unsigned ano) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO_FAIXA_FINAL, ano);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorDuracao(const unsigned duracao) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::DURACAO, duracao);
		this->adicionarNo(no);
	}

	void adicionarBuscaPorGenero(const std::string genero) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::GENERO, genero);
		this->adicionarNo(no);
	}

	void adicionarOperadorE(void) {
		ConsultaNo *op = new ConsultaNo(TipoConsulta::OP_E);
		this->adicionarOp(op);
	}

	void adicionarOperadorOu(void) {
		ConsultaNo *op = new ConsultaNo(TipoConsulta::OP_OU);
		this->adicionarOp(op);
	}

	void adicionarOperadorNao(void) {
		ConsultaNo *op = new ConsultaNo(TipoConsulta::OP_NAO);
		this->adicionarOp(op);
	}

	void stringificar(void) {
		this->stringificada = raiz->stringificar();
	}

	bool operator==(const Consulta &rhs) const {
		return this->stringificada == rhs.stringificada;
	}
};

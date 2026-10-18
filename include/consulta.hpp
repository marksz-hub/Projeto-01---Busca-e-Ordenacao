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

#include "quadtree.hpp"

enum struct TipoConsulta {
	TITULO,
	ADULTO,
/*	ANO,
	ANO_FAIXA_INICIAL,
	ANO_FAIXA_FINAL,	*/
	ANO_INTERVALO,
	DURACAO_INTERVALO,
	GENERO,
	TIPO,
	LOCALIZACAO,
	PRECO,
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
	 * - TipoConsulta::ANO_INTERVALO e TipoConsulta::DURACAO
	 */ 
	unsigned num;

	struct {
		unsigned minimo;
		unsigned maximo;
	} intervalo;

	/* Para: TipoConsulta::PRECO */
	double preco;

	/* Para: TipoConsulta::OP_* */
	struct {
		ConsultaNo *esq;
		ConsultaNo *dir;
	} filho;

	/* Para: TipoConsulta::OP_LOCALIZACAO */
	Loc loc;
};

/* 'tagged union' com a consulta */
struct ConsultaNo {
private:
	/* Dado relevante (depende do tipo) */
	ConsultaDado dado;

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
/*
	ConsultaNo(unsigned minimo, unsigned maximo) {
		this->tipo = TipoConsulta::DURACAO_INTERVALO;
		this->dado.intervalo.minimo = minimo;
		this->dado.intervalo.maximo = maximo;
	}
*/
	ConsultaNo(TipoConsulta tipo, unsigned minimo, unsigned maximo) {
	    assert(tipo == TipoConsulta::ANO_INTERVALO || tipo == TipoConsulta::DURACAO_INTERVALO);
	
	    this->tipo = tipo;
	    this->dado.intervalo.minimo = minimo;
	    this->dado.intervalo.maximo = maximo;
	}

	ConsultaNo(TipoConsulta tipo, double preco) {
		this->tipo = tipo;
		this->dado.preco = preco;
	}

	ConsultaNo(Loc loc) {
		this->tipo = TipoConsulta::LOCALIZACAO;
		this->dado.loc = loc;
	}

	~ConsultaNo(void) {
		switch( tipo ) {
		case TipoConsulta::TITULO:
		case TipoConsulta::GENERO:
		case TipoConsulta::TIPO:
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
			|| this->tipo == TipoConsulta::GENERO
			|| this->tipo == TipoConsulta::TIPO);
		return this->dado.str;
	}

	bool isAdulto(void) const {
		assert(this->tipo == TipoConsulta::ADULTO);
		return this->dado.adulto;
	}

	unsigned getNum(void) const {
		return this->dado.num;
	}

	unsigned getMinimo(void) const {
		assert(tipo == TipoConsulta::DURACAO_INTERVALO || tipo == TipoConsulta::ANO_INTERVALO);
		return dado.intervalo.minimo;
	}
	unsigned getMaximo(void) const {
		assert(tipo == TipoConsulta::DURACAO_INTERVALO || tipo == TipoConsulta::ANO_INTERVALO);
		return dado.intervalo.maximo;
	}

	double getPreco(void) const {
		assert(tipo == TipoConsulta::PRECO);
		return this->dado.preco;
	}

	Loc getLoc(void) const {
		assert(tipo == TipoConsulta::LOCALIZACAO);
		return this->dado.loc;
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
		case TipoConsulta::TITULO:
			return "t:" + this->getStr();
		case TipoConsulta::ADULTO:
			if( this->isAdulto() ) {
				return "a:1";
			}
			return "a:0";
/*		case TipoConsulta::ANO:
			return "A:" + std::to_string(this->getNum());
		case TipoConsulta::ANO_FAIXA_INICIAL:
			return "Ai:" + std::to_string(this->getNum());
		case TipoConsulta::ANO_FAIXA_FINAL:
			return "Af:" + std::to_string(this->getNum());
*/		case TipoConsulta::ANO_INTERVALO:
		    return "AI:" + std::to_string(getMinimo())
		        + "-" + std::to_string(getMaximo());
		case TipoConsulta::DURACAO_INTERVALO:
			return "d:" + std::to_string(getMinimo())
				+ "-" + std::to_string(getMaximo());
		case TipoConsulta::GENERO:
			return "g:" + this->getStr();
		case TipoConsulta::PRECO:
			return "p:" + std::to_string(this->getPreco());
		case TipoConsulta::TIPO:
			return "T:" + this->getStr();
		case TipoConsulta::LOCALIZACAO: {
			const Loc &loc = this->getLoc();
			std::string x = std::to_string(loc.x);
			std::string y = std::to_string(loc.y);
			std::string raio = std::to_string(loc.raio);
			return "L:" + x + " " + y + " " + raio;
		}
		case TipoConsulta::OP_E:
			return "&:" + this->getEsq()->stringificar()
				+ " &:" + this->getDir()->stringificar();
		case TipoConsulta::OP_OU:
			return "|:" + this->getEsq()->stringificar()
				+ " |:" + this->getDir()->stringificar();
		case TipoConsulta::OP_NAO:
			return "~:" + this->getDir()->stringificar();
		}

		return "?";
	}
};

/* A consulta em si */
struct Consulta {
private:
	void adicionarNo(ConsultaNo *no, bool negar) {
		if( negar ) {
			ConsultaNo *op = new ConsultaNo(TipoConsulta::OP_NAO);
			op->setDir(no);
			no = op;
		}

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

	void adicionarBuscaPorTitulo(const std::string titulo, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::TITULO, titulo);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorAdulto(const bool adulto, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ADULTO, adulto);
		this->adicionarNo(no, negar);
	}
/*
	void adicionarBuscaPorAno(const unsigned ano, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO, ano);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorFaixaAnoInicial(const unsigned ano, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO_FAIXA_INICIAL, ano);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorFaixaAnoFinal(const unsigned ano, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO_FAIXA_FINAL, ano);
		this->adicionarNo(no, negar);
	}
*/
	/* Todas as buscas por ano de lancamento sao representadas por um intervalo */
	void adicionarBuscaPorIntervaloAno(unsigned minimo, unsigned maximo, bool negar) {
		assert(minimo > 0);
    	assert(maximo > 0);
    	assert(minimo <= maximo);

		ConsultaNo *no = new ConsultaNo(TipoConsulta::ANO_INTERVALO, minimo, maximo);
		this->adicionarNo(no, negar);
	}
/*
	void adicionarBuscaPorAnoMinimo(unsigned ano, unsigned ano_inicial_max, bool negar) {
		this->adicionarBuscaPorIntervaloAno(ano, ano_inicial_max, negar);
	}
	void adicionarBuscaPorAnoMaximo(unsigned ano, bool negar) {
		this->adicionarBuscaPorIntervaloAno(1, ano, negar);
	}
	void adicionarBuscaPorAnoEspecifico(unsigned ano, bool negar) {
		this->adicionarBuscaPorIntervaloAno(ano, ano, negar);
	}
*/
	/* Todas as buscas por duracao sao representadas por um intervalo */
	void adicionarBuscaPorIntervaloDuracao(unsigned minimo, unsigned maximo, bool negar) {
		assert(minimo > 0);
    	assert(maximo > 0);
    	assert(minimo <= maximo);

		ConsultaNo *no = new ConsultaNo(TipoConsulta::DURACAO_INTERVALO, minimo, maximo);
		this->adicionarNo(no, negar);
	}
/*
	void adicionarBuscaPorDuracaoMinima(unsigned duracao, unsigned duracao_max, bool negar) {
		this->adicionarBuscaPorIntervaloDuracao(duracao, duracao_max, negar);
	}
	void adicionarBuscaPorDuracaoMaxima(unsigned duracao, bool negar) {
		this->adicionarBuscaPorIntervaloDuracao(1, duracao, negar);
	}
	void adicionarBuscaPorDuracaoExata(unsigned duracao, bool negar) {
		this->adicionarBuscaPorIntervaloDuracao(duracao, duracao, negar);
	}
*/
	void adicionarBuscaPorGenero(const std::string genero, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::GENERO, genero);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorTipo(const std::string tipo, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::TIPO, tipo);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorPreco(const double preco, bool negar) {
		ConsultaNo *no = new ConsultaNo(TipoConsulta::PRECO, preco);
		this->adicionarNo(no, negar);
	}

	void adicionarBuscaPorLocalizacao(const Loc loc, bool negar) {
		ConsultaNo *no = new ConsultaNo(loc);
		this->adicionarNo(no, negar);
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

	std::string stringificada(void) const {
		return raiz->stringificar();
	}

	bool operator!(void) const {
		return !this->raiz;
	}

	bool operator!=(const Consulta &rhs) const {
		return this->stringificada() != rhs.stringificada();
	}
};

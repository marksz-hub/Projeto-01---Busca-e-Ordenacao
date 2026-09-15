#pragma once

#include <string>

struct Filme {
public:
	std::string id;
	std::string tipo;
	std::string titulo_primario;
	std::string titulo_original;
	int ano_inicial;
	int ano_final;
	bool adulto;
	int duracao;
	std::string generos;

	Filme(std::string id, std::string tipo, std::string titulo_primario,
		std::string titulo_original, int ano_inicial, int ano_final,
		bool adulto, int duracao, std::string generos) {
		this->id = id;
		this->tipo = tipo;
		this->titulo_primario = titulo_primario;
		this->titulo_original = titulo_original;
		this->ano_inicial = ano_inicial;
		this->ano_final = ano_final;
		this->adulto = adulto;
		this->duracao = duracao;
		this->generos = generos;
	}
};

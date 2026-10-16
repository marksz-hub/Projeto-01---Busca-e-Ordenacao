struct NoIndice {
		unsigned valor;
		std::vector<unsigned> filmes;
		NoIndice *esq;
		NoIndice *dir;

		NoIndice(unsigned valor, unsigned indiceFilme) {
			this->valor = valor;
			filmes.push_back(indiceFilme);
			esq = nullptr;
			dir = nullptr;
		}
	};

	/* NoIndice *&raiz é uma referência para um ponteiro.
	   Ao encontrar uma posição vazia, podemos modificar
	   diretamente o ponteiro que aponta para ela */
	void inserir(NoIndice *&raiz, unsigned valor, unsigned indiceFilme) {
		if(raiz == nullptr) {
			raiz = new NoIndice(valor, indiceFilme);
			return;
		}

		if(valor < raiz->valor) {
			return inserir(raiz->esq, valor, indiceFilme);
		}

		else if(valor > raiz->valor) {
			return inserir(raiz->dir, valor, indiceFilme);
		}

		else {
			return raiz->filmes.push_back(indiceFilme);
		}
	}

/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Estrutura de dados: Quadtree
 *
 * INÍCIO:
 *   2026-10-13
 */

#include <cstdlib>
#include <memory>
#include <vector>

/* Um ponto na Quadtree
 * Cada ponto representa um cinema
 */
struct Ponto {
	unsigned x;
	unsigned y;
	unsigned ind; /* Índice no vetor de cinemas */
};

/* Struct representando uma consulta por localização */
struct Loc {
	unsigned x, y, raio;

	bool contem(const Ponto &p) const {
		const unsigned dx = std::abs((int)p.x - (int)this->x);
		const unsigned dy = std::abs((int)p.y - (int)this->y);

		/* Raio maior do que a diferença */
		if( dx > this->raio || dy > this->raio ) {
			return false;
		}

		/* Raio menor do que a diferença */
		if( dx + dy <= this->raio ) {
			return true;
		}

		const unsigned dx2 = dx * dx;
		const unsigned dy2 = dy * dy;
		const unsigned r2 = this->raio * this->raio;

		/* Pitágoras */
		return dx2 + dy2 <= r2;
	}
};

/* Retângulo */
struct Rect {
	unsigned x;
	unsigned y;
	unsigned w;
	unsigned h;

	/* Verifica se o ponto está contido no retângulo */
	bool contem(const Ponto &p) const {
		return (p.x >= this->x && p.x <= (this->x + this->w) && p.y >= this->y
			&& p.y <= (this->y + this->h));
	}

	/* Verifica se o raio intersecta este retângulo */
	bool intersecta(const Loc &loc) const {
		const unsigned dx = std::abs((int)loc.x - (int)this->x);
		const unsigned dy = std::abs((int)loc.y - (int)this->y);

		const unsigned mw = this->w / 2;
		const unsigned mh = this->h / 2;

		/* Longe demais */
		if( dx > (mw + loc.raio) || dy > (mh + loc.raio) ) {
			return false;
		}

		/* Perto o suficiente */
		if( dx <= mw || dy <= mh ) {
			return true;
		}

		const unsigned d2 = (dx - mw) * (dx - mw) + (dy - mh) * (dy - mh);
		const unsigned r2 = loc.raio * loc.raio;

		/* Por distância */
		return d2 <= r2;
	}

	/* Verifica se um retângulo intersecta este retângulo */
	bool intersecta(const Rect &r) const {
		const unsigned xmax1 = this->x + this->w;
		const unsigned ymax1 = this->y + this->h;
		const unsigned xmax2 = r.x + r.w;
		const unsigned ymax2 = r.y + r.h;

		return this->x <= xmax2 && r.x <= xmax1 && this->y <= ymax2
			&& r.y <= ymax1;
	}
};

struct Quadtree {
private:
	unsigned capacidade;
	Rect limites;
	std::vector<Ponto> pontos;
	bool subdividido;

	/* Filhos a Noroeste, Nordeste, etc. */
	std::unique_ptr<Quadtree> noroeste;
	std::unique_ptr<Quadtree> nordeste;
	std::unique_ptr<Quadtree> sudoeste;
	std::unique_ptr<Quadtree> sudeste;

public:
	Quadtree(Rect r, unsigned n)
		: capacidade { n }
		, limites { r }
		, subdividido { false } { }

	Quadtree(unsigned n)
		: capacidade { n }
		, limites { 0, 0, 0, 0 }
		, subdividido { false } { }

	void atualizarLimites(unsigned x_max, unsigned y_max) {
		const unsigned w = x_max;
		const unsigned h = y_max;
		const Rect r = Rect { 0, 0, w, h };

		this->atualizarLimites(r);
	}

	void atualizarLimites(const Rect r) {
		this->limites = r;
	}

	void subdividir() {
		const unsigned x = this->limites.x;
		const unsigned y = this->limites.y;
		const unsigned w = this->limites.w / 2;
		const unsigned h = this->limites.h / 2;

		const unsigned mx = x + w;
		const unsigned my = y + h;

		const unsigned w2 = w + (this->limites.w % 2);
		const unsigned h2 = h + (this->limites.h % 2);

		const Rect no = Rect { x, y, w, h };
		const Rect ne = Rect { mx, y, w2, h };
		const Rect so = Rect { x, my, w, h2 };
		const Rect se = Rect { mx, my, w2, h2 };

		this->noroeste = std::make_unique<Quadtree>(no, this->capacidade);
		this->nordeste = std::make_unique<Quadtree>(ne, this->capacidade);
		this->sudoeste = std::make_unique<Quadtree>(so, this->capacidade);
		this->sudeste = std::make_unique<Quadtree>(se, this->capacidade);

		this->subdividido = true;
	}

	bool inserir(unsigned x, unsigned y, unsigned ind) {
		return this->inserir(Ponto { x, y, ind });
	}

	bool inserir(Ponto p) {
		if( !this->limites.contem(p) ) {
			return false;
		}

		if( this->pontos.size() < this->capacidade ) {
			this->pontos.push_back(p);
			return true;
		}

		if( !this->subdividido ) {
			this->subdividir();
		}

		return this->noroeste->inserir(p) || this->nordeste->inserir(p)
			|| this->sudoeste->inserir(p) || this->sudeste->inserir(p);
	}

	std::vector<unsigned> buscar(const Loc &loc) const {
		std::vector<unsigned> v;
		buscar(loc, v);

		return v;
	}

	void buscar(const Loc &loc, std::vector<unsigned> &v) const {
		if( !this->limites.intersecta(loc) ) {
			return;
		}

		for( const Ponto &p : this->pontos ) {
			if( loc.contem(p) ) {
				v.push_back(p.ind);
			}
		}

		if( !this->subdividido ) {
			return;
		}

		this->nordeste->buscar(loc, v);
		this->noroeste->buscar(loc, v);
		this->sudoeste->buscar(loc, v);
		this->sudeste->buscar(loc, v);
	}

	std::vector<unsigned> buscar(const Rect &r) const {
		std::vector<unsigned> v;
		buscar(r, v);

		return v;
	}

	void buscar(const Rect &r, std::vector<unsigned> &v) const {
		if( !this->limites.intersecta(r) ) {
			return;
		}

		for( const Ponto &p : this->pontos ) {
			if( r.contem(p) ) {
				v.push_back(p.ind);
			}
		}

		if( !this->subdividido ) {
			return;
		}

		this->nordeste->buscar(r, v);
		this->noroeste->buscar(r, v);
		this->sudoeste->buscar(r, v);
		this->sudeste->buscar(r, v);
	}
};

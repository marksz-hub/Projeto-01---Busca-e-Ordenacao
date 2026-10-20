/* Trabalho I
 * Técnicas de Busca e Ordenação
 *
 * Estrutura de dados: Quadtree
 *
 * INÍCIO:
 *   2026-10-13
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

/* Um ponto na Quadtree
 * Cada ponto representa um cinema
 */
struct Ponto {
	int64_t x;
	int64_t y;
	int64_t ind; /* Índice no vetor de cinemas */
};

/* Struct representando uma consulta por localização */
struct Loc {
	int64_t x, y, raio;

	bool contem(const Ponto &p) const {
		const int64_t dx = std::abs(p.x - this->x);
		const int64_t dy = std::abs(p.y - this->y);

		/* Raio maior do que a diferença */
		if( dx > this->raio || dy > this->raio ) {
			return false;
		}

		/* Raio menor do que a diferença */
		if( dx + dy <= this->raio ) {
			return true;
		}

		const int64_t dx2 = dx * dx;
		const int64_t dy2 = dy * dy;
		const int64_t r2 = this->raio * this->raio;

		/* Pitágoras */
		return dx2 + dy2 <= r2;
	}
};

/* Retângulo */
struct Rect {
	int64_t x;
	int64_t y;
	int64_t w;
	int64_t h;

	/* Verifica se o ponto está contido no retângulo */
	bool contem(const Ponto &p) const {
		return (p.x >= this->x && p.x <= (this->x + this->w) && p.y >= this->y
			&& p.y <= (this->y + this->h));
	}

	/* Verifica se o raio intersecta este retângulo */
	bool intersecta(const Loc &loc) const {
		const int64_t mw = this->w / 2;
		const int64_t mh = this->h / 2;

		const int64_t dx = std::abs(loc.x - this->x - mw);
		const int64_t dy = std::abs(loc.y - this->y - mh);

		/* Longe demais */
		if( (dx > (mw + loc.raio)) ) {
			return false;
		}

		if( (dy > (mh + loc.raio)) ) {
			return false;
		}

		/* Perto o suficiente */
		if( dx <= mw ) {
			return true;
		}

		if( dy <= mh ) {
			return true;
		}

		int64_t dx2 = dx - mw;
		dx2 *= dx2;

		int64_t dy2 = dy - mh;
		dy2 *= dy2;

		const int64_t r2 = loc.raio * loc.raio;

		/* Por distância */
		return (dx2 + dy2) <= r2;
	}

	/* Verifica se um retângulo intersecta este retângulo */
	bool intersecta(const Rect &r) const {
		const int64_t r1x1 = this->x;
		const int64_t r1x2 = this->x + this->w;
		const int64_t r1y1 = this->y;
		const int64_t r1y2 = this->y + this->h;

		const int64_t r2x1 = r.x;
		const int64_t r2x2 = r.x + r.w;
		const int64_t r2y1 = r.y;
		const int64_t r2y2 = r.y + r.h;

		return r1x1 < r2x2 && r1x2 > r2x1 && r1y1 < r2y2 && r1y2 > r2y1;
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

	void atualizarLimites(int64_t x_max, unsigned y_max) {
		const int64_t w = x_max;
		const int64_t h = y_max;
		const Rect r = Rect { 0, 0, w, h };

		this->atualizarLimites(r);
	}

	void atualizarLimites(const Rect r) {
		this->limites = r;
	}

	void subdividir() {
		const int64_t x = this->limites.x;
		const int64_t y = this->limites.y;
		const int64_t w = this->limites.w / 2;
		const int64_t h = this->limites.h / 2;

		const int64_t mx = x + w;
		const int64_t my = y + h;

		const int64_t w2 = w + (this->limites.w % 2);
		const int64_t h2 = h + (this->limites.h % 2);

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

	bool inserir(int64_t x, int64_t y, int64_t ind) {
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
			std::cout << "b: " << loc.x << " " << loc.y << " " << loc.raio
					  << std::endl;
			std::cout << this->limites.x << " " << this->limites.y << " "
					  << this->limites.w << " " << this->limites.h << std::endl
					  << std::endl;
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

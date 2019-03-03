//csimpGUI.h
#include <SDL.h>
#include <vector>
#include <csimpLog.h>

class cintRect
{
public:
	SDL_Rect r;

	cintRect(){};
	cintRect(const cintRect &b){ r = b.r; };
	cintRect(const SDL_Rect &_r){ r = _r; };

	bool ismypoint(const int &x, const int &y) const
		{
			return (r.x <= x && x < r.x + r.w && r.y <= y && y < r.y + r.h);
		}
};

const int max_number_of_unsorted_buttons = 4;

//very basic quadtreenode
class cQuad
{
protected:
	const cQuad *pparent;
	cQuad *pquadLT, *pquadRT, *pquadLB, *pquadRB;

	std::vector<int> indexes;
	int residents; //does not fit in child

	int x0, y0; //middle axes

	void addtoquadrant(cQuad *&pquad, const int &_x, const int &_y, std::vector<cintRect> &buttons_vector, const int &ix);
	bool addchild(const int &ix, std::vector<cintRect> &buttons_vector);

public:
	cQuad(cQuad *pp, const int &x0, const int &y0);
	~cQuad();

	void add(const int &ix, std::vector<cintRect> &buttons_vector); //SDL_Rect to sort, ix: button index ind vector
	void draw(void);
	void print(int level);

	int get(const int &x, const int &y, const std::vector<cintRect> &bv); //returns the index of cintRect found or -1 in case not found
};

class cAllWidgets
{
protected:
	cQuad quad;
	std::vector<cintRect> allbuttons;

	int selected;
public:
	cAllWidgets(const int &w, const int &h);

	int add_button(const cintRect &button_to_store);
	void drawall(void);
	void printall(void);

	cintRect &get_selected(void){ return allbuttons.at(selected); }
	int &get_selectedindex(void){ return selected; }
	void hoover(const int &x, const int &y);
};

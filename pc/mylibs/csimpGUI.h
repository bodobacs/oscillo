//csimpGUI.h
#include <SDL.h>
#include <vector>
#include <csimpLog.h>

class cButton
{
public:
	SDL_Rect r;

	cButton(){};
	cButton(const cButton &b){ r = b.r; };
	cButton(const SDL_Rect &_r){ r = _r; };

	bool ismypoint(const int &x, const int &y) const
		{
			return (r.x <= x && x < r.x + r.w && r.y <= y && y < r.y + r.h);
		}

	void event(SDL_Event);
};

const int max_number_of_unsorted_buttons = 4;

class cQuad
{
protected:
	const cQuad *pparent;
	cQuad *pquadLT, *pquadRT, *pquadLB, *pquadRB;

	std::vector<int> indexes;
	int residents; //does not fit in child

	int x0, y0; //middle axes

	void addtoquadrant(cQuad *&pquad, const int &_x, const int &_y, std::vector<cButton> &buttons_vector, const int &ix);
	bool addchild(const int &ix, std::vector<cButton> &buttons_vector);

public:
	cQuad(cQuad *pp, const int &x0, const int &y0);
	~cQuad();

	void add(const int &ix, std::vector<cButton> &buttons_vector); //SDL_Rect to sort, ix: button index ind vector
	void draw(void);
	void print(int level);

	int get(const int &x, const int &y, const std::vector<cButton> &bv);
};

class cAllWidgets
{
protected:
	cQuad quad;
	std::vector<cButton> allbuttons;

	int selected;
public:
	cAllWidgets(const int &w, const int &h);

	int add_button(const cButton &button_to_store);
	void drawall(void);
	void printall(void);

//	cButton &get_button(const int &index);
//	int get(const int &x, const int &y){}
	const int &get_selected(void){ return selected; }
	bool hoover(const int &x, const int &y);
};

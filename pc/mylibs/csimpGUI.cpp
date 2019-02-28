#include <csimpGUI.h>
#include <cstdlib>
#include <csimpSDL.h>
#include <algorithm>

cAllWidgets::cAllWidgets(const int &w, const int &h) : quad(nullptr, w/2, h/2)
{
	selected = -1;
}

void cAllWidgets::add_button(const cButton &b)
{
	allbuttons.push_back(b);
//	csimplog << "button " << v.size() << ": x: " << b.r.x << ", y: " << b.r.y << ", w: " << b.r.w << ", h: " << b.r.h << std::endl;
	quad.add(allbuttons.size()-1, allbuttons);
}

void cAllWidgets::drawall(void)
{
	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200,150,0,0);

	int i = 0;
	for(std::vector<cButton>::iterator itr = allbuttons.begin(); itr != allbuttons.end(); itr++, i++)
	{
		if(i == selected)
		{
			SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200,0,0,0);
			SDL_RenderDrawRect(csimpsdl.sdl_ren, &(*itr).r);
			SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200,150,0,0);
		}else{
			SDL_RenderDrawRect(csimpsdl.sdl_ren, &(*itr).r);
		}
	}
	quad.draw();
}

bool cAllWidgets::click(const int &x, const int &y)
{
	selected = quad.get(x, y, allbuttons);
	csimplog << selected << std::endl;
	if(-1 != selected) return true;
	return false;
}

void cAllWidgets::printall(void)
{
	quad.print(0);
}

void cQuad::print(int level)
{
	csimplog << level << std::endl;
	for(int i = 0; i < level+1; i++) csimplog << " --> ";

	for(auto n : indexes) csimplog << n << " ";
	csimplog << std::endl;

	if(nullptr != pquadLT){ csimplog << "LT: "; pquadLT->print(level+1); }
	if(nullptr != pquadRT){ csimplog << "RT: "; pquadRT->print(level+1); }
	if(nullptr != pquadLB){ csimplog << "LB: "; pquadLB->print(level+1); }
	if(nullptr != pquadRB){ csimplog << "RB: "; pquadRB->print(level+1); }
}

cQuad::cQuad(cQuad *pp, const int &x, const int &y)
{
	pquadLT = pquadRT = pquadLB = pquadRB = nullptr;
	pparent = pp;
	x0 = x; y0 = y;
}

cQuad::~cQuad()
{
	if(nullptr != pquadLT) delete pquadLT;
	if(nullptr != pquadRT) delete pquadRT;
	if(nullptr != pquadLB) delete pquadLB;
	if(nullptr != pquadRB) delete pquadRB;
}

//locate point in quadrant
int cQuad::get(const int &x, const int &y, const std::vector<cButton> &bv)
{
	int ret = -1;

	if(x0 <= x)
	{//RIGHT
		if(y0 <= y)
		{//BOTTOM
			if(nullptr != pquadRB) ret = pquadRB->get(x, y, bv);
		}else{//TOP
			if(nullptr != pquadRT) ret = pquadRT->get(x, y, bv);
		}
	}else{//LEFT
		if(y0 <= y)
		{//BOTTOM
			if(nullptr != pquadLB) ret = pquadLB->get(x, y, bv);
		}else{//TOP
			if(nullptr != pquadLT) ret = pquadLT->get(x, y, bv);
		}
	}

//	csimplog << "indexes.size(): " << indexes.size() << ", ret: " << ret << std::endl;
	for(std::vector<int>::iterator itr = indexes.begin(); itr != indexes.end() && -1 == ret; itr++)
	{
		ret = bv[*itr].ismypoint(x, y) ? *itr : -1;
	}
//	csimplog << "ret: " << ret << std::endl;

	return ret;
}

//rect is in quadrant, first time need to be allocated
void cQuad::addtoquadrant(cQuad *&pquad, const int &_x, const int &_y, std::vector<cButton> &bv, const int &ix)
{
	if(nullptr == pquad) pquad = new cQuad(this, _x, _y);

	if(nullptr != pquad)
	{
		pquad->add(ix, bv);
		return;
	}

	csimplog << "Memory alloc failed" << std::endl;
}

//do not just store distribute
//locate rectangle in quadrant
bool cQuad::addchild(const int &ix, std::vector<cButton> &bv)
{
	if(x0 <= bv[ix].r.x)
	{//RIGHT
		if(y0 <= bv[ix].r.y)
		{//BOTTOM
			if(nullptr == pparent) addtoquadrant(pquadRB, x0+x0/2, y0+y0/2, bv, ix); //first quad has no parent
			else addtoquadrant(pquadRB, x0 + abs(pparent->x0 - x0)/2, y0 + abs(pparent->y0 - y0)/2, bv, ix);
			return true;
		}else if(bv[ix].r.y + bv[ix].r.h < y0)
		{//TOP
			if(nullptr == pparent) addtoquadrant(pquadRT, x0+x0/2, y0-y0/2, bv, ix);
			else addtoquadrant(pquadRT, x0 + abs(pparent->x0 - x0)/2, y0 - abs(pparent->y0 - y0)/2, bv, ix);
			return true;
		}
	}else if(bv[ix].r.x + bv[ix].r.w < x0)
	{//LEFT
		if(y0 <= bv[ix].r.y)
		{//BOTTOM
			if(nullptr == pparent) addtoquadrant(pquadLB, x0-x0/2, y0+y0/2, bv, ix);
			else addtoquadrant(pquadLB, x0 - abs(pparent->x0 - x0)/2, y0 + abs(pparent->y0 - y0)/2, bv, ix);
			return true;
		}else if(bv[ix].r.y + bv[ix].r.h < y0)
		{//TOP
			if(nullptr == pparent) addtoquadrant(pquadLT, x0-x0/2, y0-y0/2, bv, ix);
			else addtoquadrant(pquadLT, x0 - abs(pparent->x0 - x0)/2, y0 - abs(pparent->y0 - y0)/2, bv, ix);
			return true;
		}
	}

	return false;
}

void cQuad::add(const int &ix, std::vector<cButton> &bv)
{
	indexes.push_back(ix);

	if(max_number_of_unsorted_buttons < indexes.size())
	{
		for(auto &n : indexes)
		{
//			csimplog << "n: " << n;
			if(addchild(n, bv)) n = -1;
//			csimplog << " -> " << n << std::endl;
		}

		if(indexes.rbegin() != indexes.rend()) std::sort(indexes.rbegin(), indexes.rend());
		while(indexes.rbegin() != indexes.rend() && -1==*indexes.rbegin()) indexes.pop_back();
	}
}

void cQuad::draw(void)
{
	int w, h;
	SDL_GetWindowSize(csimpsdl.sdl_win, &w, &h);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,200,200,0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, x0, 0, x0, h);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0, y0, w, y0);

	if(nullptr != pquadLT) pquadLT->draw();
	if(nullptr != pquadRT) pquadRT->draw();
	if(nullptr != pquadLB) pquadLB->draw();
	if(nullptr != pquadRB) pquadRB->draw();
}

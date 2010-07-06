/************************************************/
/* flie name : CVertexSet.cpp					*/
/* writen by :B.B.M								*/
/* reviewed by: R.K								*/
/************************************************/
#include "string.h"
#include <vector>
using namespace std;

#include "CVertexSet.h"

VertexSet::~VertexSet(void)
{
}
VertexSet::VertexSet()
{
	INIT_SIZE	= 50;
	INC			= 50;
	_set		= new int[INIT_SIZE];
	memset(_set,0,INIT_SIZE);
	_sp			= 0;
}
VertexSet::VertexSet(VertexSet* ot)
{
	INIT_SIZE	= 20;
	INC			= 50;
	_set		= new int[INIT_SIZE];
	memset(_set,0,INIT_SIZE);
	_sp			= 0;
	for(int i=0;i<ot->VertexSetSize();i++) 
		this->add(ot->VertexSetAt(i)); 
}

void VertexSet::add(int a)
{ 
	if(_sp==INIT_SIZE) 
		resize();//increas INIT_SIZE 
	_set[_sp] = a; 
	_sp++; 
}

/*string VertexSet::toString() 
{ 
	string ans = "Set: |"+size()+"| "; 
	for(int i=0;i<size();i++) 
		ans+=this.at(i)+", "; 
	return ans; 
}*/ 

VertexSet* VertexSet::intersection(VertexSet ot)
{ 
	VertexSet* ans = new VertexSet(); 
	int i1=0,i2=0; 
	while((i1<this->VertexSetSize()) && (i2 < ot.VertexSetSize())) { 
		int a1=this->VertexSetAt(i1), a2 = ot.VertexSetAt(i2); 
		if(a1==a2) { 
			ans->add(a1); i1++; i2++;} 
		else if(a1<a2) {i1++;} 
		else i2++; 
	} 
	return ans; 
} 
void VertexSet::resize()
{ 
	int* tmp = new int[_sp+INC];
	memset(tmp,0,_sp+INC);
	for(int i=0;i<_sp;i++) 
		tmp[i]=_set[i]; 
	INIT_SIZE = _sp+INC;
	_set=tmp; 
}
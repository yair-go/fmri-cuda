/************************************************/
/* flie name : CVertexSet.h						*/
/* writen by :B.B.M								*/
/* reviewed by: R.K								*/
/************************************************/
#ifndef VERTEX_SET_H
#define VERTEX_SET_H

class VertexSet {
public:
	VertexSet();
	~VertexSet(void);
	VertexSet(VertexSet *ot);
	void add(int a);
	//	string toString();
	VertexSet* intersection(VertexSet ot);
	int VertexSetSize() {return _sp;} 
	int VertexSetAt(int i) {return _set[i];} 

private:
	int INIT_SIZE;
	int INC; 
	int* _set; 
	int _sp; 

	void resize();
};
#endif
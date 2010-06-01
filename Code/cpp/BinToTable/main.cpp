/************************************************/
/* flie name : main.cpp							*/
/* writen by :R.K								*/
/************************************************/
#include <vector>
#include <fstream>
#include <iostream>// for cout
#include <string>// for string
#include <stdio.h>
#include <tchar.h>
using namespace std;
#include "Corrcoef.h"
#include "CVertexSet.h"


static string IN_FILE = "FMRI.bin";
static string OUT_FILE = "FMRI.txt";
static double _TH	= 0.75;
static int _ROW		= 15;
static int _COL		= 15;
static int _TIME	= 132;

void PrintTable(string outputFileName,vector<VertexSet>* table);
vector<VertexSet>* PrepareTable(float** pVox,float TH,int Size,int Time);
float** readVoxels(string inputFileName,int Size,int Time); 
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc>1)
		IN_FILE = argv[1];
	if(argc>2)
		OUT_FILE= argv[2];
	if(argc>3)
		_ROW	= atoi(argv[3]);
	if(argc>4)
		_COL	= atoi(argv[4]);
	if(argc>5)
		_TIME	= atoi(argv[7]);
	if(argc>6)
		_TH		= atof(argv[8]);

	int size = _COL*_ROW;
	float** pVox;
	vector<VertexSet>* Table = new vector<VertexSet>();
	size=1000;
	pVox = readVoxels(IN_FILE,size,_TIME);//pVox is matrix of size [Raw*Col][Length]
	if(pVox!=NULL)
	{
		Table = PrepareTable(pVox,(float)_TH,size,_TIME);
		PrintTable(OUT_FILE,Table);
	}
	else
		cout<<"something is wrong"<<endl;

	return 0;
}

/***************************************************/
float** readVoxels(string inputFileName,int Size,int Time) 
{
	int f=0;
	ifstream inputFile(inputFileName.c_str(),ios::binary);
	// construct and fill the 2D array
	if(!inputFile.bad())
	{
		float **ans = new float *[Size];
		for(int iy=0;iy< Size;iy++)
		{
			ans[iy] = new float[Time];    
			for(int ix=0;ix< Time;ix++) 
			{
				inputFile.read(reinterpret_cast<char*>(&f), 4);
				ans[(int)iy][(int)ix] = (float)f;
			}
		}
		inputFile.close();
		return ans;
	}
	else
		return NULL;	
}
/***************************************************/
vector<VertexSet>* PrepareTable(float** pVox,float TH,int Size,int Time)
{
    vector<VertexSet>* ans = new vector<VertexSet>();
	int i,j;
	for (i=0;i<Size;i++)
	{
		VertexSet* vs = new VertexSet();
		ans->push_back(vs);
	}
	for(i=0;i<Size;i++)
	{
		for(j=i;j<Size;j++)
		{
			if(Corrcoef(&pVox[i][0],&pVox[j][0],Time)>TH)
			{
				if (i!=j)
				{
					ans->at(j).add(i);
					ans->at(i).add(j);
				} 
				else
				{
					ans->at(j).add(i);				
				}
				
				//vs->add(j);				
			}
		}
		//ans->push_back(vs);
	}
	return ans;
}
/***************************************************/
void PrintTable(string outputFileName,vector<VertexSet>* table)
{
	int i,j;
	ofstream outputFile(outputFileName.c_str());
	for(i=0;i<table->size();i++)
	{
		outputFile << i << ": ";
		for(j=0;j<table->at(i).VertexSetSize();j++)
			outputFile << table->at(i).VertexSetAt(j) << " ";
		outputFile << endl;
	}
	outputFile.close();
}


























//void write2file(Vector<VertexSet>* c) 
//{
//	FileWriter fw=null; 
//	try {fw = new FileWriter("ALL_Cliques_"+in_file+"_"+TH+"_"+Q_size+".txt");}  
//	catch (IOException e) {e.printStackTrace();} 
//	PrintWriter os = new PrintWriter(fw); 
//	os.println("ALL_Cliques: of file: "+in_file+",  TH:"+TH+" Max Q:"+Q_size); 
//	for(int i=0;i<c.size();i++) 
//	{ 
//		os.println(i+") "+c.elementAt(i)); 
//	} 
//	os.close(); 
//	try {fw.close();}
//	catch (IOException e) {e.printStackTrace();}
//} 

//void Graph::print() 
//{
//	cout << "Graph: |V| = " << this->_V->size()<<endl;
//	/*System.out.println("Graph: |V| = "+this._V.size()); */
//	for(int i=0;(i<this->_V->size());i++) { 
//		cout<<endl;/*System.out.println(); */
//		cout<<"V["<<i<<"] = "<<endl;/* System.out.print("V["+i+"] = "); */
//		VertexSet curr = this->_V->at(i); 
//		for(int a=0;a<curr.VertexSetSize();a++)  
//			cout<<curr.VertexSetAt(a)<<", "<<endl; /*System.out.print(curr.at(a)+", ");*/ 
//	} 
//}
//VertexSet Graph::Ni(int i) 
//{ 
//	VertexSet ans = _V->at(i); 
//	return ans; 
//}

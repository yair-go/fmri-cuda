/************************************************/
/* flie name : main.cpp							*/
/* writen by :R.K								*/
/************************************************/
#include <atltime.h>
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
vector<VertexSet>* PrepareTable(float** pVox,float TH,int Size,int Time,int jump);
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

	// Calculate the difference between two times
	CFileTime myFT1, myFT2;
	CFileTimeSpan myFTS;

	int size = 6400;//_COL*_ROW;
	int cacheJump = 640;
	float** pVox;

	ULONGLONG min,sec,msec;
	

	pVox = readVoxels(IN_FILE,size,_TIME);//pVox is matrix of size [Raw*Col][Length]
	if(pVox!=NULL)
	{
		cout <<"Time check by ronen.....\n"<<"VECTOR SIZE: "<< size <<", TH: "<< _TH << endl<< endl;
		int i=10; // iri comment - change loop to one iteration
			cacheJump = size/i;
			if((float)(size)/i == cacheJump)
			{
				vector<VertexSet>* Table = new vector<VertexSet>();
				cout <<"Start calculation with cacheJump "<<cacheJump<<" (132 * 4(float) * "<<cacheJump<<" = "<< 132*4*cacheJump/1000 <<"KB)"<<endl;
				/** Get the first time **************/
				myFT1 = CFileTime::GetCurrentTime();
				/************************************/
				Table = PrepareTable(pVox,(float)_TH,size,_TIME,cacheJump);
				PrintTable(OUT_FILE,Table);
				/** Get the second time *************/
				myFT2 = CFileTime::GetCurrentTime();
				/************************************/
				// Calculate the time difference
				myFTS = myFT2 - myFT1;
				min = myFTS.GetTimeSpan()/CFileTime::Minute;
				sec = (myFTS.GetTimeSpan()-(min*CFileTime::Minute))/CFileTime::Second;
				msec = (myFTS.GetTimeSpan()-(min*CFileTime::Minute)-(sec*CFileTime::Second))/CFileTime::Millisecond;
				cout <<"time: "<< min << " : "<< sec << " : "<< msec << endl << endl;
				/************************************/
				delete Table;
			}
		
	}
	else
		cout<<"something is wrong"<<endl;
	getchar();
	
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
vector<VertexSet>* PrepareTable(float** pVox,float TH,int Size,int Time,int jump)
{
	vector<VertexSet>* ans = new vector<VertexSet>();
	int x = 0;
	int y = 0;
	int i = 0;
	int j = 0;
	float* pStatistics = new float[2*Size];
	for (i=0;i<Size;i++)
	{
		VertexSet* vs = new VertexSet();
		ans->push_back(vs);
		CorrcoefPrepare (pVox[i] , Time, &pStatistics[2*i]);
	}
	for(x = 0;x < Size;x = x+jump){
		for(y = x;y < Size;y = y+jump){
			for(i=0;i<jump;i++){
				for(j=0;j<jump;j++){
					if(Corrcoef(&pVox[x+i][0],&pStatistics[2*(x+i)],&pVox[y+j][0],&pStatistics[2*(y+j)],Time)>TH){
						if (x!=y){
						ans->at(x+i).add(y+j);
						ans->at(y+j).add(x+i);
						} 
						else{
						ans->at(x+i).add(y+j);				
						}
					}
				}
			}
		}
	}
	return ans;
}
/***************************************************/
void PrintTable(string outputFileName,vector<VertexSet>* table)
{
	int i,j;
	ofstream outputFile(outputFileName.c_str(),ios_base::out | ios_base::trunc);
	for(i=0;i<table->size();i++)
	{
		outputFile << i << ": ";
		for(j=0;j<table->at(i).VertexSetSize();j++)
			outputFile << table->at(i).VertexSetAt(j) << " ";
		outputFile << endl;
	}
	outputFile.close();
}
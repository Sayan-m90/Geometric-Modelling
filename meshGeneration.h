//
//  meshGeneration.h
//  CGLab1
//
//  Created by Jiayuan Wang on 2/16/16.
//  Copyright (c) 2016 Jiayuan Wang. All rights reserved.
//

#ifndef __CGLab1__meshGeneration__
#define __CGLab1__meshGeneration__

#include <stdio.h>
#include <vector>
#include "Mesh.h"
#include <math.h>
//#include "curves.h"
#include <fstream>

#define PI 3.14159265


using namespace std;

void outputOFF_mesh(Mesh m, string filename);
bool selfIntersection(Mesh m);
void revolution(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m,bool& overlap);
void revolutionx(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m,bool& overlap);
void revolutionCubic(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap);
void revolutionQuadric(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap);
void extrusion(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir);
void extrusionCubic(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir);
void extrusionQuadric(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir);
void sweepOperator(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m);
void sweepOperatorQuadric(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m);
void sweepOperatorCubic(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m);
void loft(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m);
void loftQuadric(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m);
void loftCubic(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m);

//for bspline
void cubicSample(vector<vector<float> > ctlPts, int n,vector<vector<float> >& sample);

//test
void nonSelfInter(Mesh& m);
#endif /* defined(__CGLab1__meshGeneration__) */

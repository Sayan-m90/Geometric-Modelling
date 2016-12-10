#include "mesh.h"
#include "meshGeneration.h"
#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Bspline.h"

float Bspline::splinegenRat(float u, float a[4][4], float v, float h[4][4])
{
    float b[4], c[4];
    //float hh[4][4] = {{1,10,7,9},{4,8,32,2},{1,1,12,4},{4,5,6,7}};

    b[0] = pow(1-u,3)/6.0;
    b[1] = (3*pow(u,3) - 6*u*u +4)/6.0;
    b[2] = (-3*u*u*u +3*u*u + 3*u + 1)/6.0;
    b[3] =  u*u*u/6.0;

    c[0] = pow(1-v,3)/6.0;
    c[1] = (3*pow(v,3) - 6*v*v +4)/6.0;
    c[2] = (-3*v*v*v +3*v*v + 3*v + 1)/6.0;
    c[3] =  v*v*v/6.0;

    float x=0.0, div = 0.0;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            x += h[i][j]*a[i][j]*b[i]*c[j];
            div += h[i][j]*b[i]*c[j];

        }
    }
    return x/div;

}

float Bspline::splinegen(float u, float a[4][4], float v)
{

    float b[4], c[4];

    b[0] = pow(1-u,3)/6.0;
    b[1] = (3*pow(u,3) - 6*u*u +4)/6.0;
    b[2] = (-3*u*u*u +3*u*u + 3*u + 1)/6.0;
    b[3] =  u*u*u/6.0;


    c[0] = pow(1-v,3)/6.0;
    c[1] = (3*pow(v,3) - 6*v*v +4)/6.0;
    c[2] = (-3*v*v*v +3*v*v + 3*v + 1)/6.0;
    c[3] =  v*v*v/6.0;

    float x=0.0;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            x+=a[i][j]*b[i]*c[j];
        }
    }

    return x;
}

Mesh Bspline::BsplineMesh(Mesh mesh, string filename, int &arclengthu, int &arclengthv)
{

    Mesh outmesh;
    //int nVertices = m.GetNumberVertices();

    ifstream file(filename);
    char str[10];

    file>>str;  int n = atoi(str);
    file>>str;  int m = atoi(str);
    cout<<" m: "<<m<<" n: "<<n;

    float *pinx[m], *piny[m], *pinz[m];
    vector<vector<int>> coord;

for(int i=0;i<m;i++)
{
    pinx[i] = (float*) malloc(n*sizeof(float));
    piny[i] = (float*) malloc(n*sizeof(float));
    pinz[i] = (float*) malloc(n*sizeof(float));
}
for(int i=0;i<n;i++)
{
    for(int j=0;j<m;j++)
    {
        file>>str;              pinx[j][i] = atof(str);
        file>>str;              piny[j][i] = atof(str);
        file>>str;              pinz[j][i] = atof(str);

    }
}

float px1 = 0, py1 = 0, pz1 = 0, px2 =0,py2 =0, pz2 =0, px3 =0, py3 =0, pz3 =0, px4 =0, py4 =0, pz4 =0;
int D = 3;
int R = 3;
float Px[4][4], Py[4][4], Pz[4][4];  //point matrix
float pres[m-4][n-4];
float ustep = 0.2, wstep = 0.2;
//cout<<"entering";
//getchar();


/////////////////////////////   Generate the point sets
int knot[4] = {0,1,2,3};
vector<vector<vector<GeomVert>> > BSplinePoint;
for(int i=0;i<m-3;i++)
{
    for(int j=0;j<n-3;j++)
    {
        vector<vector<GeomVert>> Sa;

        for(int indexk = 0;indexk<4;indexk++)
        {
            int k = knot[indexk];
            vector<GeomVert> tmp;
            for(int indexq = 0;indexq<4;indexq++)
            {
                int q=knot[indexq];
                GeomVert v(pinx[i+k][j+q],piny[i+k][j+q],pinz[i+k][j+q]);
                tmp.push_back(v);
                if(i+k<n)
                {
                Px[indexk][indexq] = pinx[i+k+1][j+q];
                Py[indexk][indexq] = piny[i+k+1][j+q];
                Pz[indexk][indexq] = pinz[i+k+1][j+q];
                }
                else
                {
                Px[indexk][indexq] = pinx[i+k][j+q];
                Py[indexk][indexq] = piny[i+k][j+q];
                Pz[indexk][indexq] = pinz[i+k][j+q];
                }

            }
            Sa.push_back(tmp);

        }
        vector<vector<GeomVert>> tmp1;
        for(float u=0;u<=1;u+=ustep)
        {
            vector<GeomVert> tmp;
            for(float v=0;v<=1;v+=wstep)
            {
                //vector<GeomVert> tmp;
                GeomVert vem(splinegen(u,Px,v),splinegen(u,Py,v),splinegen(u,Pz,v));

                tmp.push_back(vem);

            }//w loop
            tmp1.push_back(tmp);
        }//u loop
        BSplinePoint.push_back(tmp1);
    }//end of j loop

}//end of i loop

        for(int i=0;i<BSplinePoint.size();i++)

        {

            for(int j=0;j<BSplinePoint[i].size()-1;j++)

            {

                for(int k=0;k<BSplinePoint[i][j].size()-1;k++)

                {

                 //   glColor3f(0, 0, 0);

                   // glBegin(GL_LINE_LOOP);

                    GeomVert g1(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));

                    GeomVert g2(BSplinePoint[i][j+1][k].GetCo(0),    BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));

                    GeomVert g3(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));

                    GeomVert g4(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    float x1 = BSplinePoint[i][j][k].GetCo(0), y1 = BSplinePoint[i][j][k].GetCo(1), z1 = BSplinePoint[i][j][k].GetCo(2);
                    float x2 = BSplinePoint[i][j+1][k+1].GetCo(0), y2 = BSplinePoint[i][j+1][k+1].GetCo(1), z2 = BSplinePoint[i][j+1][k+1].GetCo(2);
                    float dist = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
                    if(k==0)arclengthu += (float)dist;
                    if(j==0)arclengthv += (float)dist;
                    vector<GeomVert> geomfacet;
                    geomfacet.push_back(g1);    geomfacet.push_back(g2);    geomfacet.push_back(g3);    geomfacet.push_back(g4);
                    outmesh.AddFacet(geomfacet);

                    GeomVert g5(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));

                    GeomVert g6(BSplinePoint[i][j+1][k].GetCo(0), BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));

                    GeomVert g7(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));

                    GeomVert g8(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    vector<GeomVert> geomfacet2;
                    geomfacet2.push_back(g5);    geomfacet2.push_back(g6);    geomfacet2.push_back(g7);    geomfacet2.push_back(g8);
                   // outmesh.AddFacet(geomfacet2);


                }

                }

        }
return outmesh;
 for(float u=0;u<=0.8;u+=ustep)
    {
        for(float w=0;w<=0.8;w+=wstep)
        {
            for(int i=0;i<m-3;i++)
            {
                for(int j=0;j<n-3;j++)      //1 point will be created for this patch
                {
                    for(int subi=0;subi<4;subi++)
                    {for(int subj =0; subj<4;subj++)     //filling point matrix
                    {
                        Px[subi][subj] = pinx[i+subi][j+subj];
                        Py[subi][subj] = piny[i+subi][j+subj];
                        Pz[subi][subj] = pinz[i+subi][j+subj];
                      //  cout<<Px[subi][subj]<<" ";     //<<" "<<Py[subi][subj]<<" "<<Pz[subi][subj]
                    }
                    //cout<<"\n";
                    }
                    //cout<<"getting in splinegen";
                    //getchar();
                    px1 = splinegen(u,Px,w);    py1 = splinegen(u,Py,w);    pz1 = splinegen(u,Pz,w);
                    px2 = splinegen(u+ustep,Px,w);    py2 = splinegen(u+ustep,Py,w);    pz2 = splinegen(u+ustep,Pz,w);
                    px3 = splinegen(u+ustep,Px,w+wstep);    py3 = splinegen(u+ustep,Py,w+wstep);    pz3 = splinegen(u+ustep,Pz,w+wstep);
                    px4 = splinegen(u,Px,w+wstep);    py4 = splinegen(u,Py,w+wstep);    pz4 = splinegen(u,Pz,w+wstep);
                    //cout<<"out of splinegen";

                    //px1 = (floor(px1*1000))/1000;    py1 = (floor(py1*1000))/1000;     pz1 = (floor(pz1*1000))/1000;
                    GeomVert v1(px1,py1,pz1);
                    //px2 = (floor(px2*1000))/1000;    py2 = (floor(py2*1000))/1000;     pz2 = (floor(pz2*1000))/1000;
                    GeomVert v2(px2,py2,pz2);
                    //px3 = (floor(px3*1000))/1000;    py3 = (floor(py3*1000))/1000;    pz3 = (floor(pz3*1000))/1000;
                    GeomVert v3(px3,py3,pz3);
                    //px4 = (floor(px4*1000))/1000;    py4 = (floor(py4*1000))/1000;    pz4 = (floor(pz4*1000))/1000;
                    GeomVert v4(px4,py4,pz4);
                    //cout<<px1<<" "<<py1<<" "<<pz1<<" "<<px2<<" "<<py2<<" "<<pz2<<" "<<px3<<" "<<py3<<" "<<pz3<<" "<<"\n";
                    //getchar();
                    vector<GeomVert> geomfacet;
                    geomfacet.push_back(v1);    geomfacet.push_back(v2);    geomfacet.push_back(v3);    geomfacet.push_back(v4);
                    outmesh.AddFacet(geomfacet);
                }
            }



        }//end of w loop
    }//end of u loop
return outmesh;
}//end of program

Mesh Bspline::BsplineMeshRat(Mesh mesh, string filename, float h[4][4])
{

    Mesh outmesh;

    ifstream file(filename);
    char str[10];

    file>>str;  int n = atoi(str);
    file>>str;  int m = atoi(str);
    cout<<" m: "<<m<<" n: "<<n;

    float *pinx[m], *piny[m], *pinz[m];
    vector<vector<int>> coord;

for(int i=0;i<m;i++)
{
    pinx[i] = (float*) malloc(n*sizeof(float));
    piny[i] = (float*) malloc(n*sizeof(float));
    pinz[i] = (float*) malloc(n*sizeof(float));
}
for(int i=0;i<n;i++)
{
    for(int j=0;j<m;j++)
    {
        file>>str;              pinx[j][i] = atof(str);
        file>>str;              piny[j][i] = atof(str);
        file>>str;              pinz[j][i] = atof(str);

    }
}

float px1 = 0, py1 = 0, pz1 = 0, px2 =0,py2 =0, pz2 =0, px3 =0, py3 =0, pz3 =0, px4 =0, py4 =0, pz4 =0;
int D = 3;
int R = 3;
float Px[4][4], Py[4][4], Pz[4][4];  //point matrix
float pres[m-4][n-4];
float ustep = 0.2, wstep = 0.2;
//cout<<"entering";
//getchar();

/////////////////////////////   Generate the point sets

vector<vector<vector<GeomVert>> > BSplinePoint;
for(int i=0;i<m-3;i++)
{
    for(int j=0;j<n-3;j++)
    {
        vector<vector<GeomVert>> Sa;
        for(int k=0;k<4;k++)
        {
            vector<GeomVert> tmp;
            for(int q=0;q<4;q++)
            {
                GeomVert v(pinx[i+k][j+q],piny[i+k][j+q],pinz[i+k][j+q]);
                tmp.push_back(v);
                Px[k][q] = pinx[i+k][j+q];
                Py[k][q] = piny[i+k][j+q];
                Pz[k][q] = pinz[i+k][j+q];
            }
            Sa.push_back(tmp);

        }
        vector<vector<GeomVert>> tmp1;

        for(float u=0;u<=1;u+=ustep)
        {
            vector<GeomVert> tmp;
            for(float v=0;v<=1;v+=wstep)
            {
                GeomVert vem(splinegenRat(u,Px,v,h),splinegenRat(u,Py,v,h),splinegenRat(u,Pz,v,h));
                tmp.push_back(vem);
            }//w loop
            tmp1.push_back(tmp);
        }//u loop
        BSplinePoint.push_back(tmp1);
    }//end of j loop

}//end of i loop

        for(int i=0;i<BSplinePoint.size();i++)
        {
            for(int j=0;j<BSplinePoint[i].size()-1;j++)
            {
                for(int k=0;k<BSplinePoint[i][j].size()-1;k++)
                {
                    GeomVert g1(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));
                    GeomVert g2(BSplinePoint[i][j+1][k].GetCo(0),    BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));
                    GeomVert g3(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));
                    GeomVert g4(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    vector<GeomVert> geomfacet;
                    geomfacet.push_back(g1);    geomfacet.push_back(g2);    geomfacet.push_back(g3);    geomfacet.push_back(g4);
                    outmesh.AddFacet(geomfacet);

                    GeomVert g5(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));
                    GeomVert g6(BSplinePoint[i][j+1][k].GetCo(0), BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));
                    GeomVert g7(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));
                    GeomVert g8(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    vector<GeomVert> geomfacet2;
                    geomfacet2.push_back(g5);    geomfacet2.push_back(g6);    geomfacet2.push_back(g7);    geomfacet2.push_back(g8);

                }
            }

        }
return outmesh;

}//end of program



Mesh Bspline::BsplineMeshReparam(Mesh mesh, string filename, float urep, float wrep)
{

    Mesh outmesh;

    ifstream file(filename);
    char str[10];

    file>>str;  int n = atoi(str);
    file>>str;  int m = atoi(str);
    cout<<" m: "<<m<<" n: "<<n;

    float *pinx[m], *piny[m], *pinz[m];
    vector<vector<int>> coord;

for(int i=0;i<m;i++)
{
    pinx[i] = (float*) malloc(n*sizeof(float));
    piny[i] = (float*) malloc(n*sizeof(float));
    pinz[i] = (float*) malloc(n*sizeof(float));
}
for(int i=0;i<n;i++)
{
    for(int j=0;j<m;j++)
    {
        file>>str;              pinx[j][i] = atof(str);
        file>>str;              piny[j][i] = atof(str);
        file>>str;              pinz[j][i] = atof(str);

    }
}

float px1 = 0, py1 = 0, pz1 = 0, px2 =0,py2 =0, pz2 =0, px3 =0, py3 =0, pz3 =0, px4 =0, py4 =0, pz4 =0;
int D = 3;
int R = 3;
float Px[4][4], Py[4][4], Pz[4][4];  //point matrix
float pres[m-4][n-4];
float ustep = 0.2, wstep = 0.2;
//cout<<"entering";
//getchar();

/////////////////////////////   Generate the point sets

vector<vector<vector<GeomVert>> > BSplinePoint;
for(int i=0;i<m-3;i++)
{
    for(int j=0;j<n-3;j++)
    {
        vector<vector<GeomVert>> Sa;
        for(int k=0;k<4;k++)
        {
            vector<GeomVert> tmp;
            for(int q=0;q<4;q++)
            {
                GeomVert v(pinx[i+k][j+q],piny[i+k][j+q],pinz[i+k][j+q]);
                tmp.push_back(v);
                Px[k][q] = pinx[i+k][j+q];
                Py[k][q] = piny[i+k][j+q];
                Pz[k][q] = pinz[i+k][j+q];
            }
            Sa.push_back(tmp);

        }
        vector<vector<GeomVert>> tmp1;
        for(float u=0;u<=urep;u+=ustep)
        {
            vector<GeomVert> tmp;
            for(float v=0;v<=wrep;v+=wstep)
            {
                GeomVert vem(splinegen(u,Px,v),splinegen(u,Py,v),splinegen(u,Pz,v));
                tmp.push_back(vem);
            }//w loop
            tmp1.push_back(tmp);
        }//u loop
        BSplinePoint.push_back(tmp1);
    }//end of j loop

}//end of i loop

        for(int i=0;i<BSplinePoint.size();i++)
        {
            for(int j=0;j<BSplinePoint[i].size()-1;j++)
            {
                for(int k=0;k<BSplinePoint[i][j].size()-1;k++)
                {
                    GeomVert g1(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));
                    GeomVert g2(BSplinePoint[i][j+1][k].GetCo(0),    BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));
                    GeomVert g3(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));
                    GeomVert g4(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    vector<GeomVert> geomfacet;
                    geomfacet.push_back(g1);    geomfacet.push_back(g2);    geomfacet.push_back(g3);    geomfacet.push_back(g4);
                    outmesh.AddFacet(geomfacet);

                    GeomVert g5(BSplinePoint[i][j][k].GetCo(0), BSplinePoint[i][j][k].GetCo(1), BSplinePoint[i][j][k].GetCo(2));
                    GeomVert g6(BSplinePoint[i][j+1][k].GetCo(0), BSplinePoint[i][j+1][k].GetCo(1), BSplinePoint[i][j+1][k].GetCo(2));
                    GeomVert g7(BSplinePoint[i][j+1][k+1].GetCo(0), BSplinePoint[i][j+1][k+1].GetCo(1), BSplinePoint[i][j+1][k+1].GetCo(2));
                    GeomVert g8(BSplinePoint[i][j][k+1].GetCo(0), BSplinePoint[i][j][k+1].GetCo(1), BSplinePoint[i][j][k+1].GetCo(2));

                    vector<GeomVert> geomfacet2;
                    geomfacet2.push_back(g5);    geomfacet2.push_back(g6);    geomfacet2.push_back(g7);    geomfacet2.push_back(g8);

                }
            }

        }
return outmesh;

}//end of program


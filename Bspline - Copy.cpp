#include "mesh.h"
#include "meshGeneration.h"
#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Bspline.h"
float Bspline::splinegen(float u, float a[4][4], float v)
{

    //QString t = QString::number(a.size()+1, 10);

//    setWindowTitle(t);

    float b[4], c[4];

    b[0] = pow(1-u,3)/6.0;

    b[1] = (3*pow(u,3) - 6*u*u +4)/6.0;

    b[2] = (-3*u*u*u +3*u*u + 3*u + 1)/6.0;

    b[3] =  u*u*u/6.0;


    c[0] = pow(1-v,3)/6.0;

    c[1] = (3*pow(v,3) - 6*v*v +4)/6.0;

    c[2] = (-3*v*v*v +3*v*v + 3*v + 1)/6.0;

    c[3] =  v*v*v/6.0;



    float x;

    for(int i=0;i<4;i++)

    {

        for(int j=0;j<4;j++)

        {

            x+=a[i][j]*b[i]*c[j];

        }

    }

    return x;

}
float splinegen2(float u1, float P[4][4], float w1)
{
//    cout<<"inside splinegen";
    //getchar();
    float u[4] = {u1*u1*u1,u1*u1,u1,1};
    float w[4] =  { w1*w1*w1, w1*w1, w1,1};
    float M[4][4] = {{-1,3,-3,1},{3,-6,3,0},{-3,0,3,0},{1,4,1,0}};
    float M1[4][4];
    float mult[4], mult2[4], mult3[4];
    float finally=0;
    ////////////////////
    //transpose matrix
    ///////////////////

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            M1[i][j] = M[j][i];

     for(int i=0; i<4; ++i)             //M1 * w
            for(int k=0; k<4; k++)
                 { // mult[i]+=M1[i][k]*w[k];
                    mult[i]+=M1[i][k]*w[k];
                 }

    for(int i=0; i<4; i++)              //P * M1 * w
            for(int k=0; k<4; k++)
                 {//mult2[i] += P[i][k]*mult[k];
                 mult2[i] += P[i][k]*mult[k];}//   cout<<"  M1*w: "<<mult[i];}


    for(int i=0; i<4; i++)              //M * P * M1 * w
            for(int k=0; k<4; k++)
                 mult3[i] += M[i][k]*mult2[k];

     for(int k=0; k<4; k++)  // u * M * P * M1 * w
            finally += u[k]*mult3[k];

    return (float)(finally/36);
}
Mesh Bspline::BsplineMesh(Mesh mesh, string filename)
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
cout<<"entering";
getchar();
 for(float u=0;u<=0.8;u+=ustep)
    {
        for(float w=0;w<=0.8;w+=wstep)
        {
            for(int i=0;i<m-4;i++)
            {
                for(int j=0;j<n-4;j++)      //1 point will be created for this patch
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

                    px1 = (floor(px1*1000))/1000;    py1 = (floor(py1*1000))/1000;     pz1 = (floor(pz1*1000))/1000;
                    GeomVert v1(px1,py1,pz1);
                    px2 = (floor(px2*1000))/1000;    py2 = (floor(py2*1000))/1000;     pz2 = (floor(pz2*1000))/1000;
                    GeomVert v2(px2,py2,pz2);
                    px3 = (floor(px3*1000))/1000;    py3 = (floor(py3*1000))/1000;    pz3 = (floor(pz3*1000))/1000;
                    GeomVert v3(px3,py3,pz3);
                    px4 = (floor(px4*1000))/1000;    py4 = (floor(py4*1000))/1000;    pz4 = (floor(pz4*1000))/1000;
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



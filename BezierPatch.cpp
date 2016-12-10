#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>

#include "MeshGeneration.h"
#include "meshGeneration.h"
using namespace std;
class Bezier
{
public:
    deCasteljauCurve3D(vector<vector<float>> ctlPt,float u,vector<float> &outPt);
    deCasteljau(Mesh mesh, vector<vector<int>> Pij,float u,float w,vector<float>& outPt);
    BezierMesh(Mesh inMesh, Mesh& outMesh,int stepu,int stepw,vector<vector<int>> Pij);
    BezierMesh(Mesh inMesh, Mesh& outMesh,int stepu,int stepw,vector<vector<int>> Pij);
};
void Bezier::deCasteljauCurve3D(vector<vector<float>> ctlPt,float u,vector<float> &outPt){

    int n=ctlPt.size()-1;



    vector<vector<float>> p=ctlPt;



    for(int i=1;i<=n;i++){

        for(int k=0;k<=n-i;k++){

            for(int coor=0;coor<3;coor++)p[k][coor]=p[k][coor]*(1-u)+u*p[k+1][coor];

        }

    }



    outPt=p[0];

}


void Bezier::deCasteljau(Mesh mesh, vector<vector<int>> Pij,float u,float w,vector<float>& outPt){



    int m=Pij.size()-1;

    int n=Pij[0].size()-1;





    vector<vector<float>> pi;//new fun



    for(int i=0;i<=m;i++){

        vector<vector<float>> p;

        for(int j=0;j<=n;j++){

            vector<float> pt;

            GeomVert vertex = mesh.GetGeomVertex(Pij[i][j]);

            for(int coor=0;coor<3;coor++)pt.push_back(vertex.GetCo(coor));

            p.push_back(pt);

        }

        vector<float> outPi;

        deCasteljauCurve3D(p, w, outPi);

//        for(int j=1;j<=n;j++){

//            for(int k=0;k<=n-j;k++){

//                for(int coor=0;coor<3;coor++){

//                    p[k][coor]=p[k][coor]*(1-w)+w*p[k+1][coor];

//                }

//            }

//        }

        pi.push_back(outPi);

    }

    deCasteljauCurve3D(pi,u,outPt);

}


void BezierMesh(Mesh inMesh, Mesh& outMesh,int stepu,int stepw,vector<vector<int>> Pij){

    float uInter=1/float(stepu);

    float wInter=1/float(stepw);



    for(float u=0;u<1;u=u+uInter){

        for(float w=0;w<1;w=w+wInter){

            //generate four points

            vector<GeomVert> geomfacet;

            vector<float> pt;



            deCasteljau(inMesh, Pij, u, w, pt);

            GeomVert v1(pt[0],pt[1],pt[2]);



            deCasteljau(inMesh, Pij, u, w+wInter, pt);

            GeomVert v2(pt[0],pt[1],pt[2]);



            deCasteljau(inMesh, Pij, u+uInter, w+wInter, pt);

            GeomVert v3(pt[0],pt[1],pt[2]);



            deCasteljau(inMesh, Pij, u+uInter, w, pt);

            GeomVert v4(pt[0],pt[1],pt[2]);



            geomfacet.push_back(v1);

            geomfacet.push_back(v4);

            geomfacet.push_back(v3);

            geomfacet.push_back(v2);




            outMesh.AddFacet(geomfacet);

        }

    }



}

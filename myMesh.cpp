#include "MeshGeneration.h"
#include "meshGeneration.h"
#include "myMesh.h"
#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <iomanip>
#include "Catmull.h"
#include "Loop.h"
#include "Bspline.h"
#include <algorithm>

using namespace std;
Mesh getfile(string filename)
{
    ifstream input(filename);
    char str[20];
    input>>str;

    if(strcmp(str,"OFF")!=0)    {cout<<"Not an OFF file"; exit(0);}
    input>>str;

    int nVertices = atoi(str);
    input>>str;
    int nFaces = atoi(str);
    input>>str;
    int nEdges = atoi(str);
    //cout<<nVertices<<" "<<nFaces<<" "<<nEdges<<"\n";
    vector<GeomVert> coord;
    for(int i=0;i<nVertices;i++)
    {
        input>>str;    float x = atof(str);
        input>>str;    float y = atof(str);
        input>>str;    float z = atof(str);

        GeomVert v(x,y,z);
        coord.push_back(v);
    }
    Mesh outmesh;
    for(int i=0;i<nFaces;i++)
    {
        input>>str;     int nov = atoi(str);
        vector<GeomVert> geomfacet;
        for(int j=0;j<nov;j++)
        {
            input>>str;
            //cout<<str<<" ";
            int index = atoi(str);
            geomfacet.push_back(coord[index]);
        }
        outmesh.AddFacet(geomfacet);

    }
    return outmesh;
}


Mesh DooSabinFace(Mesh m)
{
    Mesh mout;
    int noVertices = m.GetNumberVertices();
    int noFace = m.GetNumberFacets();
    int noEdges = m.GetNumberEdges();

    //int a,b,c,d;
    for(int iface=0;iface<noFace;iface++)
    {
        vector<GeomVert> geomfacet;
        TopoFacet f = m.GetFacet(iface);

        int nVertex = f.GetNumberVertices();
        int nEdge = f.GetNumberEdges();
        float centroidx = 0,centroidy = 0, centroidz = 0;
        /////////////////////
        //Find the centroid
        //////////////////////


        for(int i=0;i<nVertex;i++)
        {
            int index = f.GetVertexInd(i);
            GeomVert vertex = m.GetGeomVertex(index);
            centroidx += vertex.GetCo(0);
            centroidy += vertex.GetCo(1);
            centroidz += vertex.GetCo(2);
            //cout<<" Vertex "<<i<<": "<<vertex.GetCo(0)<<" "<<vertex.GetCo(1)<<" "<<vertex.GetCo(2)<<"\n";
        }
        //getchar();
        centroidx /= nVertex;        centroidy /= nVertex;        centroidz /= nVertex;

        float x1,y1,z1;        float x2,y2,z2;

        for(int i=0;i<nEdge;i++)
        {
            int indext1 = f.GetIncEdge(i);
            TopoEdge t1 = m.GetEdge(indext1);
            int indext2;
            TopoEdge t2;
            if((i+1) == (nEdge))
            {
                indext2 = f.GetIncEdge(0);
                t2 = m.GetEdge(indext2);

            }
            else
            {
                indext2 = f.GetIncEdge(i+1);
                t2 = m.GetEdge(indext2);

            }
                //t2 = f.GetIncEdge(i+1);

            GeomVert vertexact1 = m.GetGeomVertex(t1.GetVertex(0));
            GeomVert vertexact2 = m.GetGeomVertex(t1.GetVertex(1));

            x1 = (vertexact1.GetCo(0) + vertexact2.GetCo(0))/2;
            y1 = (vertexact1.GetCo(1) + vertexact2.GetCo(1))/2;
            z1 = (vertexact1.GetCo(2) + vertexact2.GetCo(2))/2;

            int idvert1;    int idvert2;
            vertexact1 = m.GetGeomVertex(t2.GetVertex(0));
            vertexact2 = m.GetGeomVertex(t2.GetVertex(1));

            x2 = (vertexact1.GetCo(0) + vertexact2.GetCo(0))/2;
            y2 = (vertexact1.GetCo(1) + vertexact2.GetCo(1))/2;
            z2 = (vertexact1.GetCo(2) + vertexact2.GetCo(2))/2;

            if(t2.GetVertex(0)==t1.GetVertex(1) || t2.GetVertex(0)==t1.GetVertex(0))
                idvert1 = t2.GetVertex(0);
            else    idvert1 = t2.GetVertex(1);

            GeomVert v = m.GetGeomVertex(idvert1);
            float x = (centroidx + x1 + x2 + v.GetCo(0) )/4;
            float y = (centroidy + y1 + y2 + v.GetCo(1) )/4;
            float z = (centroidz + z1 + z2 + v.GetCo(2) )/4;
            GeomVert newvert(x,y,z);
            newvert.setMapp(idvert1);
           // cout<<"Gen vertex "<<i<<": "<<x<<" "<<y<<" "<<z<<"\n";

            geomfacet.push_back(newvert);
        }
        if(geomfacet.size()!=nVertex && geomfacet.size()<3)
        {
            cout<<"No of vertex not matching face. "<<geomfacet.size();
            exit(0);
        }
        mout.AddFacet(geomfacet);

        //for(int vIndex=0;vIndex<nIncVertex;vIndex++)output<<f.GetVertexInd(vIndex)<<" ";
        //output<<"\n";
    }//iface

    return mout;
}


void GenSubEdge2(TopoFacet t1, Mesh m, float *v1dx, float *v1dy, float *v1dz, float *v2dx, float *v2dy, float *v2dz, int iEdge)
{
    float cenx = 0, ceny = 0, cenz = 0;
    TopoEdge e1, e2, e3;
    for(int i=0;i<t1.GetNumberVertices();i++)
    {
        GeomVert v = m.GetGeomVertex(t1.GetVertexInd(i));       //get the ith vertex
        cenx += v.GetCo(0); ceny += v.GetCo(1);     cenz += v.GetCo(2);

    }//end of i loop
    cenx /= t1.GetNumberVertices(); ceny /= t1.GetNumberVertices();     cenz /= t1.GetNumberVertices();

    if(iEdge!=0)
        e1 = m.GetEdge(t1.GetIncEdge(iEdge-1));
    else
        e1 = m.GetEdge(t1.GetIncEdge(t1.GetNumberEdges()-1));

        e2 = m.GetEdge(t1.GetIncEdge(iEdge));

        e3 = m.GetEdge(t1.GetIncEdge(((iEdge+1)%t1.GetNumberVertices())));

        // common vertex between e1 and e2
        int v12, v11, v22;      // common vertex, other vertex of e1, other vertex of e2

        if(e1.GetVertex(0) == e2.GetVertex(1) || e1.GetVertex(0) == e2.GetVertex(0))
        {
            v12 = e1.GetVertex(0);
            v11 = e1.GetVertex(1);
            if(e1.GetVertex(0) == e2.GetVertex(0))
                v22 = e2.GetVertex(1);
            else v22 = e2.GetVertex(0);
        }
        else
        {
            v12 = e1.GetVertex(1);
            v11 = e1.GetVertex(0);
            if(e1.GetVertex(1) == e2.GetVertex(0))
                v22 = e2.GetVertex(1);
            else v22 = e2.GetVertex(0);

        }


        GeomVert vert11 = m.GetGeomVertex(v11);     GeomVert vert12 = m.GetGeomVertex(v12);     GeomVert vert22 = m.GetGeomVertex(v22);

        float vcommonx1 = (vert11.GetCo(0)+vert12.GetCo(0))/2 ;
        float vcommony1 = (vert11.GetCo(1)+vert12.GetCo(1))/2 ;
        float vcommonz1 = (vert11.GetCo(2)+vert12.GetCo(2))/2 ;

        float vcommonx2 = (vert22.GetCo(0)+vert12.GetCo(0))/2 ;
        float vcommony2 = (vert22.GetCo(1)+vert12.GetCo(1))/2 ;
        float vcommonz2 = (vert22.GetCo(2)+vert12.GetCo(2))/2 ;

        *v2dx = (cenx+vcommonx1+vert12.GetCo(0)+vcommonx2)/4;
        *v2dy = (ceny+vcommony1+vert12.GetCo(1)+vcommony2)/4;
        *v2dz = (cenz+vcommonz1+vert12.GetCo(2)+vcommonz2)/4;


        int v21, v32;      //v2, v21, v32 common vertex, other vertex of e2, other vertex of e3
        if(e2.GetVertex(0) == e3.GetVertex(1) || e2.GetVertex(0) == e3.GetVertex(0))
        {
            v22 = e2.GetVertex(0);
            v21 = e2.GetVertex(1);
            if(e2.GetVertex(0) == e3.GetVertex(0))
                v32 = e3.GetVertex(1);
            else v32 = e3.GetVertex(0);
        }
        else
        {
            v22 = e2.GetVertex(1);
            v21 = e2.GetVertex(0);
            if(e2.GetVertex(1) == e3.GetVertex(0))
                v32 = e3.GetVertex(1);
            else v32 = e3.GetVertex(0);

        }
        //cout<<"vertex: "<<v21<<" "<<v22<<" "<<v32;
        //getchar();
        GeomVert vert21 = m.GetGeomVertex(v21);     vert22 = m.GetGeomVertex(v22);     GeomVert vert32 = m.GetGeomVertex(v32);
        vcommonx1 = (vert21.GetCo(0)+vert22.GetCo(0))/2 ;
        vcommony1 = (vert21.GetCo(1)+vert22.GetCo(1))/2 ;
        vcommonz1 = (vert21.GetCo(2)+vert22.GetCo(2))/2 ;

        vcommonx2 = (vert22.GetCo(0)+vert32.GetCo(0))/2 ;
        vcommony2 = (vert22.GetCo(1)+vert32.GetCo(1))/2 ;
        vcommonz2 = (vert22.GetCo(2)+vert32.GetCo(2))/2 ;

        *v1dx = (cenx+vcommonx1+vert22.GetCo(0)+vcommonx2)/4;
        *v1dy = (ceny+vcommony1+vert22.GetCo(1)+vcommony2)/4;
        *v1dz = (cenz+vcommonz1+vert22.GetCo(2)+vcommonz2)/4;
        //cout<<"result:"<<*v1dx<<" "<<*v1dy<<" "<<*v1dz<<"|"<<*v2dx<<" "<<*v2dy<<" "<<*v2dz;
        //getchar();
}


Mesh DooSabinEdge(Mesh mDooF, Mesh m)
{
    int nVertex = m.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();
    for(int iEdge = 0;iEdge<nEdges;iEdge++)
    {
        TopoEdge e = m.GetEdge(iEdge);
        if(e.GetNumberIncFacets()==1)
        {
           // cout<<"Boundary edge. Continue\n";
            continue;
        }
        else if(e.GetNumberIncFacets()!=2)
        {
            //cout<<"Edge: "<<e.GetNumberIncFacets()<<" Unknown !! ";
            //exit(0);
            //int f1 =  e.GetIncFacet(0);            int f2 =  e.GetIncFacet(1);            int f3 =  e.GetIncFacet(2);            int f4 =  e.GetIncFacet(3);
            //cout<<f1<<" "<<f2<<" "<<f3<<" "<<f4<<" \n";
        }
        else
        {
            vector<GeomVert> geomfacet;
            //cout<<"Edge: 2.cont\n";
            int v1 = e.GetVertex(0);
            int v2 = e.GetVertex(1);
            GeomVert vdiff1 = m.GetGeomVertex(v1);
            GeomVert vdiff2 = m.GetGeomVertex(v2);

            int v3,v4;
            int f1 =  e.GetIncFacet(0);     //face ID 1
            int f2 =  e.GetIncFacet(1);     //face ID 2
            TopoFacet t1 = m.GetFacet(f1);
            TopoFacet t2 = m.GetFacet(f2);
            bool foun = false;
            int kkk,kk;
            for(kkk=0;kkk<t1.GetNumberEdges();kkk++)
            {
                TopoEdge ee = m.GetEdge(t1.GetIncEdge(kkk));    //gets the kkk th edge of t1
                int index = m.FindTopoEdge(ee);
                if(index == iEdge)          //kkk th edge is the one
                    {foun = true;   break;}
            }


            if(foun == false)
            {  cout<<"no Match found";  exit(0);    }

            foun = false;
            for(kk=0; kk<t2.GetNumberEdges(); kk++)
            {
                TopoEdge ee = m.GetEdge(t2.GetIncEdge(kk));    //gets the kkk th edge of t1
                int index = m.FindTopoEdge(ee);
                if(index == iEdge)          //kkk th edge is the one
                    {foun = true;   break;}
            }

            int el1 = t2.GetIncEdge(kk);
            TopoEdge ee1 = m.GetEdge(el1);
            GeomVert vv1 = m.GetGeomVertex(ee1.GetVertex(0));
            GeomVert vv2 = m.GetGeomVertex(ee1.GetVertex(1));
            GeomVert vx1 = m.GetGeomVertex(v1);
            GeomVert vx2 = m.GetGeomVertex(v2);
          //  cout<<" ee1:"<<vv1.GetCo(0)<<" "<<vv1.GetCo(1)<<" "<<vv1.GetCo(2)<<"|"<<vv2.GetCo(0)<<" "<<vv2.GetCo(1)<<" "<<vv2.GetCo(2)<<"\n";
           // cout<<" e:"<<vx1.GetCo(0)<<" "<<vx1.GetCo(1)<<" "<<vx1.GetCo(2)<<"|"<<vx2.GetCo(0)<<" "<<vx2.GetCo(1)<<" "<<vx2.GetCo(2)<<"\n";

            if(foun == false)
            {
                cout<<"no Match found";  exit(0);
            }

            float v1dx,v1dy,v1dz, v2dx,v2dy,v2dz, v3dx,v3dy,v3dz, v4dx,v4dy,v4dz;
            float v1dxq,v1dyq,v1dzq, v2dxq,v2dyq,v2dzq, v3dxq,v3dyq,v3dzq, v4dxq,v4dyq,v4dzq;

            GenSubEdge2(t1,m,&v2dx,&v2dy,&v2dz,&v1dx,&v1dy,&v1dz, kkk);
            GeomVert v1d(v1dx,v1dy,v1dz);
            GeomVert v2d(v2dx,v2dy,v2dz);

            GenSubEdge2(t2,m,&v3dx,&v3dy,&v3dz,&v4dx,&v4dy,&v4dz,kk);

            GeomVert v3d(v3dx,v3dy,v3dz);
            GeomVert v4d(v4dx,v4dy,v4dz);
            float minx1 = 100,miny1 = 100,minz1 = 100;
            float minx2 = 100,miny2 = 100,minz2 = 100;
            float minx3 = 100,miny3 = 100,minz3 = 100;
            float minx4 = 100,miny4 = 100,minz4 = 100;
            float xtest,ytest,ztest;
            int vTot = mDooF.GetNumberVertices();
            //GeomVert vg1,vg2,vg3,vg4;
            for(int vertex=0;vertex<vTot;vertex++)
            {
                GeomVert v = mDooF.GetGeomVertex(vertex);
                xtest = v.GetCo(0);
                ytest = v.GetCo(1);
                ztest = v.GetCo(2);

                if(minx1 >= fabs(xtest-v1dx) && miny1 >= fabs(ytest-v1dy) && minz1 >= fabs(ztest-v1dz))
                    { minx1 = fabs(xtest-v1dx);     miny1 = fabs(ytest-v1dy);          minz1 = fabs(ztest-v1dz); v1d = v;}

                if(minx2 >= fabs(xtest-v2dx) && miny2 >= fabs(ytest-v2dy) && minz2 >= fabs(ztest-v2dz))
                { minx2 = fabs(xtest-v2dx);     miny2 = fabs(ytest-v2dy);          minz2 = fabs(ztest-v2dz); v2d = v;}

                if(minx3 >= fabs(xtest-v3dx) && miny3 >= fabs(ytest-v3dy) && minz3 >= fabs(ztest-v3dz))
                    {minx3 = fabs(xtest-v3dx);     miny3 = fabs(ytest-v3dy);          minz3 = fabs(ztest-v3dz); v3d = v;}

                if(minx4 >= fabs(xtest-v4dx) && miny4 >= fabs(ytest-v4dy) && minz4 >= fabs(ztest-v4dz))
                    {minx4 = fabs(xtest-v4dx);     miny4 = fabs(ytest-v4dy);          minz4 = fabs(ztest-v4dz);    v4d = v;}
            }
            geomfacet.push_back(v2d);
            geomfacet.push_back(v1d);
            geomfacet.push_back(v3d);
            geomfacet.push_back(v4d);

            mDooF.AddFacet(geomfacet);
        }//else == 2

    }//for(int iEdge = 0;iEdge<nEdges;iEdge++)
return mDooF;
}

Mesh DooSabinVertex(Mesh mDooFE,Mesh m)
{
    int nVertex = mDooFE.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();
    //Mesh mDooFEV = mDooFE;
    for(int iVertex = 0;iVertex<nVertex;iVertex++)
    {
        GeomVert g = mDooFE.GetGeomVertex(iVertex);
        if(g.getMapp()== -1)
            {cout<<"vertex not mapped"; continue;}
        vector<GeomVert> geomfacet;
        geomfacet.push_back(g);
        //cout<<iVertex<<" mapped to: "<<g.getMapp()<<" ";
//        getchar();
        for(int innerVertex = 0;innerVertex<nVertex;innerVertex++)
        {
            GeomVert g1 = mDooFE.GetGeomVertex(innerVertex);
            if(g1.getMapp()==g.getMapp() && innerVertex != iVertex)
            {
                bool resides = false;
                g1.setMapp(-1);
                //check if duplicate vertex
                for(int r=0;r<geomfacet.size();r++)
                {
                    GeomVert local = geomfacet[r];
                    if(local.GetCo(0)==g1.GetCo(0) && local.GetCo(1)==g1.GetCo(1) && local.GetCo(2)==g1.GetCo(2))
                        resides = true;
                }
                if(resides==false)
                geomfacet.push_back(g1);
            }

        }//end of inner vertex
        if(geomfacet.size()>=3)
            mDooFE.AddFacet(geomfacet);
        //else
        //{
         //   cout<<"geomfacet vertex does not match"<<geomfacet.size();
         //   continue;
        //}
    }//end of for loop iVertex for all vertices

return mDooFE;

}//end of DooSabinVertex

void bezform(vector<vector<float>> ctlPt,float u,vector<float> &outPt,vector<float> h)
{
        int n=ctlPt.size()-1;
    vector<vector<float>> p=ctlPt;
//    for(i=0;i<n;i++)
 //   {

 //   }


}
void deCasteljauCurve3D(vector<vector<float>> ctlPt,float u,vector<float> &outPt)
{
    int n=ctlPt.size()-1;
    vector<vector<float>> p=ctlPt;

    for(int i=1;i<=n;i++){
        for(int k=0;k<=n-i;k++){
            for(int coor=0;coor<3;coor++)p[k][coor]=p[k][coor]*(1-u)+u*p[k+1][coor];
        }
    }
    outPt=p[0];
}


void deCasteljau(Mesh mesh, vector<vector<int>> Pij,float u,float w,vector<float>& outPt)
{
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
bool checkG1(Mesh mes)
{
    int noFaces = mes.GetNumberFacets();
    TopoFacet t = mes.GetFacet(0);
    vector<int> scanned;
    scanned.push_back(0);
    //do{

  //  }while(std::find(vector.begin(), vector.end(), item) != vector.end());



}

float dist(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return ((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2)) + ((z1-z2)*(z1-z2));
}

float checkPlanar(Mesh m)      //check if volume of tetrahedron is zero
{
    int nFaces = m.GetNumberFacets();
    float avgVol = 0;
    for(int i=0;i<nFaces;i++)
    {
        TopoFacet face = m.GetFacet(i);
        if(face.GetNumberVertices()!=4)
            {cout<<"Not four "<<face.GetNumberVertices(); continue;}
        int vind1 = face.GetVertexInd(0);   int vind2 = face.GetVertexInd(1);   int vind3 = face.GetVertexInd(2);   int vind4 = face.GetVertexInd(3);
        GeomVert v1 = m.GetGeomVertex(vind1);       GeomVert v2 = m.GetGeomVertex(vind2);
        GeomVert v3 = m.GetGeomVertex(vind3);       GeomVert v4 = m.GetGeomVertex(vind4);
        //Find area  of first three vertices
//        vector<vector<float>> v;
//        vector<float> coord;
//        coord.push_back(v1.GetCo(0));   coord.push_back(v1.GetCo(1));   coord.push_back(v1.GetCo(2));
//        v.push_back(coord);
//        coord.clear();
//        coord.push_back(v2.GetCo(0));   coord.push_back(v2.GetCo(1));   coord.push_back(v2.GetCo(2));
//        v.push_back(coord);
//        coord.clear();
//        coord.push_back(v3.GetCo(0));   coord.push_back(v3.GetCo(1));   coord.push_back(v3.GetCo(2));
//        v.push_back(coord);
//        coord.clear();
//        coord.push_back(v4.GetCo(0));   coord.push_back(v4.GetCo(1));   coord.push_back(v4.GetCo(2));
//        v.push_back(coord);

        ////////////////////////////////////    http://keisan.casio.com/exec/system/1329962711  ///////////////////////
        float dist4 = dist(v1.GetCo(0),v1.GetCo(1),v1.GetCo(2),v2.GetCo(0),v2.GetCo(1),v2.GetCo(2));    float a4 = dist4;
        float dist5 = dist(v1.GetCo(0),v1.GetCo(1),v1.GetCo(2),v3.GetCo(0),v3.GetCo(1),v3.GetCo(2));    float a5 = dist5;
        float dist6 = dist(v2.GetCo(0),v2.GetCo(1),v2.GetCo(2),v3.GetCo(0),v3.GetCo(1),v3.GetCo(2));    float a6 = dist6;

        float dist1 = dist(v1.GetCo(0),v1.GetCo(1),v1.GetCo(2),v4.GetCo(0),v4.GetCo(1),v4.GetCo(2));    float a1 = dist1;
        float dist2 = dist(v2.GetCo(0),v2.GetCo(1),v2.GetCo(2),v4.GetCo(0),v4.GetCo(1),v4.GetCo(2));    float a2 = dist2;
        float dist3 = dist(v3.GetCo(0),v3.GetCo(1),v3.GetCo(2),v4.GetCo(0),v4.GetCo(1),v4.GetCo(2));    float a3 = dist3;

        //cout<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<a5<<" "<<a6<<" ";

        float volumesq = (a1*a5*(a2+a3+a4+a6-a1-a5)  +
                          a2*a6*(a1+a3+a4+a5-a2-a6)  +
                          a3*a4*(a1+a2+a5+a6-a3-a4)  +
                          -(a1*a2*a4)-(a2*a3*a5)-(a1*a3*a6)-(a4*a5*a6)
                                  )/144.0;
        //cout<<volumesq<<"\n";
        //getchar();
        avgVol += (float)(volumesq);
        //cout<<"avgvol "<<avgVol;

        //getchar();
    }//end of face loop

return avgVol/nFaces;
}


Mesh BezierPatch(Mesh mes, string filename, int us, int ws)
{
//getchar();
Mesh outMesh;
int nVertices = mes.GetNumberVertices();

ifstream file(filename);//"C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusionpoints.off");
char str[10];

file>>str;  int n = atoi(str);
file>>str;  int m = atoi(str);
//cout<<" m: "<<m<<" n: "<<n;

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
        file>>str;
        pinx[j][i] = atof(str);


        file>>str;
        piny[j][i] = atof(str);

        file>>str;
        pinz[j][i] = atof(str);

//       bool match = false;
    }
}

for(int i=0;i<m;i++)
{
    vector<int> xyz;
    for(int j=0;j<n;j++)
    {
        GeomVert v =mes.GetGeomVertex(0);
        int vertsearch;
        bool match = false;
        for( vertsearch = 0; vertsearch< nVertices; vertsearch++)
        {
            v = mes.GetGeomVertex(vertsearch);
            if(v.GetCo(0) == pinx[i][j] && v.GetCo(1) == piny[i][j] && v.GetCo(2) == pinz[i][j])
                    {xyz.push_back(vertsearch); match = true; break; }
        }
        if(match == false)
            {cout<<"not matched";
            cout<<v.GetCo(0)<<" "<<pinx[i][j]<<" "<<v.GetCo(0)-pinx[i][j]<<" ";
            cout<<v.GetCo(1)<<" "<<piny[i][j]<<" "<<v.GetCo(1)-piny[i][j]<<" ";
            cout<<v.GetCo(2)<<" "<<pinz[i][j]<<" "<<v.GetCo(2)-pinz[i][j]<<" ";
            exit(0); }


    }//end of j loop
  //  cout<<"\nsize xyz "<<xyz.size();
    coord.push_back(xyz);


}//end of i loop

BezierMesh(mes, outMesh,us,ws,coord);
cout<<"average volume: "<<checkPlanar(outMesh);
return outMesh;

}//end of function


void outputLoop(string fileinput, string meshcreated, string fileoutput)
{
    Mesh m = getfile(fileinput);
    Loop lup;
    Mesh inmesh = lup.LoopMeshGen(m);
    outputOFF_mesh(inmesh, meshcreated);

    Mesh outmesh = lup.createVertex(inmesh,5/8);
    cout<<"created vertex outputloop";

    Mesh m1 = lup.createNet(outmesh);

    outputOFF_mesh(m1, fileoutput);

}
void outputbspline(string fileinput, string fileoutput, string filerat, string filereparam, string filepoints,string fileNURB)
{
    Bspline bip;
    Mesh m = getfile(fileinput);
    int arclengthu = 0;
    int arclengthv = 0;
    cout<<"\n< create normal b-spline>";
    Mesh outmesh = bip.BsplineMesh(m,filepoints);//, arclengthu, arclengthv);
    outputOFF_mesh(outmesh, fileoutput);
    cout<<"\n< create NURB b-spline>";
   // outmesh = bip.BsplineMeshNURB(m, filepoints);
    //outputOFF_mesh(outmesh, fileNURB);
    float h[4][4] = {{1,1,5,1},{2,3,4,1},{1,5,6,8},{2,2,2,2}};
    m = getfile(fileinput);
    /*cout<<"\n<create rational b-spline>";
   // outmesh = bip.BsplineMeshRat(m,filepoints,h);
    //outputOFF_mesh(outmesh, filerat);

    //getchar();
    float urep=1000, wrep=1000;
    float times  = 1;
    do
    {
        times = times*10;
        urep  = times/(float)sqrt(arclengthu);
        wrep = times/(float)sqrt(arclengthv);
    }while (urep>1 || wrep>1);
if(urep<0.1)
    urep = urep*10;
if(wrep<0.1)
    wrep = wrep*10;
    cout<<"arclengthu:"<<urep;
    cout<<" arclengthv:"<<wrep;
    urep =0.9;
    wrep = 0.6;
    m = getfile(fileinput);
    cout<<"\n<create re-parameterized b-spline";
    outmesh = bip.BsplineMeshReparam(m,filepoints,urep,wrep);
    outputOFF_mesh(outmesh, filereparam);
*/
}

void outputbez(string fileinput, string fileoutput, string filepoints, int us, int ws)
{
    Mesh mbez = getfile(fileinput);
    Mesh mb = BezierPatch(mbez, filepoints, us,ws);
    outputOFF_mesh(mb, fileoutput);

}

void outputDooSabin(string fileinput, string fileoutput, bool flag)
{
    Mesh m = getfile(fileinput);
    if(flag ==1)
    {
    Catmull cat;
    Mesh vmap = cat.createVertex(m);
    Mesh v1 = cat.createNet2(vmap);
    Mesh vmap2 = cat.createVertex(v1);
    Mesh out = cat.createNet2(vmap2);
    outputOFF_mesh(out,fileoutput);
    return;
    }
    Mesh mDooF  = DooSabinFace(m);
    //cout<<"out of face";
    //getchar();
    Mesh mDooFE =  DooSabinEdge(mDooF,m);
    cout<<"out of edge";
    //getchar();
    Mesh mDooFEV = DooSabinVertex(mDooFE,m);

    Mesh  mDooF1  = DooSabinFace(mDooFEV);
outputOFF_mesh(mDooFEV,fileoutput);
return;
    //cout<<"out of face";
    //getchar();
    Mesh mDooFE1 =  DooSabinEdge(mDooF1,m);
    cout<<"out of edge";
    //getchar();
    Mesh mDooFEV1 = DooSabinVertex(mDooFE1,m);
    //filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\DooSabin.off";
    outputOFF_mesh(mDooFEV1,fileoutput);

}

void outputCatmull(string fileinput, string fileoutput)
{
    Mesh m = getfile(fileinput);
    Catmull cat;

    Mesh vmap = cat.createVertex(m);
    Mesh vfinal = cat.createNet2(vmap);

    Mesh vmap2 = cat.createVertex(vfinal);
    Mesh vfinal2 = cat.createNet2(vmap2);
    //filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\catmull.off";
    outputOFF_mesh(vfinal2,fileoutput);

}

int main()
{

    cout<<"Main program\n";
   /* string filename  =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusion.off";
    string fileLoop  =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\loopNet.off";
    string fileoutloop= "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\loop.off";
    string filename2 =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusionpoints.off";
    string fileBspline ="C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bspline.off";
    string fileBNURB ="C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bsplineNURB.off";
    string fileBRat ="C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bsplineRat.off";
    string fileBRep ="C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bsplineReparam.off";
    string fileBez = "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bezier.off";
    string fileDoo = "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\DooSabin.off";
    string fileCat =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\catmull.off";
    */
    string filename = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\input\\extrusion.off";
	string fileLoop = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\input\\loopNet.off";
	string fileoutloop = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\loop.off";
	string filename2 = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\input\\extrusionpoints.off";
	string fileBspline = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\bspline.off";
	string fileBNURB = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\bsplineNURB.off";
	string fileBRat = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\bsplineRat.off";
	string fileBRep = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\bsplineReparam.off";
	string fileBez = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\bezier.off";
	string fileDoo = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\DooSabin.off";
	string fileCat = "C:\\Users\\mesay\\OneDrive\\Documents\\Visual\ Studio\ 2015\\Projects\\Meshing\\Meshing\\output\\catmull.off";
do{
    Mesh m = getfile(filename);
    cout<<"\n< Creating loop >\n";
    //outputLoop(filename, fileLoop, fileoutloop);
    cout<<"\n< Loop Created >\n";
    cout<<"\n< Creating B-spline >\n";
    //getchar();
   outputbspline(filename, fileBspline, fileBRat, fileBRep, filename2,fileBNURB);
    cout<<"\n< B-spline created >\n";
    cout<<"\n< Creating Bezier >\n";
    //outputbez(filename, fileBez, filename2, 80, 80);
    cout<<"\n< Bezier Created >\n";
    cout<<"\n< Creating DooSabin >\n";
    outputDooSabin(filename, fileDoo,0);
    cout<<"\n< Doo-sabin Created >\n";
    cout<<"\n< Creating Catmull >\n";
    outputCatmull(filename, fileCat);
    cout<<"\n< Catm Created >\n";
    getchar();
   }while(true);
    //return 1;

    //
    //cout<<"outside";
    //exit(0);

    //cout<<"returned\n";
    //Catmull cat;
    //for(int i=0;i<m.GetNumberVertices();i++)
   // {
    //    TopoVert v = m.GetVertex(i);
       // cout<<v.GetNumberIncFacets()<<" ";
    //   for(int j=0;j<v.GetNumberIncFacets();j++)
     //  {
     //      TopoFacet ff = m.GetFacet(j);
     //      if(i==82)
     //       cout<<

     //  }

    //}
    //getchar();
    //BezierPatch(m,10,21,2,3);
    return 0;

}

#include "MeshGeneration.h"
#include "meshGeneration.h"
#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Loop.h"


Mesh Loop::LoopMeshGen(Mesh m)
{
  int nVertex = m.GetNumberVertices();
  int nFace = m.GetNumberFacets();
  int nEdges = m.GetNumberEdges();
  Mesh outmesh;
  for(int facec = 0; facec<nFace; facec++)
  {
      TopoFacet f = m.GetFacet(facec);
      if(f.GetNumberVertices()==3)
      {
        int indv0 = f.GetVertexInd(0);     int indv1 = f.GetVertexInd(1);     int indv2 = f.GetVertexInd(2);
        GeomVert v0=m.GetGeomVertex(indv0);
        GeomVert v1=m.GetGeomVertex(indv1);
        GeomVert v2=m.GetGeomVertex(indv2);
        vector<GeomVert> geomfacet1;
        geomfacet1.push_back(v0);   geomfacet1.push_back(v1);   geomfacet1.push_back(v2);
        outmesh.AddFacet(geomfacet1);
      }
    else if(f.GetNumberVertices() == 4)
    {
    int indv0 = f.GetVertexInd(0);     int indv1 = f.GetVertexInd(1);
    int indv2 = f.GetVertexInd(2);     int indv3 = f.GetVertexInd(3);
    GeomVert v0=m.GetGeomVertex(indv0);
    GeomVert v1=m.GetGeomVertex(indv1);
    GeomVert v2=m.GetGeomVertex(indv2);
    GeomVert v3=m.GetGeomVertex(indv3);
    vector<GeomVert> geomfacet1;
    vector<GeomVert> geomfacet2;
    geomfacet1.push_back(v0);   geomfacet1.push_back(v1);   geomfacet1.push_back(v2);
    geomfacet2.push_back(v0);   geomfacet2.push_back(v2);   geomfacet2.push_back(v3);
    outmesh.AddFacet(geomfacet1);
    outmesh.AddFacet(geomfacet2);
    }
    else {cout<<"Unknown number of vertices from loop.cpp"; exit(0);}
  }//end of face loop

return outmesh;
}//end of loop mesh generator

Mesh Loop::createVertex(Mesh m, float alpha)
{
  //Mesh mout;

  int nVertex = m.GetNumberVertices();
  int nFace = m.GetNumberFacets();
  int nEdges = m.GetNumberEdges();
    for(int edgeever = 0; edgeever<nEdges;edgeever++)
    {
        TopoEdge ed = m.GetEdge(edgeever);
        bool oneedge = false;
        if(ed.GetNumberIncFacets()>2  )
            {cout<<"Check here for error";
                //exit(0);
            }

        if(ed.getMapStatus() == true)
            continue;
        if(ed.GetNumberIncFacets()==1)
            oneedge = true;
        int face1 = ed.GetIncFacet(0);
        int face2;      if(oneedge == false)    face2 = ed.GetIncFacet(1);      //two neighbors of edge
        if(face1>=nFace)
        {   cout<<face1<<" out of bound "<<ed.GetNumberIncFacets(); exit(0);}

        TopoFacet f1 = m.GetFacet(face1);
        TopoFacet f2;    if(oneedge == false) f2 = m.GetFacet(face2);
        int face1p = f1.GetVertexInd(0), face1q = f1.GetVertexInd(1), face1r = f1.GetVertexInd(2);
        int face2p, face2q, face2r;
        if(oneedge == false) {face2p = f2.GetVertexInd(0); face2q = f2.GetVertexInd(1); face2r = f2.GetVertexInd(2);}
        int edger = ed.GetVertex(0), edges = ed.GetVertex(1);

        GeomVert vert1p = m.GetGeomVertex(face1p), vert1q = m.GetGeomVertex(face1q), vert1r = m.GetGeomVertex(face1r);
        GeomVert vert2p = m.GetGeomVertex(0), vert2q = m.GetGeomVertex(0), vert2r = m.GetGeomVertex(0); //dummy values
        if(oneedge == false) {vert2p = m.GetGeomVertex(face2p), vert2q = m.GetGeomVertex(face2q), vert2r = m.GetGeomVertex(face2r);}
        GeomVert vertr = m.GetGeomVertex(edger), verts = m.GetGeomVertex(edges);
        float mCo[3];
        if(oneedge == false)
        {
            mCo[0] = vert1p.GetCo(0)+vert1q.GetCo(0)+vert1r.GetCo(0)+vert2p.GetCo(0)+vert2q.GetCo(0)+vert2r.GetCo(0)+vertr.GetCo(0)+verts.GetCo(0);
            mCo[1] = vert1p.GetCo(1)+vert1q.GetCo(1)+vert1r.GetCo(1)+vert2p.GetCo(1)+vert2q.GetCo(1)+vert2r.GetCo(1)+vertr.GetCo(1)+verts.GetCo(1);
            mCo[2] = vert1p.GetCo(2)+vert1q.GetCo(2)+vert1r.GetCo(2)+vert2p.GetCo(2)+vert2q.GetCo(2)+vert2r.GetCo(2)+vertr.GetCo(2)+verts.GetCo(2);
            mCo[0] = mCo[0]/8;      mCo[1] = mCo[1]/8;      mCo[2] = mCo[2]/8;
        }
        else
        {
            mCo[0] = vert1p.GetCo(0)+vert1q.GetCo(0)+vert1r.GetCo(0)+vertr.GetCo(0)+verts.GetCo(0);
            mCo[1] = vert1p.GetCo(1)+vert1q.GetCo(1)+vert1r.GetCo(1)+vertr.GetCo(1)+verts.GetCo(1);
            mCo[2] = vert1p.GetCo(2)+vert1q.GetCo(2)+vert1r.GetCo(2)+vertr.GetCo(2)+verts.GetCo(2);
            mCo[0] = mCo[0]/5;      mCo[1] = mCo[1]/5;      mCo[2] = mCo[2]/5;
        }
                ed.setMap(mCo[0],mCo[1],mCo[2]);
                m.SetEdge(edgeever,ed);
    }//end of edge loop
cout<<"out of edge loop";
//getchar();
    for(int vertexever = 0; vertexever<nVertex; vertexever++)
    {
        TopoVert vert = m.GetVertex(vertexever);
        GeomVert vertv=m.GetGeomVertex(vertexever);
        float xsum=0,ysum=0,zsum=0;
        for(int incedEdge = 0;incedEdge<vert.GetNumberIncEdges();incedEdge++)
        {
            int edgeind = vert.GetIncEdge(incedEdge);
            TopoEdge ed = m.GetEdge(edgeind);
            float x1,y1,z1, x2,y2,z2;
            int vertind1 = ed.GetVertex(0);     int vertind2 = ed.GetVertex(1);
            GeomVert v1 = m.GetGeomVertex(vertind1);    GeomVert v2 = m.GetGeomVertex(vertind2);
            if(v1.GetCo(0)== vertv.GetCo(0) && v1.GetCo(1)== vertv.GetCo(1) && v1.GetCo(2)== vertv.GetCo(2))
                {xsum+= v2.GetCo(0);    ysum+= v2.GetCo(1);     zsum+= v2.GetCo(2);}
            else if(v2.GetCo(0)== vertv.GetCo(0) && v2.GetCo(1)== vertv.GetCo(1) && v2.GetCo(2)== vertv.GetCo(2))
                {xsum+= v1.GetCo(0);    ysum+= v1.GetCo(1);     zsum+= v1.GetCo(2);}
            else {cout<<"No match of vertices"; exit(0);}

        }
        xsum = xsum/vert.GetNumberIncEdges();
        ysum = ysum/vert.GetNumberIncEdges();
        zsum = zsum/vert.GetNumberIncEdges();
        xsum = (1-alpha)*xsum + alpha*vertv.GetCo(0);
        ysum = (1-alpha)*ysum + alpha*vertv.GetCo(1);
        zsum = (1-alpha)*zsum + alpha*vertv.GetCo(2);
        vert.setMap(xsum,ysum,zsum);
        m.SetVertex(vertexever,vert);
    }//end of vertex loop

    cout<<"out of vertex loop";
//getchar();
return m;
}//end of function

//1. for each face
//2. pick up all edges and connect edge points to form one net
//3. for each pair of edges, find common vertex and connect the three
Mesh Loop::createNet(Mesh m)
{
    Mesh outmesh;
    int nVertex = m.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();

    for(int facec = 0; facec<nFace; facec++)
    {
        TopoFacet f = m.GetFacet(facec);
        if(f.GetNumberEdges()!=3)
        {   cout<<"Invalid number of edges: "<<f.GetNumberEdges()<<" for face "<<facec;     exit(0);    }

        int edgeind1 = f.GetIncEdge(0), edgeind2 = f.GetIncEdge(1), edgeind3 = f.GetIncEdge(2);
        TopoEdge e1 = m.GetEdge(edgeind1), e2 = m.GetEdge(edgeind2), e3 = m.GetEdge(edgeind3);
        if(e1.getMapStatus()==false || e2.getMapStatus() == false || e3.getMapStatus() == false)
        {  cout<<"Not mapped"<< std::boolalpha <<e1.getMapStatus()<<" "<<e2.getMapStatus()<<" "<<e3.getMapStatus()<<"\n";    exit(0);        }

        float *mCo1 = e1.getMap(), *mCo2 = e2.getMap(), *mCo3 = e3.getMap();    //get edge vertices of all three edges
        vector<GeomVert> geomfacet;
        GeomVert v1(mCo1[0], mCo1[1], mCo1[2]);
        GeomVert v2(mCo2[0], mCo2[1], mCo2[2]);
        GeomVert v3(mCo3[0], mCo3[1], mCo3[2]);
        geomfacet.push_back(v1);    geomfacet.push_back(v2);    geomfacet.push_back(v3);
        outmesh.AddFacet(geomfacet);        //inner edge face added

        int v0e1 = e1.GetVertex(0), v1e1 = e1.GetVertex(1);
        int v0e2 = e2.GetVertex(0), v1e2 = e2.GetVertex(1);
        int v0e3 = e3.GetVertex(0), v1e3 = e3.GetVertex(1);

        TopoVert vmiddle;
        //  Find common face between e1 and e2

        if(v0e1 == v0e2 || v0e1 == v1e2)
            vmiddle = m.GetVertex(v0e1);
        else
            vmiddle = m.GetVertex(v1e1);
        float *mCoV = vmiddle.getMap();

        vector<GeomVert> geomfacet12;
        GeomVert v4(mCo1[0], mCo1[1], mCo1[2]);
        GeomVert v5(mCo2[0], mCo2[1], mCo2[2]);
        GeomVert v6(mCoV[0], mCoV[1], mCoV[2]);
        geomfacet12.push_back(v4);    geomfacet12.push_back(v5);    geomfacet12.push_back(v6);
        outmesh.AddFacet(geomfacet12);        //face between e1 and e2

        // Find common face between e1 and e3

        if(v0e1 == v0e3 || v0e1 == v1e3)
            vmiddle = m.GetVertex(v0e1);
        else
            vmiddle = m.GetVertex(v1e1);

        mCoV = vmiddle.getMap();
        vector<GeomVert> geomfacet13;
        GeomVert v7(mCo1[0], mCo1[1], mCo1[2]);
        GeomVert v8(mCo3[0], mCo3[1], mCo3[2]);
        GeomVert v9(mCoV[0], mCoV[1], mCoV[2]);
        geomfacet13.push_back(v7);    geomfacet13.push_back(v8);    geomfacet13.push_back(v9);
        outmesh.AddFacet(geomfacet13);        //face between e1 and e2

        // Find common face between e2 and e3

        if(v0e2 == v0e3 || v0e2 == v1e3)
            vmiddle = m.GetVertex(v0e2);
        else
            vmiddle = m.GetVertex(v1e2);

        mCoV = vmiddle.getMap();
        vector<GeomVert> geomfacet23;
        GeomVert v10(mCo2[0], mCo2[1], mCo2[2]);
        GeomVert v11(mCo3[0], mCo3[1], mCo3[2]);
        GeomVert v12(mCoV[0], mCoV[1], mCoV[2]);
        geomfacet23.push_back(v10);    geomfacet23.push_back(v11);    geomfacet23.push_back(v12);
        outmesh.AddFacet(geomfacet23);        //face between e1 and e2

    }//end of face loop
return outmesh;
}//end of function loop

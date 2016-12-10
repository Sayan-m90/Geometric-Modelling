#include "MeshGeneration.h"
#include "meshGeneration.h"

#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Catmull.h"

typedef		float	datatype;

Mesh Catmull::createVertex(Mesh m)
{
    Mesh mout;

    int nVertex = m.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();
    for(int facever=0;facever<nFace;facever++)
    {
        TopoFacet f = m.GetFacet(facever);
        if(f.getMapStatus()==true)
            {cout<<"already true."<<facever<<"\n"; continue;}
        int nVertex0 = f.GetNumberVertices();
        float facevertexx = 0,facevertexy = 0, facevertexz = 0;
        /////////////////////
        //Find the face vertex
        //////////////////////


        for(int i=0;i<nVertex0;i++)
        {
            int index = f.GetVertexInd(i);
            GeomVert vertex = m.GetGeomVertex(index);
            facevertexx += vertex.GetCo(0);
            facevertexy += vertex.GetCo(1);
            facevertexz += vertex.GetCo(2);
            //cout<<" Vertex "<<i<<": "<<vertex.GetCo(0)<<" "<<vertex.GetCo(1)<<" "<<vertex.GetCo(2)<<"\n";

        }
        //getchar();
        facevertexx /= nVertex0;
        facevertexy /= nVertex0;
        facevertexz /= nVertex0;
        f.setMap(facevertexx,facevertexy,facevertexz);
        //cout<<"here";
       //getchar();
        m.SetFacet(facever,f);

       // cout<<"came back";
        //getchar();
        //cout<<f.getMapStatus();

    }   //end of face loop
//getchar();

   // for(int facever=0;facever<nFace;facever++)
    //{
    //    TopoFacet f = m.GetFacet(facever);
    //    cout<<f.getMapStatus();
   // }

cout<<"\nout of face";

    for(int edgeever = 0; edgeever<nEdges;edgeever++)
    {
        TopoEdge ed = m.GetEdge(edgeever);
        bool oneedge = false;
        if(ed.GetNumberIncFacets()>2  )
            {cout<<"Check here for error";
                continue;
            }

        if(ed.getMapStatus() == true)
            continue;
        if(ed.GetNumberIncFacets()==1)
            oneedge = true;
        //cout<<"ed: "<<edgeever<<" ";
        //getchar();
        int face1 = ed.GetIncFacet(0);
        int face2;
        if(oneedge == false)    face2 = ed.GetIncFacet(1);
        if(face1>=nFace)
        {
            cout<<face1<<" out of bound in catmull.cpp "<<ed.GetNumberIncFacets();
            exit(0);
            //getchar();
        }
        TopoFacet f1 = m.GetFacet(face1);

        TopoFacet f2;
        if(oneedge == false) f2 = m.GetFacet(face2);


        //cout<<f1.getMapStatus()<<f2.getMapStatus();
        //getchar();
        if(f1.getMapStatus()==false )
        {
            cout<<"Face not mapped. face1: "<<face1<<" is:"<<f1.getMapStatus();
            exit(0);

        }

  //      cout<<"no ghapla";
  //      getchar();
        float *mCo1 = f1.getMap(), *mCo2;
        if(oneedge ==false) mCo2 = f2.getMap();

        int vind1 = ed.GetVertex(0);    int vind2 = ed.GetVertex(1);
        GeomVert vedge1 = m.GetGeomVertex(vind1);
        GeomVert vedge2 = m.GetGeomVertex(vind2);
        float mCo3[3],mCo4[3];
        mCo3[0] = vedge1.GetCo(0);  mCo3[1] = vedge1.GetCo(1);  mCo3[2] = vedge1.GetCo(2);
        mCo4[0] = vedge2.GetCo(0);  mCo4[1] = vedge2.GetCo(1);  mCo4[2] = vedge2.GetCo(2);

        ////////////Now all four vertices to form edge vertex are in mCo1, mCo2, mCo3, mCo4
        float xedge,yedge, zedge;
        if(oneedge ==false)
        {
             xedge = (mCo1[0]+mCo2[0]+mCo3[0]+mCo4[0])/4;
             yedge = (mCo1[1]+mCo2[1]+mCo3[1]+mCo4[1])/4;
             zedge = (mCo1[2]+mCo2[2]+mCo3[2]+mCo4[2])/4;
        }
        else
        {
             xedge = (mCo3[0]+mCo4[0])/2;       //mCo1[0]+
             yedge = (mCo3[1]+mCo4[1])/2;       //mCo1[1]+
             zedge = (mCo3[2]+mCo4[2])/2;       //mCo1[2]+

        }
        ed.setMap(xedge,yedge,zedge);
        m.SetEdge(edgeever,ed);
    }//end of edge loop
   // for(int edgever=0;edgever<nEdges;edgever++)
   // {
   //     TopoEdge ed = m.GetEdge(edgever);
   //     cout<<ed.getMapStatus();
   // }

cout<<"out of edge";

        for(int vertexever = 0; vertexever<nVertex; vertexever++)
        {
            TopoVert vet = m.GetVertex(vertexever);
            GeomVert gvet= m.GetGeomVertex(vertexever);

            if(vet.getMapStatus()==true)
                continue;
            int noFaces = vet.GetNumberIncFacets();
            int noEdges = vet.GetNumberIncEdges();


            float Qx=0.0, Qy=0.0, Qz=0.0;
            float Rx=0.0, Ry=0.0, Rz=0.0;

            for(int ij=0; ij<noFaces; ij++)
            {
                int facetindex = vet.GetIncFacet(ij);
                if(facetindex>nFace)
                {
                    cout<<"problem";
                    exit(0);


                }

                TopoFacet ff = m.GetFacet(facetindex);

                if(ff.getMapStatus() == false)
                {
                    cout<<"Face of vertex not mapped";
                    exit(0);
                }
                float *mcp = ff.getMap();
                float mCo[3];   mCo[0] = mcp[0];    mCo[1] = mcp[1];    mCo[2] = mcp[2];

                Qx += mCo[0];   Qy += mCo[1];   Qz += mCo[2];

          //  if(vertexever == 82){cout<<"ended"; getchar();}
            }//end of face loop
            Qx /= noFaces;      Qy /= noFaces;      Qz /= noFaces;
            //cout<<"Qx"<<Qx<<"Qy"<<Qy<<"Qz"<<Qz<<"\n";
            for(int ij=0; ij<noEdges; ij++)
            {
                int edgeindex = vet.GetIncEdge(ij);
                TopoEdge ee = m.GetEdge(edgeindex);

                if(ee.getMapStatus() == false)
                {
                    cout<<"Edge of vertex not mapped"<<edgeindex<<" "<<ee.GetNumberIncFacets();
                   // exit(0);
                }

                float *mCo = ee.getMap();
                Rx += mCo[0];   Ry += mCo[1];   Rz += mCo[2];


            }//face of vertex
            Rx /= noEdges;      Ry /= noEdges;      Rz /= noEdges;
            //cout<<"Rx: "<<Rx<<"Ry: "<<Ry<<" Rz: "<<Rz<<"\n";
            float vx = gvet.GetCo(0);       float vy = gvet.GetCo(1);       float vz = gvet.GetCo(2);
            float vfx = (Qx+2*Rx+((noEdges-3)*vx))/noEdges;
            float vfy = (Qy+2*Ry+((noEdges-3)*vy))/noEdges;
            float vfz = (Qz+2*Rz+((noEdges-3)*vz))/noEdges;
            vet.setMap(vfx,vfy,vfz);
            m.SetVertex(vertexever,vet);
            //cout<<"vertex:"<<vfx<<" "<<vfy<<" "<<vfz;
            //getchar();
            //cout<<"vertex:"<<vfx<<" "<<vfy<<" "<<vfz;
            //cout<<vertexever<<" "<<vet.GetNumberIncEdges()<<" "<<vet.GetNumberIncFacets()<<" "<<vet.getMapStatus();
            //getchar();
        }//end of vertex loop
    return m;

}

Mesh Catmull::createNet2(Mesh m)
{
    Mesh outnet;
    int nFaces = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();
    int nVertices = m.GetNumberVertices();

    for(int i=0;i<nFaces;i++)
    {
        TopoFacet t = m.GetFacet(i);
        int noVerticesF = t.GetNumberVertices();
        int noEdgeF = t.GetNumberEdges();      //no of edges in the face
        float* mCoface = t.getMap();
        GeomVert vface(mCoface[0],mCoface[1],mCoface[2]);

        for(int j=0;j<noVerticesF;j++)      //find common edges between vertex and face
        {
            int vindex = t.GetVertexInd(j);
            TopoVert v = m.GetVertex(vindex);   //each vertex which is neighbor of face t
            int noEdgeVertex = v.GetNumberIncEdges();   //no of edges in this vertex
            vector<GeomVert> geomFace;
            int countcommon =0;
            float* mCovertex = v.getMap();
            GeomVert vvertex(mCovertex[0],mCovertex[1],mCovertex[2]);

            geomFace.push_back(vface);   //insert face
            //cout<<"\nface: "<<mCoface[0]<<" "<<mCoface[1]<<" "<<mCoface[2]<<" ";
            for(int l=0;l<noEdgeVertex;l++)
            {
                int indexVertex = v.GetIncEdge(l);      //each edge of the vertex

            for(int k=0;k<noEdgeF;k++)
            {
                int indexFace = t.GetIncEdge(k);     //each edge of the face
                if(indexFace == indexVertex)        //common edge index
                    {

                        TopoEdge ed = m.GetEdge(indexFace);
                        float* mCo2 = ed.getMap();
                        countcommon++;
                        GeomVert vpush(mCo2[0],mCo2[1],mCo2[2]);
                        geomFace.push_back(vpush);        //insert edge 1
                       // cout<<" edge: "<<mCo2[0]<<" "<<mCo2[1]<<" "<<mCo2[2];
                        if(countcommon == 1)
                            {geomFace.push_back(vvertex);}//   cout<<" vertex: "<<vvertex.GetCo(0)<<" "<<vvertex.GetCo(1)<<" "<<vvertex.GetCo(2);}

                    }
            }//check if edge is common between edge and vertex. k loop. each edge of face

            }//l loop.. each edge of vertex
            //cout<<countcommon<<" ";
            outnet.AddFacet(geomFace);
        }//vertex within face loop


    }//face loop


    return outnet;

}//end of createNet2
Mesh Catmull::createNet(Mesh m)
{
    Mesh outnet;
    int nFaces = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();
    int nVertices = m.GetNumberVertices();

    for(int i=0;i<nFaces;i++)
    {
        TopoFacet t = m.GetFacet(i);

        float* mCo = t.getMap();
        GeomVert vert1(mCo[0],mCo[1],mCo[2]);

        for(int j=0;j<t.GetNumberEdges();j++)
        {
            vector<GeomVert> geomEdge;
            int index = t.GetIncEdge(j);
            TopoEdge ed = m.GetEdge(index);
            float* mCo2 = ed.getMap();
            GeomVert vert2(mCo2[0],mCo2[1],mCo2[2]);
            geomEdge.push_back(vert1);
            geomEdge.push_back(vert2);
            outnet.AddFacet(geomEdge);                   //face edge added
        }
    }   //end of face loop

    for(int i=0;i<nVertices;i++)
    {
        TopoVert v = m.GetVertex(i);

        float* mCo = v.getMap();
        GeomVert vert1(mCo[0],mCo[1],mCo[2]);

        for(int j=0;j<v.GetNumberIncEdges();j++)
        {
            vector<GeomVert> geomEdge;
            int index = v.GetIncEdge(j);
            TopoEdge ed = m.GetEdge(index);
            float* mCo2 = ed.getMap();
            GeomVert vert2(mCo2[0],mCo[1],mCo[2]);
            geomEdge.push_back(vert1);
            geomEdge.push_back(vert2);
            outnet.AddFacet(geomEdge);
        }   //vertex edge added

    }//end of vertex loop

    return outnet;
}//end of function

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

using namespace std;
void getword2(string line, char* x, char* y, char* z, char* a)
{
    int length11 = line.length();
    int i=0,j=0;
    while(line[i]!=' ')
    {
        x[j]=line[i];

        j++;
        i++;


    }
    x[j] = '\0';
      i++;
    //cout<<" ";
    if(x=="4")
        {
            cout<<"Beware not four!";
            return;
        }
    j=0;
    while(line[i]!=' ')
    {

        y[j] = line[i];
        j++;
        i++;

    }

    y[j] = '\0';

    i++;
    j=0;

    while(line[i]!=' ')
    {
        z[j] = line[i];
        j++;
        i++;

    }
    z[j] = '\0';
    i++;
    j=0;

    while(i <= length11)
    {
        a[j] = line[i];
        j++;
        i++;

    }
    a[j] = '\0';
}


void getword(string line, char* x, char* y, char* z)
{
    int length11 = line.length();
    int i=0,j=0;
    while(line[i]!=' ')
    {
        x[j]=line[i];

        j++;
        i++;


    }
    x[j] = '\0';

    i++;

    if(x=="4")
        return;
    j=0;
    while(line[i]!=' ')
    {

        y[j] = line[i];
        j++;
        i++;

    }

    y[j] = '\0';

    i++;
    j=0;
    while(i <= length11)
    {
        z[j] = line[i];
        j++;
        i++;

    }
    z[j] = '\0';

}

Mesh getfile(string filename)
{
    ifstream input;
    Mesh m;
    int count1 = 0;
    string line , off = "OFF", four = "4";
    vector<GeomVert> coord;
    char *facecoord1, *facecoord2, *facecoord3, *facecoord4;

    char *xs,*ys,*zs;
    input.open(filename);

    getline(input,line);

    if( line != off)
        exit(0);
    xs  = (char*)malloc(10*sizeof(char));
    ys  = (char*)malloc(10*sizeof(char));
    zs  = (char*)malloc(10*sizeof(char));
    getline(input,line);      //no of edge vertex face
    facecoord1  = (char*)malloc(10*sizeof(char));
    facecoord2  = (char*)malloc(10*sizeof(char));
    facecoord3  = (char*)malloc(10*sizeof(char));
    facecoord4  = (char*)malloc(10*sizeof(char));
    string line1 ;
    while( getline(input,line) )
    {
        count1++;
        line1 = line;
        getword(line, xs, ys, zs);
        //cout<<xs<<" "<<ys<<" "<<zs<<"\n";
        //x.push
        if(strcmp(xs,"4")==0 || strcmp(xs,"3")==0)
                break;
        GeomVert v(stof(xs),stof(ys),stof(zs));
        coord.push_back(v);
    }

//cout<<"coord: "<<coord.size();
    //for(int i =0;i<coord.size();i++)
    //    cout<<coord[i].GetCo(0)<<" "<<coord[i].GetCo(1)<<" "<<coord[i].GetCo(2)<<"\n";
    line = line1;
    do
    {

    string newl = line.substr(2,line.length());
//    cout<<newl;
    getword2(newl,facecoord1,facecoord2,facecoord3,facecoord4);
        //cout<<stoi(facecoord1)<<" "<<stoi(facecoord2)<<" "<<stoi(facecoord3)<<" "<<stoi(facecoord4)<<"\n";
        //x.push
        //if(strcmp(xs,"4")==0 || strcmp(xs,"3")==0)
        //        break;
    //GeomVert v(stof(xs),stof(ys),stof(zs));
    vector<GeomVert> geomfacet;
    geomfacet.push_back(coord[stoi(facecoord1)]);
    geomfacet.push_back(coord[stoi(facecoord2)]);
    geomfacet.push_back(coord[stoi(facecoord3)]);
    geomfacet.push_back(coord[stoi(facecoord4)]);
    m.AddFacet(geomfacet);

    }while( getline(input,line) );
   // outputOFF_mesh(m);
    return m;
}

Mesh DooSabinFace(Mesh m)
{
    Mesh mout;
    int nVertex = m.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();

    //int a,b,c,d;
    for(int iface=0;iface<nFace;iface++)
    {
        vector<GeomVert> geomfacet;
        TopoFacet f = m.GetFacet(iface);

        int nVertex = f.GetNumberVertices();
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
        centroidx /= nVertex;
        centroidy /= nVertex;
        centroidz /= nVertex;
        //cout<<"Centroid: "<<centroidx<<" "<<centroidy<<" "<<centroidz<<"\n";
        float x1,y1,z1;
        float x2,y2,z2;
        for(int i=0;i<nVertex;i++)
        {
            int indext1 = f.GetIncEdge(i);
            TopoEdge t1 = m.GetEdge(indext1);
            int indext2;
            TopoEdge t2;
            if((i+1) == (nVertex))
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

            int idvert1 = t1.GetVertex(0);
            int idvert2 = t1.GetVertex(1);
            GeomVert vertexact1 = m.GetGeomVertex(idvert1);
            GeomVert vertexact2 = m.GetGeomVertex(idvert2);
          //  cout<<"vertexact1 "<<vertexact1.GetCo(0)<<" "<<vertexact1.GetCo(1)<<" "<<vertexact1.GetCo(2);
          //  cout<<"\n vertexact2 "<<vertexact2.GetCo(0)<<" "<<vertexact2.GetCo(1)<<" "<<vertexact2.GetCo(2);
            //GeomVert vertex1 = vertexact1 + vertexact2;
            x1 = (vertexact1.GetCo(0) + vertexact2.GetCo(0))/2;
            y1 = (vertexact1.GetCo(1) + vertexact2.GetCo(1))/2;
            z1 = (vertexact1.GetCo(2) + vertexact2.GetCo(2))/2;

            idvert1 = t2.GetVertex(0);
            idvert2 = t2.GetVertex(1);
            vertexact1 = m.GetGeomVertex(idvert1);
            vertexact2 = m.GetGeomVertex(idvert2);
            //cout<<"\n vertexact3 "<<vertexact1.GetCo(0)<<" "<<vertexact1.GetCo(1)<<" "<<vertexact1.GetCo(2);
            //cout<<"\n vertexact4 "<<vertexact2.GetCo(0)<<" "<<vertexact2.GetCo(1)<<" "<<vertexact2.GetCo(2)<<"\n";
            x2 = (vertexact1.GetCo(0) + vertexact2.GetCo(0))/2;
            y2 = (vertexact1.GetCo(1) + vertexact2.GetCo(1))/2;
            z2 = (vertexact1.GetCo(2) + vertexact2.GetCo(2))/2;

            if(t2.GetVertex(0)==t1.GetVertex(1))
                idvert1 = t2.GetVertex(0);
            else if(t2.GetVertex(1)==t1.GetVertex(0))
                idvert1 = t2.GetVertex(1);
            else if(t2.GetVertex(0)==t1.GetVertex(0))
                idvert1 = t2.GetVertex(0);
            else if(t2.GetVertex(1)==t1.GetVertex(1))
                idvert1 = t2.GetVertex(1);
            else {cout<<"Not equal";    exit(0);}
            //idvert1 = t2.GetVertex(0);
            GeomVert v = m.GetGeomVertex(idvert1);
            float x = (centroidx + x1 + x2 + v.GetCo(0) )/4;
            float y = (centroidy + y1 + y2 + v.GetCo(1) )/4;
            float z = (centroidz + z1 + z2 + v.GetCo(2) )/4;
            GeomVert newvert(x,y,z);
            newvert.setMapp(idvert1);
           // cout<<"Gen vertex "<<i<<": "<<x<<" "<<y<<" "<<z<<"\n";

            geomfacet.push_back(newvert);
        }
        mout.AddFacet(geomfacet);

        //for(int vIndex=0;vIndex<nIncVertex;vIndex++)output<<f.GetVertexInd(vIndex)<<" ";
        //output<<"\n";
    }

    return mout;
}
void GenSubEdge(TopoFacet t1, Mesh m, float *v1dx, float *v1dy, float *v1dz, float *v2dx, float *v2dy, float *v2dz,int v1,int v2)
{
    int v3,v4;
            if(t1.GetNumberVertices()!=4)
            {
                cout<<"Face more than 4 vertices!\n\n";

            }
            if((t1.GetVertexInd(0) == v1 && t1.GetVertexInd(1) == v2)) //|| (t1.GetVertexInd(0) == v2 && t1.GetVertexInd(1) == v1))
                {
                    v3 = t1.GetVertexInd(2);
                    v4 = t1.GetVertexInd(3);

                }
                else if((t1.GetVertexInd(1) == v1 && t1.GetVertexInd(0) == v2)) //|| (t1.GetVertexInd(0) == v2 && t1.GetVertexInd(1) == v1))
                {
                    v4 = t1.GetVertexInd(2);
                    v3 = t1.GetVertexInd(3);

                }
            else if((t1.GetVertexInd(1) == v1 && t1.GetVertexInd(2) == v2))
                {
                    v3 = t1.GetVertexInd(3);
                    v4 = t1.GetVertexInd(0);

                }
                else if((t1.GetVertexInd(2) == v1 && t1.GetVertexInd(1) == v2))
                {
                    v3 = t1.GetVertexInd(0);
                    v4 = t1.GetVertexInd(3);

                }
            else if((t1.GetVertexInd(2) == v1 && t1.GetVertexInd(3) == v2))
                {
                    v3 = t1.GetVertexInd(0);
                    v4 = t1.GetVertexInd(1);
                 //   cout<<"t1: "<<t1.GetVertexInd(0)<<" "<<t1.GetVertexInd(1)<<" "<<t1.GetVertexInd(2)<<" "<<t1.GetVertexInd(3)<<" ";
                //    cout<<"v1: "<<v1<<" v2: "<<v2<<" v3: "<<v3<<" v4: "<<v4<<" \n";

                }
                else if((t1.GetVertexInd(3) == v1 && t1.GetVertexInd(2) == v2))
                {
                    v3 = t1.GetVertexInd(1);
                    v4 = t1.GetVertexInd(0);
                //    cout<<"t1: "<<t1.GetVertexInd(0)<<" "<<t1.GetVertexInd(1)<<" "<<t1.GetVertexInd(2)<<" "<<t1.GetVertexInd(3)<<" ";
                //    cout<<"v1: "<<v1<<" v2: "<<v2<<" v3: "<<v3<<" v4: "<<v4<<" \n";

                }
                else if((t1.GetVertexInd(3) == v1 && t1.GetVertexInd(0) == v2))
                {
                    v3 = t1.GetVertexInd(1);
                    v4 = t1.GetVertexInd(2);
              //      cout<<"t1: "<<t1.GetVertexInd(0)<<" "<<t1.GetVertexInd(1)<<" "<<t1.GetVertexInd(2)<<" "<<t1.GetVertexInd(3)<<" ";
              //      cout<<"v1: "<<v1<<" v2: "<<v2<<" v3: "<<v3<<" v4: "<<v4<<" \n";

                }
                else if((t1.GetVertexInd(0) == v1 && t1.GetVertexInd(3) == v2))
                {
                    v3 = t1.GetVertexInd(2);
                    v4 = t1.GetVertexInd(1);
              //      cout<<"t1: "<<t1.GetVertexInd(0)<<" "<<t1.GetVertexInd(1)<<" "<<t1.GetVertexInd(2)<<" "<<t1.GetVertexInd(3)<<" ";
              //     cout<<"v1: "<<v1<<" v2: "<<v2<<" v3: "<<v3<<" v4: "<<v4<<" \n";

                }
                else{
                    cout<<"t1: "<<t1.GetVertexInd(0)<<" "<<t1.GetVertexInd(1)<<" "<<t1.GetVertexInd(2)<<" "<<t1.GetVertexInd(3)<<" ";
                    cout<<"v1: "<<v1<<" "<<"v2: "<<v2<<"\n";
                    exit(0);
                }
                GeomVert vv1 = m.GetGeomVertex(v1);
                GeomVert vv2 = m.GetGeomVertex(v2);
                GeomVert vv3 = m.GetGeomVertex(v3);
                GeomVert vv4 = m.GetGeomVertex(v4);
              //  cout<<"vv1 "<<vv1.GetCo(0)<<" "<<vv1.GetCo(1)<<" "<<vv1.GetCo(2)<<" \n";
              //  cout<<"vv2 "<<vv2.GetCo(0)<<" "<<vv2.GetCo(1)<<" "<<vv2.GetCo(2)<<" \n";
              //  cout<<"vv3 "<<vv3.GetCo(0)<<" "<<vv3.GetCo(1)<<" "<<vv3.GetCo(2)<<" \n";
              //  cout<<"vv4 "<<vv4.GetCo(0)<<" "<<vv4.GetCo(1)<<" "<<vv4.GetCo(2)<<" \n";
                *v1dx = (9*vv1.GetCo(0)+3*vv2.GetCo(0)+3*vv4.GetCo(0)+1*vv3.GetCo(0))/16;
                *v1dy = (9*vv1.GetCo(1)+3*vv2.GetCo(1)+3*vv4.GetCo(1)+1*vv3.GetCo(1))/16;
                *v1dz = (9*vv1.GetCo(2)+3*vv2.GetCo(2)+3*vv4.GetCo(2)+1*vv3.GetCo(2))/16;

                *v2dx = (9*vv2.GetCo(0)+3*vv3.GetCo(0)+3*vv1.GetCo(0)+1*vv4.GetCo(0))/16;
                *v2dy = (9*vv2.GetCo(1)+3*vv3.GetCo(1)+3*vv1.GetCo(1)+1*vv4.GetCo(1))/16;
                *v2dz = (9*vv2.GetCo(2)+3*vv3.GetCo(2)+3*vv1.GetCo(2)+1*vv4.GetCo(2))/16;


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
        else if(e.GetNumberIncFacets()==4)
        {
            //cout<<"Edge: "<<e.GetNumberIncFacets()<<" Unknown !! ";
            int f1 =  e.GetIncFacet(0);
            int f2 =  e.GetIncFacet(1);
            int f3 =  e.GetIncFacet(2);
            int f4 =  e.GetIncFacet(3);
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
            //cout<<"Edge vertices: "<<vdiff1.GetCo(0)<<" "<<vdiff1.GetCo(1)<<" "<<vdiff1.GetCo(2);
            //cout<<"Edge vertices: "<<vdiff2.GetCo(0)<<" "<<vdiff2.GetCo(1)<<" "<<vdiff2.GetCo(2)<<"\nfacets1:";
            int v3,v4;
            int f1 =  e.GetIncFacet(0);     //face ID 1
            int f2 =  e.GetIncFacet(1);     //face ID 2
            TopoFacet t1 = m.GetFacet(f1);
            TopoFacet t2 = m.GetFacet(f2);


            int vv = t1.GetNumberVertices();
            for(int hj=0;hj<vv;hj++)
            {
                int jh1 = t1.GetVertexInd(hj);
                GeomVert vgh = m.GetGeomVertex(jh1);
                //cout<<vgh.GetCo(0)<<" "<<vgh.GetCo(1)<<" "<<vgh.GetCo(2)<<"\n";
            }
            //cout<<"facets2\n";
            vv = t2.GetNumberVertices();
            for(int hj=0;hj<vv;hj++)
            {
                int jh1 = t2.GetVertexInd(hj);
                GeomVert vgh = m.GetGeomVertex(jh1);
              //  cout<<vgh.GetCo(0)<<" "<<vgh.GetCo(1)<<" "<<vgh.GetCo(2)<<"\n";
            }
            //getchar();

            float v1dx,v1dy,v1dz, v2dx,v2dy,v2dz, v3dx,v3dy,v3dz, v4dx,v4dy,v4dz;
            GenSubEdge(t1,m,&v1dx,&v1dy,&v1dz,&v2dx,&v2dy,&v2dz,v1,v2);
           // cout<<"v1dx: "<<v1dx<<" v1dy: "<<v1dy<<" v1dz: "<<v1dz<<" \nv2dx: "<<v2dx<<" v2dy: "<<v2dy<<" v2dz: "<<v2dz;

            //getchar();
           // cout<<"facets2\n";
            /*
            vv = t2.GetNumberVertices();
            for(int hj=0;hj<vv;hj++)
            {
                int jh1 = t2.GetVertexInd(hj);
                GeomVert vgh = m.GetGeomVertex(jh1);
                cout<<vgh.GetCo(0)<<" "<<vgh.GetCo(1)<<" "<<vgh.GetCo(2)<<"\n";
            }
            */
            GeomVert v1d(v1dx,v1dy,v1dz);
            GeomVert v2d(v2dx,v2dy,v2dz);
            GenSubEdge(t2,m,&v3dx,&v3dy,&v3dz,&v4dx,&v4dy,&v4dz,v1,v2);
            //cout<<"v3dx: "<<v3dx<<" v3dy: "<<v3dy<<" v3dz: "<<v3dz<<" \nv4dx: "<<v4dx<<" v4dy: "<<v4dy<<" v4dz: "<<v4dz<<"\n";
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
//                cout<<"xtest: "<<xtest<<" "<<ytest<<" "<<ztest<<"\n";
//                cout<<"v1dx: "<<v1dx<<" v1dy: "<<v1dy<<" v1dz: "<<v1dz<<" \nv2dx: "<<v2dx<<" v2dy: "<<v2dy<<" v2dz: "<<v2dz;
//                cout<<"\nv3dx: "<<v3dx<<" v3dy: "<<v3dy<<" v3dz: "<<v3dz<<" \nv4dx: "<<v4dx<<" v4dy: "<<v4dy<<" v4dz: "<<v4dz<<"\n";
//                cout<<fabs(xtest-v1dx)<<" "<<fabs(ytest-v1dy)<<" "<<fabs(ztest-v1dz)<<"\n";

                //minx = 100,miny = 100,minz = 100;
                if(minx1 >= fabs(xtest-v1dx) && miny1 >= fabs(ytest-v1dy) && minz1 >= fabs(ztest-v1dz))
                    { minx1 = fabs(xtest-v1dx);     miny1 = fabs(ytest-v1dy);          minz1 = fabs(ztest-v1dz); v1d = v;}

                if(minx2 >= fabs(xtest-v2dx) && miny2 >= fabs(ytest-v2dy) && minz2 >= fabs(ztest-v2dz))
                { minx2 = fabs(xtest-v2dx);     miny2 = fabs(ytest-v2dy);          minz2 = fabs(ztest-v2dz); v2d = v;}

                if(minx3 >= fabs(xtest-v3dx) && miny3 >= fabs(ytest-v3dy) && minz3 >= fabs(ztest-v3dz))
                    {minx3 = fabs(xtest-v3dx);     miny3 = fabs(ytest-v3dy);          minz3 = fabs(ztest-v3dz); v3d = v;}

                if(minx4 >= fabs(xtest-v4dx) && miny4 >= fabs(ytest-v4dy) && minz4 >= fabs(ztest-v4dz))
                    {minx4 = fabs(xtest-v4dx);     miny4 = fabs(ytest-v4dy);          minz4 = fabs(ztest-v4dz);    v4d = v;}
//                    cout<<"minx1: "<<minx1<<" "<<miny1<<" "<<minz1<<"\n";
//                    cout<<"minx2: "<<minx2<<" "<<miny2<<" "<<minz2<<"\n";
//                    cout<<"minx3: "<<minx3<<" "<<miny3<<" "<<minz3<<"\n";
//                    cout<<"minx4: "<<minx4<<" "<<miny4<<" "<<minz4<<"\n";
//                    getchar();


            }
            //cout<<minx1<<" "<<miny1<<" "<<minz1<<"    \n";//<<(xtest-v1dx)<<" "<<(ytest-v1dy)<<" "<<(ztest-v1dz);

//            cout<<"Pushed vertex: "<<v1d.GetCo(0)<<" "<<v1d.GetCo(1)<<" "<<v1d.GetCo(2)<<"\n";
//            cout<<"Pushed vertex: "<<v2d.GetCo(0)<<" "<<v2d.GetCo(1)<<" "<<v2d.GetCo(2)<<"\n";
//            cout<<"Pushed vertex: "<<v3d.GetCo(0)<<" "<<v3d.GetCo(1)<<" "<<v3d.GetCo(2)<<"\n";
//            cout<<"Pushed vertex: "<<v4d.GetCo(0)<<" "<<v4d.GetCo(1)<<" "<<v4d.GetCo(2)<<"\n";
           // getchar();
            geomfacet.push_back(v1d);
            geomfacet.push_back(v2d);
            geomfacet.push_back(v4d);
            geomfacet.push_back(v3d);

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
    Mesh mDooFEV = mDooFE;
    for(int iVertex = 0;iVertex<nVertex;iVertex++)
    {
        GeomVert g = mDooFE.GetGeomVertex(iVertex);
        if(g.getMapp()== -1)
            continue;
        vector<GeomVert> geomfacet;
        geomfacet.push_back(g);
        //cout<<iVertex<<" mapped to: "<<g.getMapp()<<" ";
//        getchar();
        for(int innerVertex = 0;innerVertex<nVertex;innerVertex++)
        {
            GeomVert g1 = mDooFE.GetGeomVertex(innerVertex);
            if(g1.getMapp()==g.getMapp())
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
        if(geomfacet.size()>=4)
        mDooFEV.AddFacet(geomfacet);
    }//end of for loop iVertex for all vertices

return mDooFEV;

}//end of DooSabinVertex

void deCasteljauCurve3D(vector<vector<float>> ctlPt,float u,vector<float> &outPt){

    int n=ctlPt.size()-1;



    vector<vector<float>> p=ctlPt;



    for(int i=1;i<=n;i++){

        for(int k=0;k<=n-i;k++){

            for(int coor=0;coor<3;coor++)p[k][coor]=p[k][coor]*(1-u)+u*p[k+1][coor];

        }

    }



    outPt=p[0];

}


void deCasteljau(Mesh mesh, vector<vector<int>> Pij,float u,float w,vector<float>& outPt){



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

Mesh BezierPatch(Mesh mes)
{
//getchar();
Mesh outmesh;
int nVertices = mes.GetNumberVertices();

float px1 =0 ,py1 =0 ,pz1 =0 ,px2 =0,py2 =0,pz2 =0,px3 =0,py3 =0,pz3 =0,px4 =0,py4 =0,pz4 =0;
ifstream file("C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusionpoints.off");
char str[10];
getchar();
file>>str;  int n = atoi(str);
file>>str;  int m = atoi(str);
int D = m;
int R = n;
cout<<" m: "<<m<<" n: "<<n;

float *pinx[m], *piny[m], *pinz[m];
vector<vector<int>> coord;
bool picked[nVertices];
for(int i=0;i<nVertices;i++)
        picked[i]= false;
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

       bool match = false;
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
                    {xyz.push_back(vertsearch); match = true; picked[vertsearch]==true; break; }
        }
        if(match == false)
            {cout<<"not matched";
            cout<<v.GetCo(0)<<" "<<pinx[i][j]<<" "<<v.GetCo(0)-pinx[i][j]<<" ";
            cout<<v.GetCo(1)<<" "<<piny[i][j]<<" "<<v.GetCo(1)-piny[i][j]<<" ";
            cout<<v.GetCo(2)<<" "<<pinz[i][j]<<" "<<v.GetCo(2)-pinz[i][j]<<" ";
            exit(0); }


    }//end of j loop
    cout<<"\nsize xyz "<<xyz.size();
    coord.push_back(xyz);


}//end of i loop
cout<<"\ncoord size:"<<coord.size();
//cout<<"|||||||||||||||||||||";
//for(int i=0;i<nVertices;i++)
//    {if(picked[i] = true) cout<<"true ";
//        else {cout<<"false "; exit(0);}
//    }
//cout<<"|||||||||||||||||||||";
cout<<"came back";
getchar();
Mesh outMesh;
BezierMesh(mes, outMesh,10,10,coord);
return outMesh;

    for(float u=0;u<=0.8;u+=0.02)
    {
        for(float w=0;w<=0.8;w+=0.02)
        {
            vector<GeomVert> vert;
            for(int i=0;i<m;i++)
            {
                for(int j=0;j<n;j++)
                {
                    //GeomVert v = mes.GetGeomVertex(i+j);
                    //if(i>=10 && j==20)
                    //{cout<<"her1 "<<pinx[i][j];    getchar();}
                    px1 += pinx[i][j]*NiD(D,i,u)*NiD(R,j,w);
                    py1 += piny[i][j]*NiD(D,i,u)*NiD(R,j,w);
                    pz1 += pinz[i][j]*NiD(D,i,u)*NiD(R,j,w);

                    //getchar();

                    px2 += pinx[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w);
                    py2 += piny[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w);
                    pz2 += pinz[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w);

                    //GeomVert v2(px2,py2,pz2);
                    //vert.push_back(v2);
    //                if(i>=10 )
    //                {cout<<"her3";    getchar();}
                    px3 += pinx[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w+0.02);
                    py3 += piny[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w+0.02);
                    pz3 += pinz[i][j]*NiD(D,i,u+0.02)*NiD(R,j,w+0.02);
              //      cout<<"     py3: "<<py3<<" "<<piny[i][j]<<" "<<NiD(D,i,u+0.2)<<" "<<NiD(R,j,w+0.2);
                    //GeomVert v3(px3,py3,pz3);
                    //vert.push_back(v3);
      //              if(i>=10 )
      //              {cout<<"her4";    getchar();}
                    px4 += pinx[i][j]*NiD(D,i,u)*NiD(R,j,w+0.02);
                    py4 += piny[i][j]*NiD(D,i,u)*NiD(R,j,w+0.02);
                    pz4 += pinz[i][j]*NiD(D,i,u)*NiD(R,j,w+0.02);
                    //GeomVert v4(px4,py4,pz4);
                    //vert.push_back(v4);
        //            cout<<i<<" "<<j<<" ";//;<<uin<<" "<<win<<" "<<px[uin][win]<<" "<<py[uin][win]<<" "<<pz[uin][win]<<" \n";
                    //getchar();
                }// j loop
            }//i loop
            //cout<<"\npx1: "<<px1<<" py1:"<<py1<<" pz1: "<<pz1<<" u: "<<u<<" w:"<<w<<"\n";//<<" i:"<<i<<" j:"<<j;
            //cout<<"px2: "<<px2<<" py2:"<<py2<<" pz2: "<<pz2<<" u: "<<u+0.2<<" w:"<<w<<"\n";
            //cout<<"px3: "<<px3<<" py3:"<<py3<<" pz3: "<<pz3<<" u: "<<u+0.2<<" w:"<<w+0.2<<"\n";
            //cout<<"px4: "<<px4<<" py4:"<<py4<<" pz4: "<<pz4<<" u: "<<u<<" w:"<<w+0.2<<"\n";
                    //cout<<" NiD(D,i,u) "<<NiD(D,i,u)<<" NiD(R,j,w) "<<NiD(R,j,w);
            //getchar();
        GeomVert v1((float)floor(px1*1000)/1000,(float)floor(py1*1000)/1000,(float)floor(pz1*1000)/1000);
        GeomVert v2((float)floor(px2*1000)/1000,(float)floor(py2*1000)/1000,(float)floor(pz2*1000)/1000);
        GeomVert v3((float)floor(px3*1000)/1000,(float)floor(py3*1000)/1000,(float)floor(pz3*1000)/1000);
        GeomVert v4((float)floor(px4*1000)/1000,(float)floor(py4*1000)/1000,(float)ceil(pz4*1000)/1000);
        vert.push_back(v1);
        vert.push_back(v2);
        vert.push_back(v3);
        vert.push_back(v4);
        outmesh.AddFacet(vert);
        px1 =0; py1 =0; pz1 =0;     px2 =0; py2 =0; pz2 =0;     px3 =0; py3 =0; pz3 =0;     px4 =0; py4 =0; pz4 =0;

        //cout<<"u: "<<u<<" w:"<<w;
        }//end of w loop

    }//end of u loop

    //ofstream output;
    //output.open("C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bez.off");
    //output<<"OFF"<<endl;
    //output<<2500<<" "<<0<<" "<<0<<endl;
//    for(uin=0;uin<50;uin++)
//    {
//        for(win=0;win<50;win++)
//        {
//            output<<px[uin][win]<<" "<<py[uin][win]<<" "<<pz[uin][win];
//        }
//        output<<"\n";
//    }
return outmesh;
}//end of function


int main()
{

    cout<<"Main program\n";
    string filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusion.off";

    Mesh m = getfile(filename);
    cout<<"inside";
    Mesh m1 = BezierPatch(m);

    outputOFF_mesh(m1, "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\bezier.off");
    //cout<<"outside";
    //exit(0);
    //Mesh mDooF  = DooSabinFace(m);

    //filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\output.off";
    //outputOFF_mesh(m,filename);
    //Mesh mDooFE =  DooSabinEdge(mDooF,m);
    //Mesh mDooFEV = DooSabinVertex(mDooFE,m);

    //filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\DooSabin.off";
    //outputOFF_mesh(mDooFEV,filename);
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
    //Mesh vmap = cat.createVertex(m);
    //Mesh vfinal = cat.createNet2(vmap);
    //filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\catmull.off";
    //outputOFF_mesh(vfinal,filename);
    //BezierPatch(m,10,21,2,3);
    return 0;

}

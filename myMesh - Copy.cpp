#include "MeshGeneration.h"
#include "meshGeneration.h"
#include "iostream"
#include <string>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

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

    while(i < length11)
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
    while(i < length11)
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

    facecoord1  = (char*)malloc(10*sizeof(char));
    facecoord2  = (char*)malloc(10*sizeof(char));
    facecoord3  = (char*)malloc(10*sizeof(char));
    facecoord4  = (char*)malloc(10*sizeof(char));

    while( getline(input,line) )
    {

        getword(line, xs, ys, zs);
        //cout<<xs<<" "<<ys<<" "<<zs<<"\n";
        //x.push
        if(strcmp(xs,"4")==0 || strcmp(xs,"3")==0)
                break;
        GeomVert v(stof(xs),stof(ys),stof(zs));
        coord.push_back(v);
    }


    //for(int i =0;i<coord.size();i++)
    //    cout<<coord[i].GetCo(0)<<" "<<coord[i].GetCo(1)<<" "<<coord[i].GetCo(2)<<"\n";

    while( getline(input,line) )
    {

    string newl = line.substr(2,line.length());
//    cout<<newl;
    getword2(newl,facecoord1,facecoord2,facecoord3,facecoord4);
        //cout<<xs<<" "<<ys<<" "<<zs<<"\n";
        //x.push
        //if(strcmp(xs,"4")==0 || strcmp(xs,"3")==0)
        //        break;
    GeomVert v(stof(xs),stof(ys),stof(zs));
    vector<GeomVert> geomfacet;
    geomfacet.push_back(coord[stof(facecoord1)]);
    geomfacet.push_back(coord[stof(facecoord2)]);
    geomfacet.push_back(coord[stof(facecoord3)]);
    geomfacet.push_back(coord[stof(facecoord4)]);
    m.AddFacet(geomfacet);

    }
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
        }

        centroidx /= nVertex;
        centroidy /= nVertex;
        centroidz /= nVertex;
        float x1,y1,z1;
        float x2,y2,z2;
        for(int i=0;i<nVertex;i++)
        {
            TopoEdge t1 = m.GetEdge(i);
            TopoEdge t2;
            if((i+1) == (nVertex-1))
                t2 = m.GetEdge(i+1);
            else
                t2 = m.GetEdge(0);

            int idvert1 = t1.GetVertex(0);
            int idvert2 = t1.GetVertex(1);
            GeomVert vertexact1 = m.GetGeomVertex(idvert1);
            GeomVert vertexact2 = m.GetGeomVertex(idvert2);
            //GeomVert vertex1 = vertexact1 + vertexact2;
            x1 = (vertexact1.GetCo(0) + vertexact2.GetCo(0))/2;
            y1 = (vertexact1.GetCo(1) + vertexact2.GetCo(1))/2;
            z1 = (vertexact1.GetCo(2) + vertexact2.GetCo(2))/2;

            idvert1 = t2.GetVertex(0);
            idvert2 = t2.GetVertex(1);
            vertexact1 = m.GetGeomVertex(idvert1);
            vertexact2 = m.GetGeomVertex(idvert2);

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
            int v3,v4;
            int f1 =  e.GetIncFacet(0);     //face ID 1
            int f2 =  e.GetIncFacet(1);     //face ID 2
            TopoFacet t1 = m.GetFacet(f1);
            TopoFacet t2 = m.GetFacet(f2);
            float v1dx,v1dy,v1dz, v2dx,v2dy,v2dz, v3dx,v3dy,v3dz, v4dx,v4dy,v4dz;
            GenSubEdge(t1,m,&v1dx,&v1dy,&v1dz,&v2dx,&v2dy,&v2dz,v1,v2);
            //cout<<"v1dx: "<<v1dx<<" v1dy: "<<v1dy<<" v1dz: "<<v1dz<<" v2dx: "<<v2dx<<" v2dy: "<<v2dy<<" v2dz: "<<v2dz;
            GeomVert v1d(v1dx,v1dy,v1dz);
            GeomVert v2d(v2dx,v2dy,v2dz);
            GenSubEdge(t2,m,&v3dx,&v3dy,&v3dz,&v4dx,&v4dy,&v4dz,v1,v2);
            //cout<<"v1dx: "<<v1dx<<" v1dy: "<<v1dy<<" v1dz: "<<v1dz<<" v2dx: "<<v2dx<<" v2dy: "<<v2dy<<" v2dz: "<<v2dz<<"\n";
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
                //minx = 100,miny = 100,minz = 100;
                if(minx1 > fabs(xtest-v1dx) && miny1 > fabs(ytest-v1dy) && minz1 > fabs(ztest-v1dz))
                    { minx1 = (xtest-v1dx);     miny1 = (ytest-v1dy);          minz1 = (ytest-v1dy); }//v1d = v;}

                //if(minx2 > fabs(xtest-v2dx) && miny2 > fabs(ytest-v2dy) && minz2 > fabs(ztest-v2dz))
                //{ minx2 = (xtest-v2dx);     miny2 = (ytest-v2dy);          minz2 = (ytest-v2dy); v2d = v;}

                //if(minx3 > fabs(xtest-v3dx) && miny3 > fabs(ytest-v3dy) && minz3 > fabs(ztest-v3dz))
                //    {minx3 = (xtest-v3dx);     miny3 = (ytest-v3dy);          minz3 = (ytest-v3dy); v3d = v;}

                //if(minx4 > fabs(xtest-v4dx) && miny4 > fabs(ytest-v4dy) && minz4 > fabs(ztest-v4dz))
                //    {minx4 = (xtest-v4dx);     miny4 = (ytest-v4dy);          minz4 = (ytest-v4dy);    v4d = v;}

            }
            //cout<<minx1<<" "<<miny1<<" "<<minz1<<"    \n";//<<(xtest-v1dx)<<" "<<(ytest-v1dy)<<" "<<(ztest-v1dz);
            //getchar();
            geomfacet.push_back(v1d);
            geomfacet.push_back(v2d);
            geomfacet.push_back(v3d);
            geomfacet.push_back(v4d);

            mDooF.AddFacet(geomfacet);
        }//else == 2

    }//for(int iEdge = 0;iEdge<nEdges;iEdge++)
return mDooF;
}



int main()
{

    cout<<"Main program\n";
    string filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\input\\extrusion.off";
    Mesh m = getfile(filename);
    Mesh mDooF  = DooSabinFace(m);

    filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\output.off";
    outputOFF_mesh(m,filename);
    Mesh mDooFE =  DooSabinEdge(mDooF,m);
    cout<<"returned";
    filename =  "C:\\Users\\mesay\\OneDrive\\Documents\\C++\\Mesh\\output\\output2.off";
    outputOFF_mesh(mDooFE,filename);

    return 0;

}

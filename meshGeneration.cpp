//
//  meshGeneration.cpp
//  CGLab1
//
//  Created by Jiayuan Wang on 2/16/16.
//  Copyright (c) 2016 Jiayuan Wang. All rights reserved.
//

#include "meshGeneration.h"
#include "iostream"
using namespace std;

vector<float> subVer(GeomVert v1,GeomVert v2){
    //return v1-v2
    vector<float> result;
    for(int i=0;i<3;i++){
        result.push_back(v1.GetCo(i)-v2.GetCo(i));
    }
    return result;
}

vector<float> crossProduct(vector<float> u,vector<float> v){
    //u x v
    vector<float> result;
    float co1=u[1]*v[2]-u[2]*v[1];
    float co2=u[2]*v[0]-u[0]*v[2];
    float co3=u[0]*v[1]-u[1]*v[0];
    result.push_back(co1);
    result.push_back(co2);
    result.push_back(co3);
    return result;
}

float dotProduct(vector<float> u,vector<float> v){
    return u[0]*v[0]+u[1]*v[1]+u[2]*v[2];

}

float vectorLength(vector<float> v){
    return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

bool pointInTriangle(vector<GeomVert> tri, vector<float> intersection){
    vector<float> u = subVer(tri[1], tri[0]);
    vector<float> v = subVer(tri[2], tri[0]);
    vector<float> w;
    for(int i=0;i<3;i++)w.push_back(intersection[i]-tri[0].GetCo(i));

    vector<float> vCrossW=crossProduct(v, w);
    vector<float> vCrossU=crossProduct(v, u);

    if(dotProduct(vCrossW, vCrossU)<0)return false;

    vector<float> uCrossW = crossProduct(u, w);
    vector<float> uCrossV = crossProduct(u, v);

    if(dotProduct(uCrossW, uCrossV)<0)return false;

    float denom = vectorLength(uCrossV);
    float r = vectorLength(vCrossW)/denom;
    float t = vectorLength(uCrossW)/denom;

    return (r+t<=1);
}

bool triangleEdgeInter(vector<GeomVert> tri1,GeomVert v1,GeomVert v2){
    //Intersection of line and plane...
    GeomVert p0=tri1[0];

    vector<float> v01=subVer(tri1[1],tri1[0]);
    vector<float> v02=subVer(tri1[2],tri1[0]);
    vector<float> n=crossProduct(v01,v02);

    vector<float> I=subVer(v1, v2);
    GeomVert I0=v1;

    //if two point at the same side return false
    vector<float> av1=subVer(v1, tri1[0]);
    vector<float> av2=subVer(v2, tri1[0]);

    float av1DotN=dotProduct(av1, n);
    float av2DotN=dotProduct(av2, n);

    if(av1DotN*av2DotN>=0)return false;

    //d=(p0-l0)*n/I*n
    float d;
    if(dotProduct(I,n)==0){
        if(dotProduct(subVer(p0,I0),n)==0)return false;//the line contained in the plane
        else return false; //the line is paralell to the plane
    }
    else{
        d=dotProduct(subVer(p0, I0),n)/dotProduct(I,n);
        //The intersection is given by dI+I0
        vector<float> intersection;
        for(int i=0;i<3;i++){
            intersection.push_back(d*I[i]+I0.GetCo(i));
        }
        //check whether intersection is in the triangle
        if(pointInTriangle(tri1,intersection))return true;
        else return false;

    }
    return false;
}

bool triangleInter(vector<GeomVert> tri1,vector<GeomVert> tri2){
    //return true;
    //check every edge from tri2 intersect with tri1
    if(triangleEdgeInter(tri1,tri2[0],tri2[1]))return true;
    if(triangleEdgeInter(tri1,tri2[1],tri2[2]))return true;
    if(triangleEdgeInter(tri1,tri2[2],tri2[0]))return true;

    return false;

}

void faceTriangle(Mesh m, int faceInd,vector<GeomVert>& tri1, vector<GeomVert>& tri2){
    TopoFacet f = m.GetFacet(faceInd);
    vector<int> verInd;
    for(int i=0;i<f.GetNumberVertices();i++)verInd.push_back(f.GetVertexInd(i));

    for(int i=0;i<3;i++){
        GeomVert vertex1 = m.GetGeomVertex(verInd[i]);
        GeomVert vertex2 = m.GetGeomVertex(verInd[(i+2)%4]);
        tri1.push_back(vertex1);
        tri2.push_back(vertex2);
    }
}

bool faceInter(Mesh m,int i,int j){
    //bool intersection=false;
    //2-2 trangle intersection
    GeomVert va = m.GetGeomVertex(i);
    vector<GeomVert> tria;
    vector<GeomVert> trib;
    vector<GeomVert> tri1;
    vector<GeomVert> tri2;

    faceTriangle(m,i,tria,trib);
    faceTriangle(m,j,tri1,tri2);

    if(triangleInter(tria,tri1))return true;
    if(triangleInter(tria,tri2))return true;
    if(triangleInter(trib,tri1))return true;
    if(triangleInter(trib,tri2))return true;

    return false;
}

bool adjacent(Mesh m, int i, int j){
    //share edge...
    TopoFacet fi = m.GetFacet(i);
    TopoFacet fj = m.GetFacet(j);

//    int nAdjEdgeFi=fi.GetNumberEdges();
//    int nAdjEdgeFj=fj.GetNumberEdges();
//
//    for(int index=0;index<nAdjEdgeFi;index++){
//        int edgei=fi.GetIncEdge(index);
//        for(int j_index=0;j_index<nAdjEdgeFj;j_index++){
//            if(edgei==fj.GetIncEdge(j_index))return true;
//        }
//    }

    int adjVi=fi.GetNumberVertices();
    int adjVj=fj.GetNumberVertices();

    for(int ind_i=0;ind_i<adjVi;ind_i++){
        int vertexI=fi.GetVertexInd(ind_i);
        for(int ind_j=0;ind_j<adjVj;ind_j++){
            if(vertexI==fj.GetVertexInd(ind_j))return true;
        }
    }



//    TopoFacet f = m.GetFacet(i);
//    int nAdjF=f.GetNumberFacets();
//    cout<<"nAdj: "<<nAdjF<<endl;
//    for(int index=0;index<nAdjF;index++){
//        if(f.GetIncFacet(index)==j)return true;
//    }
    return false;
}

bool selfIntersection(Mesh m){
    bool intersection = false;
    int nFace = m.GetNumberFacets();

    for(int i=0;i<nFace;i++){
        for(int j=i+1;j<nFace;j++){
            //check by face
            //intersection of face i and face j
            if(adjacent(m,i,j))intersection=false;
            else if(faceInter(m,i,j)){
                intersection=true;
                cout<<"i j inter: "<<i<<" "<<j<<endl;
            } //not adjacent

            if(intersection)break;
        }
        if(intersection)break;
    }

    return intersection;
}

void outputOFF_mesh(Mesh m, string filename)
{
    int nVertex = m.GetNumberVertices();
    int nFace = m.GetNumberFacets();
    int nEdges = m.GetNumberEdges();

    ofstream output;
    output.open(filename);
    output<<"OFF"<<endl;
    output<<nVertex<<" "<<nFace<<" "<<nEdges<<endl;

    for(int i=0;i<nVertex;i++){
        GeomVert vertex = m.GetGeomVertex(i);
        for(int coor=0;coor<3;coor++)output<<vertex.GetCo(coor)<<" ";  //cout<<vertex.GetCo(coor)<<" ";}
        output<<"\n"; //cout<<"\n";
        //getchar();

    }

    for(int i=0;i<nFace;i++){
        TopoFacet f = m.GetFacet(i);
        int nIncVertex = f.GetNumberVertices();
        output<<nIncVertex<<" ";
        for(int vIndex=0;vIndex<nIncVertex;vIndex++)output<<f.GetVertexInd(vIndex)<<" ";
        output<<"\n";
    }

}

void revolution(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap)
{
    //revolve along y axis

//    int n1;//#sample along curve
//    int n2;//#sample of curve


    //original curve z=0
    vector<vector<float> > curveSample;
    overlap=false;
    float sign=0;

    for(int i=0;i<=n1;i++){
    //sample i*1/n1
    //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
//        //bezierCurve(float(i)/n1, ctlPts, x, y);
        pt.push_back(x);
        if(sign*x<0)overlap=true;
        else sign=x;
        pt.push_back(y);
        curveSample.push_back(pt);
    }
    //revolution & insert faces
    for(int i=0;i<n2;i++){
        float theta1=i*2*PI/float(n2);//revolution angle
        float theta2=(i+1)*2*PI/float(n2);
        for(int j=0;j<n1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]*cos(theta1);
            float y1 = curveSample[j][1];
            float z1 = curveSample[j][0]*sin(theta1);
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]*cos(theta1);
            float y2 = curveSample[j+1][1];
            float z2 = curveSample[j+1][0]*sin(theta1);
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]*cos(theta2);
            float y3 = curveSample[j+1][1];
            float z3 = curveSample[j+1][0]*sin(theta2);
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]*cos(theta2);
            float y4 = curveSample[j][1];
            float z4 = curveSample[j][0]*sin(theta2);
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v4);
            geomfacet.push_back(v3);
            geomfacet.push_back(v2);

            m.AddFacet(geomfacet);
        }
    }

}

void revolutionCubic(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap)
{
    //revolve along y axis

    //    int n1;//#sample along curve
    //    int n2;//#sample of curve


    //    for(int k=0;k<=n-3;k++){
    //        //curve equation for Pk, Pk+1, Pk+2
    //
    //        for(int j=0;j<=NUM_SEGMENT_BSPLINE_EACH;j++){
    //            float u = float(j)/NUM_SEGMENT_BSPLINE_EACH;
    //            float x=0;
    //            float y=0;
    //            cubicP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],ctlPts[k+3],u,x,y);
    //            glVertex2d(x, y);
    //            //cout<<"x: "<<x<<" y: "<<y<<endl;
    //        }
    //    }

    //original curve z=0
    vector<vector<float> > curveSample;
    overlap=false;
    float sign=0;

    int n=ctlPts.size()-1;

    for(int k=0;k<=n-3;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
//            cubicP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],ctlPts[k+3],u,x,y);
            pt.push_back(x);
            if(sign*x<0)overlap=true;
            else sign=x;
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }
    //revolution & insert faces
    for(int i=0;i<n2;i++){
        float theta1=i*2*PI/float(n2);//revolution angle
        float theta2=(i+1)*2*PI/float(n2);
        for(int j=0;j<curveSample.size()-1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]*cos(theta1);
            float y1 = curveSample[j][1];
            float z1 = curveSample[j][0]*sin(theta1);
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]*cos(theta1);
            float y2 = curveSample[j+1][1];
            float z2 = curveSample[j+1][0]*sin(theta1);
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]*cos(theta2);
            float y3 = curveSample[j+1][1];
            float z3 = curveSample[j+1][0]*sin(theta2);
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]*cos(theta2);
            float y4 = curveSample[j][1];
            float z4 = curveSample[j][0]*sin(theta2);
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v4);
            geomfacet.push_back(v3);
            geomfacet.push_back(v2);

            m.AddFacet(geomfacet);
        }
    }

}

void revolutionQuadric(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap)
{
    //revolve along y axis

    //    int n1;//#sample along curve
    //    int n2;//#sample of curve

//    for(int k=0;k<=n-2;k++){
//        //curve equation for Pk, Pk+1, Pk+2
//
//        for(int j=0;j<=NUM_SEGMENT_BSPLINE_EACH;j++){
//            float u = float(j)/NUM_SEGMENT_BSPLINE_EACH;
//            float x=0;
//            float y=0;
//            //quadricP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],u,x,y);
//            glVertex2d(x, y);
//            cout<<"x: "<<x<<" y: "<<y<<endl;
//        }
//    }

    //original curve z=0
    vector<vector<float> > curveSample;
    overlap=false;
    float sign=0;

    int n=ctlPts.size()-1;

    for(int k=0;k<=n-2;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
            //quadricP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],u,x,y);
            pt.push_back(x);
            if(sign*x<0)overlap=true;
            else sign=x;
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }
    //revolution & insert faces
    for(int i=0;i<n2;i++){
        float theta1=i*2*PI/float(n2);//revolution angle
        float theta2=(i+1)*2*PI/float(n2);
        for(int j=0;j<curveSample.size()-1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]*cos(theta1);
            float y1 = curveSample[j][1];
            float z1 = curveSample[j][0]*sin(theta1);
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]*cos(theta1);
            float y2 = curveSample[j+1][1];
            float z2 = curveSample[j+1][0]*sin(theta1);
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]*cos(theta2);
            float y3 = curveSample[j+1][1];
            float z3 = curveSample[j+1][0]*sin(theta2);
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]*cos(theta2);
            float y4 = curveSample[j][1];
            float z4 = curveSample[j][0]*sin(theta2);
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v4);
            geomfacet.push_back(v3);
            geomfacet.push_back(v2);

            m.AddFacet(geomfacet);
        }
    }

}



void revolutionx(vector<vector<float> > ctlPts, int n1, int n2, Mesh& m, bool& overlap)
{
    //revolve along y axis

    //    int n1;//#sample along curve
    //    int n2;//#sample of curve


    //original curve z=0
    vector<vector<float> > curveSample;
    overlap=false;
    float sign=0;

    for(int i=0;i<=n1;i++){
        //sample i*1/n1
        //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
        //bezierCurve(float(i)/n1, ctlPts, x, y);
        pt.push_back(x);
        if(sign*y<0)overlap=true;
        else sign=y;
        pt.push_back(y);
        curveSample.push_back(pt);
    }
    //revolution & insert faces
    for(int i=0;i<n2;i++){
        float theta1=i*2*PI/float(n2);//revolution angle
        float theta2=(i+1)*2*PI/float(n2);
        for(int j=0;j<n1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0];
            float y1 = curveSample[j][1]*cos(theta1);
            float z1 = curveSample[j][1]*sin(theta1);
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0];
            float y2 = curveSample[j+1][1]*cos(theta1);
            float z2 = curveSample[j+1][1]*sin(theta1);
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0];
            float y3 = curveSample[j+1][1]*cos(theta2);
            float z3 = curveSample[j+1][1]*sin(theta2);
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0];
            float y4 = curveSample[j][1]*cos(theta2);
            float z4 = curveSample[j][1]*sin(theta2);
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v4);
            geomfacet.push_back(v3);
            geomfacet.push_back(v2);

            m.AddFacet(geomfacet);
        }
    }

}

vector<float> cVector(float c, vector<float> v){
    vector<float> result;
    for(int i=0;i<v.size();i++)result.push_back(c*v[i]);
    return result;
}

void extrusion(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir){
    //original curve z=0
    vector<vector<float> > curveSample;
    vector<float> regularDir;
    for(int i=0;i<3;i++){
        regularDir.push_back(dir[i]*zDepth/dir[2]);
    }

    for(int i=0;i<=n1;i++){
        //sample i*1/n1
        //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
        //bezierCurve(float(i)/n1, ctlPts, x, y);
        pt.push_back(x);
        pt.push_back(y);
        curveSample.push_back(pt);
    }

    for(int i=0;i<n2;i++){
//        float depth1 = i/float(n2)*zDepth;
//        float depth2 = (i+1)/float(n2)*zDepth;
        float depth1 = i/float(n2);
        float depth2 = (i+1)/float(n2);

        vector<float> center1=cVector(depth1, regularDir);
        vector<float> center2=cVector(depth2, regularDir);



        for(int j=0;j<n1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]+center1[0];
            float y1 = curveSample[j][1]+center1[1];
            float z1 = center1[2];
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]+center1[0];
            float y2 = curveSample[j+1][1]+center1[1];
            float z2 = center1[2];
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]+center2[0];
            float y3 = curveSample[j+1][1]+center2[1];
            float z3 = center2[2];
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]+center2[0];
            float y4 = curveSample[j][1]+center2[1];
            float z4 = center2[2];
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}


void extrusionQuadric(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir){
    //original curve z=0
    vector<vector<float> > curveSample;
    vector<float> regularDir;
    for(int i=0;i<3;i++){
        regularDir.push_back(dir[i]*zDepth/dir[2]);
    }

    int n=ctlPts.size()-1;

    for(int k=0;k<=n-2;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
            //quadricP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }

    for(int i=0;i<n2;i++){
        //        float depth1 = i/float(n2)*zDepth;
        //        float depth2 = (i+1)/float(n2)*zDepth;
        float depth1 = i/float(n2);
        float depth2 = (i+1)/float(n2);

        vector<float> center1=cVector(depth1, regularDir);
        vector<float> center2=cVector(depth2, regularDir);



        for(int j=0;j<curveSample.size()-1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]+center1[0];
            float y1 = curveSample[j][1]+center1[1];
            float z1 = center1[2];
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]+center1[0];
            float y2 = curveSample[j+1][1]+center1[1];
            float z2 = center1[2];
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]+center2[0];
            float y3 = curveSample[j+1][1]+center2[1];
            float z3 = center2[2];
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]+center2[0];
            float y4 = curveSample[j][1]+center2[1];
            float z4 = center2[2];
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}

void extrusionCubic(vector<vector<float> > ctlPts, float zDepth, int n1, int n2, Mesh& m,vector<float> dir){
    vector<vector<float> > curveSample;
    vector<float> regularDir;
    for(int i=0;i<3;i++){
        regularDir.push_back(dir[i]*zDepth/dir[2]);
    }

    int n=ctlPts.size()-1;

    for(int k=0;k<=n-3;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
          //  cubicP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],ctlPts[k+3],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }

    for(int i=0;i<n2;i++){
        //        float depth1 = i/float(n2)*zDepth;
        //        float depth2 = (i+1)/float(n2)*zDepth;
        float depth1 = i/float(n2);
        float depth2 = (i+1)/float(n2);

        vector<float> center1=cVector(depth1, regularDir);
        vector<float> center2=cVector(depth2, regularDir);



        for(int j=0;j<curveSample.size()-1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0]+center1[0];
            float y1 = curveSample[j][1]+center1[1];
            float z1 = center1[2];
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0]+center1[0];
            float y2 = curveSample[j+1][1]+center1[1];
            float z2 = center1[2];
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0]+center2[0];
            float y3 = curveSample[j+1][1]+center2[1];
            float z3 = center2[2];
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0]+center2[0];
            float y4 = curveSample[j][1]+center2[1];
            float z4 = center2[2];
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}

void sweepOperator(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m){
    vector<vector<float> > curveSample;

    for(int i=0;i<=n1;i++){
        //sample i*1/n1
        //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
        //bezierCurve(float(i)/n1, ctlPts1, x, y);
        pt.push_back(x);
        pt.push_back(y);
        curveSample.push_back(pt);
    }

    for(int i=0;i<n2;i++){
        float ycenter1=-1;
        float zcenter1=-1;
        float ycenter2=-1;
        float zcenter2=-1;

        //bezierCurve(float(i)/n2, ctlPts2, ycenter1, zcenter1);
        //bezierCurve(float(i+1)/n2, ctlPts2, ycenter2, zcenter2);

        for(int j=0;j<n1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0];
            float y1 = curveSample[j][1]+ycenter1;
            float z1 = 0+zcenter1;
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0];
            float y2 = curveSample[j+1][1]+ycenter1;
            float z2 = 0+zcenter1;
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0];
            float y3 = curveSample[j+1][1]+ycenter2;
            float z3 = 0+zcenter2;
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0];
            float y4 = curveSample[j][1]+ycenter2;
            float z4 = 0+zcenter2;
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}

void sweepOperatorQuadric(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m){
    vector<vector<float> > curveSample;

    int n=ctlPts1.size()-1;

    for(int k=0;k<=n-2;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
            //quadricP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }
    int nn=ctlPts2.size()-1;
    for(int k=0;k<=nn-2;k++){
    for(int i=0;i<n2;i++){
        float ycenter1=-1;
        float zcenter1=-1;
        float ycenter2=-1;
        float zcenter2=-1;

        //quadricP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],float(i)/n2,ycenter1,zcenter1);
        //quadricP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],float(i+1)/n2,ycenter2,zcenter2);
//        //bezierCurve(float(i)/n2, ctlPts2, ycenter1, zcenter1);
//        //bezierCurve(float(i+1)/n2, ctlPts2, ycenter2, zcenter2);

        for(int j=0;j<n1;j++){
            vector<GeomVert> geomfacet;
            float x1 = curveSample[j][0];
            float y1 = curveSample[j][1]+ycenter1;
            float z1 = 0+zcenter1;
            GeomVert v1(x1,y1,z1);
            float x2 = curveSample[j+1][0];
            float y2 = curveSample[j+1][1]+ycenter1;
            float z2 = 0+zcenter1;
            GeomVert v2(x2,y2,z2);
            float x3 = curveSample[j+1][0];
            float y3 = curveSample[j+1][1]+ycenter2;
            float z3 = 0+zcenter2;
            GeomVert v3(x3,y3,z3);
            float x4 = curveSample[j][0];
            float y4 = curveSample[j][1]+ycenter2;
            float z4 = 0+zcenter2;
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
    }
}

void sweepOperatorCubic(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,Mesh& m){
    vector<vector<float> > curveSample;

    int n=ctlPts1.size()-1;

    for(int k=0;k<=n-3;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
          //  cubicP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],ctlPts1[k+3],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample.push_back(pt);
        }
    }
    int nn=ctlPts2.size()-1;
    for(int k=0;k<=nn-3;k++){
        for(int i=0;i<n2;i++){
            float ycenter1=-1;
            float zcenter1=-1;
            float ycenter2=-1;
            float zcenter2=-1;

           // cubicP(ctlPts2[k],ctlPts2[k+1],ctlPts2[k+2],ctlPts2[k+3],float(i)/n2,ycenter1,zcenter1);
           // cubicP(ctlPts2[k],ctlPts2[k+1],ctlPts2[k+2],ctlPts2[k+3],float(i+1)/n2,ycenter2,zcenter2);
            //        //bezierCurve(float(i)/n2, ctlPts2, ycenter1, zcenter1);
            //        //bezierCurve(float(i+1)/n2, ctlPts2, ycenter2, zcenter2);

            for(int j=0;j<n1;j++){
                vector<GeomVert> geomfacet;
                float x1 = curveSample[j][0];
                float y1 = curveSample[j][1]+ycenter1;
                float z1 = 0+zcenter1;
                GeomVert v1(x1,y1,z1);
                float x2 = curveSample[j+1][0];
                float y2 = curveSample[j+1][1]+ycenter1;
                float z2 = 0+zcenter1;
                GeomVert v2(x2,y2,z2);
                float x3 = curveSample[j+1][0];
                float y3 = curveSample[j+1][1]+ycenter2;
                float z3 = 0+zcenter2;
                GeomVert v3(x3,y3,z3);
                float x4 = curveSample[j][0];
                float y4 = curveSample[j][1]+ycenter2;
                float z4 = 0+zcenter2;
                GeomVert v4(x4,y4,z4);

                geomfacet.push_back(v1);
                geomfacet.push_back(v2);
                geomfacet.push_back(v3);
                geomfacet.push_back(v4);

                m.AddFacet(geomfacet);
            }
        }
    }
}

void nonSelfInter(Mesh& m){
    vector<GeomVert> geomfacet1;
        GeomVert v1(1,1,0);
        GeomVert v2(1,3,0);
        GeomVert v3(2,-1,0);
        GeomVert v4(-1,-4,5);

//    GeomVert v1(1,1,0);
//    GeomVert v2(1,-1,0);
//    GeomVert v3(-1,-1,0);
//    GeomVert v4(-1,1,0);

//    GeomVert v1(0.731667,0.381667,0);
//    GeomVert v2(-0.365833,0.381667,0.633642);
//    GeomVert v3(-0.295,-0.01,0.510955);
//    GeomVert v4(0.59,-0.01,0);

    geomfacet1.push_back(v1);
    geomfacet1.push_back(v2);
    geomfacet1.push_back(v3);
    geomfacet1.push_back(v4);

    m.AddFacet(geomfacet1);

    vector<GeomVert> geomfacet2;
        GeomVert u1(5,0.2,-2);
        GeomVert u2(-1,3,1);
        GeomVert u3(-1,4,-1);
        GeomVert u4(1,0.2,-1);

//    GeomVert u1(-0.218333,0.67,-0.378164);
//    GeomVert u2(0.436667,0.67,7.63492e-08 );
//    GeomVert u3(0.731667,0.381667,1.27929e-07);
//    GeomVert u4(-0.365833,0.381667,-0.633642);

//    GeomVert u1(1,0,2);
//    GeomVert u2(-1,0,2);
//    GeomVert u3(-1,0,3);
//    GeomVert u4(1,0,3);

    geomfacet2.push_back(u1);
    geomfacet2.push_back(u2);
    geomfacet2.push_back(u3);
    geomfacet2.push_back(u4);

    m.AddFacet(geomfacet2);

}

void loft(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m){
    vector<vector<float> > curveSample1;
    vector<vector<float> > curveSample2;

    for(int i=0;i<=n1;i++){
        //sample i*1/n1
        //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
        //bezierCurve(float(i)/n1, ctlPts1, x, y);
        pt.push_back(x);
        pt.push_back(y);
        curveSample1.push_back(pt);
    }

    for(int i=0;i<=n1;i++){
        //sample i*1/n1
        //Construct topoFace
        vector<float> pt;
        float x=-1;
        float y=-1;
        //bezierCurve(float(i)/n1, ctlPts2, x, y);
        pt.push_back(x);
        pt.push_back(y);
        curveSample2.push_back(pt);
    }

    for(int i=0;i<n1;i++){

//        vector<float> pt1;
//        vector<float> pt2;
        for(int j=0;j<n2;j++){
            float coeffi=float(j)/n2;
            float coe2=float(j+1)/n2;
            vector<GeomVert> geomfacet;
            float x1 = (1-coeffi)*curveSample1[i][0]+coeffi*curveSample2[i][0];
            float y1 = (1-coeffi)*curveSample1[i][1]+coeffi*curveSample2[i][1];
            float z1 = depth+coeffi*interval;
            GeomVert v1(x1,y1,z1);
            float x2 = (1-coeffi)*curveSample1[i+1][0]+coeffi*curveSample2[i+1][0];
            float y2 = (1-coeffi)*curveSample1[i+1][1]+coeffi*curveSample2[i+1][1];
            float z2 = depth+coeffi*interval;
            GeomVert v2(x2,y2,z2);
            float x3 = (1-coe2)*curveSample1[i+1][0]+coe2*curveSample2[i+1][0];
            float y3 = (1-coe2)*curveSample1[i+1][1]+coe2*curveSample2[i+1][1];
            float z3 = depth+coe2*interval;
            GeomVert v3(x3,y3,z3);
            float x4 = (1-coe2)*curveSample1[i][0]+coe2*curveSample2[i][0];
            float y4 = (1-coe2)*curveSample1[i][1]+coe2*curveSample2[i][1];
            float z4 = depth+coe2*interval;
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }

}

void loftQuadric(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m){
    vector<vector<float> > curveSample1;
    vector<vector<float> > curveSample2;

    int nCtl1=ctlPts1.size()-1;

    for(int k=0;k<=nCtl1-2;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
            //quadricP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample1.push_back(pt);
        }
    }

    int nCtl2=ctlPts2.size()-1;

    for(int k=0;k<=nCtl2-2;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
            //quadricP(ctlPts2[k],ctlPts2[k+1],ctlPts2[k+2],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample2.push_back(pt);
        }
    }

    int nsample=(curveSample1.size()<curveSample2.size())?curveSample1.size():curveSample2.size();

    for(int i=0;i<nsample-1;i++){

        //        vector<float> pt1;
        //        vector<float> pt2;
        for(int j=0;j<n2;j++){
            float coeffi=float(j)/n2;
            float coe2=float(j+1)/n2;
            vector<GeomVert> geomfacet;
            float x1 = (1-coeffi)*curveSample1[i][0]+coeffi*curveSample2[i][0];
            float y1 = (1-coeffi)*curveSample1[i][1]+coeffi*curveSample2[i][1];
            float z1 = depth+coeffi*interval;
            GeomVert v1(x1,y1,z1);
            float x2 = (1-coeffi)*curveSample1[i+1][0]+coeffi*curveSample2[i+1][0];
            float y2 = (1-coeffi)*curveSample1[i+1][1]+coeffi*curveSample2[i+1][1];
            float z2 = depth+coeffi*interval;
            GeomVert v2(x2,y2,z2);
            float x3 = (1-coe2)*curveSample1[i+1][0]+coe2*curveSample2[i+1][0];
            float y3 = (1-coe2)*curveSample1[i+1][1]+coe2*curveSample2[i+1][1];
            float z3 = depth+coe2*interval;
            GeomVert v3(x3,y3,z3);
            float x4 = (1-coe2)*curveSample1[i][0]+coe2*curveSample2[i][0];
            float y4 = (1-coe2)*curveSample1[i][1]+coe2*curveSample2[i][1];
            float z4 = depth+coe2*interval;
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}

void loftCubic(vector<vector<float> > ctlPts1,vector<vector<float> > ctlPts2,int n1,int n2,float depth,float interval,Mesh& m){
    vector<vector<float> > curveSample1;
    vector<vector<float> > curveSample2;

    int nCtl1=ctlPts1.size()-1;

    for(int k=0;k<=nCtl1-3;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
           // cubicP(ctlPts1[k],ctlPts1[k+1],ctlPts1[k+2],ctlPts1[k+3],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample1.push_back(pt);
        }
    }

    int nCtl2=ctlPts2.size()-1;

    for(int k=0;k<=nCtl2-3;k++){
        for(int i=0;i<=n1;i++){
            //sample i*1/n1
            //Construct topoFace
            vector<float> pt;
            float u = float(i)/n1;
            float x=-1;
            float y=-1;
           // cubicP(ctlPts2[k],ctlPts2[k+1],ctlPts2[k+2],ctlPts2[k+3],u,x,y);
            pt.push_back(x);
            pt.push_back(y);
            curveSample2.push_back(pt);
        }
    }

    int nsample=(curveSample1.size()<curveSample2.size())?curveSample1.size():curveSample2.size();

    for(int i=0;i<nsample-1;i++){

        //        vector<float> pt1;
        //        vector<float> pt2;
        for(int j=0;j<n2;j++){
            float coeffi=float(j)/n2;
            float coe2=float(j+1)/n2;
            vector<GeomVert> geomfacet;
            float x1 = (1-coeffi)*curveSample1[i][0]+coeffi*curveSample2[i][0];
            float y1 = (1-coeffi)*curveSample1[i][1]+coeffi*curveSample2[i][1];
            float z1 = depth+coeffi*interval;
            GeomVert v1(x1,y1,z1);
            float x2 = (1-coeffi)*curveSample1[i+1][0]+coeffi*curveSample2[i+1][0];
            float y2 = (1-coeffi)*curveSample1[i+1][1]+coeffi*curveSample2[i+1][1];
            float z2 = depth+coeffi*interval;
            GeomVert v2(x2,y2,z2);
            float x3 = (1-coe2)*curveSample1[i+1][0]+coe2*curveSample2[i+1][0];
            float y3 = (1-coe2)*curveSample1[i+1][1]+coe2*curveSample2[i+1][1];
            float z3 = depth+coe2*interval;
            GeomVert v3(x3,y3,z3);
            float x4 = (1-coe2)*curveSample1[i][0]+coe2*curveSample2[i][0];
            float y4 = (1-coe2)*curveSample1[i][1]+coe2*curveSample2[i][1];
            float z4 = depth+coe2*interval;
            GeomVert v4(x4,y4,z4);

            geomfacet.push_back(v1);
            geomfacet.push_back(v2);
            geomfacet.push_back(v3);
            geomfacet.push_back(v4);

            m.AddFacet(geomfacet);
        }
    }
}

void cubicSample(vector<vector<float> > ctlPts, int nSample,vector<vector<float> >& sample){
    int n=ctlPts.size()-1;
        for(int k=0;k<=n-3;k++){
            //curve equation for Pk, Pk+1, Pk+2

            for(int j=0;j<=nSample;j++){
                float u = float(j)/nSample;
                float x=0;
                float y=0;
               // cubicP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],ctlPts[k+3],u,x,y);

                //cout<<"x: "<<x<<" y: "<<y<<endl;
            }
        }
}

//void cubicBspline(vector<vector<float> > ctlPts){
//    //Draw here
//    //Every three points
//    int n=ctlPts.size()-1;
//
//    glBegin(GL_LINE_STRIP);
//    for(int k=0;k<=n-3;k++){
//        //curve equation for Pk, Pk+1, Pk+2
//
//        for(int j=0;j<=NUM_SEGMENT_BSPLINE_EACH;j++){
//            float u = float(j)/NUM_SEGMENT_BSPLINE_EACH;
//            float x=0;
//            float y=0;
//            cubicP(ctlPts[k],ctlPts[k+1],ctlPts[k+2],ctlPts[k+3],u,x,y);
//            glVertex2d(x, y);
//            //cout<<"x: "<<x<<" y: "<<y<<endl;
//        }
//    }
//    glEnd();
//
//}

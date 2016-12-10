#ifndef BSPLINE_H_INCLUDED
#define BSPLINE_H_INCLUDED

class Bspline
{
    public:
    Bspline(){}
    Mesh BsplineMesh(Mesh mesh, string filename);//, int &arclengthu, int &arclengthv);
    Mesh BsplineMeshNURB(Mesh mesh, string filename);
    float splinegen(float u,float P[4][4],float w);
    float splinegenRat(float u, float a[4][4], float v, float h[4][4]);
    Mesh BsplineMeshRat(Mesh mesh, string filename, float h[4][4]);
    Mesh BsplineMeshReparam(Mesh mesh, string filename, float urep, float wrep);
    int fact(int n)
    {
    if(n==0)
        return 1;
    else return n*fact(n-1);

    }
    float combination(int n, int i)
    {
    return fact(n)/(fact(n-i)*fact(i));

    }
    float NiD(int n, int i, float u)
    {

    return (combination(n,i)*pow(u,i)*pow(1-u,n-i));

    }



};


#endif // BSPLINE_H_INCLUDED

#ifndef LOOP_H_INCLUDED
#define LOOP_H_INCLUDED

class Loop
{
public:
    Loop()   {}
    Mesh createVertex(Mesh m, float alpha);
    Mesh createNet(Mesh m);
    Mesh LoopMeshGen(Mesh m);
};


#endif // LOOP_H_INCLUDED

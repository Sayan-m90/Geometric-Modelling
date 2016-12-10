
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

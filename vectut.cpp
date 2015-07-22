
struct Foo { int; } __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));
long sum(Foo * __restrict__ v) throw()
{
    long s = 0;
    for (unsigned i=0; i<SIZE; i++) 
		s += v->v[i]*2;
    return s;
};

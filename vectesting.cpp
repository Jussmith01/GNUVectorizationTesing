#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <omp.h>

struct Data
{
	std::vector<float> a;
	std::vector<float> c;

	Data(long int N)
	{
		a.resize(N);
		c.resize(N);
	}

	void free()
	{
		a.clear();
		c.clear();
	}
};

//template<typename T>
void fill(Data* dat,float val,size_t N)
{
	for (int i=0;i<(int)N;++i)
	{
			dat->a[i]=val;
			dat->c[i]=val;
	}
};

//template<typename T>
void axpy (float con,Data* dat,size_t set_s,size_t block_s)
{
	//#pragma omp parallel for default(none) shared(dat) firstprivate(set_s,block_s,con)
	for (int i=0;i<(int)set_s;++i)
	{
		//#pragma simd
		//#pragma GCC ivdep Unroll Vector
		#pragma vector aligned
		for (int j=0;j<(int)block_s;++j)
		{
			dat->c[j+i*block_s] += con * dat->a[j+i*block_s];
		}
	}
};

/*template<typename T>
inline void axpy (float con,T* __restrict__ a,T* __restrict__ b,T* __restrict__ c,size_t N,size_t BS)
{
        int blocks = N/(int)BS;

        //#pragma omp parallel for default(none) shared(a,b,c) firstprivate(BS,con,N,blocks)
        for (int i=0;i<(int)blocks;++i)
        {
                int idx = i*(int)BS;

                //#pragma ivdep
                for (int j=idx;j<(int)BS+idx;++j)
                {
                        c[j] += con * a[j] * b[j];
                }
        }
};*/

template<typename T>
float reduction(std::vector<T>& temp,size_t set_s, size_t block_s)
{
	float sum=0.0;
	#pragma omp parallel for default(none) shared(temp) firstprivate(set_s,block_s) reduction(+:sum)
	for (int i=0;i<(int)set_s;++i)
	{
		for (int j=0;j<(int)block_s;++j)
			sum+=temp[i][j];
	};
	return sum;
};

int main ()
{
	long int N=1024*1024*32;
	float K = 1.0;
	int loop=4;

        int max=1024*16;
        int block_s=2;

        std::ofstream graph;
        graph.open("graph.dat");

        while(block_s<max)
        {
		int set_s = N/block_s;

		Data dat(N);
		//std::vector< float > c;

		//float wstart = omp_get_wtime();
		fill(&dat,K,N);
		//float wend = omp_get_wtime() - wstart;
		//std::cout << "fill time: " << wend << " Throughput: " << (N*1.0e-9)/(double)wend << std::endl;
	
		float con = K;
	
		double wstart = omp_get_wtime();
		axpy(con,&dat,set_s,block_s);

		double wend = omp_get_wtime() - wstart;

		std::cout << "set_s: " << set_s << " block_s: " << block_s << " axpy time: " << wend << " Throughput: " << (2*N*1.0E-9)/(double)wend << std::endl;

		graph << block_s << "  " << (2*N*1.0E-9)/(double)wend << std::endl;

        	//wstart = omp_get_wtime();
		//std::cout << " SUM: " << reduction(c,set_s,block_s) << std::endl;
        	//wend = omp_get_wtime() - wstart;
		//std::cout << "reduction time: " << wend << " Throughput: " << (N*1.0e-9)/(double)wend << std::endl;

		block_s+=64;

		dat.free();
	}

	graph.close();

	return 0;
};

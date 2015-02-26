// Evgenii B. Rudnyi, http://MatrixProgramming.com

#include <iostream>
#include <cstdlib>
#include "matrix.h"
#include "sight.h"
#include "Measure_DCSL.h"
#include <sstream>
using namespace sight;

using namespace::std;
namedMeasures getMeasures()
{

   return namedMeasures(
#ifdef RAPL
                              "RAPL", new RAPLMeasure()
#else
                              "time", new timeMeasure(),
                              "PAPI", new PAPIMeasure(papiEvents(PAPI_TOT_INS, PAPI_L2_DCM))
                              //"PAPI", new PAPIMeasure(papiEvents(PAPI_LD_INS))
#endif
                             );

}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: mm dim1 dim2 dim3\n";
		std::cout<< "Please specify matrix dimensions\n" ;
		return 1;
	}
	int dim1, dim2, dim3;
	dim1 = atoi(argv[1]);
	if (argc < 3)
		dim2 = dim1;
	else
		dim2 = atoi(argv[2]);
	if (argc < 4)
		dim3 = dim1;
	else
		dim3 = atoi(argv[3]);

	char* expId = "-1"; if(getenv("EXP_ID")) expId = getenv("EXP_ID");

	stringstream outDir;
	outDir << "dbg.MATRIX_MULTIPLY_" << dim1 << "_" << dim2 << "_" << dim3 << "_" << expId; 

	SightInit(argc, argv, "MATRIX_MULTIPLY",txt()<< outDir.str());

	Matrix A(dim1, dim2), B(dim2, dim3), C(dim1, dim3);
	srand(86456);
	double maxr=(double)RAND_MAX;
	for (int i = 0; i < dim1; i++)
		for (int j = 0; j < dim2; j++)
			A(i, j) = rand()/maxr;
	for (int i = 0; i < dim2; i++)
		for (int j = 0; j < dim3; j++)
			B(i, j) = rand()/maxr;
	Timing mul;
       {
	module multiplyModule(instance("multiply", 1, 0),
                     inputs(port(context("dim1",  dim1,  sight::common::module::notes(sight::common::module::publicized()),
                                         "dim2", dim2, sight::common::module::notes(sight::common::module::publicized()),
                                         "dim3", dim3, sight::common::module::notes(sight::common::module::publicized())))),
                     getMeasures() );
       
	for (int j = 0; j < dim3; j++)
	{
		for (int i = 0; i < dim1; i++)
			C(i, j) = 0.;
		for (int k = 0; k < dim2; k++)
			for (int i = 0; i < dim1; i++)
				C(i, j) += A(i, k)*B(k, j);
	}
       }
	double time = mul.time();
	cout << "time for C(" << dim1 << "," << dim3 << ") = A(" << dim1 << "," << dim2 << ") B(" << dim2 << "," << dim3 << ") is " << time << " s" << endl;	
	return 0;
}


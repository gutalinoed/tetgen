// main function for start tetrahedralize

#include <stdio.h>
#include "tetgenio_ext.h"
#include "tetgen.h"

#include <exception>
///exception for tetramesh construction
class TetFailException : public std::exception {};

bool tetrahedralize_ext(char *switches, tetgenio_ext& in, tetgenio_ext &out);

int main(int argc, char *argv[]) 
{
	int res = 0;
	//default test parameters
	std::string parameters = "pq1.41aAnV";
	std::string s_infile =  "meshin.bin";
	std::string s_outfile = "meshout.bin"; 

	if (argc >= 2) {
		parameters = argv[1];
		if (argc >= 3) s_infile = argv[2];
		if (argc >= 4) s_outfile = argv[3];
	}
#ifndef _DEBUG
	else {
		printf("Not enough parameters.\n");
		return 1;
	}
#endif

	tetgenio_ext in_mesh; // input mesh
	tetgenio_ext out_mesh;// output mesh

	if (res == 0 && !in_mesh.loadmesh(s_infile)) {
		res = 2;
	}

	if (res == 0 && !tetrahedralize_ext(const_cast<char*>(parameters.c_str()), in_mesh, out_mesh)) {
		res = 3;
	}
	
	if (res == 0 && !out_mesh.dumpmesh(s_outfile)) {
		res = 4;
	}
	
	return res;
}

bool checkTetrahedronattributelist(const tetgenio& tet){
	bool res = true;
	const size_t tetra_num = (size_t)tet.numberoftetrahedra;
	for (size_t i = 0; (i < tetra_num) && res; i++) {
		res &= (tet.tetrahedronattributelist[i] != 0);
	}
	return res;
}

bool tetrahedralize_ext(char *switches, tetgenio_ext& in, tetgenio_ext &out) {
	if (&in == nullptr || &out == nullptr) return false;

	//simple patch
	if (in.numberoffacets == 0 && in.numberoftetrahedra == 0){ 
		out = in;
		return true;
	}

	//result flag
	bool res = false;

	//check for key 'Y' in switches
	bool isYinswitches = false;
	const size_t switches_len = strlen(switches) + 1; //+1 for zero end
	for (size_t i = 0; i < switches_len; i++) {
		isYinswitches |= (switches[i] == 'Y');
	}

	//dump for in and out mesh, if something goes wrong
	tetgenio_ext in_dump(in);
	tetgenio_ext out_dump(out);

	//first try - as is, Tetgen 1.5 out
	try{
		tetrahedralize(switches, &in, &out);
		
		//if there is a 'Y' key, then tetgen tried to save the grid, but sometimes it fails:
		//incorrect values of the tetrahedronattribute are present (the trifacemarkerlist is correct)
		//this causes a drop in the post-processing
		//you can patch there, or you can try to restart here with a different key
		//it is also noticed that an incorrect tetrahedronattribute is obtained when "Added% ld Steiner points in segments"

		if (isYinswitches && !checkTetrahedronattributelist(out)) {
			//use the "Y/2" key if the key "Y" was present and the tetrahedronattribute check failed
			size_t switches_Y2_len = switches_len + 2; // "/2"
			char *switches_Y2 = new char[switches_Y2_len];
			for (size_t i = 0, j = 0; i < switches_len; i++) {
				switches_Y2[j++] = switches[i];
				if (switches[i] == 'Y') {
					switches_Y2[j++] = '/';
					switches_Y2[j++] = '2';
				}
			}

			in = in_dump;
			out = out_dump;

			printf("Warning: degrading to Y/2\n"); fflush(stdout);
			tetrahedralize(switches_Y2, &in, &out);
			//if exception -> go to non-Y catch

			delete[] switches_Y2;

			//check again. if there is a tetrahedronattribute == 0, then exception
			if (!checkTetrahedronattributelist(out)) {
				throw TetFailException();
			}
		}
		res = true;
	}
	catch(...) {
		// second try - if "Y" exists - remove it
		if (isYinswitches) {
			char *switches_nonY = new char[switches_len];
			for (size_t i = 0, j = 0; i < switches_len; i++) {
				if (!(switches[i] == 'Y')) {
					switches_nonY[j++] = switches[i];
				}
			}

			in = in_dump;
			out = out_dump;

			printf("Warning: degrading to non-Y\n"); fflush(stdout);
			try {
				tetrahedralize(switches_nonY, &in, &out);
				res = true;
			}
			catch (...) {
				// complete give up
				printf("Failed to generate target representation\n"); fflush(stdout);
			}

			delete[] switches_nonY;
		}
	}

	return res;
}


///\file .\projects\TetgenWrapper\src\tetgenio_ext.cpp
///implementation of tetgenio_ext class
//////////////////////////////////////////////////////////////////////

#include "tetgenio_ext.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

/// copy constructor for tetgenio_ext
tetgenio_ext::tetgenio_ext(const tetgenio_ext& in) {
	//initialize() call from tetgenio() constructor
	copy(in);
}

tetgenio_ext& tetgenio_ext::operator=(const tetgenio_ext& in) {
	deinitialize(); //for delete arrays
	initialize();   //for init pointer to NULL
	copy(in);
	return *this;
}

void tetgenio_ext::copy(const tetgenio_ext& in){
	if (&in == NULL) return;
	//general
	firstnumber = in.firstnumber;
	mesh_dim = in.mesh_dim;
	useindex = in.useindex;
	//points
	numberofpoints = in.numberofpoints;
	numberofpointattributes = in.numberofpointattributes;
	numberofpointmtrs = in.numberofpointmtrs;
	//tetrahedra
	numberoftetrahedra = in.numberoftetrahedra;
	numberofcorners = in.numberofcorners;
	numberoftetrahedronattributes = in.numberoftetrahedronattributes;
	//trifaces
	numberoftrifaces = in.numberoftrifaces;
	//edges
	numberofedges = in.numberofedges;
	//facets
	numberoffacets = in.numberoffacets;
	//other
	numberofholes = in.numberofholes;
	numberofregions = in.numberofregions;
	numberoffacetconstraints = in.numberoffacetconstraints;
	numberofsegmentconstraints = in.numberofsegmentconstraints;
	//Voronoi
	numberofvpoints = in.numberofvpoints;
	numberofvedges = in.numberofvedges;
	numberofvfacets = in.numberofvfacets;
	numberofvcells = in.numberofvcells;

	//points
	if (numberofpoints > 0) {
		if (in.pointlist != NULL) {
			pointlist = new TET_REAL[numberofpoints * 3];
			memcpy(pointlist, in.pointlist, sizeof(TET_REAL) * numberofpoints * 3);
		}

		if (numberofpointattributes > 0 && in.pointattributelist != NULL) {
			pointattributelist = new TET_REAL[numberofpoints * numberofpointattributes];
			memcpy(pointattributelist, in.pointattributelist, sizeof(TET_REAL) * numberofpoints * numberofpointattributes);
		}

		if (numberofpointmtrs > 0 && in.pointmtrlist != NULL) {
			pointmtrlist = new TET_REAL[numberofpoints * numberofpointmtrs];
			memcpy(pointmtrlist, in.pointmtrlist, sizeof(TET_REAL) * numberofpoints * numberofpointmtrs);
		}

		if (in.pointmarkerlist != NULL) {
			pointmarkerlist = new int[numberofpoints];
			memcpy(pointmarkerlist, in.pointmarkerlist, sizeof(int) * numberofpoints);
		}

		if (in.pointparamlist != NULL) {
			pointparamlist = new pointparam[numberofpoints];
			memcpy(pointparamlist, in.pointparamlist, sizeof(pointparam) * numberofpoints);
		}
	}

	//tetrahedra
	if (numberoftetrahedra > 0) {
		if (numberofcorners > 0 && in.tetrahedronlist != NULL) {
			tetrahedronlist = new int[numberoftetrahedra * numberofcorners];
			memcpy(tetrahedronlist, in.tetrahedronlist, sizeof(int) * numberoftetrahedra * numberofcorners);
		}

		if (numberoftetrahedronattributes > 0 && in.tetrahedronattributelist != NULL) {
			tetrahedronattributelist = new TET_REAL[numberoftetrahedra * numberoftetrahedronattributes];
			memcpy(tetrahedronattributelist, in.tetrahedronattributelist, sizeof(TET_REAL) * numberoftetrahedra * numberoftetrahedronattributes);
		}

		if (in.tetrahedronvolumelist != NULL) {
			tetrahedronvolumelist = new TET_REAL[numberoftetrahedra];
			memcpy(tetrahedronvolumelist, in.tetrahedronvolumelist, sizeof(TET_REAL) * numberoftetrahedra);
		}

		if (in.neighborlist != NULL) { //assume that size is numberoftetrahedra
			neighborlist = new int[numberoftetrahedra * 4];
			memcpy(neighborlist, in.neighborlist, sizeof(int) * numberoftetrahedra * 4);
		}
	}

	//trifaces
	if (numberoftrifaces > 0) {
		if (in.trifacelist != NULL) {
			trifacelist = new int[numberoftrifaces * 3];
			memcpy(trifacelist, in.trifacelist, sizeof(int) * numberoftrifaces * 3);
		}

		if (in.trifacemarkerlist != NULL) {
			trifacemarkerlist = new int[numberoftrifaces];
			memcpy(trifacemarkerlist, in.trifacemarkerlist, sizeof(int) * numberoftrifaces);
		}

		if (in.o2facelist != NULL) {
			o2facelist = new int[numberoftrifaces * 3];
			memcpy(o2facelist, in.o2facelist, sizeof(int) * numberoftrifaces * 3);
		}

		if (in.adjtetlist != NULL) {
			adjtetlist = new int[numberoftrifaces * 2];
			memcpy(adjtetlist, in.adjtetlist, sizeof(int) * numberoftrifaces * 2);
		}
	}

	//edges
	if (numberofedges > 0) {
		if (in.edgelist != NULL) {
			edgelist = new int[numberofedges * 2];
			memcpy(edgelist, in.edgelist, sizeof(int) * numberofedges * 2);
		}

		if (in.edgemarkerlist != NULL) {
			edgemarkerlist = new int[numberofedges];
			memcpy(edgemarkerlist, in.edgemarkerlist, sizeof(int) * numberofedges);
		}

		if (in.o2edgelist != NULL) {
			o2edgelist = new int[numberofedges];
			memcpy(o2edgelist, in.o2edgelist, sizeof(int) * numberofedges);
		}

		if (in.edgeadjtetlist != NULL) {
			edgeadjtetlist = new int[numberofedges];
			memcpy(edgeadjtetlist, in.edgeadjtetlist, sizeof(int) * numberofedges);
		}
	}

	//facets
	if (numberoffacets > 0) {
		facetlist = new facet[numberoffacets];
		for (int i = 0; i < numberoffacets; i++) {
			facetlist[i].numberofpolygons = in.facetlist[i].numberofpolygons;
			facetlist[i].polygonlist = new polygon[facetlist[i].numberofpolygons];
			for (int j = 0; j < facetlist[i].numberofpolygons; j++) {
				facetlist[i].polygonlist[j].numberofvertices = in.facetlist[i].polygonlist[j].numberofvertices;
				facetlist[i].polygonlist[j].vertexlist = new int[facetlist[i].polygonlist[j].numberofvertices];
				memcpy(facetlist[i].polygonlist[j].vertexlist, in.facetlist[i].polygonlist[j].vertexlist, sizeof(int) * facetlist[i].polygonlist[j].numberofvertices);
			}

			facetlist[i].numberofholes = in.facetlist[i].numberofholes;
			if (facetlist[i].numberofholes > 0) {
				facetlist[i].holelist = new TET_REAL[facetlist[i].numberofholes * 3];
				memcpy(facetlist[i].holelist, in.facetlist[i].holelist, sizeof(TET_REAL) * facetlist[i].numberofholes * 3);
			} else {
				facetlist[i].holelist = NULL;
			}
		}

		if (in.facetmarkerlist != NULL) {
			facetmarkerlist = new int[numberoffacets];
			memcpy(facetmarkerlist, in.facetmarkerlist, sizeof(int) * numberoffacets);
		}
	}

	//other
	if (numberofholes > 0 && in.holelist != NULL) {
		holelist = new TET_REAL[numberofholes * 3];
		memcpy(holelist, in.holelist, sizeof(TET_REAL) * numberofholes * 3);
	}

	if (numberofregions > 0 && in.regionlist != NULL) {
		regionlist = new TET_REAL[numberofregions * 5];
		memcpy(regionlist, in.regionlist, sizeof(TET_REAL) * numberofregions * 5);
	}

	if (numberoffacetconstraints > 0 && in.facetconstraintlist != NULL) {
		facetconstraintlist = new TET_REAL[numberoffacetconstraints * 2];
		memcpy(facetconstraintlist, in.facetconstraintlist, sizeof(TET_REAL) * numberoffacetconstraints * 2);
	}
	if (numberofsegmentconstraints > 0 && in.segmentconstraintlist != NULL) {
		segmentconstraintlist = new TET_REAL[numberofsegmentconstraints * 3];
		memcpy(segmentconstraintlist, in.segmentconstraintlist, sizeof(TET_REAL) * numberofsegmentconstraints * 3);
	}

	//Voronoi
	if (numberofvpoints > 0 && in.vpointlist != NULL) {
		vpointlist = new TET_REAL[numberofvpoints * 3];
		memcpy(vpointlist, in.vpointlist, sizeof(TET_REAL) * numberofvpoints * 3);
	}
	if (numberofvedges > 0 && in.vedgelist != NULL) {
		vedgelist = new voroedge[numberofvedges];
		memcpy(vedgelist, in.vedgelist, sizeof(voroedge) * numberofvedges);
	}
	if (numberofvfacets > 0 && in.vfacetlist != NULL) {
		vfacetlist = new vorofacet[numberofvfacets];
		memcpy(vfacetlist, in.vfacetlist, sizeof(vorofacet) * numberofvfacets);
		for (int i = 0; i < numberofvfacets; i++) {
			if (in.vfacetlist[i].elist != NULL) {
				vfacetlist[i].elist = new int[in.vfacetlist[i].elist[0] + 1];
				memcpy(vfacetlist[i].elist, in.vfacetlist[i].elist, sizeof(int) * (in.vfacetlist[i].elist[0] + 1));
			}
		}
	}
	if (numberofvcells > 0 && in.vcelllist != NULL) {
		vcelllist = new int*[numberofvcells];
		for (int i = 0; i < numberofvcells; i++) {
			if (in.vcelllist[i] != NULL) {
				vcelllist[i] = new int[in.vcelllist[i][0] + 1];
				memcpy(vcelllist[i], in.vcelllist[i], sizeof(int) * (in.vcelllist[i][0] + 1));
			} else {
				vcelllist[i] = NULL;
			}
		}
	}
}

/// write to ostream
class SimpleFile_ofstream : public std::ofstream
{
public:
	std::ostream& Write(void* buf, std::streamsize size) {
		return write(reinterpret_cast<const char*>(buf), size);
	}
	virtual std::ostream& operator<<(int i) {
		return write(reinterpret_cast<char*>(&i), sizeof(int));
	}
	virtual std::ostream& operator<<(bool b) {
		int i = b ? 1 : 0;
		return write(reinterpret_cast<char*>(&i), sizeof(int));
	}

};

/// read from istream
class SimpleFile_ifstream : public std::ifstream
{
public:
	std::istream& Read(void* buf, std::streamsize size) {
		return read(reinterpret_cast<char*>(buf), size);
	}

	virtual std::istream& operator>>(int& i) {
		return read(reinterpret_cast<char*>(&i), sizeof(int));
	}
	virtual std::istream& operator>>(bool& b) {
		int i;
		std::istream& out = read(reinterpret_cast<char*>(&i), sizeof(int));
		b = (i == 1);
		return out;
	}
};

///for dump mesh to binary file
bool tetgenio_ext::dumpmesh(const std::string& filename) {
	bool res = false;

	SimpleFile_ofstream tetdump;
	//tetdump.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		tetdump.open(filename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

		//general
		tetdump << firstnumber;
		tetdump << mesh_dim;
		tetdump << useindex;
		//points
		tetdump << numberofpoints;
		tetdump << numberofpointattributes;
		tetdump << numberofpointmtrs;
		//tetrahedra
		tetdump << numberoftetrahedra;
		tetdump << numberofcorners;
		tetdump << numberoftetrahedronattributes;
		//trifaces
		tetdump << numberoftrifaces;
		//edges
		tetdump << numberofedges;
		//facets
		tetdump << numberoffacets;
		//other
		tetdump << numberofholes;
		tetdump << numberofregions;
		tetdump << numberoffacetconstraints;
		tetdump << numberofsegmentconstraints;
		//Voronoi
		tetdump << numberofvpoints;
		tetdump << numberofvedges;
		tetdump << numberofvfacets;
		tetdump << numberofvcells;
		
		//points
		if (numberofpoints > 0) {
			tetdump.Write(pointlist, sizeof(TET_REAL) * numberofpoints * 3);

			if (numberofpointattributes > 0) {
				tetdump.Write(pointattributelist, sizeof(TET_REAL) * numberofpoints * numberofpointattributes);
			}

			if (numberofpointmtrs > 0) {
				tetdump.Write(pointmtrlist, sizeof(TET_REAL) * numberofpoints * numberofpointmtrs);
			}

			tetdump << (pointmarkerlist != NULL);
			if (pointmarkerlist != NULL) {
				tetdump.Write(pointmarkerlist, sizeof(int) * numberofpoints);
			}

			tetdump << (pointparamlist != NULL);
			if (pointparamlist != NULL) {
				tetdump.Write(pointparamlist, sizeof(pointparam) * numberofpoints);
			}
		}
			   
		//tetrahedra
		if (numberoftetrahedra > 0) {
			if (numberofcorners > 0) {
				tetdump.Write(tetrahedronlist, sizeof(int) * numberoftetrahedra * numberofcorners);
			}

			if (numberoftetrahedronattributes > 0) {
				tetdump.Write(tetrahedronattributelist, sizeof(TET_REAL) * numberoftetrahedra * numberoftetrahedronattributes);
			}

			tetdump << (tetrahedronvolumelist != NULL);
			if (tetrahedronvolumelist != NULL) {
				tetdump.Write(tetrahedronvolumelist, sizeof(TET_REAL) * numberoftetrahedra);
			}

			tetdump << (neighborlist != NULL);
			if (neighborlist != NULL) {
				tetdump.Write(neighborlist, sizeof(int) * numberoftetrahedra * 4);
			}
		}

		//trifaces
		if (numberoftrifaces > 0) {
			tetdump.Write(trifacelist, sizeof(int) * numberoftrifaces * 3);

			tetdump << (trifacemarkerlist != NULL);
			if (trifacemarkerlist != NULL) {
				tetdump.Write(trifacemarkerlist, sizeof(int) * numberoftrifaces);
			}

			tetdump << (o2facelist != NULL);
			if (o2facelist != NULL) {
				tetdump.Write(o2facelist, sizeof(int) * numberoftrifaces * 3);
			}

			tetdump << (adjtetlist != NULL);
			if (adjtetlist != NULL) {
				tetdump.Write(adjtetlist, sizeof(int) * numberoftrifaces * 2);
			}
		}

		//edges
		if (numberofedges > 0) {
			tetdump.Write(edgelist, sizeof(int) * numberofedges * 2);

			tetdump << (edgemarkerlist != NULL);
			if (edgemarkerlist != NULL) {
				tetdump.Write(edgemarkerlist, sizeof(int) * numberofedges);
			}
			
			tetdump << (o2edgelist != NULL);
			if (o2edgelist != NULL) {
				tetdump.Write(o2edgelist, sizeof(int) * numberofedges);
			}
			
			tetdump << (edgeadjtetlist != NULL);
			if (edgeadjtetlist != NULL) {
				tetdump.Write(edgeadjtetlist, sizeof(int) * numberofedges);
			}
		}

		//facets
		if (numberoffacets > 0) {
			for (int i = 0; i < numberoffacets; i++) {
				tetdump << facetlist[i].numberofpolygons;
				for (int j = 0; j < facetlist[i].numberofpolygons; j++) {
					tetdump << facetlist[i].polygonlist[j].numberofvertices;
					tetdump.Write(facetlist[i].polygonlist[j].vertexlist, sizeof(int) * facetlist[i].polygonlist[j].numberofvertices);
				}

				tetdump << facetlist[i].numberofholes;
				if (facetlist[i].numberofholes > 0) {
					tetdump.Write(facetlist[i].holelist, sizeof(TET_REAL) * facetlist[i].numberofholes * 3);
				}
			}

			tetdump << (facetmarkerlist != NULL);
			if (facetmarkerlist != NULL) {
				tetdump.Write(facetmarkerlist, sizeof(int) * numberoffacets);
			}
		}

		//other
		if (numberofholes > 0) {
			tetdump.Write(holelist, sizeof(TET_REAL) * numberofholes * 3);
		}

		if (numberofregions > 0) {
			tetdump.Write(regionlist, sizeof(TET_REAL) * numberofregions * 5);
		}

		if (numberoffacetconstraints > 0) {
			tetdump.Write(facetconstraintlist, sizeof(TET_REAL) * numberoffacetconstraints * 2);
		}
		if (numberofsegmentconstraints > 0) {
			tetdump.Write(segmentconstraintlist, sizeof(TET_REAL) * numberofsegmentconstraints * 3);
		}

		//Voronoi
		if (numberofvpoints > 0) {
			tetdump.Write(vpointlist, sizeof(TET_REAL) * numberofvpoints *3);
		}
		if (numberofvedges > 0) {
			tetdump.Write(vedgelist, sizeof(voroedge) * numberofvedges);
		}
		int null_val = -1;
		if (numberofvfacets > 0) {
			tetdump.Write(vfacetlist, sizeof(vorofacet) * numberofvfacets);
			for (int i = 0; i < numberofvfacets; i++) {
				if (vfacetlist[i].elist != NULL) {
					tetdump << vfacetlist[i].elist[0];
					tetdump.Write(vfacetlist[i].elist, sizeof(int) * (vfacetlist[i].elist[0] + 1));
				} else {
					tetdump << null_val;
				}
			}
		}
		if (numberofvcells > 0) {
			for (int i = 0; i < numberofvcells; i++) {
				if (vcelllist[i] != NULL) {
					tetdump << vcelllist[i][0];
					tetdump.Write(vcelllist[i], sizeof(int) * (vcelllist[i][0] + 1));
				} else {
					tetdump << null_val;
				}
			}
		}

		tetdump.close();

		res = true;
	}
	catch (std::ofstream::failure e) {
		std::cerr << "Exception opening/writing/closing file.\n";
	}
	catch (...) {
		std::cerr << "Something is wrong in mesh.\n";
	}

	return res;
}

///for load mesh from binary file
bool tetgenio_ext::loadmesh(const std::string& filename) {
	bool res = false;
	//clean current tetgenio
	deinitialize();
	initialize();

	SimpleFile_ifstream tetdump;
	tetdump.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		tetdump.open(filename, std::ifstream::in | std::ifstream::binary);

		//general
		tetdump >> firstnumber;
		tetdump >> mesh_dim;
		tetdump >> useindex;
		//points
		tetdump >> numberofpoints;
		tetdump >> numberofpointattributes;
		tetdump >> numberofpointmtrs;
		//tetrahedra
		tetdump >> numberoftetrahedra;
		tetdump >> numberofcorners;
		tetdump >> numberoftetrahedronattributes;
		//trifaces
		tetdump >> numberoftrifaces;
		//edges
		tetdump >> numberofedges;
		//facets
		tetdump >> numberoffacets;
		//other
		tetdump >> numberofholes;
		tetdump >> numberofregions;
		tetdump >> numberoffacetconstraints;
		tetdump >> numberofsegmentconstraints;
		//Voronoi
		tetdump >> numberofvpoints;
		tetdump >> numberofvedges;
		tetdump >> numberofvfacets;
		tetdump >> numberofvcells;

		//temp vars for read
		bool flag;
		int num;

		//points
		if (numberofpoints > 0) {
			pointlist = new TET_REAL[numberofpoints * 3];
			tetdump.Read(pointlist, sizeof(TET_REAL) * numberofpoints * 3);

			if (numberofpointattributes > 0) {
				pointattributelist = new TET_REAL[numberofpoints * numberofpointattributes];
				tetdump.Read(pointattributelist, sizeof(TET_REAL) * numberofpoints * numberofpointattributes);
			}

			if (numberofpointmtrs > 0) {
				pointmtrlist = new TET_REAL[numberofpoints * numberofpointmtrs];
				tetdump.Read(pointmtrlist, sizeof(TET_REAL) * numberofpoints * numberofpointmtrs);
			}

			tetdump >> flag;
			if (flag) {
				pointmarkerlist = new int[numberofpoints];
				tetdump.Read(pointmarkerlist, sizeof(int) * numberofpoints);
			}

			tetdump >> flag;
			if (flag) {
				pointparamlist = new pointparam[numberofpoints];
				tetdump.Read(pointparamlist, sizeof(pointparam) * numberofpoints);
			}
		}

		//tetrahedra
		if (numberoftetrahedra > 0) {
			if (numberofcorners > 0) {
				tetrahedronlist = new int[numberoftetrahedra * numberofcorners];
				tetdump.Read(tetrahedronlist, sizeof(int) * numberoftetrahedra * numberofcorners);
			}

			if (numberoftetrahedronattributes > 0) {
				tetrahedronattributelist = new TET_REAL[numberoftetrahedra * numberoftetrahedronattributes];
				tetdump.Read(tetrahedronattributelist, sizeof(TET_REAL) * numberoftetrahedra * numberoftetrahedronattributes);
			}

			tetdump >> flag;
			if (flag) {
				tetrahedronvolumelist = new TET_REAL[numberoftetrahedra];
				tetdump.Read(tetrahedronvolumelist, sizeof(TET_REAL) * numberoftetrahedra);
			}

			tetdump >> flag;
			if (flag) {
				neighborlist = new int[numberoftetrahedra * 4];
				tetdump.Read(neighborlist, sizeof(int) * numberoftetrahedra * 4);
			}
		}

		//trifaces
		if (numberoftrifaces > 0) {
			trifacelist = new int[numberoftrifaces * 3];
			tetdump.Read(trifacelist, sizeof(int) * numberoftrifaces * 3);

			tetdump >> flag;
			if (flag) {
				trifacemarkerlist = new int[numberoftrifaces];
				tetdump.Read(trifacemarkerlist, sizeof(int) * numberoftrifaces);
			}

			tetdump >> flag;
			if (flag) {
				o2facelist = new int[numberoftrifaces * 3];
				tetdump.Read(o2facelist, sizeof(int) * numberoftrifaces * 3);
			}

			tetdump >> flag;
			if (flag) {
				adjtetlist = new int[numberoftrifaces * 2];
				tetdump.Read(adjtetlist, sizeof(int) * numberoftrifaces * 2);
			}
		}

		//edges
		if (numberofedges > 0) {
			edgelist = new int[numberofedges * 2];
			tetdump.Read(edgelist, sizeof(int) * numberofedges * 2);

			tetdump >> flag;
			if (flag) {
				edgemarkerlist = new int[numberofedges];
				tetdump.Read(edgemarkerlist, sizeof(int) * numberofedges);
			}

			tetdump >> flag;
			if (flag) {
				o2edgelist = new int[numberofedges];
				tetdump.Read(o2edgelist, sizeof(int) * numberofedges);
			}

			tetdump >> flag;
			if (flag) {
				edgeadjtetlist = new int[numberofedges];
				tetdump.Read(edgeadjtetlist, sizeof(int) * numberofedges);
			}
		}

		//facets
		if (numberoffacets > 0) {
			facetlist = new facet[numberoffacets];
			for (int i = 0; i < numberoffacets; i++) {
				tetdump >> facetlist[i].numberofpolygons;
				facetlist[i].polygonlist = new polygon[facetlist[i].numberofpolygons];
				for (int j = 0; j < facetlist[i].numberofpolygons; j++) {
					tetdump >> facetlist[i].polygonlist[j].numberofvertices;
					facetlist[i].polygonlist[j].vertexlist = new int[facetlist[i].polygonlist[j].numberofvertices];
					tetdump.Read(facetlist[i].polygonlist[j].vertexlist, sizeof(int) * facetlist[i].polygonlist[j].numberofvertices);
				}

				tetdump >> facetlist[i].numberofholes;
				if (facetlist[i].numberofholes > 0) {
					facetlist[i].holelist = new TET_REAL[facetlist[i].numberofholes * 3];
					tetdump.Read(facetlist[i].holelist, sizeof(TET_REAL)*facetlist[i].numberofholes * 3);
				} else {
					facetlist[i].holelist = NULL;
				}
			}

			tetdump >> flag;
			if (flag) {
				facetmarkerlist = new int[numberoffacets];
				tetdump.Read(facetmarkerlist, sizeof(int) * numberoffacets);
			}
		}

		//other
		if (numberofholes > 0) {
			holelist = new TET_REAL[numberofholes * 3];
			tetdump.Read(holelist, sizeof(TET_REAL) * numberofholes * 3);
		}

		if (numberofregions > 0) {
			regionlist = new TET_REAL[numberofregions * 5];
			tetdump.Read(regionlist, sizeof(TET_REAL) * numberofregions * 5);
		}

		if (numberoffacetconstraints > 0) {
			facetconstraintlist = new TET_REAL[numberoffacetconstraints * 2];
			tetdump.Read(facetconstraintlist, sizeof(TET_REAL) * numberoffacetconstraints * 2);
		}
		if (numberofsegmentconstraints > 0) {
			segmentconstraintlist = new TET_REAL[numberofsegmentconstraints * 3];
			tetdump.Read(segmentconstraintlist, sizeof(TET_REAL) * numberofsegmentconstraints * 3);
		}

		//Voronoi
		if (numberofvpoints > 0) {
			vpointlist = new TET_REAL[numberofvpoints *3];
			tetdump.Read(vpointlist, sizeof(TET_REAL) * numberofvpoints * 3);
		}
		if (numberofvedges > 0) {
			vedgelist = new voroedge[numberofvedges];
			tetdump.Read(vedgelist, sizeof(voroedge) * numberofvedges);
		}
		if (numberofvfacets > 0) {
			vfacetlist = new vorofacet[numberofvfacets];
			tetdump.Read(vfacetlist, sizeof(vorofacet) * numberofvfacets);
			for (int i = 0; i < numberofvfacets; i++) {
				tetdump >> num;
				if (num >= 0) {
					vfacetlist[i].elist = new int[num + 1];
					tetdump.Read(vfacetlist[i].elist, sizeof(int) * (num + 1));
				} else {
					vfacetlist[i].elist = NULL;
				}
			}
		}
		if (numberofvcells > 0) {
			vcelllist = new int*[numberofvcells];
			for (int i = 0; i < numberofvcells; i++) {
				tetdump >> num;
				if (num >= 0) {
					vcelllist[i] = new int[num + 1];
					tetdump.Read(vcelllist[i], sizeof(int) * (num + 1));
				} else {
					vcelllist[i] = NULL;
				}
			}
		}

		tetdump.close();

		res = true;
	} 
	catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file.\n";
	}
	catch (...) {
		std::cerr << "Something is wrong in mesh file.\n";
	}

	return res;
}

///////////////////////////////////////////////////////////////////////////////
// extended save 
///////////////////////////////////////////////////////////////////////////////

// save_nodes()    Save points to a .node file
void tetgenio_ext::save_nodes(const std::string& filebasename)
{
	FILE *fout;
	int i, j;
	const std::string outnodefilename = filebasename + ".node";
	const std::string outmtrfilename  = filebasename + ".mtr";

	printf("Saving nodes to %s\n", outnodefilename.c_str());
	fout = fopen(outnodefilename.c_str(), "w");
	fprintf(fout, "%d  %d  %d  %d\n", numberofpoints, mesh_dim,
		numberofpointattributes, pointmarkerlist != NULL ? 1 : 0);
	for (i = 0; i < numberofpoints; i++) {
		if (mesh_dim == 2) {
			fprintf(fout, "%d  %.16g  %.16g", i + firstnumber, pointlist[i * 3],
				pointlist[i * 3 + 1]);
		}
		else {
			fprintf(fout, "%d  %.16g  %.16g  %.16g", i + firstnumber,
				pointlist[i * 3], pointlist[i * 3 + 1], pointlist[i * 3 + 2]);
		}
		for (j = 0; j < numberofpointattributes; j++) {
			fprintf(fout, "  %.16g",
				pointattributelist[i * numberofpointattributes + j]);
		}
		if (pointmarkerlist != NULL) {
			fprintf(fout, "  %d", pointmarkerlist[i]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);

	// If the point metrics exist, output them to a .mtr file.
	if ((numberofpointmtrs > 0) && (pointmtrlist != (REAL *)NULL)) {
		printf("Saving metrics to %s\n", outmtrfilename.c_str());
		fout = fopen(outmtrfilename.c_str(), "w");
		fprintf(fout, "%d  %d\n", numberofpoints, numberofpointmtrs);
		for (i = 0; i < numberofpoints; i++) {
			for (j = 0; j < numberofpointmtrs; j++) {
				fprintf(fout, "%.16g ", pointmtrlist[i * numberofpointmtrs + j]);
			}
			fprintf(fout, "\n");
		}
		fclose(fout);
	}
}

// save_elements()    Save elements to a .ele file
void tetgenio_ext::save_elements(const std::string& filebasename)
{
	FILE *fout;
	int i, j;
	const std::string outelefilename = filebasename + ".ele";

	printf("Saving elements to %s\n", outelefilename.c_str());
	fout = fopen(outelefilename.c_str(), "w");
	if (mesh_dim == 3) {
		fprintf(fout, "%d  %d  %d\n", numberoftetrahedra, numberofcorners,
			numberoftetrahedronattributes);
		for (i = 0; i < numberoftetrahedra; i++) {
			fprintf(fout, "%d", i + firstnumber);
			for (j = 0; j < numberofcorners; j++) {
				fprintf(fout, "  %5d", tetrahedronlist[i * numberofcorners + j]);
			}
			for (j = 0; j < numberoftetrahedronattributes; j++) {
				fprintf(fout, "  %g",
					tetrahedronattributelist[i * numberoftetrahedronattributes + j]);
			}
			fprintf(fout, "\n");
		}
	}
	else {
		// Save a two-dimensional mesh.
		fprintf(fout, "%d  %d  %d\n", numberoftrifaces, 3, trifacemarkerlist ? 1 : 0);
		for (i = 0; i < numberoftrifaces; i++) {
			fprintf(fout, "%d", i + firstnumber);
			for (j = 0; j < 3; j++) {
				fprintf(fout, "  %5d", trifacelist[i * 3 + j]);
			}
			if (trifacemarkerlist != NULL) {
				fprintf(fout, "  %d", trifacemarkerlist[i]);
			}
			fprintf(fout, "\n");
		}
	}

	fclose(fout);
}

// save_faces()    Save faces to a .face file
void tetgenio_ext::save_faces(const std::string& filebasename)
{
	FILE *fout;
	int i;
	const std::string outfacefilename = filebasename + ".face";

	printf("Saving faces to %s\n", outfacefilename.c_str());
	fout = fopen(outfacefilename.c_str(), "w");
	fprintf(fout, "%d  %d\n", numberoftrifaces,
		trifacemarkerlist != NULL ? 1 : 0);
	for (i = 0; i < numberoftrifaces; i++) {
		fprintf(fout, "%d  %5d  %5d  %5d", i + firstnumber, trifacelist[i * 3],
			trifacelist[i * 3 + 1], trifacelist[i * 3 + 2]);
		if (trifacemarkerlist != NULL) {
			fprintf(fout, "  %d", trifacemarkerlist[i]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}

// save_edges()    Save egdes to a .edge file
void tetgenio_ext::save_edges(const std::string& filebasename)
{
	FILE *fout;
	int i;
	const std::string outedgefilename = filebasename + ".edge";

	printf("Saving edges to %s\n", outedgefilename.c_str());
	fout = fopen(outedgefilename.c_str(), "w");
	fprintf(fout, "%d  %d\n", numberofedges, edgemarkerlist != NULL ? 1 : 0);
	for (i = 0; i < numberofedges; i++) {
		fprintf(fout, "%d  %4d  %4d", i + firstnumber, edgelist[i * 2],
			edgelist[i * 2 + 1]);
		if (edgemarkerlist != NULL) {
			fprintf(fout, "  %d", edgemarkerlist[i]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}

// save_neighbors()    Save egdes to a .neigh file
void tetgenio_ext::save_neighbors(const std::string& filebasename)
{
	FILE *fout;
	int i;
	const std::string outneighborfilename = filebasename + ".neigh";

	printf("Saving neighbors to %s\n", outneighborfilename.c_str());
	fout = fopen(outneighborfilename.c_str(), "w");
	fprintf(fout, "%d  %d\n", numberoftetrahedra, mesh_dim + 1);
	for (i = 0; i < numberoftetrahedra; i++) {
		if (mesh_dim == 2) {
			fprintf(fout, "%d  %5d  %5d  %5d", i + firstnumber, neighborlist[i * 3],
				neighborlist[i * 3 + 1], neighborlist[i * 3 + 2]);
		}
		else {
			fprintf(fout, "%d  %5d  %5d  %5d  %5d", i + firstnumber,
				neighborlist[i * 4], neighborlist[i * 4 + 1],
				neighborlist[i * 4 + 2], neighborlist[i * 4 + 3]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}

// save_poly()    Save segments or facets to a .poly file
void tetgenio_ext::save_poly(const std::string& filebasename)
{
	FILE *fout;
	facet *f;
	polygon *p;
	int i, j, k;
	const std::string outpolyfilename = filebasename + ".poly";

	printf("Saving poly to %s\n", outpolyfilename.c_str());
	fout = fopen(outpolyfilename.c_str(), "w");

	// The zero indicates that the vertices are in a separate .node file.
	//   Followed by number of dimensions, number of vertex attributes,
	//   and number of boundary markers (zero or one).
	fprintf(fout, "%d  %d  %d  %d\n", 0, mesh_dim, numberofpointattributes,
		pointmarkerlist != NULL ? 1 : 0);

	// Save segments or facets.
	if (mesh_dim == 2) {
		// Number of segments, number of boundary markers (zero or one).
		fprintf(fout, "%d  %d\n", numberofedges, edgemarkerlist != NULL ? 1 : 0);
		for (i = 0; i < numberofedges; i++) {
			fprintf(fout, "%d  %4d  %4d", i + firstnumber, edgelist[i * 2],
				edgelist[i * 2 + 1]);
			if (edgemarkerlist != NULL) {
				fprintf(fout, "  %d", edgemarkerlist[i]);
			}
			fprintf(fout, "\n");
		}
	}
	else {
		// Number of facets, number of boundary markers (zero or one).
		fprintf(fout, "%d  %d\n", numberoffacets, facetmarkerlist != NULL ? 1 : 0);
		for (i = 0; i < numberoffacets; i++) {
			f = &(facetlist[i]);
			fprintf(fout, "%d  %d  %d  # %d\n", f->numberofpolygons, f->numberofholes,
				facetmarkerlist != NULL ? facetmarkerlist[i] : 0, i + firstnumber);
			// Output polygons of this facet.
			for (j = 0; j < f->numberofpolygons; j++) {
				p = &(f->polygonlist[j]);
				fprintf(fout, "%d  ", p->numberofvertices);
				for (k = 0; k < p->numberofvertices; k++) {
					if (((k + 1) % 10) == 0) {
						fprintf(fout, "\n  ");
					}
					fprintf(fout, "  %d", p->vertexlist[k]);
				}
				fprintf(fout, "\n");
			}
			// Output holes of this facet.
			for (j = 0; j < f->numberofholes; j++) {
				fprintf(fout, "%d  %.12g  %.12g  %.12g\n", j + firstnumber,
					f->holelist[j * 3], f->holelist[j * 3 + 1], f->holelist[j * 3 + 2]);
			}
		}
	}

	// Save holes.
	fprintf(fout, "%d\n", numberofholes);
	for (i = 0; i < numberofholes; i++) {
		// Output x, y coordinates.
		fprintf(fout, "%d  %.12g  %.12g", i + firstnumber, holelist[i * mesh_dim],
			holelist[i * mesh_dim + 1]);
		if (mesh_dim == 3) {
			// Output z coordinate.
			fprintf(fout, "  %.12g", holelist[i * mesh_dim + 2]);
		}
		fprintf(fout, "\n");
	}

	// Save regions.
	fprintf(fout, "%d\n", numberofregions);
	for (i = 0; i < numberofregions; i++) {
		if (mesh_dim == 2) {
			// Output the index, x, y coordinates, attribute (region number)
			//   and maximum area constraint (maybe -1).
			fprintf(fout, "%d  %.12g  %.12g  %.12g  %.12g\n", i + firstnumber,
				regionlist[i * 4], regionlist[i * 4 + 1],
				regionlist[i * 4 + 2], regionlist[i * 4 + 3]);
		}
		else {
			// Output the index, x, y, z coordinates, attribute (region number)
			//   and maximum volume constraint (maybe -1).
			fprintf(fout, "%d  %.12g  %.12g  %.12g  %.12g  %.12g\n", i + firstnumber,
				regionlist[i * 5], regionlist[i * 5 + 1],
				regionlist[i * 5 + 2], regionlist[i * 5 + 3],
				regionlist[i * 5 + 4]);
		}
	}

	fclose(fout);
}

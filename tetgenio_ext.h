///\file .\projects\TetgenWrapper\include\tetgenio_ext.h
///used for import/export binary data for tetgen
//////////////////////////////////////////////////////////////////////

#pragma once

#include "tetgen.h"
#include <string>

#define TET_REAL REAL

///extendede class for tetgenio
class tetgenio_ext : public tetgenio
{
public:
	// Constructor & destructor.
	tetgenio_ext() {} //call initialize() from tetgenio()
	tetgenio_ext(const tetgenio_ext& in); // copy constructor
	//!!! Attention! tetgenio has no virtual destructor

	// clean and copy 
	tetgenio_ext& operator=(const tetgenio_ext& in);
	// simple copy function
	void copy(const tetgenio_ext& in);
	//for export mesh to binary file
	bool dumpmesh(const std::string& filename);
	//for import mesh from binary file
	bool loadmesh(const std::string& filename);

	// extended save 
	void save_nodes    (const std::string& filebasename);
	void save_elements (const std::string& filebasename);
	void save_faces    (const std::string& filebasename);
	void save_edges    (const std::string& filebasename);
	void save_neighbors(const std::string& filebasename);
	void save_poly     (const std::string& filebasename);
};

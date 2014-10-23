#pragma once
#include <mesh/my_mesh.h>

class object
{
     public:
	
     /* get */	 		 	 
     mesh&  getMesh() { return m_mesh; }
	 ui*    getVBO () { return VBO;    }
	 ui*    getUBO () { return UBO;    }

     private:
	 mesh m_mesh;

	 /* VBO for vertices, normals, texture coordinates */
	 ui VBO[3];

     /* UBO for matrix */
     ui UBO[1];
};
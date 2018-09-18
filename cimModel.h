#ifndef CIM_MODEL_H_INC
#define CIM_MODEL_H_INC

#include <vector>

struct cimModel {
	cimModel();
	~cimModel();



};

struct cimObject {
	enum ObjectType {
		ObjType,
		GroupType
	};

	//!	Object name
	std::string m_strObjName;
	//!	Transformation matrix, stored in OpenGL format
//	aiMatrix4x4 m_Transformation;
	//!	All sub-objects referenced by this object
	std::vector<cimObject*> m_SubObjects;
	///	Assigned meshes
	std::vector<unsigned int> m_Meshes;

	//!	\brief	Default constructor
	cimObject() :
		m_strObjName("") {
		// empty
	}

	//!	\brief	Destructor	
	~cimObject() {
		for (std::vector<cimObject*>::iterator it = m_SubObjects.begin();
			it != m_SubObjects.end(); ++it) {
			delete *it;
		}
		m_SubObjects.clear();
	}
};

#endif
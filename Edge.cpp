#include "Edge.h"


Edge::Edge(ImageClass* class1, ImageClass* class2, double val)
{
	m_class1 = class1;
	m_class2 = class2;
	m_val = val;
}

Edge::~Edge()
{
}

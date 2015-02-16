#pragma once
#include "NodePositioner.h"

class NewNodePositioner : public NodePositioner
{
public:
	NewNodePositioner(vector<NodeProperties*> nodes);
	NewNodePositioner();
	~NewNodePositioner();
};


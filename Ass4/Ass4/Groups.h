#pragma once
#include <stdlib.h>
#include <vector>
#include "data.h";
class Groups
{
public:
	Groups(void);
	~Groups(void);


private:
		std::vector<Data*> fData;
};


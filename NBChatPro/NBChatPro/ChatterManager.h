#pragma once

#include "Chatter.h"
#include <string>
#include <map>
#include <Winsock2.h>

using namespace std;

/* 
	ChatterManager : �����߹�����
*/
typedef map<string, Chatter> ChatterNameMap;
class ChatterManager
{
public:
	ChatterManager();
	~ChatterManager();
private:

};


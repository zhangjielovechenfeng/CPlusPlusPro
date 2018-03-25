#pragma once

#include "Chatter.h"
#include <string>
#include <map>

using namespace std;

/* 
	ChatterManager : 聊天者管理者
*/
typedef map<string, Chatter> ChatterNameMap;
class ChatterManager
{
public:
	ChatterManager();
	~ChatterManager();
private:

};


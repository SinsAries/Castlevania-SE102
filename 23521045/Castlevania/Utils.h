#pragma once
#include <Windows.h>
#include<string>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "GameIDs.h"
#include <map>
#include "Simon.h"

using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);
LPCWSTR ToLPCWSTR(string st);
ItemType StringToItemType(const std::string& str);
int GetAttackAnimationId(CSimon* simon);
extern const std::map<ItemType, AnimationID> ITEM_ANIMATION_MAP;
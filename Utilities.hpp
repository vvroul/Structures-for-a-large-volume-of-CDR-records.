#ifndef UTILS
#define UTILS

#include "HashList.hpp"

void UICommands(ifstream& costFile, int hashEntries_1, int hashEntries_2, int bucketSize);
int TimeSearch(char* time1, char* time2, HashList* list, int pos);
int YearSearch(char* year1, char* year2, HashList* list, int pos);
float CalCost(RestRecord* newRecord, ifstream& costFile);
bool is_empty(ifstream& pFile);

#endif

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "MainClass.h"

int _tmain(int argc, _TCHAR* argv[])
{
  int CRadius = 150;
  int QLen  = 1000;
  TOnlyClusters *OnlyClusters = new TOnlyClusters(CRadius,QLen);
  if(OnlyClusters->LoadFromFile("task.csv"))
   {
	OnlyClusters->FindClusters();
	OnlyClusters->SaveToFile("task_out.csv");
   }
  delete OnlyClusters;
  return 0;
}

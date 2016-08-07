//---------------------------------------------------------------------------

#pragma hdrstop
#include <memory>
//#include "stdafx.h"
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "MainClass.h"

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TPoint::TPoint( void )
 {
   Num = 0;
   x = 0;
   y = 0;
   z = 0;
   Cluster = -1;
   Sector = -1;
 }
//---------------------------------------------------------------------------
TPoint::~TPoint( void )
  {

  }
//---------------------------------------------------------------------------
void TPoint::FromCSV(const char * buff)
 {
//  TPoint tp;
//  sscanf(buff, "%d;%d;%d;%d", &tp.Num, &tp.x, &tp.y, &tp.z);
  sscanf(buff, "%d;%d;%d;%d", &Num, &x, &y, &z);
  Cluster = -1;
  //Sector = SetorNum();
//  return tp;
 }
//---------------------------------------------------------------------------
char * TPoint::ToCSV()
 {
  char *str = new char[256];
  sprintf(str, "%d;%d;%d;%d;%d", Num, x, y, z, Cluster);
//  sprintf(str, "%d;%d;%d;%d;%d;%d", Num, x, y, z, Cluster, Sector);
  return str;
 }
//---------------------------------------------------------------------------
TOnlyClusters::TOnlyClusters ( int CRadius, int QLen )
{
 FRadius = CRadius;
 FQLen = QLen;
 SectorCount =  QLen / CRadius + (QLen % CRadius == 0 ? 0 : 1);

 FPointsCount = 0;
 FClastersCount = 0;
}
//---------------------------------------------------------------------------
TOnlyClusters::~TOnlyClusters ( void )
{

}
//---------------------------------------------------------------------------
int SetorN(int x, int L)
 {
  int sNum = 1;
  sNum = x / L + (x % L == 0 ? 0 : 1);
  return sNum;
 }
//---------------------------------------------------------------------------
int TOnlyClusters::SetorNum(TPoint *FPoint)
 {
  int sNum = 1;
  sNum = min(SetorN(FPoint->x + FQLen, 2*FRadius), SectorCount) +
	 (min(SetorN(FPoint->y + FQLen, 2*FRadius), SectorCount) +
	  min(SetorN(FPoint->z + FQLen, 2*FRadius), SectorCount)*SectorCount)*SectorCount;
  FPoint->Sector =  sNum;

  return sNum;
 }
//---------------------------------------------------------------------------
bool TOnlyClusters::LoadFromFile (const char *FName)
{
 bool ret = true;
 int bufsize = 256;
 char *buff = new char[bufsize]; // буфер промежуточного хранения считываемого из файла текста
 ifstream FRead(FName);
 TPoint FPoint;
 int num = 0;
 try
  {
   if (!FRead.is_open()) // если файл не открыт
	{
	 return false;
	}
   else
	{
	 FRead.getline(buff, bufsize); // считали строку из файла
//	 while(!FRead.eof())
	 while(FRead.getline(buff, bufsize))
	  {
//	   FRead >> buff;
	   FPoint.FromCSV(buff);
	   SetorNum(&FPoint);
	   Points.push_back(FPoint);
	   num++;
	  }
	FPointsCount = num;
	FRead.close(); // закрываем файл
	}
  }
 catch (...)
  {
	ret = false;
  }
 delete buff;
//  std::auto_ptr<TStringList> MyList(new TStringList());
// FILE *FRead;
//  FRead = fopen(FName, "r");
//
 return ret;
}
//---------------------------------------------------------------------------
bool TOnlyClusters::SaveToFile (const char *FName)
{
 return SaveToFile (FName, PointsNew);
}
//---------------------------------------------------------------------------
bool TOnlyClusters::SaveToFile (const char *FName, vector<TPoint> tPoint)
{
 bool ret = true;

 char buff[256]; // буфер промежуточного хранения считываемого из файла текста
 ofstream fOut(FName);
 TPoint FPoint;
 try
  {
   if (!fOut.is_open()) // если файл не открыт
	{
	 ret = false;
	}
   else
	{
	 vector<TPoint>::iterator iter;
	 for ( iter = tPoint.begin(); iter != tPoint.end(); iter++ )
	  {
	   fOut <<  iter->ToCSV() << endl;
	  }
	 fOut.close(); // закрываем файл
	}
  }
 catch(...)
  {
	ret = false;
  }
 return ret;
}
//---------------------------------------------------------------------------
int TOnlyClusters::FindClusters()
{
 while (!Points.empty())
  {
   FClastersCount++; // Номер очередного кластера

//  Выбор базовой точки кластера
   GetPoint();

//  Формирование кластера
   MakeCluster();

//  Перераспределение точек - обработанные и оставшиеся
   ChangeStatus();
  }
 return FClastersCount;
}
//---------------------------------------------------------------------------
//  Упорядочивание по номеру кластера
bool CompCluster(TPoint p1, TPoint p2)
{
	return p1.Cluster < p2.Cluster;
}
//---------------------------------------------------------------------------
//  Упорядочивание по номеру сектора
bool CompSector(TPoint p1, TPoint p2)
{
	return p1.Sector < p2.Sector;
}
//---------------------------------------------------------------------------
//  Упорядочивание по x-координате
bool CompX(TPoint p1, TPoint p2)
{
	return p1.x < p2.x;
}
//---------------------------------------------------------------------------
//  Критерий нахождения в одном кластере
bool IsInCluster(TPoint p1, TPoint p2, int r)
{
//  return sqrt((double)(sqr(p1.x - p2.x) + sqr(p1.y - p2.y) + sqr(p1.z - p2.z))) < 2*r;
 return sqrt((double)(pow(p1.x - p2.x,2) + pow(p1.y - p2.y,2) + pow(p1.z - p2.z,2))) <= 2.0*r;
}

bool IsInCluster2(TPoint p1, TPoint p2, int r)
{
 bool ret = true;
 ret = ret && (abs(p1.x - p2.x) <= 2*r) && (abs(p1.y - p2.y) <= 2*r) && (abs(p1.z - p2.z) <= 2*r);
 return ret;
}
//---------------------------------------------------------------------------
//  Выбор базовой точки кластера
bool TOnlyClusters::GetPoint()
{
 bool ret = true;
// Сортировка по координате x
 sort(Points.begin(), Points.end(), CompSector);
 int  nSector = Points.begin()->Sector;
 vector<TPoint>::iterator iter, bPoint;
// Поиск точки с минимальной координатой
 bPoint = Points.begin();
 for ( iter = Points.begin()+1; iter != Points.end() && iter->Sector == nSector; iter++ )
  {
   if(CompX(*iter, *bPoint))
	 bPoint = iter;
  }
// Базовая точка кластера
 SectorPoint = bPoint;
 return ret;
}
//---------------------------------------------------------------------------
//  Формирование кластера
bool TOnlyClusters::MakeCluster()
{
 bool ret = true;
 vector<TPoint>::iterator iter;
 SectorPoint->Cluster = FClastersCount;
 for ( iter = Points.begin(); iter != Points.end(); iter++ )
  {
   if(IsInCluster(*iter, *SectorPoint, FRadius))
	 iter->Cluster = FClastersCount;
/*
   if(iter->Num == 7897)
	{
	   string ss = iter->ToCSV();
	}
*/
  }

 return ret;
}
//---------------------------------------------------------------------------
//  Перераспределение точек - обработанные и оставшиеся
bool TOnlyClusters::ChangeStatus()
{
 bool ret = true;
 int iCl;
 pair<vector<TPoint>::iterator,vector<TPoint>::iterator> bounds;

 sort(Points.begin(), Points.end(), CompCluster);
 reverse(Points.begin(),Points.end());

//  Поиск начала и конца последовательности точек с данным номером кластера
// bounds = equal_range (Points.begin(), Points.end(), SectorPoint, CompCluster);
 vector<TPoint>::iterator iter;
 iter = Points.begin();
 iCl = Points.begin()->Cluster;
 do
 {
  iter++;
 }
 while((iter->Cluster == iCl) && (iter != Points.end()));
 bounds.first = Points.begin();
 bounds.second = iter;

//  Копирование точек найденного кластера в результирующий вектор
 copy(bounds.first, bounds.second, back_inserter(PointsNew));

//  Удаление точек найденного кластера из вектора даных
 Points.erase(bounds.first, bounds.second);
/*
 int rr = Points.size();
 string FName = "out";
 char ss[10]; itoa(rr,ss,10);
 FName += ss;  FName += ".csv";
 SaveToFile ( FName.c_str(), Points);
 FName += "1";
 SaveToFile ( FName.c_str(), PointsNew);
*/
//  remove_copy(bounds.first, bounds.second, PointsNew.begin());
 return ret;
}
//---------------------------------------------------------------------------


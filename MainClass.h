//---------------------------------------------------------------------------
#ifndef MainClassH
#define MainClassH
#include <System.hpp>
#include <iterator>
#include <vector>

using namespace std;
//---------------------------------------------------------------------------
class TPoint
{
 public:
  TPoint( void );
  ~TPoint( void );
  int Num;           // ����� �����
  int x;             // ���������� x
  int y;             // ���������� y
  int z;             // ���������� z
  int Cluster;       // ����� ��������
  int Sector;        // ����� ������� ������

  void FromCSV(const char * buff);  // ���������� ������� �� ������
  char * ToCSV();                   // ������������ ������ ������
};

class TOnlyClusters
{
public:
   TOnlyClusters ( int CRadius, int QLen );
   ~TOnlyClusters ( void );

//   bool Ok;
//   int Error;

   bool LoadFromFile (const char *FName);      // ������ ������ �� �����
   bool SaveToFile (const char *FName);        // ������ ������ � ����
   int FindClusters();                         // ����� ���������

   __property int ClastersCount = {read = FClastersCount};  // ���������� ��������� ���������
   __property int PointsCount = {read = FPointsCount};      // ���������� �����
//   virtual AnsiString ErrorStr ( void );
//   __property AnsiString ErrorStrDefault = {read = GetErrorStrDefault};
protected:
//   AnsiString GetErrorStrDefault ( void );
private:
   int FPointsCount;       //  ���������� �����
   int FClastersCount;     //  ���������� ���������
   int FRadius;            //  ������
   int FQLen;              //  ����� �������
   int SectorCount;        //  ���������� �������� �� ���
   vector<TPoint> Points;                  // �������� ������
   vector<TPoint> PointsNew;               // ���������
   vector<TPoint>::iterator SectorPoint;   // ������� ����� ��������
//  ����� ������� ����� ��������
   bool GetPoint();

//  ������������ ��������
   bool MakeCluster();

//  ����������������� ����� - ������������ � ����������
   bool ChangeStatus();

   bool SaveToFile (const char *FName, vector<TPoint> tPoint);
   int SetorNum(TPoint *FPoint);   //  ���������� ������ ������� �����
};


#endif

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
  int Num;           // Номер точки
  int x;             // Координата x
  int y;             // Координата y
  int z;             // Координата z
  int Cluster;       // Номер кластера
  int Sector;        // Номер сектора поиска

  void FromCSV(const char * buff);  // Заполнение данными из строки
  char * ToCSV();                   // Формирование строки данных
};

class TOnlyClusters
{
public:
   TOnlyClusters ( int CRadius, int QLen );
   ~TOnlyClusters ( void );

//   bool Ok;
//   int Error;

   bool LoadFromFile (const char *FName);      // Чтение данных из файла
   bool SaveToFile (const char *FName);        // Запись данных в файл
   int FindClusters();                         // Поиск кластеров

   __property int ClastersCount = {read = FClastersCount};  // Количество найденных кластеров
   __property int PointsCount = {read = FPointsCount};      // Количество точек
//   virtual AnsiString ErrorStr ( void );
//   __property AnsiString ErrorStrDefault = {read = GetErrorStrDefault};
protected:
//   AnsiString GetErrorStrDefault ( void );
private:
   int FPointsCount;       //  Количество точек
   int FClastersCount;     //  Количество кластеров
   int FRadius;            //  Радиус
   int FQLen;              //  Длина стороны
   int SectorCount;        //  Количество секторов на оси
   vector<TPoint> Points;                  // Исходные данные
   vector<TPoint> PointsNew;               // Результат
   vector<TPoint>::iterator SectorPoint;   // Базовая точка кластера
//  Выбор базовой точки кластера
   bool GetPoint();

//  Формирование кластера
   bool MakeCluster();

//  Перераспределение точек - обработанные и оставшиеся
   bool ChangeStatus();

   bool SaveToFile (const char *FName, vector<TPoint> tPoint);
   int SetorNum(TPoint *FPoint);   //  Вычисление номера сектора точки
};


#endif

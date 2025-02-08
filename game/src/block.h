#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"
class Block
{
public:
   Block();
   void Draw(int offsetX , int offsetY);//bloğu ofset ile çizdirme
   void Move(int rows,int columns);
   std :: vector <Position> GetCellPositions();
   void Rotate();
   void UndoRotation();
   int id;
std::map<int, std::vector<Position>> cells;//bloğun pozisyonlarını tutan

private:
int cellSize;//hücrelerin boyutunu temsil eden sayı
int rotationState;//bloğun dönüş durumunu temsil eden sayı
std::vector<Color>colors;
int rowOffset;
int columnOffset;
};
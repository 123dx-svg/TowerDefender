#ifndef __TILE_H__
#define __TILE_H__

#include <vector>
//像素大小
#define SIZE_TILE 48
struct Tile
{
	enum class EDirection
	{
		None =0,
		Up,
		Down,
		Left,
		Right
	};


    //地形
	int Terrian = 0;
	//装饰
	int Decoration = -1;
	//特殊标识 0代表房屋 大于0其余值代表刷怪点
	int SpecialFlag = -1;
	//是否有塔 用于运行时判断是否存在塔
	bool HasTower = false;
	//路径默认不是行进路径
	EDirection Direction = EDirection::None;

};

//地图 二维数组
typedef std::vector<std::vector<Tile>> TileMap;

#endif

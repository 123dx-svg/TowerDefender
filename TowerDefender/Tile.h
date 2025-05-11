#ifndef __TILE_H__
#define __TILE_H__

#include <vector>
//���ش�С
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


    //����
	int Terrian = 0;
	//װ��
	int Decoration = -1;
	//�����ʶ 0������ ����0����ֵ����ˢ�ֵ�
	int SpecialFlag = -1;
	//�Ƿ����� ��������ʱ�ж��Ƿ������
	bool HasTower = false;
	//·��Ĭ�ϲ����н�·��
	EDirection Direction = EDirection::None;

};

//��ͼ ��ά����
typedef std::vector<std::vector<Tile>> TileMap;

#endif

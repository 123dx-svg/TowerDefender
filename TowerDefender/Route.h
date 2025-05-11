#ifndef __ROUTE_H__
#define __ROUTE_H__
#include "Tile.h"
#include <SDL.h>
#include <vector>

class Route
{

public:
	typedef std::vector<SDL_Point> IdxList;

public:
	Route() = default;
	Route(const TileMap& map, const SDL_Point& idx_origin)
	{
		size_t width_map = map[0].size();
		size_t height_map = map.size();
		SDL_Point idx_next = idx_origin;

		while (true)
		{
			//�����߽�
			if (idx_next.x>=width_map || idx_next.y>=height_map)
				break;
			//�����ظ���Ԫ��
			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next);

			bool is_next_dir_exist = true;
			const Tile& tile = map[idx_next.y][idx_next.x];

			//�������ص����ֹͣ��
			if (tile.SpecialFlag==0)
				break;

			switch (tile.Direction)
			{
			case Tile::EDirection::Up:
				idx_next.y--;
				break;
			case Tile::EDirection::Down:
				idx_next.y++;
				break;
			case Tile::EDirection::Left:
				idx_next.x--;
				break;
			case Tile::EDirection::Right:
				idx_next.x++;
				break;
			default:
				is_next_dir_exist = false;
				break;

			}
			//��һ�����򲻴��ھ����ɽ���
			if (!is_next_dir_exist)
				break;
			
		}
	}
	~Route()=default;

	//��ȡ·�����б�
	const IdxList& get_idx_list() const
	{
		return idx_list;
	}

private:

	IdxList idx_list;
private:
//�жϵ�ͼ·�����Ƿ��ظ� ��ֹ��ͼ����ʱ����
	bool check_duplicate_idx(const SDL_Point& target_idx) const
	{
		for (const SDL_Point& idx : idx_list)
		{
			if (idx.x == target_idx.x && idx.y == target_idx.y)
			{
				return true;
			}
		}
		return	false;
	}

};



#endif // 

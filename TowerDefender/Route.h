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
			//超出边界
			if (idx_next.x>=width_map || idx_next.y>=height_map)
				break;
			//遇到重复单元格
			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next);

			bool is_next_dir_exist = true;
			const Tile& tile = map[idx_next.y][idx_next.x];

			//遇到防守点可以停止了
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
			//下一个方向不存在就生成结束
			if (!is_next_dir_exist)
				break;
			
		}
	}
	~Route()=default;

	//获取路径点列表
	const IdxList& get_idx_list() const
	{
		return idx_list;
	}

private:

	IdxList idx_list;
private:
//判断地图路径点是否重复 防止地图加载时卡死
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

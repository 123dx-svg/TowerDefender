#ifndef __MAP_H__
#define __MAP_H__

#include "Tile.h"
#include "Route.h"
#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Map
{

public:
	typedef std::unordered_map<int,Route> SpwanerRoutePool;
public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.good()) return false;
		
		TileMap tile_map_temp;


		//地图从上到下从左到右读取
		int index_x = -1 , index_y = -1;

		std::string line;
		while (std::getline(file,line))
		{
		//切割首尾空格
			line = trim_str(line);
			if (line.empty())
				continue;

			index_x = -1, index_y++;
			tile_map_temp.emplace_back();

			std::string str_tile;
			std::stringstream str_stream(line);
			while (std::getline(str_stream, str_tile, ','))
			{
				index_x++;
				tile_map_temp[index_y].emplace_back();
				Tile& tile = tile_map_temp[index_y].back();
				load_tile_from_string(tile,str_tile);
			}

		}

		file.close();
		//数据全空一行也没有或者第一行为空
		if (tile_map_temp.empty()|| tile_map_temp[0].empty())
			return false;
		//安全赋值
		tile_map = tile_map_temp;

		//生成缓存
		generate_map_cache();

		return true;
	}

	//获取地图宽度
	size_t get_width() const
	{
		return tile_map.empty()? 0 : tile_map[0].size();
	}
	//获取地图高度
	size_t get_height() const
	{
		return tile_map.size();
	}
	//获取地图
	const TileMap& get_tile_map() const
	{
		return tile_map;
	}
	//获取防守索引
	const SDL_Point& get_index_home() const
	{
		return index_home;
	}
	//获取路径池
	const SpwanerRoutePool& get_spawner_route_pool() const
	{
		return spawner_route_pool;
	}	

	//放置防御塔
	void place_tower(const SDL_Point& index_tile)
	{
		tile_map[index_tile.y][index_tile.x].HasTower = true;
	}

protected:


private:
	TileMap tile_map;
	//家的索引位置
	SDL_Point index_home={0};
	//路径池
	SpwanerRoutePool spawner_route_pool;

private:
	//处理空格符号
	std::string trim_str(const std::string& str)
	{
		// "   10\-1\0\-1   "处理完成之后 "10\-1\0\-1"

	//去除空格和制表符号
		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
			return ""; 

		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;

		return str.substr(begin_idx, idx_range);
	}

	void load_tile_from_string(Tile& tile, const std::string& str_tile)
	{
		std::string str_tidy = trim_str(str_tile);

		std::string str_value;
		std::vector<int> values;
		std::stringstream str_stream(str_tidy);

		//注意\ 要进行转义
		while (std::getline(str_stream, str_value, '\\'))
		{
			int value;
			try
			{
				value = std::stoi(str_value);//转换为int类型
			}
			catch (const std::invalid_argument&)
			{
			//值异常转成-1
				value = -1;
			}
			
			values.push_back(value);

		}
		//设置地图属性并做异常处理
		tile.Terrian = (values.size()<1||values[0]<0)?0:values[0];
		tile.Decoration = (values.size()<2)?-1:values[1];
		tile.Direction =(Tile::EDirection)((values.size()<3||values[2]<0)?0:values[2]);
		tile.SpecialFlag = (values.size()<=3)?-1:values[3];

	}


	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				//遍历地图所有单元格
			    const Tile& tile = tile_map[y][x];
				if (tile.SpecialFlag<0)
					continue;
				//设置房子的位置
				if (tile.SpecialFlag==0)
				{
					index_home={x,y};
				}
				else
				{
					spawner_route_pool[tile.SpecialFlag]=Route(tile_map,{x,y});
				}
			}
		}
	}
};






#endif // __MAP_H__

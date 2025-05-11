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


		//��ͼ���ϵ��´����Ҷ�ȡ
		int index_x = -1 , index_y = -1;

		std::string line;
		while (std::getline(file,line))
		{
		//�и���β�ո�
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
		//����ȫ��һ��Ҳû�л��ߵ�һ��Ϊ��
		if (tile_map_temp.empty()|| tile_map_temp[0].empty())
			return false;
		//��ȫ��ֵ
		tile_map = tile_map_temp;

		//���ɻ���
		generate_map_cache();

		return true;
	}

	//��ȡ��ͼ���
	size_t get_width() const
	{
		return tile_map.empty()? 0 : tile_map[0].size();
	}
	//��ȡ��ͼ�߶�
	size_t get_height() const
	{
		return tile_map.size();
	}
	//��ȡ��ͼ
	const TileMap& get_tile_map() const
	{
		return tile_map;
	}
	//��ȡ��������
	const SDL_Point& get_index_home() const
	{
		return index_home;
	}
	//��ȡ·����
	const SpwanerRoutePool& get_spawner_route_pool() const
	{
		return spawner_route_pool;
	}	

	//���÷�����
	void place_tower(const SDL_Point& index_tile)
	{
		tile_map[index_tile.y][index_tile.x].HasTower = true;
	}

protected:


private:
	TileMap tile_map;
	//�ҵ�����λ��
	SDL_Point index_home={0};
	//·����
	SpwanerRoutePool spawner_route_pool;

private:
	//����ո����
	std::string trim_str(const std::string& str)
	{
		// "   10\-1\0\-1   "�������֮�� "10\-1\0\-1"

	//ȥ���ո���Ʊ����
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

		//ע��\ Ҫ����ת��
		while (std::getline(str_stream, str_value, '\\'))
		{
			int value;
			try
			{
				value = std::stoi(str_value);//ת��Ϊint����
			}
			catch (const std::invalid_argument&)
			{
			//ֵ�쳣ת��-1
				value = -1;
			}
			
			values.push_back(value);

		}
		//���õ�ͼ���Բ����쳣����
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
				//������ͼ���е�Ԫ��
			    const Tile& tile = tile_map[y][x];
				if (tile.SpecialFlag<0)
					continue;
				//���÷��ӵ�λ��
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

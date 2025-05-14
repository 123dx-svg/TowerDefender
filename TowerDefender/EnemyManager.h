#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "Enemy.h"
#include "Manager.h"
#include "ConfigManager.h"
#include "HomeManager.h"
#include <vector>
#include <SDL.h>

class EnemyManager : public Manager<EnemyManager>
{
    friend class Manager<EnemyManager>;


public:
	typedef std::vector<Enemy*> EnemyList;

public:
	void OnUpdate(double deltaTime)
	{
		for (Enemy* enemy : enemies_list)
			enemy->OnUpdate(deltaTime);
		process_home_collision();
		process_bullet_collision();

		
	}
	void OnRender(SDL_Renderer* renderer)
	{
		for (Enemy* enemy : enemies_list)
			enemy->OnRednder(renderer);
		
	}


public:
	~EnemyManager()
	{
		for (Enemy* enemy : enemies_list)
		{
			delete enemy;
			enemy = nullptr;
		}
	};



private:
	EnemyManager()
	{

	}

private:
	EnemyList enemies_list;

private:
	//处理房屋碰撞
	void process_home_collision()
	{
	//计算房屋所在世界位置
		static const SDL_Point& idx_home = ConfigManager::Instance()->map.get_index_home();
		static const SDL_Rect& rect_tile_map = ConfigManager::Instance()->rect_tile_map;
		static const Vector2D position_home_title=
		{
			(double)rect_tile_map.x+idx_home.x*SIZE_TILE,
			(double)rect_tile_map.y+idx_home.y*SIZE_TILE
		};

		for (Enemy* enemy : enemies_list)
		{
			if(enemy->CanRemove()) continue;

			const Vector2D& position = enemy->GetPosition();
			//判断是否碰撞 也就是范围内
			if (position.x>=position_home_title.x && position.x<=position_home_title.x+SIZE_TILE &&
				position.y>=position_home_title.y && position.y<=position_home_title.y+SIZE_TILE)
			{
				enemy->Make_Invalid();
				HomeManager::Instance()->DecreaseHP(enemy->GetDamage());
			}
		}
		
	}
	//处理子弹碰撞
	void process_bullet_collision()
	{
		
	}
	//移除敌人
	void remove_invalid_enemy()
	{	
		enemies_list.erase(std::remove_if(enemies_list.begin(), enemies_list.end(), [](const Enemy* e)
		{ 
			bool deletable = e->CanRemove();
			if (deletable)
				delete e;
			return deletable;
		}),enemies_list.end());
	}

};	





#endif
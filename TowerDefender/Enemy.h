#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Timer.h"
#include "Vector2D.h"
#include "Animation.h"
#include "Route.h"
#include "ConfigManager.h"
#include <functional>
class Enemy
{

public:
	typedef std::function<void(Enemy* enemy)> SkillCallback;


public:
	Enemy()
	{
		timer_skill.SetOneShot(false);
		timer_skill.SetOnTimeout([&](){on_skill_release(this);});

		//受击打击后重新设置为false
		timer_sketch.SetOneShot(true);
		timer_sketch.SetWaitTime(0.075);
		timer_sketch.SetOnTimeout([&](){is_show_sketch = false;});

		timer_restore_speed.SetOneShot(true);
		timer_restore_speed.SetOnTimeout([&](){speed = max_speed;});
	}

	~Enemy() = default;


	void OnUpdate(float delta_time)
	{
		timer_skill.OnUpdate(delta_time);
		timer_sketch.OnUpdate(delta_time);
		timer_restore_speed.OnUpdate(delta_time);

		//防止越过目标位置
		Vector2D move_distance = velocity*delta_time;
		Vector2D target_distance = position_target - position;
		position+=move_distance<target_distance?move_distance:target_distance;

		//即将到达目标点，切换到下一个目标点
		if (target_distance.ApproxZero())
		{
		//更新目标点
			idx_target++;
			refresh_position_target();
		//更新方向
			direction = (position_target-position).Normailize();
		}
		//更新速度
		velocity.x = direction.x * speed * SIZE_TILE;
		velocity.y = direction.y * speed * SIZE_TILE;

		//水平速度大于垂向速度
		bool is_show_x_amin = abs(velocity.x)>=abs(velocity.y);
		//动画选择
		if (is_show_sketch)
		{
			if (is_show_x_amin)
				anim_current = velocity.x>0? &anim_right_sketch : &anim_left_sketch;
			else
				anim_current = velocity.y>0? &anim_down_sketch : &anim_up_sketch;
		}
		else
		{
			if (is_show_x_amin)
				anim_current = velocity.x > 0 ? &anim_right : &anim_left;
			else
				anim_current = velocity.y > 0 ? &anim_down : &anim_up;
		}
		anim_current->OnUpdate(delta_time);
	}

	void OnRednder(SDL_Renderer* renderer)
	{	
	    static SDL_Rect rect;
		static SDL_Point point;
		static const int offset_y = 2;
		static const Vector2D size_hp_bar = {40,8};
		static const SDL_Color color_border = {116,185,124,255};
		static const SDL_Color color_content = {226,255,194,255};

		//渲染动画位置
		point.x =(int)position.x - size.x / 2;
		point.y = (int)position.y - size.y / 2;
		anim_current->OnRender(renderer, point);
		//绘制血条
		if (hp<=max_hp)
		{
			rect.x = (int)(position.x - size_hp_bar.x / 2);
			rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y-offset_y );
			rect.w = (int)(size_hp_bar.x*(hp/max_hp));
			rect.h = (int)size_hp_bar.y;

			//渲染血条
			SDL_SetRenderDrawColor(renderer,color_content.r,color_content.g,color_content.b,color_content.a);
			SDL_RenderFillRect(renderer,&rect);
			//渲染血条边框
			rect.w=(int)size_hp_bar.x;
			SDL_SetRenderDrawColor(renderer,color_border.r,color_border.g,color_border.b,color_border.a);
			SDL_RenderDrawRect(renderer,&rect);
		}
	}

	void SetOnSkillReleased(SkillCallback on_skill_release)
	{
		this->on_skill_release = on_skill_release;
	}

	void IncreaseHP(double val)
	{
		hp+=val;

		if (hp>max_hp)
			hp = max_hp;
	}
	void DecreaseHP(double val)
	{
		hp-=val;
		if (hp <= 0)
		{
			hp = 0;
			is_valid = false;
		}
		//提示受击特效并重置
		is_show_sketch = true;
		timer_sketch.Restart();
	}
	void SlowDown()
	{
	//减速减1秒
		speed = max_speed - 0.5;
		timer_restore_speed.SetWaitTime(1);
		timer_restore_speed.Restart();
	}

	void SetPositon(const Vector2D& pos)
	{
		position = pos;
	}

	void SetRoute(const Route* route)
	{
		this->route = route;
		refresh_position_target();
	}

	void make_Invalid()
	{
		is_valid = false;
	}
	
	double GetHp() const{return hp;}

	const Vector2D& GetSize() const {return size;}

	const Vector2D& GetPosition() const {return position;}
	const Vector2D& GetVelocity() const { return velocity; }
	const Vector2D& GetDirection() const { return direction; }
	double GetDamage() const { return damage; }
	double GetRewardRatio() const { return reward_ratio; }
	double GetRecoverInterval() const { return recover_interval; }
	//记得转换为世界坐标
	double GetRecoverRange() const { return recover_range*SIZE_TILE; }
	double GetRecoverIntensity() const { return recover_intensity; }
	//能都被移除
	bool CanRemove() const {return !is_valid;}
	//获取寻路进度 防御塔攻击优先级
	double GetRouteProgress() const
	{
		if (route->get_idx_list().size()==1)
			return 1;
		return (double)idx_target / (route->get_idx_list().size()-1);
	}

protected:
	Vector2D size;

	Timer timer_skill;

	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;


	double hp=0;
	double max_hp=0;
	double speed = 0;
	double max_speed = 0;
	double damage = 0;
	double reward_ratio = 0;
	double recover_interval = 0;
	double recover_range = 0;
    double recover_intensity = 0;
private:
   Vector2D position;
   Vector2D velocity;
   Vector2D direction;

   bool is_valid = true;

   //受击打击动画
   Timer timer_sketch;
   bool is_show_sketch = false;

   Animation* anim_current = nullptr;

   //技能回调
   SkillCallback on_skill_release;

   //受击会有减速恢复
   Timer timer_restore_speed;

   //路径
   const Route* route = nullptr;
   int idx_target = 0;
   Vector2D position_target;

private:
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		if (idx_target < idx_list.size())
		{
			const SDL_Point& point  =  idx_list[idx_target];
			static const SDL_Rect& rect_tile_map = ConfigManager::Instance()->rect_tile_map;

			//下一个目标点的中心点
			position_target.x = rect_tile_map.x + point.x * SIZE_TILE+SIZE_TILE/2;
			position_target.y = rect_tile_map.y + point.y * SIZE_TILE+SIZE_TILE/2;
		}
		
	}
};




#endif // __ENEMY_H__

#ifndef __GOBLIN_ENEMY_H_
#define __GOBLIN_ENEMY_H_
#include "Enemy.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"


class GoblinEnemy : public Enemy
{
public:
	GoblinEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::Instance()->GetTexturePool();
		static SDL_Texture* tex_goblin = texture_pool.find(ResID::Tex_Goblin)->second;
		static SDL_Texture* tex_goblin_sketch = texture_pool.find(ResID::Tex_GoblinSketch)->second;
		static ConfigManager::EnemyTemplate& goblin_template = ConfigManager::Instance()->goblin_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.SetLoop(true); anim_up.SetInterval(0.15);
		anim_up.SetFrameData(tex_goblin, 5, 4, idx_list_up);
		anim_down.SetLoop(true); anim_down.SetInterval(0.15);
		anim_down.SetFrameData(tex_goblin, 5, 4, idx_list_down);
		anim_left.SetLoop(true); anim_left.SetInterval(0.15);
		anim_left.SetFrameData(tex_goblin, 5, 4, idx_list_left);
		anim_right.SetLoop(true); anim_right.SetInterval(0.15);
		anim_right.SetFrameData(tex_goblin, 5, 4, idx_list_right);

		anim_up_sketch.SetLoop(true); anim_up_sketch.SetInterval(0.15);
		anim_up_sketch.SetFrameData(tex_goblin_sketch, 5, 4, idx_list_up);
		anim_down_sketch.SetLoop(true); anim_down_sketch.SetInterval(0.15);
		anim_down_sketch.SetFrameData(tex_goblin_sketch, 5, 4, idx_list_down);
		anim_left_sketch.SetLoop(true); anim_left_sketch.SetInterval(0.15);
		anim_left_sketch.SetFrameData(tex_goblin_sketch, 5, 4, idx_list_left);
		anim_right_sketch.SetLoop(true); anim_right_sketch.SetInterval(0.15);
		anim_right_sketch.SetFrameData(tex_goblin_sketch, 5, 4, idx_list_right);

		max_hp = goblin_template.hp;
		max_speed = goblin_template.speed;
		damage = goblin_template.damage;
		reward_ratio = goblin_template.reward_ratio;
		recover_interval = goblin_template.recover_interval;
		recover_range = goblin_template.recover_range;
		recover_intensity = goblin_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~GoblinEnemy() = default;

};



#endif
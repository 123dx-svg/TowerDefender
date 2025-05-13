#ifndef __KING_SLIM_ENEMY_H__
#define __KING_SLIM_ENEMY_H__

#include "Enemy.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"
class KingSlimEnemy : public Enemy
{
public:		
	KingSlimEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::Instance()->GetTexturePool();
		static SDL_Texture* tex_king_slime = texture_pool.find(ResID::Tex_KingSlime)->second;
		static SDL_Texture* tex_king_slime_sketch = texture_pool.find(ResID::Tex_KingSlimeSketch)->second;
		static ConfigManager::EnemyTemplate& king_slim_template = ConfigManager::Instance()->king_slim_template;

		static const std::vector<int> idx_list_up = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.SetLoop(true); anim_up.SetInterval(0.1);
		anim_up.SetFrameData(tex_king_slime, 6, 4, idx_list_up);
		anim_down.SetLoop(true); anim_down.SetInterval(0.1);
		anim_down.SetFrameData(tex_king_slime, 6, 4, idx_list_down);
		anim_left.SetLoop(true); anim_left.SetInterval(0.1);
		anim_left.SetFrameData(tex_king_slime, 6, 4, idx_list_left);
		anim_right.SetLoop(true); anim_right.SetInterval(0.1);
		anim_right.SetFrameData(tex_king_slime, 6, 4, idx_list_right);

		anim_up_sketch.SetLoop(true); anim_up_sketch.SetInterval(0.1);
		anim_up_sketch.SetFrameData(tex_king_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.SetLoop(true); anim_down_sketch.SetInterval(0.1);
		anim_down_sketch.SetFrameData(tex_king_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.SetLoop(true); anim_left_sketch.SetInterval(0.1);
		anim_left_sketch.SetFrameData(tex_king_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.SetLoop(true); anim_right_sketch.SetInterval(0.1);
		anim_right_sketch.SetFrameData(tex_king_slime_sketch, 6, 4, idx_list_right);

		max_hp = king_slim_template.hp;
		max_speed = king_slim_template.speed;
		damage = king_slim_template.damage;
		reward_ratio = king_slim_template.reward_ratio;
		recover_interval = king_slim_template.recover_interval;
		recover_range = king_slim_template.recover_range;
		recover_intensity = king_slim_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}
	~KingSlimEnemy(){}

	
}
#endif

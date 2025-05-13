#ifndef __SLIM_ENEMY_H__
#define __SLIM_ENEMY_H__
#include "Enemy.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"

class SlimEnemy : public Enemy
{
public:
	SlimEnemy()
	{
		static const ResourcesManager::TexturePool& texturePool
		= ResourcesManager::Instance()->GetTexturePool();

		//加载动画素材
		static SDL_Texture* tex_slim = texturePool.find(ResID::Tex_Slime)->second;
		static SDL_Texture* tex_slim_sketch = texturePool.find(ResID::Tex_SlimeSketch)->second;
		//加载配置文件
		static ConfigManager::EnemyTemplate& slim_template =  ConfigManager::Instance()->slim_template;
		//对应图片中的位置
		static const std::vector<int> idx_list_up = {6,7,8,9,10,11};
		static const std::vector<int> idx_list_down = {0,1,2,3,4,5};
		static const std::vector<int> idx_list_left = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_right = { 12,13,14,15,16,17 };

		//纹理属性
		anim_up.SetLoop(true);
		anim_up.SetInterval(0.1);
		anim_up.SetFrameData(tex_slim,6,4, idx_list_up);
		anim_down.SetLoop(true);
		anim_down.SetInterval(0.1);
		anim_down.SetFrameData(tex_slim,6,4, idx_list_down);
		anim_left.SetLoop(true);
		anim_left.SetInterval(0.1);
		anim_left.SetFrameData(tex_slim,6,4, idx_list_left);	
		anim_right.SetLoop(true);
		anim_right.SetInterval(0.1);
		anim_right.SetFrameData(tex_slim,6,4, idx_list_right);

		anim_up_sketch.SetLoop(true);
		anim_up_sketch.SetInterval(0.1);
		anim_up_sketch.SetFrameData(tex_slim_sketch, 6, 4, idx_list_up);
		anim_down_sketch.SetLoop(true);
		anim_down_sketch.SetInterval(0.1);
		anim_down_sketch.SetFrameData(tex_slim_sketch, 6, 4, idx_list_down);
		anim_left_sketch.SetLoop(true);
		anim_left_sketch.SetInterval(0.1);
		anim_left_sketch.SetFrameData(tex_slim_sketch, 6, 4, idx_list_left);
		anim_right_sketch.SetLoop(true);
		anim_right_sketch.SetInterval(0.1);			
		anim_right_sketch.SetFrameData(tex_slim_sketch, 6, 4, idx_list_right);

		//数据属性
		max_hp = slim_template.hp;
		max_speed = slim_template.speed;
		damage = slim_template.damage;
		reward_ratio = slim_template.reward_ratio;
		recover_interval = slim_template.recover_interval;
		recover_range = slim_template.recover_range;
		recover_intensity = slim_template.recover_intensity;

		//图片大小
		size.x = 48; size.y	 = 48;
		hp = max_hp;
		speed = max_speed;


	}
	~SlimEnemy(){};
};			





#endif // __SLIMNEMY_H__

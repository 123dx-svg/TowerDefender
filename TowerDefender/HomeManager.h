#ifndef __HOME_MANAGER_H__
#define __HOME_MANAGER_H__

#include "Manager.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"

class HomeManager : public Manager<HomeManager>
{
	friend class Manager<HomeManager>;
public:
	~HomeManager()=default;

public:
	double GetCurrentHP() const { return num_hp; }
	void DecreaseHP(double val)
	{
		num_hp -= val;
		if (num_hp < 0)
			num_hp = 0;

		//׼����Ч
		static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::Instance()->GetSoundPool();
		//���һ����������ѭ��
		Mix_PlayChannel(-1,sound_pool.find(ResID::Sound_HomeHurt)->second,0);


	}


private:
	HomeManager(){
		num_hp = ConfigManager::Instance()->num_initial_hp;
	}
		

private:
	double num_hp = 0;
};





#endif // 

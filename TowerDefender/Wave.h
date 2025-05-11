#ifndef __WAVE_H__
#define __WAVE_H__

#include "EnemyType.h"
#include <vector>
struct Wave
{
	struct SpawnEvent
	{
		double interval= 0.0;
		int spawn_point = 1;
		EEnemyType enemy_type = EEnemyType::Slim;
	};
	double rewards = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;
};


#endif // 

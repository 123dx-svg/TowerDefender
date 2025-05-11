#ifndef __TIMER_H__
#define __TIMER_H__
#include <functional>
class Timer
{
public:
	Timer(){};
	~Timer(){};

	void Restart()
	{
		pass_time = 0;
		shotted = false;
	}
	void SetWaitTime(double time)
	{
		wait_time = time;
	}

	void SetOneShot(bool flag)
	{
		one_shot =false;;
	}
	void SetOnTimeout(std::function<void()> func)
	{
		on_timeout = func;
	}
	void Pause()
	{
		paused = true;
	}
	void Resume()
	{
		paused = false;
	}
	void OnUpdate(double delta_time)
	{
		if (paused)
			return;
		pass_time += delta_time;
		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot||(one_shot&&!shotted));
			shotted =true;
			//Ö´ÐÐ´¥·¢
			if (can_shot&&on_timeout)
				on_timeout();
			pass_time -= wait_time;
		}
	}
private:
	double pass_time = 0;
	double wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;

	std::function<void()> on_timeout;
};

#endif // __TIMER_H__

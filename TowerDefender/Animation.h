#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Timer.h"
#include <functional>
#include <vector>
#include <SDL.h>
class Animation
{
public:
    typedef std::function<void()> PlayCallback;

public:
	Animation(){
	
		timer.SetOneShot(false);
		timer.SetOnTimeout(
			[&]() {
				idx_frame++;
				//是否到达动画最后一帧
				if (idx_frame>= rect_src_list.size())
				{
					idx_frame =is_loop?0:rect_src_list.size()-1;
					if (!is_loop&&on_finshed)
						on_finshed();
				}
			});
	};
	~Animation(){};

	void Reset()
	{
		timer.Restart();
		idx_frame = 0;
	}
	// idx_list判断行进方向图片索引
	void SetFrameData(SDL_Texture* texture, int num_h, int num_v,std::vector<int>& idx_list)
	{
		int width_tex ,height_tex;

		this->texture = texture;
		SDL_QueryTexture(texture, nullptr, nullptr, &width_tex, &height_tex);
		width_frame = width_tex / num_h;
		height_frame = height_tex/num_v;

		rect_src_list.resize(idx_list.size());
		for (int i = 0; i < idx_list.size(); i++)
		{
			int x = (idx_list[i] % num_h) * width_frame;
			int y = (idx_list[i] / num_h) * height_frame;
			rect_src_list[i] = { x,y,width_frame,height_frame };
		}	

	}

	void SetLoop(bool bLoop)
	{
		is_loop = bLoop;
	}
	void SetInterval(double interval)
	{
		timer.SetWaitTime(interval);
	}
	void SetOnFinished(PlayCallback callback)
	{
		on_finshed = callback;
	}
	void OnUpdate(double delta)
	{
		timer.OnUpdate(delta);
	}
	void OnRender(SDL_Renderer* renderer, const SDL_Point& pos_dst, double angle = 0) const
	{
		static SDL_Rect rect_dst;

		rect_dst.x = pos_dst.x,rect_dst.y= pos_dst.y;
		rect_dst.w = width_frame,rect_dst.h = height_frame;
		SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_FLIP_NONE);
	}
private:
	Timer timer;
	bool is_loop = true;
	size_t idx_frame = 0;
	PlayCallback on_finshed;
	SDL_Texture* texture = nullptr;
	//源矩形
	std::vector<SDL_Rect> rect_src_list;
	int width_frame = 0,height_frame = 0;
};

#endif // __ANIMATION_H__	




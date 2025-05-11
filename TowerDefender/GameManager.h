#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__
#include "Manager.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"
#include <SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
class GameManager : public Manager<GameManager>
{

	// 添加友元声明让父类可以访问受保护构造函数
	friend class Manager<GameManager>;

public:
	//获取游戏退出状态
	int Run(int argc, char** argv)
	{
		while (!isExit)
		{
			Uint64 last_counter = SDL_GetPerformanceCounter();
			const Uint64 frequency = SDL_GetPerformanceFrequency();

			//处理事件
			while (SDL_PollEvent(&event))
				OnInput();


			//控制帧率60
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / (double)frequency;
			last_counter=current_counter;
			if (delta*1000<1000.0/60.0)
				SDL_Delay((Uint32)(1000.0/60-delta*1000));
			//处理数据
			OnUpdate(delta);

			//渲染背景颜色
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//清空窗口
			SDL_RenderClear(renderer);

			//处理渲染
			OnRender();
			
			//刷新窗口
			SDL_RenderPresent(renderer);
		}
		return 0;
	}


protected:
	GameManager(){
		
		InitAssert(!SDL_Init(SDL_INIT_EVERYTHING),u8"SDL 初始化失败");
		InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG),u8"IMG 初始化失败");
		InitAssert(Mix_Init(MIX_INIT_MP3), u8"Mix 初始化失败");
		InitAssert(!TTF_Init(), u8"TTF 初始化失败");

		//主流音频解码方式
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		//设置IME输入法 启用显示中文能出词汇
		SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");

		//加载配置文件
		ConfigManager* config = ConfigManager::Instance();
		InitAssert(config->map.Load("map.csv"), u8"加载地图配置文件失败");
		InitAssert(config->LoadLevelConfig("level.json"), u8"加载关卡配置文件失败");
		InitAssert(config->LoaGameConfig("config.json"), u8"加载游戏配置文件失败");

		//窗口标题
		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config->basic_template.window_width,
		config->basic_template.window_height, SDL_WINDOW_SHOWN);
		InitAssert(window, u8"创建窗口失败");

		//渲染器 分别启用硬件加速、垂直同步、渲染到纹理(瓦片纹理地图)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
		InitAssert(renderer, u8"创建渲染器失败");

		
		//加载资源
		InitAssert(ResourcesManager::Instance()->LoadFromFile(renderer), u8"加载游戏资源失败");

		//加载整张地图
		InitAssert(GenerateTileMapTexture(), u8"生成瓦片地图纹理失败");
	}

public:
	~GameManager(){
	
		//释放资源倒序
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}


private:
	//事件
	SDL_Event event;
	//是否退出
	bool isExit = false;

	//弹窗
	SDL_Window* window = nullptr;
	//渲染器
	SDL_Renderer* renderer = nullptr;

	//瓦片地图纹理
	SDL_Texture* tex_tile_map = nullptr;

private:
	//断言函数
	void InitAssert(bool flag, const char* err_message)
	{
		if(flag) return;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏初始化失败", err_message, window);
		exit(-1);
	}
	
	void OnInput()
	{
		//处理输入
	}
	void OnUpdate(double delta)
	{
		//处理数据
	}
	void OnRender()
	{
		//渲染
		static ConfigManager* config = ConfigManager::Instance();
		static SDL_Rect& rect_dst = config->rect_tile_map;
		SDL_RenderCopy(renderer,tex_tile_map,nullptr,&rect_dst);
	}

	//生成瓦片处理成一张地图纹理
	bool GenerateTileMapTexture()
	{
		const Map& map = ConfigManager::Instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::Instance()->rect_tile_map;
		//注意这里用find
		SDL_Texture* tex_tile_set = ResourcesManager::Instance()->GetTexturePool().find(ResID::Tex_Tileset)->second;

		//素材图片
		int width_tex_tile_set , height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set,nullptr,nullptr, &width_tex_tile_set,&height_tex_tile_set);
		//整张图除以单个瓦片大小算出一行有多少个向上取整
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set/SIZE_TILE);

		//整张地图的大小
		int width_tile_map, height_tile_map;
		width_tile_map = (int)map.get_width()*SIZE_TILE;
		height_tile_map = (int)map.get_height()*SIZE_TILE;
		tex_tile_map = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,width_tile_map,height_tile_map);
		if(!tex_tile_map)return false;

		//渲染大小与位置
		ConfigManager* config = ConfigManager::Instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tile_map) / 2;
		rect_tile_map.w = width_tile_map;
		rect_tile_map.h = height_tile_map;

		//渲染混合模式
		SDL_SetTextureBlendMode(tex_tile_map,SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer,tex_tile_map);

		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
			//源
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];
			//渲染到目标
				const SDL_Rect& rect_dst =
				{
					x*SIZE_TILE,
					y*SIZE_TILE,
					SIZE_TILE,
					SIZE_TILE
				};
				rect_src=
				{
				//取模运算求所在列数
					(tile.Terrian%num_tile_single_line)*SIZE_TILE,
				//整除求所在行数
					(tile.Terrian/num_tile_single_line)*SIZE_TILE,
					SIZE_TILE,
					SIZE_TILE
				};
				SDL_RenderCopy(renderer,tex_tile_set,&rect_src,&rect_dst);
				if (tile.Decoration>=0)
				{
					rect_src =
					{
						//取模运算求所在列数
							(tile.Decoration % num_tile_single_line) * SIZE_TILE,
							//整除求所在行数
								(tile.Decoration / num_tile_single_line) * SIZE_TILE,
								SIZE_TILE,
								SIZE_TILE
					};
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		//绘制基地
		const SDL_Point& idx_home = map.get_index_home();
		const SDL_Rect rect_dst=
		{
			idx_home.x*SIZE_TILE,idx_home.y*SIZE_TILE,
			SIZE_TILE,SIZE_TILE
		};
		//房屋不需要裁剪
		SDL_RenderCopy(renderer, ResourcesManager::Instance()->GetTexturePool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);
		return true;
	}
};
		
#endif // __GAME_MANAGER_H__

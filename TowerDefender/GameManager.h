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

	// �����Ԫ�����ø�����Է����ܱ������캯��
	friend class Manager<GameManager>;

public:
	//��ȡ��Ϸ�˳�״̬
	int Run(int argc, char** argv)
	{
		while (!isExit)
		{
			Uint64 last_counter = SDL_GetPerformanceCounter();
			const Uint64 frequency = SDL_GetPerformanceFrequency();

			//�����¼�
			while (SDL_PollEvent(&event))
				OnInput();


			//����֡��60
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / (double)frequency;
			last_counter=current_counter;
			if (delta*1000<1000.0/60.0)
				SDL_Delay((Uint32)(1000.0/60-delta*1000));
			//��������
			OnUpdate(delta);

			//��Ⱦ������ɫ
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//��մ���
			SDL_RenderClear(renderer);

			//������Ⱦ
			OnRender();
			
			//ˢ�´���
			SDL_RenderPresent(renderer);
		}
		return 0;
	}


protected:
	GameManager(){
		
		InitAssert(!SDL_Init(SDL_INIT_EVERYTHING),u8"SDL ��ʼ��ʧ��");
		InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG),u8"IMG ��ʼ��ʧ��");
		InitAssert(Mix_Init(MIX_INIT_MP3), u8"Mix ��ʼ��ʧ��");
		InitAssert(!TTF_Init(), u8"TTF ��ʼ��ʧ��");

		//������Ƶ���뷽ʽ
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		//����IME���뷨 ������ʾ�����ܳ��ʻ�
		SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");

		//���������ļ�
		ConfigManager* config = ConfigManager::Instance();
		InitAssert(config->map.Load("map.csv"), u8"���ص�ͼ�����ļ�ʧ��");
		InitAssert(config->LoadLevelConfig("level.json"), u8"���عؿ������ļ�ʧ��");
		InitAssert(config->LoaGameConfig("config.json"), u8"������Ϸ�����ļ�ʧ��");

		//���ڱ���
		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config->basic_template.window_width,
		config->basic_template.window_height, SDL_WINDOW_SHOWN);
		InitAssert(window, u8"��������ʧ��");

		//��Ⱦ�� �ֱ�����Ӳ�����١���ֱͬ������Ⱦ������(��Ƭ�����ͼ)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
		InitAssert(renderer, u8"������Ⱦ��ʧ��");

		
		//������Դ
		InitAssert(ResourcesManager::Instance()->LoadFromFile(renderer), u8"������Ϸ��Դʧ��");

		//�������ŵ�ͼ
		InitAssert(GenerateTileMapTexture(), u8"������Ƭ��ͼ����ʧ��");
	}

public:
	~GameManager(){
	
		//�ͷ���Դ����
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}


private:
	//�¼�
	SDL_Event event;
	//�Ƿ��˳�
	bool isExit = false;

	//����
	SDL_Window* window = nullptr;
	//��Ⱦ��
	SDL_Renderer* renderer = nullptr;

	//��Ƭ��ͼ����
	SDL_Texture* tex_tile_map = nullptr;

private:
	//���Ժ���
	void InitAssert(bool flag, const char* err_message)
	{
		if(flag) return;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��Ϸ��ʼ��ʧ��", err_message, window);
		exit(-1);
	}
	
	void OnInput()
	{
		//��������
	}
	void OnUpdate(double delta)
	{
		//��������
	}
	void OnRender()
	{
		//��Ⱦ
		static ConfigManager* config = ConfigManager::Instance();
		static SDL_Rect& rect_dst = config->rect_tile_map;
		SDL_RenderCopy(renderer,tex_tile_map,nullptr,&rect_dst);
	}

	//������Ƭ�����һ�ŵ�ͼ����
	bool GenerateTileMapTexture()
	{
		const Map& map = ConfigManager::Instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::Instance()->rect_tile_map;
		//ע��������find
		SDL_Texture* tex_tile_set = ResourcesManager::Instance()->GetTexturePool().find(ResID::Tex_Tileset)->second;

		//�ز�ͼƬ
		int width_tex_tile_set , height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set,nullptr,nullptr, &width_tex_tile_set,&height_tex_tile_set);
		//����ͼ���Ե�����Ƭ��С���һ���ж��ٸ�����ȡ��
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set/SIZE_TILE);

		//���ŵ�ͼ�Ĵ�С
		int width_tile_map, height_tile_map;
		width_tile_map = (int)map.get_width()*SIZE_TILE;
		height_tile_map = (int)map.get_height()*SIZE_TILE;
		tex_tile_map = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,width_tile_map,height_tile_map);
		if(!tex_tile_map)return false;

		//��Ⱦ��С��λ��
		ConfigManager* config = ConfigManager::Instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tile_map) / 2;
		rect_tile_map.w = width_tile_map;
		rect_tile_map.h = height_tile_map;

		//��Ⱦ���ģʽ
		SDL_SetTextureBlendMode(tex_tile_map,SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer,tex_tile_map);

		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
			//Դ
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];
			//��Ⱦ��Ŀ��
				const SDL_Rect& rect_dst =
				{
					x*SIZE_TILE,
					y*SIZE_TILE,
					SIZE_TILE,
					SIZE_TILE
				};
				rect_src=
				{
				//ȡģ��������������
					(tile.Terrian%num_tile_single_line)*SIZE_TILE,
				//��������������
					(tile.Terrian/num_tile_single_line)*SIZE_TILE,
					SIZE_TILE,
					SIZE_TILE
				};
				SDL_RenderCopy(renderer,tex_tile_set,&rect_src,&rect_dst);
				if (tile.Decoration>=0)
				{
					rect_src =
					{
						//ȡģ��������������
							(tile.Decoration % num_tile_single_line) * SIZE_TILE,
							//��������������
								(tile.Decoration / num_tile_single_line) * SIZE_TILE,
								SIZE_TILE,
								SIZE_TILE
					};
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		//���ƻ���
		const SDL_Point& idx_home = map.get_index_home();
		const SDL_Rect rect_dst=
		{
			idx_home.x*SIZE_TILE,idx_home.y*SIZE_TILE,
			SIZE_TILE,SIZE_TILE
		};
		//���ݲ���Ҫ�ü�
		SDL_RenderCopy(renderer, ResourcesManager::Instance()->GetTexturePool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);
		return true;
	}
};
		
#endif // __GAME_MANAGER_H__

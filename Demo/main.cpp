#define SDL_MAIN_HANDLED

#include<string>
#include<fstream>
#include<sstream>
#include <iostream>

#include <cJSON.h>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL2_gfxPrimitives.h>


void TestJson()
{
    std::ifstream file("test.json");
    if (!file.good())
    {
        std::cout << "file not exist" << std::endl;
        return;
    }
    std::stringstream str_stream;
    str_stream << file.rdbuf();
    file.close();

    cJSON* root = cJSON_Parse(str_stream.str().c_str());

    cJSON* json_name = cJSON_GetObjectItem(root, "name");
    cJSON* json_age = cJSON_GetObjectItem(root, "age");
    cJSON* json_pets = cJSON_GetObjectItem(root, "pets");

    std::cout << "name: " << json_name->valuestring << std::endl;
    std::cout << "age: " << json_age->valueint << std::endl;
    
    //����json����
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, json_pets)
    {
        std::cout << "pet: " << json_item->valuestring << std::endl;
    }
}

void TestCsv()
{
	std::ifstream file("test.csv");
	if (!file.good())
	{
		std::cout << "file not exist" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::string str_grid;
        std::stringstream str_stream(line);
        while (std::getline(str_stream,str_grid,','))
        {
            std::cout << str_grid << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{

   //����json�ļ�
    TestJson();
    //����csv�ļ�
    TestCsv();

    //��ʼ��SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    //ͼƬ��Ⱦ
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    //��Ⱦ��Ƶ
    Mix_Init(MIX_INIT_MP3);
    //�ı���Ⱦ
    TTF_Init();
    //������Ƶ���뷽ʽ
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //�������ھ�����ʾ
    SDL_Window* window = SDL_CreateWindow(u8"����С��Ϸ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    //������Ⱦ��������Ⱦ����ʹ����Ӳ������
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //����ͼƬ
    SDL_Surface* suf_img = IMG_Load("avatar.jpg");
    //��������
    SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

    //��������
    TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
    //������ɫ
    SDL_Color color = { 255,255,255,255 };
    //�����ı�
    SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font,u8"��������",color);
    SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

    //��������
    Mix_Music* music = Mix_LoadMUS("music.mp3");
    //�������ֵ��� �����˳��Զ��ͷ�
    Mix_FadeInMusic(music, -1, 1500);
    //֡��Ҳ���Ƕ�̬�ӳ�
    int fps = 60;
    bool IsQuit = false;

    SDL_Event event;

    SDL_Point pos_cursor = { 0,0 };
    //xy����������ϽǶ��� wh������
    SDL_Rect rect_img,rect_text;

    //��ȡ��ʱ������ ��λΪ��
    Uint64 last_counter = SDL_GetPerformanceCounter();
    //��ȡ��ʱ��Ƶ��
    Uint64 counter_freq = SDL_GetPerformanceFrequency();



    //ͼƬ��С��ֵ
    rect_img.w = suf_img->w;
    rect_img.h = suf_img->h;
    //�ı���С��ֵ
    rect_text.w = suf_text->w;
    rect_text.h = suf_text->h;

    while (!IsQuit)
    {
    //��Ϸ��ѭ��
        //�����¼�:�����ܹ��϶�
        while (SDL_PollEvent(&event))
        {
            //�˳��¼�
			if (event.type==SDL_QUIT)
			{
				IsQuit=true;
			}
            //����ƶ��¼�
            else if (event.type == SDL_MOUSEMOTION)
            {
                pos_cursor.x = event.motion.x;
                pos_cursor.y = event.motion.y;
            }

        }

		   //��ȡ��ǰ����������
		   Uint64 current_counter = SDL_GetPerformanceCounter();
		   //����ʱ���
		   double delta_time = (double)(current_counter - last_counter) / (double)counter_freq;
		   last_counter = current_counter;
		   //����֡�ʼ���CPUռ��
		   if (delta_time*1000<1000.0/ fps)
			   SDL_Delay((Uint32)(1000.0/ fps - delta_time*1000));

    
        //�������� 
	    rect_img.x=pos_cursor.x;
	    rect_img.y=pos_cursor.y;
	    rect_text.x=pos_cursor.x;
	    rect_text.y=pos_cursor.y;

        //��Ⱦ������ɫ
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        //��մ���
        SDL_RenderClear(renderer);


        //��Ⱦ��ͼ �������͵��ĸ�����Ϊnullptr��ʾ��Ⱦ�������� �����ֵ������Ⱦ����
        SDL_RenderCopy(renderer, tex_img, nullptr,&rect_img);
        //����Բ����ɫ
        filledCircleRGBA(renderer,pos_cursor.x,pos_cursor.y,50,255,0,0,125);
        //��������
        SDL_RenderCopy(renderer, tex_text, nullptr,&rect_text);

        //ˢ�´���
        SDL_RenderPresent(renderer);
    }

       
    return 0;
}
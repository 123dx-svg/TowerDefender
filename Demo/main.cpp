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
    
    //遍历json数组
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

   //解析json文件
    TestJson();
    //解析csv文件
    TestCsv();

    //初始化SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    //图片渲染
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    //渲染音频
    Mix_Init(MIX_INIT_MP3);
    //文本渲染
    TTF_Init();
    //主流音频解码方式
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //创建窗口居中显示
    SDL_Window* window = SDL_CreateWindow(u8"塔防小游戏", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    //创建渲染器用于渲染这里使用了硬件加速
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //加载图片
    SDL_Surface* suf_img = IMG_Load("avatar.jpg");
    //创建纹理
    SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

    //加载字体
    TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
    //创建颜色
    SDL_Color color = { 255,255,255,255 };
    //加载文本
    SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font,u8"塔防世界",color);
    SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

    //加载音乐
    Mix_Music* music = Mix_LoadMUS("music.mp3");
    //播放音乐淡入 程序退出自动释放
    Mix_FadeInMusic(music, -1, 1500);
    //帧率也就是动态延迟
    int fps = 60;
    bool IsQuit = false;

    SDL_Event event;

    SDL_Point pos_cursor = { 0,0 };
    //xy代表矩形左上角顶点 wh代表宽高
    SDL_Rect rect_img,rect_text;

    //获取计时器总数 单位为秒
    Uint64 last_counter = SDL_GetPerformanceCounter();
    //获取计时器频率
    Uint64 counter_freq = SDL_GetPerformanceFrequency();



    //图片大小赋值
    rect_img.w = suf_img->w;
    rect_img.h = suf_img->h;
    //文本大小赋值
    rect_text.w = suf_text->w;
    rect_text.h = suf_text->h;

    while (!IsQuit)
    {
    //游戏主循环
        //处理事件:窗口能够拖动
        while (SDL_PollEvent(&event))
        {
            //退出事件
			if (event.type==SDL_QUIT)
			{
				IsQuit=true;
			}
            //鼠标移动事件
            else if (event.type == SDL_MOUSEMOTION)
            {
                pos_cursor.x = event.motion.x;
                pos_cursor.y = event.motion.y;
            }

        }

		   //获取当前计数器计数
		   Uint64 current_counter = SDL_GetPerformanceCounter();
		   //计算时间差
		   double delta_time = (double)(current_counter - last_counter) / (double)counter_freq;
		   last_counter = current_counter;
		   //控制帧率减少CPU占用
		   if (delta_time*1000<1000.0/ fps)
			   SDL_Delay((Uint32)(1000.0/ fps - delta_time*1000));

    
        //处理数据 
	    rect_img.x=pos_cursor.x;
	    rect_img.y=pos_cursor.y;
	    rect_text.x=pos_cursor.x;
	    rect_text.y=pos_cursor.y;

        //渲染背景颜色
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        //清空窗口
        SDL_RenderClear(renderer);


        //渲染绘图 第三个和第四个参数为nullptr表示渲染整个窗口 如果传值就是渲染部分
        SDL_RenderCopy(renderer, tex_img, nullptr,&rect_img);
        //绘制圆淡红色
        filledCircleRGBA(renderer,pos_cursor.x,pos_cursor.y,50,255,0,0,125);
        //绘制文字
        SDL_RenderCopy(renderer, tex_text, nullptr,&rect_text);

        //刷新窗口
        SDL_RenderPresent(renderer);
    }

       
    return 0;
}
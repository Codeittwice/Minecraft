#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <winuser.h>

using namespace std;

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 64;
const int INV_WIDTH = 32;
const int INV_HEIGHT = 32;

const char border_ch = 'x';
const char steve_ch = 'O';
const char tree_ch = '%';
const char stone_ch = '#';
const char zombie_ch = 'Z';
const char wooden_plank_ch = 178;
const char cobblestone_ch = 177;
int br4;
int br5;
int br6;
int steve_r;
int steve_c;
//int zombie_r;
//int zombie_c;
//char buff[32];
//char buff2[32];


const int MAX_TREES = 20;
const int MAX_STONES = 60;
const int MAX_TREE_HEALTH = 3;
const int MAX_STONE_HEALTH = 5;
const int MAX_ZOMBIES = 5;
const int MAX_ZOMBIE_HEALTH = 8;

char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

int health_map[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

const int pole_rows = SCREEN_HEIGHT;
const int pole_cols = SCREEN_WIDTH+INV_WIDTH;


HANDLE hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
COORD screen_buf = {pole_cols, pole_rows};
CHAR_INFO blank_screen[(pole_rows) * (pole_cols)] = {0};

enum COLORS
{
    BLACK = 0,
    BLUE = FOREGROUND_BLUE,
    CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
    PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    LIGHT_GREY =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,

    GREY = 0 | FOREGROUND_INTENSITY,
    LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    WHITE =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};


void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color)
{
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = {1, 1};
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {x, y, x+1, y+1};
    ::WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);

}

void draw_str(char* str, int r, int c, COLORS foreground_color, COLORS background_color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        draw_char(str[i], r, c+i, foreground_color, background_color);
    }

}
void clear_screen()
{
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {0, 0, screen_buf.X, screen_buf.Y};
    ::WriteConsoleOutput(hConsoleOutput, blank_screen, screen_buf, buf_coord, &screen_pos);
}

void set_background()
{
    for (int i = 0; i < screen_buf.X; i++)
    {
        for (int j = 0; j < screen_buf.Y; j++)
        {
            draw_char(' ', j, i, WHITE, GREEN);
        }
    }

}


void init_CONSOLE()
{

    SMALL_RECT rect = {0, 0, screen_buf.X, screen_buf.Y};

    ::SetConsoleScreenBufferSize(hConsoleOutput, screen_buf);
    ::SetConsoleWindowInfo(hConsoleOutput, TRUE, &rect);
    ::SetConsoleTitle("MINECRAFT...");
    set_background();

}


void print()
{
    system("cls");
    for(int r = 0; r < SCREEN_HEIGHT; r++)
    {
        for(int c = 0; c < SCREEN_WIDTH; c++)
        {
            cout << screen[r][c];
        }
        cout << endl;
    }

}
void move_unstatic_objects(const int max_count2, const char ch2, const int health2)    //da go napravq za mnogo zombita
{
    for(int r = 0; r < SCREEN_HEIGHT; r++)
    {
        for(int c = 0; c < SCREEN_WIDTH; c++)
        {
            if(screen[r][c]==zombie_ch)
            {



                int br3=0,n_r = r;
                int n_c = c;
                br3=rand()%4;
                if(br3==0)
                {
                    n_c++;
                }
                else
                {
                    if(br3==1)
                    {
                        n_c--;
                    }
                    else
                    {
                        if(br3==2)
                        {
                            n_r--;
                        }
                        else
                        {
                            if(br3==3)
                            {
                                n_r++;
                            }
                        }
                    }
                }
                if(screen[n_r][n_c] == '\0')
                {
                    screen[n_r][n_c] = zombie_ch;
                    draw_char(zombie_ch, n_r, n_c, GREEN, BLUE);
                    screen[r][c] = '\0';
                    draw_char('\0',r, c, WHITE, GREEN);
                    r = n_r;
                    c = n_c;
                }
                else if (health_map[n_r][n_c] > 0)
                {
                    health_map[n_r][n_c]--;
                    if(health_map[n_r][n_c] == 0)
                    {
                        screen[n_r][n_c] = '\0';
                    }
                }
            }
        }
    }
}
void add_static_blocks(const int max_count, const char ch, const int health)
{
    int br = 0;
    int r, c;
    while(br < max_count)
    {
        r = rand() % SCREEN_HEIGHT;
        c = rand() % SCREEN_WIDTH;
        if(screen[r][c] == '\0')
        {
            screen[r][c] = ch;
            draw_char(ch, r, c,RED, GREY);
            health_map[r][c] = health;
            br++;
        }
    }
}

void  init()
{
    for(int r = 0; r < SCREEN_HEIGHT; r++)
    {
        screen[r][0] = border_ch;
        draw_char(border_ch, r, 0,PURPLE, BLACK);
        health_map[r][0] = -1;

        screen[r][SCREEN_WIDTH - 1] = border_ch;
        draw_char(border_ch, r, SCREEN_WIDTH - 1,PURPLE, BLACK);
        health_map[r][SCREEN_WIDTH - 1] = -1;

    }
    for(int c = 0; c < SCREEN_WIDTH; c++)
    {
        screen[0][c] = border_ch;
        draw_char(border_ch, 0, c,PURPLE, BLACK);
        health_map[0][c] = -1;

        screen[SCREEN_HEIGHT - 1][c] = border_ch;
        draw_char(border_ch, SCREEN_HEIGHT - 1, c,PURPLE, BLACK);
        health_map[SCREEN_HEIGHT - 1][c] = -1;
    }

    for(int r = SCREEN_HEIGHT; r < SCREEN_HEIGHT; r++)
    {
        screen[r][0] = border_ch;
        draw_char(border_ch, r, 0,PURPLE, BLACK);
        health_map[r][0] = -1;

        screen[r][SCREEN_WIDTH - 1] = border_ch;
        draw_char(border_ch, r, SCREEN_WIDTH - 1,PURPLE, BLACK);
        health_map[r][SCREEN_WIDTH - 1] = -1;

    }
    for(int c = 0; c < SCREEN_WIDTH; c++)
    {
        screen[0][c] = border_ch;
        draw_char(border_ch, 0, c,PURPLE, BLACK);
        health_map[0][c] = -1;

        screen[SCREEN_HEIGHT - 1][c] = border_ch;
        draw_char(border_ch, SCREEN_HEIGHT - 1, c,PURPLE, BLACK);
        health_map[SCREEN_HEIGHT - 1][c] = -1;
    }




    for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH +INV_WIDTH; i++)
    {
        draw_char(border_ch, 0, i, PURPLE, BLACK);
        draw_char(border_ch, SCREEN_HEIGHT-1, i, PURPLE, BLACK);
    }
    for (int j = 0; j < SCREEN_HEIGHT -1; j++)
    {
        //draw_char(border_ch, 0, SCREEN_WIDTH +INV_WIDTH, PURPLE, BLACK);
        draw_char(border_ch, j, SCREEN_WIDTH +INV_WIDTH, PURPLE, BLACK);
    }

    srand(time(NULL));

    int r, c;
    do
    {
        r = rand() % SCREEN_HEIGHT;
        c = rand() % SCREEN_WIDTH;

        if(screen[r][c] == '\0')
        {
            screen[r][c] = steve_ch;
            draw_char(steve_ch, r, c,BLACK,BROWN);
            steve_r = r;
            steve_c = c;
        }
    }
    while(screen[r][c] != steve_ch);

    /*    do{
        r = rand() % SCREEN_HEIGHT;
        c = rand() % SCREEN_WIDTH;

        if(screen[r][c] == '\0'){
            screen[r][c] = zombie_ch;
            zombie_r = r;
            zombie_c = c;
            }
        }while(screen[r][c] != zombie_ch);
    */
    /* do{
      r = rand() % SCREEN_HEIGHT;
      c = rand() % SCREEN_WIDTH;

      if(screen[r][c] == '\0'){
          screen[r][c] = zombie_ch;
          zombie_r = r;
          zombie_c = c;
          }
      }while(screen[r][c] != zombie_ch);*/
    add_static_blocks(MAX_TREES, tree_ch, MAX_TREE_HEALTH);
    add_static_blocks(MAX_STONES, stone_ch, MAX_STONE_HEALTH);
    add_static_blocks(MAX_ZOMBIES, zombie_ch, MAX_ZOMBIE_HEALTH);

}



void move()
{
    int n_r = steve_r;
    int n_c = steve_c;


    if(GetAsyncKeyState('D'))
    {
        n_c++;
    }
    if(GetAsyncKeyState('A'))
    {
        n_c--;
    }
    if(GetAsyncKeyState('W'))
    {
        n_r--;
    }
    if(GetAsyncKeyState('S'))
    {
        n_r++;
    }

    if(screen[n_r][n_c] == '\0')
    {
        screen[n_r][n_c] = steve_ch;
        draw_char(steve_ch, n_r, n_c,BLACK, BROWN);
        screen[steve_r][steve_c] = '\0';
        draw_char('\0', steve_r,steve_c,BLACK, GREEN);
        steve_r = n_r;
        steve_c = n_c;
    }
    else if (health_map[n_r][n_c] > 0)
    {
        health_map[n_r][n_c]--;

        if(health_map[n_r][n_c] == 0)
        {


            if(screen[n_r][n_c]==tree_ch)
            {
                br4++;

                char buff[32];
                itoa(br4,buff,10);
                draw_str(buff, 4, SCREEN_WIDTH+9, WHITE, GREEN);
//building();
            }
            else
            {
                if(screen[n_r][n_c]==stone_ch)
                {

               br5++;
                   char buff2[32];
                    itoa(br5,buff2,10);
                    draw_str(buff2, 6, SCREEN_WIDTH+10, WHITE, GREEN);
                     //   building() ;
                }
            }
            screen[n_r][n_c] = '\0';


        }
        /*do{

        if(screen[n_r-1][n_c]=='\0') {
        if(GetAsyncKeyState('F')){
            //   char buff[32];
                    //    itoa(br5,buff,10);
        br4--;
                        draw_char(178, (n_r)-1, n_c, BLACK, BROWN );
                        }
                    }
        }while(br4>0);*/
    }
//building();
}


void building()
{
    int n_r = steve_r;
    int n_c = steve_c;
    if(br4>=1)
    {
        if(screen[n_r-1][n_c]=='\0')
        {
            if(GetAsyncKeyState('F'))
            {
                //   char buff[32];
                //    itoa(br5,buff,10);
                br4--;
                screen[n_r-1][n_c]==wooden_plank_ch;
                draw_char(wooden_plank_ch, (n_r)-1, n_c, BLACK, BROWN );
                char buff3[32];
                itoa(br4,buff3,10);
                    draw_str(buff3, 4 , SCREEN_WIDTH+9, WHITE, GREEN);
                     //   building() ;
                //draw_str(buff, 4, SCREEN_WIDTH+9, WHITE, GREEN);
                //draw_str(buff, 4, SCREEN_WIDTH+9, WHITE, GREEN);
            }else{
            move();
            }
        }

    }


}
void building2()
{
    int n_r = steve_r;
    int n_c = steve_c;
    if(br5>=1)
    {
        if(screen[n_r-1][n_c]=='\0')
        {
            if(GetAsyncKeyState('E'))
            {
                //   char buff[32];
                //    itoa(br5,buff,10);
                br5--;
                screen[n_r-1][n_c]==wooden_plank_ch;
                draw_char(cobblestone_ch, (n_r)-1, n_c, BLACK, GREY );
                char buff3[32];
                itoa(br5,buff3,10);
                    draw_str(buff3, 6 , SCREEN_WIDTH+10, WHITE, GREEN);
                     //   building() ;
                //draw_str(buff, 4, SCREEN_WIDTH+9, WHITE, GREEN);
                //draw_str(buff, 4, SCREEN_WIDTH+9, WHITE, GREEN);
            }else{
            move();
            }
        }

    }


}
int main()
{
    bool end_game = false;
    init_CONSOLE();
    init();

    draw_str("INVENTORY:", 2, SCREEN_WIDTH+2, WHITE, GREEN);
    draw_str("WOOD:", 4, SCREEN_WIDTH+4, WHITE, GREEN);
    draw_str("STONE:", 6, SCREEN_WIDTH+4, WHITE, GREEN);
    while(!end_game)
    {
        if (GetAsyncKeyState('F')){
            building();
            move_unstatic_objects(MAX_ZOMBIES, zombie_ch, MAX_ZOMBIE_HEALTH);
        }else{
            if (GetAsyncKeyState('E')){
            building2();
            move_unstatic_objects(MAX_ZOMBIES, zombie_ch, MAX_ZOMBIE_HEALTH);
        }else{
        move();
         //building();
        move_unstatic_objects(MAX_ZOMBIES, zombie_ch, MAX_ZOMBIE_HEALTH);
        }}
        //print();
        Sleep(200);
    }

    return 0;
}

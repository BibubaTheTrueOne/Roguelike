#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

char map[30][120];

int y;
int x;

int atc = 1;
int hp = 40;
int max_hp = 40;
int turn = 0;

bool G_placed;
bool P_placed;
bool T_placed;
bool H_placed;
bool AE_placed;
bool Sword_placed;

int r_placed;

int gold = 0;
int dlvl = 1;
int AE_count = 0;

struct monsters{

    int y;
    int x;
    int lvl;
    int type;
    bool awake;
};


struct monsters monster[10];

class MapGenerator{

public:

    int cols;
    int rows;

    int dungeon_gen(int rows, int cols){


    if (!r_placed)
    {
        int ry, rx; // room coords
        int r_size_y, r_size_x; // room size
        int r_center_y, r_center_x;
        int r_old_center_y, r_old_center_x;
        int room_num = rand() % 8    + 5;

        bool collisian;
        //Отрисовка карты и ее границ
        for (int wall = 0; wall <= rows; wall++)
        {
            for (int floor = 0; floor <= cols; floor++)
            {
                //границы
                if (wall == 0 || wall == rows - 1 || wall == 1 ||
                    floor == 0 || floor == cols)
                    map[wall][floor] = '%';
                // стены
                else
                    map[wall][floor] = '#';
            }
        }
        while(r_placed < room_num)
        {
            int try_counter = 0;

            do
            {
                collisian = 0;

                ry = rand() % (rows - 3) + 1;
                rx = rand() % (cols - 3) + 1;

                r_size_y = rand() % 7 + 6;
                r_size_x = rand() % 7    + 6;

                try_counter++;

                if(try_counter > 100)
                {
                    ry = 3;
                    rx = 3;
                    r_size_y = 3;
                    r_size_x = 3;
                    break;
                }

                for(int wall = ry; wall <= ry + r_size_y; wall++)
                {

                    for(int floor = rx; floor <= rx + r_size_x; floor++)
                    {
                        if (map[wall][floor] == '%' || map[wall][floor] == ' ' ||
                            map[wall-2][floor] == ' ' || map[wall][floor-2] == ' ' ||
                            map[wall+2][floor] == ' ' || map[wall][floor+2] == ' ')
                        {
                            collisian = 1;
                            wall = ry + r_size_y + 1;
                            break;
                        }
                    }
                }
            }
            while (collisian == 1);

            for(int wall = ry; wall <= ry + r_size_y; wall++)
            {
                for(int floor = rx; floor <= rx + r_size_x; floor++)
                {
                    if (map[wall][floor] == '%')
                    {
                        wall = ry + r_size_y + 1;
                        break;
                    }
                    else
                        map[wall][floor] = ' ';
                    }
            }
            r_placed++;

            if(r_placed > 1)
            {
                r_old_center_y = r_center_y;
                r_old_center_x = r_center_x;
            }

            r_center_y = ry + (r_size_y / 2);
            r_center_x = rx + (r_size_x / 2);

            if(r_placed > 1)
            {
                int path_y;

                for (path_y = r_old_center_y; path_y != r_center_y; )
                {
                    if(map[path_y][r_old_center_x != '%'])
                        map[path_y][r_old_center_x] = ' ';

                    if(r_old_center_y < r_center_y)
                        path_y++;
                    else if(r_old_center_y > r_center_y)
                        path_y--;
                }
                for (int path_x = r_old_center_x; path_x != r_center_x; )
                {
                    if (map[path_y][path_x] != '%')
                        map[path_y][path_x] = ' ';

                    if (r_old_center_x < r_center_x)
                        path_x++;
                    else if (r_old_center_x > r_center_x)
                        path_x--;

                }
            }
        }
        int Sy, Sx;
        do
        {
            Sy = rand() % (rows - 3) + 1;
            Sx = rand() % (cols - 3) + 1;
        }
        while(map[Sy][Sx] != ' ');
        map[Sy][Sx] = '>';

    }
    return 0;
}

};
class ItemsGetting
{
public:

    int items_getting(int rows, int cols)
    {
        if(map[y][x] == 'H')
        {
            hp += 1000*dlvl;
            if(hp > max_hp)
                hp = max_hp;
            map[y][x] = ' ';
            H_placed = 0;
        }
        if(map[y][x] == 'S')
        {
            atc *=2;
            map[y][x] = ' ';
            Sword_placed = 1;
        }
        if(map[y][x] == '0')
        {
            map[y][x] = ' ';
            AE_count++;
            AE_placed = 1;
        }

    }
};
class TrapDmg
{
public:
    int trap_dmg(int rows, int cols)
    {
        if (map[y][x] == '^')
            hp -= rand() % dlvl + 2;
        return 0;
    }
};
class Intro{
public:

    int intro_ui(void)
    {
        int c = 0;

        printw("\n");
        attron(A_BOLD);
        printw("\t\t\t\t\t\t\ Roguelike");
        attroff(A_BOLD);

        printw("\n\n\n\t\t\t\t\t\t\How to play:\n\n"
        "\tYou can use arrows(key_up....) and wasd(also WASD) to move and attack\n"
        "\tAt the levels you can find heals and buffs\n"
        "\tTo get to the next level find the stairs and press shift and >\n"
        "\tPress 'n' to start a new game.\n"
        "\tPress 'ESC' to quit the game\n\n"
        "\t\t\t\t\t\t\Legend:\n\n"
        "\t1)ABCDE.... - mobs. Depending on the letter it has different HP and attack\n"
        "\t2)H - Heal\n"
        "\t3)S - Sword. It gives you an attack buff for the rest of the run.\n"
        "\t4)^ - Traps. If you step on them you or the mobs will take damage\n");


        c = getch();
        clear();

        return 0;
    }
};
class MonstersTurn
{
public:
    int monster_turn(int rows, int cols)
    {
        int dist_y;
        int dist_x;

        for(int m = 0; m < 15; m++)
        {
            if(monster[m].lvl < 1)
                continue;

            dist_y = abs(monster[m].y - y);
            dist_x = abs(monster[m].x - x);

            if(dist_y < 5 && dist_x < 5)
                monster[m].awake = 1;

            if(monster[m].awake == 0)
                continue;

            int dir_y = monster[m].y;
            int dir_x = monster[m].x;

            //Ходьба монстров

            if(dist_y > dist_x)
            {
                //Пряолинейно
                if(dir_y < y)
                    dir_y++;

                else
                    dir_y--;
            }
            else
            {
                if(dir_x < x)
                    dir_x++;

                else
                    dir_x--;
            }
            if(map[dir_y][dir_x] == '#' || map[dir_y][dir_x] == '%')
            {
                //Диагональ
                dir_y = monster[m].y;
                dir_x = monster[m].x;

                if(dir_y < y)
                    dir_y++;

                else
                    dir_y--;

                if(dir_x < x)
                    dir_x++;

                else
                    dir_x--;
            }
            if(map[dir_y][dir_x] == '#' || map[dir_y][dir_x] == '%')
            {
                dir_y = monster[m].y;
                dir_x = monster[m].x;
                //Пряолинейно
                if(dist_y > dist_x)
                    if(dir_y > y)
                        dir_y++;

                    else
                        dir_y--;

                else

                    if(dir_x > y)
                        dir_x++;

                    else
                        dir_x--;
            }

            if(dist_y < 2 && dist_x < 2)
            {
               hp -= (rand() % 2) + monster[m].type - 64;
            }
            else if(map[dir_y][dir_x] == ' ')
            {
                map[monster[m].y][monster[m].x] = ' ';
                monster[m].y = dir_y;
                monster[m].x = dir_x;
                map[monster[m].y][monster[m].x] = 'G';
            }
            else if(map[dir_y][dir_x] == '^')
            {
                monster[m].lvl -=30;
                if(monster[m].lvl < 0)
                    map[monster[m].y][monster[m].x] = ' ';
            }
        }
    }
};


class Battle
{
public:

    int rows;
    int cols;
    int dir_y;
    int dir_x;

    int battle(int rows, int cols, int dir_y, int dir_x)
    {
        int gold_per_monster = 0;

        for(int m = 0; m < 15; m++)
        {
            if(dir_y == monster[m].y && dir_x == monster[m].x)
            {
                if (monster[m].lvl <= 0)
                {
                    map[dir_y][dir_x] = ' ';
                    gold_per_monster = rand()% 3 * monster[m].type * dlvl;
                    gold += gold_per_monster;
                }
                else
                    monster[m].lvl -=atc;
                break;
            }
        }
        return 0;
    }
};

class Action{
public:
    int action(int c, int rows, int cols)
    {
        Battle Battle;
        int dir_y = y;
        int dir_x = x;

        if(c == KEY_UP || c == 'w' || c == 'W')
        {
            dir_y--;
            if(map[dir_y-1][dir_x] != '#' || map[dir_y][dir_x] != '%')
                turn++;
        }
        else if (c == KEY_DOWN || c == 's' || c == 'S')
        {
            dir_y++;
            if(map[dir_y+1][dir_x] != '#' || map[dir_y][dir_x] != '%')
                turn++;
        }
        else if (c == KEY_LEFT || c == 'a' || c == 'A')
        {
            dir_x--;
            if(map[dir_y][dir_x-1] != '#' || map[dir_y][dir_x] != '%')
                turn++;
        }
        else if (c == KEY_RIGHT || c == 'd'  || c == 'D')
        {
            dir_x++;
            if(map[dir_y][dir_x+1] != '#' || map[dir_y][dir_x] != '%')
                turn++;
        }
        else if (c == '>' && map[y][x] == '>')
        {
            G_placed = 0;
            P_placed = 0;
            r_placed = 0;
            H_placed = 0;
            AE_placed = 0;
            Sword_placed = 0;
            atc += rand() % 3 + 1;
            if(hp >= max_hp)
            {
                hp+=4;
                max_hp+=40;
            }
            else
            {
                hp += rand() % 20 + 10;
                max_hp += 20;
            }
            return 1;
        }

        if(map[dir_y][dir_x] == ' ' || map[dir_y][dir_x] == '>' || map[dir_y][dir_x] == '^' ||
            map[dir_y][dir_x] == 'H' || map[dir_y][dir_x] == 'S' || map[dir_y][dir_x] == '0')
        {
            y = dir_y;
            x = dir_x;
        }

        else if(map[dir_y][dir_x] == 'G')
            Battle.battle(rows, cols, dir_y, dir_x);
        return 0;
    }
};
class MapDraw{
public:
    int rows;
    int cols;
    int dungeon_draw(int rows, int cols)
    {


    for (int wall = 0; wall <= rows; wall++)
    {
        for (int floor = 0; floor <= cols; floor++)
        {
        if (wall == 0 || wall == rows)
            mvaddch(wall, floor,' ');
        else if (map[wall][floor] == 'H' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
        {
            attron(A_BOLD | COLOR_PAIR(GREEN));
                mvaddch(wall, floor, 'H');
            attroff(A_BOLD | COLOR_PAIR(GREEN));
        }
        else if (map[wall][floor] == 'S'&& ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
        {
            attron(A_BOLD | COLOR_PAIR(GREEN));
                mvaddch(wall, floor, 'S');
            attroff(A_BOLD | COLOR_PAIR(GREEN));
        }
        else if (map[wall][floor] == '>' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
        {
            attron(A_BOLD);
                mvaddch(wall, floor, '>');
            attroff(A_BOLD);
        }
        else if (map[wall][floor] == '^' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
        {
            attron(A_BOLD | COLOR_PAIR(YELLOW));
                mvaddch(wall, floor, '^');
            attroff(A_BOLD | COLOR_PAIR(YELLOW));
        }
        else if (map[wall][floor] == '0' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
        {
            attron(A_BOLD | COLOR_PAIR(BLUE));
                mvaddch(wall, floor, '0');
            attroff(A_BOLD | COLOR_PAIR(BLUE));
        }
        else if (map[wall][floor] == '%')
          {
            attroff(A_BOLD | COLOR_PAIR(YELLOW));
                mvaddch(wall, floor,'%');
            attroff(A_BOLD | COLOR_PAIR(YELLOW));
          }
        else if (map[wall][floor] == ' ')

            mvaddch(wall, floor,' ');
        else if (map[wall][floor] == '#' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5)))
            mvaddch(wall, floor, '#');
        else if (map[wall][floor] == 'G' && ((wall > y - 5 && floor > x - 5) &&
                    (wall < y + 5 && floor < x + 5) && (wall != '#' & floor != '#')))
        {
           for(int m = 0; m < 10; m++)
            {
                if(monster[m].y == wall && monster[m].x == floor)
                {
                    if(monster[m].lvl < dlvl / 2 + 2)
                    {
                        attron(COLOR_PAIR(RED));
                            mvaddch(wall, floor, monster[m].type);
                    }
                    else if(monster[m].lvl < dlvl + 2)
                    {
                        attron(COLOR_PAIR(YELLOW));
                            mvaddch(wall, floor, monster[m].type);
                    }
                    else
                        mvaddch(wall, floor, monster[m].type);
                }
                standend();
            }
        }
        }
    }
    mvprintw(0 , 0, "Turn Number: %d \t\t\t\t\t\ RogueLike!", turn);
    mvprintw(29, 0, "Gold: %d \t HP: %d  / %d \t Att: %d \t lvl: %d \t Aegis: %d", gold, hp, max_hp, atc, dlvl - 1, AE_count);

    return 0;
    }
};

class Spawn{

public:
    int rows;
    int cols;

    int respawn(int rows, int cols)
    {
         if(!G_placed)
        {
            int Gy;
            int Gx;

            for(int m = 0; m < 10; m++)
            {
                do
                {
                    Gy = rand() % rows;
                    Gx = rand() % cols;

                }
                while(map[Gy][Gx] != ' ');

                monster[m].y = Gy;
                monster[m].x = Gx;
                monster[m].lvl = rand() % dlvl + 2;

                if (map[monster[m].y][monster[m].x] == '^')
                    monster[m].lvl -=10;
                if (dlvl == 1 && !(rand() % 4))
                    monster[m].lvl = 1;
                if (rand() % 3)
                    monster[m].lvl = dlvl + 2;
                monster[m].type = rand() % dlvl +65;

                monster[m].awake = 0;
                map[monster[m].y][monster[m].x] = 'G';
            }
            G_placed = 1;
        }

         if(!T_placed)
        {
            int Ty;
            int Tx;

            if(turn % 10 == 0 && turn != 0 && map[y][x+1] != '#' &&
                map[y][x-1] != '#' && map[y-1][x] != '#' &&
                map[y+1][x] != '#')
            {
                for(int m = 0; m < 2; m++)
                {
                    do
                    {
                        Ty = rand() % rows;
                        Tx = rand() % cols;

                    }
                    while(map[Ty][Tx] != ' ');
                    map[Ty][Tx] = '^';
                }
            }
        }

        if (!P_placed)
        {
            do
            {
                y = rand() % rows;
                x= rand() % cols;

            }
            while(map[y][x] == '#' || map[y][x] == '%');
            P_placed = 1;
        }
        if (!AE_placed)
        {
            int AEy;
            int AEx;
            do
            {
                AEy = rand() % rows;
                AEx = rand() % cols;

            }
            while(map[AEy][AEx] == '#' || map[AEy][AEx] == '%');
            map[AEy][AEx] = '0';
            if(rand() % 1000)
                AE_placed = 1;
        }
        if(!H_placed)
        {
            int Hy, Hx;

            do
            {
                Hy = rand() % rows;
                Hx = rand() % cols;

            }
            while(map[Hy][Hx] == '#' || map[Hy][Hx] == '%');
            map[Hy][Hx] = 'H';
            H_placed = 1;

        }

        if(!Sword_placed)
        {
            int Sy, Sx;

            do
            {
                Sy = rand() % rows;
                Sx = rand() % cols;

            }
            while(map[Sy][Sx] == '#' || map[Sy][Sx] == '%');
            map[Sy][Sx] = 'S';
            Sword_placed = 1;
        }
        return 0;
    }
};

class GameLoop{
public:
    int game_loop(int c, int rows, int cols)
    {
        int new_lvl;


        Spawn Spawn;

        TrapDmg TrapDmg;

        Action Action;

        MonstersTurn MonstersTurn;

        MapGenerator MapGenerator;

        ItemsGetting ItemsGetting;
        MapDraw MapDraw;

        srand(time(NULL));

        MapGenerator.dungeon_gen(rows, cols);

        Spawn.respawn(rows, cols);


        if(c !=0)
        {
          new_lvl = Action.action(c, rows, cols);
        }

        MonstersTurn.monster_turn(rows, cols);
        TrapDmg.trap_dmg(rows, cols);
        ItemsGetting.items_getting(rows, cols);

        MapDraw.dungeon_draw(rows, cols);

        attron(A_BOLD);
            mvaddch(y,x, '@');
        attroff(A_BOLD);

        if (new_lvl)
        {
            clear();
            mvprintw(15, 37, "Welcome to level: %d. Press Any Key to Continue", dlvl++);
            if (dlvl  > 2)
            {
                gold += 1000*dlvl;
                mvprintw(16, 37, "You received %d for moving to a new lvl", 1000*dlvl);
            }
            getch();
            clear();



        }
        if(hp <= 1 && AE_count > 0)
        {
            hp += 30 + dlvl;
            AE_count--;
        }
        if(hp < 1 && AE_count == 0)
        {
            clear();
            mvprintw(15, 50, "YOU DIED");
            mvprintw(16, 47, "You had %d gold", gold);
            mvprintw(17, 42, "You've been living %d moves", turn);
            printw("\n\n\t\t\t\t\t\t\ Try again!");
        }


        c = getch();

        return c;
    }
};
int main()
{
    int c;

    int cols;
    int rows;

    GameLoop GameLoop;
    Intro Intro;

	initscr();
	start_color();
    use_default_colors();

	init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);

	keypad(stdscr, 1);
	noecho();
	curs_set(0);

	getmaxyx(stdscr, rows, cols);

    c = Intro.intro_ui();

    if (c == 27) // 27 == 'ESC'
    {
        endwin();
        return 0;
    }

    while (1)
    {
        c = GameLoop.game_loop(c, rows - 1, cols - 1);
        if (c == 27)
        {
            endwin();
            return 0;
        }
    }
}

#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

//#define width 20            //map settings
//#define heigh 20            //      20*20 - default

//#define startXPos 10        //snake settings
//#define startYPos 10        //      x=10,y=10 - default
#define body char(248)      //
#define head '0'            //
#define dieHead '*'         //
#define startLen 4          //

#define LEFT_DOWN 187       //snake body settings
#define LEFT_UP 188         //
#define RIGHT_UP 200        //
#define RIGHT_DOWN 201      //
#define BODY_VERTICAL 186   //
#define BODY_HORISONTAL 205 //
#define BEAUTY_HEAD 2       //

#define UP_KEY 119           //movement settings
#define DOWN_KEY 115         //
#define LEFT_KEY 97          //
#define RIGHT_KEY 100        //
#define PAUSE_KEY 32         //
#define SELECT_KEY 32        //

                            //game settings
//#define DIFFICULTY 5        //1 - very slow, 2 - easy, 5 - medium, 10 - hard
#define APPLE '*'           //

#define leftArrow '<'
#define rightArrow '>'

#define SETTINGS_COUNT 5

#define MENU_COUNT 4
#define LOGO_SIZE 10

using namespace std;

string logo[LOGO_SIZE] = {"############",
                          "#          #",
                          "# #        #",
                          "# # ###    #",
                          "# # # #    #",
                          "# ### #    #",
                          "#     #    #",
                          "#     ## * #",
                          "#          #",
                          "############"};

int DIFFICULTY = 5, width = 20, heigh = 20, startXPos = 10, startYPos = 10;

vector < vector < char > > pole(0);
vector < pair < int, int > > snake(0);
vector < pair < int, int > > eatedApples(0);

int selectedSetting = 0;
bool settingsExit = 0;

int selectedMenu = 0;

bool exitGame = 0;

int xDir = 1, yDir = 0;
int appleX, appleY;
int score = 0, best;

char setBody (int x, int y, int pos){
    if(snake[pos+1].first == x-1 && snake[pos-1].second == y-1) return LEFT_UP;
    if(snake[pos-1].first == x-1 && snake[pos+1].second == y-1) return LEFT_UP;

    if(snake[pos+1].first == x-1 && snake[pos-1].second == y+1) return LEFT_DOWN;
    if(snake[pos-1].first == x-1 && snake[pos+1].second == y+1) return LEFT_DOWN;

    if(snake[pos+1].first == x+1 && snake[pos-1].second == y+1) return RIGHT_DOWN;
    if(snake[pos-1].first == x+1 && snake[pos+1].second == y+1) return RIGHT_DOWN;

    if(snake[pos+1].first == x+1 && snake[pos-1].second == y-1) return RIGHT_UP;
    if(snake[pos-1].first == x+1 && snake[pos+1].second == y-1) return RIGHT_UP;

    if(snake[pos+1].second == y+1 && snake[pos-1].second == y-1) return BODY_VERTICAL;
    if(snake[pos-1].second == y+1 && snake[pos+1].second == y-1) return BODY_VERTICAL;

    if(snake[pos+1].first == x+1 && snake[pos-1].first == x-1) return BODY_HORISONTAL;
    if(snake[pos-1].first == x+1 && snake[pos+1].first == x-1) return BODY_HORISONTAL;
}

void render (int isDie){
    for (int i = 0; i < width; ++i){
        for(int j = 0; j < heigh; ++j){
            pole[i][j] = ' ';
        }
    }

    pole[appleX][appleY] = APPLE;

    for(int i = 0; i < width; ++i){
        pole[i][0] = '#';
        pole[i][heigh-1] = '#';
    }
    for (int i = 0; i < heigh; ++i){
        pole[0][i] = '#';
        pole[width-1][i] = '#';
    }

    for (int i = 0; i < snake.size() - 1; ++i){
        pole[snake[i].first][snake[i].second] = setBody(snake[i].first, snake[i].second, i);
        //pole[snake[i].first][snake[i].second] = body;
    }
    if (isDie) pole[snake[snake.size() - 1].first][snake[snake.size() - 1].second] = dieHead;
    //else pole[snake[snake.size() - 1].first][snake[snake.size() - 1].second] = head;
    else pole[snake[snake.size() - 1].first][snake[snake.size() - 1].second] = BEAUTY_HEAD;

    system ("cls");
    for(int i = 0; i < heigh; ++i){
        for(int j = 0; j < width; ++j){
            cout<<pole[j][i];
        }
        if(i == 0){
            cout<<"   score: "<<score;
        }
        if(i == 1){
            if(best > score) cout<<"   best: "<<best;
            else cout<<"   best: "<<score;
        }
        cout<<endl;
    }
}

bool inSnake (int x, int y){
    for(int i = 0; i < snake.size(); ++i){
        if(snake[i].first == x && snake[i].second == y){
            return 1;
        }
    }
    return 0;
}

void makeApple (){
    appleX = rand() % (width-2)+1;
    appleY = rand() % (heigh-2)+1;
    while(inSnake(appleX, appleY)){
        appleX = rand() % (width-2)+1;
        appleY = rand() % (heigh-2)+1;
    }
}

bool makeLonger (){
    if(eatedApples.size() > 0){
        if(eatedApples[0].first == snake[0].first && eatedApples[0].second == snake[0].second){
            eatedApples.erase(eatedApples.begin());
            score++;
            return 1;
        }
    }
    return 0;
}

void event (){
    if(!makeLonger()){
        snake.erase(snake.begin());
    }
    snake.push_back(make_pair(snake[snake.size()-1].first + xDir, snake[snake.size()-1].second + yDir));

    if(snake[snake.size()-1].first == appleX && snake[snake.size()-1].second == appleY){
        eatedApples.push_back(make_pair(appleX, appleY));
        makeApple();
    }
}

bool die(){
    if (snake[snake.size()-1].first == 0 || snake[snake.size()-1].first == width-1 || snake[snake.size()-1].second == 0 || snake[snake.size()-1].second == heigh-1) return 1;

    for (int i = 0; i < snake.size() - 1; ++i){
        if (snake[snake.size()-1].first == snake[i].first && snake[snake.size()-1].second == snake[i].second) return 1;
    }
    return 0;
}

void movement (){
    if (kbhit()){
        int dir;
        dir = getch();

        if (dir == UP_KEY && yDir != 1){
            xDir = 0;
            yDir = -1;
        }
        if (dir == DOWN_KEY && yDir != -1){
            xDir = 0;
            yDir = 1;
        }
        if (dir == LEFT_KEY && xDir != 1){
            xDir = -1;
            yDir = 0;
        }
        if (dir == RIGHT_KEY && xDir != -1){
            xDir = 1;
            yDir = 0;
        }
        if(dir == PAUSE_KEY){
            Sleep(1000);
            system("pause");
        }
    }
}

void setNewBest (){
    if(score > best){
        ofstream setBest("Scores.txt");
        setBest<<score;
    }
}

void main_game_loop (){
    int isDie;
    makeApple();
    while (true) {
        movement();
        event();
        isDie = die();
        render(isDie);
        if(isDie) return;
        setNewBest();
        Sleep(200 / DIFFICULTY);
    }
}

void settings_render (){
    system("cls");
    cout<<"Settings:"<<endl<<endl;

    if(selectedSetting == 0) cout<<"    ";
    cout<<"Difficulty: "<<leftArrow<<" "<<DIFFICULTY<<" "<<rightArrow<<endl;

    if(selectedSetting == 1) cout<<"    ";
    cout<<"Map width: "<<leftArrow<<" "<<width<<" "<<rightArrow<<endl;

    if(selectedSetting == 2) cout<<"    ";
    cout<<"Map heigh: "<<leftArrow<<" "<<heigh<<" "<<rightArrow<<endl;

    if(selectedSetting == 3) cout<<"    ";
    cout<<"Start x: "<<leftArrow<<" "<<startXPos<<" "<<rightArrow<<endl;

    if(selectedSetting == 4) cout<<"    ";
    cout<<"Start y: "<<leftArrow<<" "<<startYPos<<" "<<rightArrow<<endl;

    cout<<endl<<"Press *space* to start game ...";
}

void change_setting(int type){
    switch (selectedSetting){
        case 0: if(!(DIFFICULTY == 1 && type == -1)) DIFFICULTY += type; break;
        case 1: if(!(width == 1 && type == -1)) width += type; break;
        case 2: if(!(heigh == 1 && type == -1)) heigh += type; break;
        case 3: if(!(startXPos == 1 && type == -1)) startXPos += type; break;
        case 4: if(!(startYPos == 1 && type == -1)) startYPos += type; break;
    }
}

void settings_navigation (){
    int button;
    while (!kbhit()){}
    button = getch();
    if(button == UP_KEY){
        if(selectedSetting == 0) selectedSetting = SETTINGS_COUNT - 1;
        else selectedSetting--;
    }
    if(button == DOWN_KEY){
        if(selectedSetting == SETTINGS_COUNT-1) selectedSetting = 0;
        else selectedSetting++;
    }
    if(button == LEFT_KEY){
        change_setting(-1);
    }
    if(button == RIGHT_KEY){
        change_setting(1);
    }
    if(button == SELECT_KEY){
        settingsExit = 1;
    }
}

void settings_menu (){
    selectedSetting = 0;
    settingsExit = 0;

    settings_render();
    while(!settingsExit){
        settings_navigation();
        settings_render();
    }
}

void restart (){
    xDir = 1;
    yDir = 0;
    score = 0;
    selectedSetting = 0;
    settingsExit = 0;
    selectedMenu = 0;


    vector < vector < char > > pole_temp(0);
    vector < pair < int, int > > snake_temp(0);
    vector < pair < int, int > > eatedApples_temp(0);
    pole = pole_temp;
    snake = snake_temp;
    eatedApples = eatedApples_temp;


    vector < char > temp(heigh, ' ');
    for(int i = 0; i < width; ++i){     //if set width, error "out of range"
        pole.push_back(temp);
    }

    for (int i = 0; i < startLen; ++i){
        snake.push_back(make_pair(startXPos + i, startYPos));
    }

    ifstream getBest("Scores.txt");
    getBest>>best;
}

void scores_window (){
    system ("cls");
    cout<<"best: "<<best;

    int key;
    while(!kbhit()){}
    key = getch();
    if(key == SELECT_KEY) return;

}

void render_menu (){
    system("cls");
    for(int i = 0; i < LOGO_SIZE; ++i){
        cout<<"    "<<logo[i]<<endl;
    }
    cout<<endl<<endl;

    if(selectedMenu == 0) cout<<"   > ";
    else cout<<"    ";
    cout<<"start"<<endl;

    if(selectedMenu == 1) cout<<"   > ";
    else cout<<"    ";
    cout<<"settings"<<endl;

    if(selectedMenu == 2) cout<<"   > ";
    else cout<<"    ";
    cout<<"scores"<<endl;

    if(selectedMenu == 3) cout<<"    > ";
    else cout<<"    ";
    cout<<"exit"<<endl;
}

bool menu_navigation (){
    int key;
    while(!kbhit()){}
    key = getch();

    if(key == UP_KEY){
        if (selectedMenu == 0) selectedMenu = MENU_COUNT-1;
        else selectedMenu--;
    }
    if(key == DOWN_KEY){
        if(selectedMenu == MENU_COUNT-1) selectedMenu = 0;
        else selectedMenu++;
    }
    if(key == SELECT_KEY){
        return 1;
    }
    return 0;
}

void main_menu (){
    bool select;
    render_menu();
    while (true){
        select = menu_navigation();
        if(select){
            if(selectedMenu == 0) return;
            if(selectedMenu == 1) settings_menu();
            if(selectedMenu == 2) scores_window();
            if(selectedMenu == 3){
                exitGame = 1;
                return;
            }
        }
        render_menu();
    }
}

int main()
{
    restart ();

    main_menu();

    if(exitGame) return 0;

    srand(time(NULL));
    //cout<<"Please select correct language on your keyboard.."<<endl;
    //system("pause");

    main_game_loop();

    Sleep(1500);
    system("cls");
    cout<<"score: "<<score<<endl;
    if(best > score) cout<<"best: "<<best<<endl;
    else cout<<"best: "<<score<<endl;
    Sleep(1500);
    system("pause");

    main();
    return 0;
}


#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <windows.h>
/*
개선내용:
개선 내용
땅에 랜드 시 속도가 계속 변화하는 버그 수정
Up 을 누를시에만 burst 스프라이트가 나타남으로서 자연스럽게 수정

지면에 착지시 land 성공을 하고 success 문구가 뜨고 엔터를 누를시 재시작 할수있음 end 상태에서는 연료감소 x
속도가 10을 넘어갈시 land 실패를 하고 fail 문구가 뜨고 엔터를 누를시 재시작 할수있음

점수 기능 추가
점수 계산 방식 = [100 - (100 - 남은연료)] x 10

사운드 추가(반복재생)
*/
using namespace sf;
using namespace std;

class LunarLander {
private:
    double x, y;
    double velocity;
    double fuel;
    string status;
    Texture t1, t2;
    Sprite spaceship, burst;
    Font font;
    Text text;
    Text result1;
    Text result2;
    Text result3;
public:
    int endGame = 0;
    int score = 0;
    LunarLander(double h, double v, double f); //생성자 
    bool checkLanded(); //착륙 검사
    void update(double rate); // 상태 업데이트
    void draw(RenderWindow& window); // 착륙선 그리기
    void fail(); //게임 실패
    void reset();//게임 리셋
    void success();//게임 성공
    double gety()
    {
        return y;
    }
    double getVelocity() {
        return velocity;
    }
};
LunarLander::LunarLander(double h, double v, double f)
{
    x = 300;
    y = h;
    velocity = v;
    fuel = f;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/burst.png");
    spaceship.setTexture(t1);
    burst.setTexture(t2);
    spaceship.setPosition(x, y);
    burst.setPosition(x + 20, y + 50);
    if (!font.loadFromFile("OpenSans-Bold.ttf")) {
        cout << "폰트 파일을 오픈할 수 없음!" << endl;
    }
    text.setFont(font);
    result1.setFont(font);
    result2.setFont(font);
    result3.setFont(font);
}

bool LunarLander::checkLanded() {
    if (y >= 435)
        return true;
    return false;
}
void LunarLander::update(double amount) {
    if (fuel <= 0) { // 기름 0이하로 되면 0이하로 떨어지지않고 소모되지도않음
        fuel = 0;
        amount = 0;
    }
    if (checkLanded() == false && endGame==0)// 착륙전에는 기름이 떨어지고 착륙시에는 연료가 소모되지않게 방지
    {
        fuel = fuel - amount;
        velocity = velocity - amount + 0.8; // 속도는 현재속도 에서 기름값만큼 떨어지고(= 우주선출력) 계속 0.8씩 더한다.
    }
    else if(checkLanded() == true&&endGame == 0)
    {
        endGame = 1;
        success();
    }
    if (endGame == 1||endGame ==2)//성공과 실패시 우주선 정지
        velocity = 0;
    y = y + velocity; //우주선의 위치는 현재위치 + 속도 만큼 이동됨(중력)
    spaceship.setPosition(x, y);
    burst.setPosition(x+8, y + 40);
    status = "Press up key!\nheight: " + to_string(y) + "\nspeed: " + to_string(velocity) + "\nfuel: " + to_string(fuel);
}
void LunarLander::draw(RenderWindow &window){
    window.draw(spaceship);
    if(Keyboard::isKeyPressed(Keyboard::Up))
        window.draw(burst);
    switch (endGame)
    {
    case 0:
    {
        text.setString(status);
        text.setCharacterSize(20);
        text.setPosition(10, 100);
        text.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(text);
        break;
    }
    case 1://success
    {
        result1.setString("SUCESS");
        result1.setCharacterSize(50);
        result1.setPosition(220, 200);
        result1.setFillColor(sf::Color(255, 255, 255, 200));
        result2.setString("RESET: ENTER");
        result2.setCharacterSize(50);
        result2.setPosition(150, 260);
        result2.setFillColor(sf::Color(255, 255, 255, 200));
        result3.setString("SCORE: " + to_string(score));
        result3.setCharacterSize(50);
        result3.setPosition(150, 320);
        result3.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(result1);
        window.draw(result2);
        window.draw(result3);
        break;
    }
    case 2://fail
        result1.setString("FAIL");
        result1.setCharacterSize(50);
        result1.setPosition(260, 200);
        result1.setFillColor(sf::Color(255, 255, 255, 200));
        result2.setString("RESET: ENTER");
        result2.setCharacterSize(50);
        result2.setPosition(150, 260);
        result2.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(result1);
        window.draw(result2);
        break;
    }

}
void LunarLander::fail(){
}
void LunarLander::success() {
    score = (100. - (100 - fuel)) * 100;
}
void LunarLander::reset() {
    x = 300;
    y = 150;
    fuel = 100;
    velocity = 0;
    endGame = 0;
}

int main()
{
    //윈도우 생성
    RenderWindow window(VideoMode(600, 480), "LUNAR LANDER");
    window.setFramerateLimit(60);

    //스프라이트 생성
    Texture t;
    Sprite background;
    t.loadFromFile("images/background.jpg");
    background.setTexture(t);
    //사용자 이벤트처리

    LunarLander lander(150.0, 1.0, 100.0);
    SoundBuffer buffer;
    if (!buffer.loadFromFile("sounds/land.wav"))
        return -1;
    Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(true);//사운드 반복재생
    while (window.isOpen())
    {
        //이벤트 검사 및 처리
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))   //윗방향키를 누를시 연료를 소모함
            lander.update(2.0);
        else
            lander.update(0.0);
        if ((lander.endGame==1||lander.endGame==2)&&Keyboard::isKeyPressed(Keyboard::Enter)) //성공이나 실패시 엔터를 누르면 게임을 리셋
            lander.reset();
         //화면을지운다
        window.clear();
        window.draw(background);
        if (lander.getVelocity() >= 10.0) // 속도 10.0을 넘을시 게임종료
        {
            lander.endGame = 2;
            //lander.fail();
        }
        lander.draw(window);
       
        window.display();
        Sleep(75);
    }   

    return 0;
}

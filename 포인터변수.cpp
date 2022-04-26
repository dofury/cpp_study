#include <iostream>
#include<time.h>
#include<windows.h>
using namespace std;

class Circle {
public:
	int x, y, radius;
	string color;
	void draw();
};

void Circle::draw()
{
	HDC hdc = GetWindowDC(GetForegroundWindow());
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}
int main()
{
	int input;
	cout << "숫자를 입력하시오: ";
	cin >> input;
	for (int i = 0; i < input; i++)
	{
		unique_ptr<Circle> ob(new Circle);
		ob->x = rand() % 500;
		ob->y = rand() % 500;
		ob->radius = rand() % 100;
		ob->draw();
	}
	return 0;
}

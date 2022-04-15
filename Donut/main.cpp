
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <windows.h>

void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	float A = 1;
	float B = 0;
	float* light = new float[3];
	light[0] = 0.f;
	light[1] = 1.f;
	light[2] = -1.f;

	char* symbols = new char[13];
	symbols[0] = ' ';
	symbols[1] = '.';
	symbols[2] = ',';
	symbols[3] = '-';
	symbols[4] = '~';
	symbols[5] = ':';
	symbols[6] = ';';
	symbols[7] = '=';
	symbols[8] = '!';
	symbols[9] = '*';
	symbols[10] = '#';
	symbols[11] = '$';
	symbols[12] = '@';

	system("cls");

	while (true)
	{
		int circleD = 40;
		int donutD = 100;

		float k1 = 5;
		float k2 = 9;
		float r1 = 1.;
		float r2 = 3.0f;
		
		int d = circleD * donutD;
		float** donut = new float* [d];
		
		float ASin = sin(A);
		float ACos = cos(A);
		float BSin = sin(B);
		float BCos = cos(B);

		for (int i = 0; i < donutD; ++i)
		{
			float phi = (M_PI * 2) / donutD * i;
			float phiSin = sin(phi);
			float phiCos = cos(phi);

			for (int j = 0; j < circleD; ++j)
			{
				float theta = (M_PI * 2) / circleD * j;
				float thetaSin = sin(theta);
				float thetaCos = cos(theta);

				donut[i * circleD + j] = new float[4];
				/*donut[i * circleD + j][0] = phiCos * (r1 * thetaCos + r2);
				donut[i * circleD + j][1] = r1 * thetaSin;
				donut[i * circleD + j][2] = -phiSin * (r1 * thetaCos + r2);*/

				donut[i * circleD + j][0] = phiCos * (r1 * thetaCos + r2);
				donut[i * circleD + j][1] = (ACos * r1 * thetaSin) + (ASin * phiSin * (r1 * thetaCos + r2));
				donut[i * circleD + j][2] = (ASin * r1 * thetaSin) - (ACos * phiSin * (r1 * thetaCos + r2));
				
				donut[i * circleD + j][0] = (r2 + r1 * thetaCos) * (BCos * phiCos + ASin * BSin * phiSin) - r1 * ACos * BSin * thetaSin;


				float* n = new float[3];
				n[0] = thetaCos * phiCos;
				n[1] = thetaSin * ACos + ASin * thetaCos * phiSin;
				n[2] = thetaSin * ASin - ACos * thetaCos * phiSin;

				float* l = new float[3];
				l[0] = n[1] * light[2] - n[2] * light[1];
				l[1] = n[2] * light[0] - n[0] * light[2];
				l[2] = n[0] * light[1] - n[1] * light[0];

				donut[i * circleD + j][3] = sqrt(l[0] * l[0] + l[1] * l[1] + l[2] * l[2]);
			}
		}

		float** mappedDonut = new float* [d];
		for (int i = 0; i < d; ++i)
		{
			mappedDonut[i] = new float[4];
			mappedDonut[i][0] = k1 * donut[i][0] / (k2 + donut[i][2]);
			mappedDonut[i][1] = k1 * donut[i][1] / (k2 + donut[i][2]);
			mappedDonut[i][2] = donut[i][2];
			mappedDonut[i][3] = donut[i][3];
		}

		int screenW = 30;
		int screenH = 30;

		char** screen = new char* [screenH];
		float** zBuffer = new float* [screenH];

		for (int i = 0; i < screenH; ++i)
		{
			screen[i] = new char[screenW];
			zBuffer[i] = new float[screenW];
			for (int j = 0; j < screenW; ++j)
			{
				screen[i][j] = ' ';
				zBuffer[i][j] = 400.f;
			}
		}

		float a = 4;
		for (int i = 0; i < d; ++i)
		{
			int x = round(mappedDonut[i][0] * a) + 10;
			int y = round(mappedDonut[i][1] * a) + 10;
			float z = mappedDonut[i][2];

			if (x >= 0 && x < screenW && y >= 0 && y < screenH)
			{
				if (zBuffer[y][x] > z)
				{
					int l = round(mappedDonut[i][3] * 9.f);
					if (l > 12)
					{
						l = 12;
					}
					screen[y][x] = symbols[l];
					zBuffer[y][x] = z;
				}
			}
		}

		for (int i = 0; i < screenH; ++i)
		{
			for (int j = 0; j < screenW; ++j)
			{
				if(j % 2 == 0)
				printf("%c %c", screen[i][j], screen[i][j]);
				else
				printf("%c", screen[i][j]);
			}
			printf("\n");
		}

		delete[] donut;

		gotoxy(0, 0);

		A += 0.3f;
		Sleep(2);
	}
	return 0;
}

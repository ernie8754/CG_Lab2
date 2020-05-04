#include<gl/GLUT.H>
#include<iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#define PI 3.14159265f

std::fstream file;
bool HaveSquare, HaveTriangle;
struct matrix
{
	std::vector<std::vector<float>> M;
	matrix multi(matrix M2) {
		matrix M3;
		M3.setSize(M.size(), M2.M[0].size());
		for (size_t i = 0; i < M.size(); i++) {
			for (size_t j = 0; j < M2.M[0].size(); j++) {
				M3.M[i][j] = 0;
				for (size_t k = 0; k < M[0].size(); k++) {
					M3.M[i][j] = M3.M[i][j] + M[i][k] * M2.M[k][j];
				}
			}
		}
		return M3;
	}
	void setSize(size_t row, size_t col) {
		M.resize(row);
		for (size_t i = 0; i < M.size(); i++) {
			M[i].resize(col);
		}
	}
};
matrix TM;
std::vector<matrix> squares;
std::vector<matrix> triangles;
//matrix square;
//matrix triangle;
class Point
{
public:
	int x;
	int y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};
std::vector<Point> AllPoints;
std::vector<Point> ShowPoint;
void writeDot(int x, int y) {
	/*glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
	glFlush();*/
	Point temp = Point(x, y);
	AllPoints.push_back(temp);
}
void writeLine(int x1, int y1, int x2, int y2) {
	writeDot(x1, y1);
	writeDot(x2, y2);
	bool swap = false;
	bool isNegX = false;
	bool isNegY = false;
	//std::cout << "x1:" << x1 << std::endl << "y1:" << y1 << std::endl << "x2:" << x2 << std::endl << "y2:" << y2 << std::endl;
	if (abs(x2 - x1) < abs(y2 - y1)) {
		swap = true;
		//std::cout << "swap";
	}
	if (x2 < x1) {
		isNegX = true;
		//std::cout << "NegX";
	}
	if (y2 < y1) {
		isNegY = true;
		//std::cout << "NegY";
	}
	if (isNegX) {
		x2 = -x2;
		x1 = -x1;
	}
	if (isNegY) {
		y2 = -y2;
		y1 = -y1;
	}
	if (swap) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	int x = x1, y = y1;
	int a = abs(y2 - y1);
	int b = -abs(x2 - x1);
	int d = (2 * a + b) / 2;
	//std::cout << "a:" << a << std::endl << "b:" << b << std::endl << "d:" << d << std::endl;
	//std::cout << "----------------" << std::endl;
	while (x < x2)
	{
		//std::cout << d << std::endl;
		if (d <= 0) {
			x++;
			d += a;
		}
		else {
			x++;
			y++;
			d += (a + b);
		}
		int newX = x, newY = y;
		if (swap) {
			std::swap(newX, newY);
		}
		if (isNegX) {
			newX = -newX;
		}
		if (isNegY) {
			newY = -newY;
		}
		//std::cout << "x:" << newX << std::endl << "y:" << newY << std::endl;
		writeDot(newX, newY);
	}
	//std::cout << "----------------" << std::endl;
}
void reset() {
	TM.M={ {1,0,0},{0,1,0},{0,0,1} };
}
void clearData() {
	HaveSquare=false, HaveTriangle=false;
	squares.clear();
	triangles.clear();
}
void Translation(float x, float y) {
	matrix Translate;
	Translate.M = { {1,0,x},{0,1,y},{0,0,1} };
	TM = Translate.multi(TM);
}
matrix Translation(float x, float y, matrix Matrix) {
	matrix Translate;
	Translate.M = { {1,0,x},{0,1,y},{0,0,1} };
	Matrix = Translate.multi(Matrix);
	return Matrix;
}
void Scaling(float x, float y) {
	matrix scale;
	scale.M = { {x,0,0},{0,y,0},{0,0,1} };
	TM = scale.multi(TM);
}
matrix Scaling(float x, float y, matrix Matrix) {
	matrix scale;
	scale.M = { {x,0,0},{0,y,0},{0,0,1} };
	Matrix = scale.multi(Matrix);
	return Matrix;
}
void Rotating(float theta) {
	matrix Rotate;
	Rotate.M = { {cos(theta*PI/180.0f),-sin(theta*PI / 180.0f),0},{sin(theta*PI / 180.0f),cos(theta*PI / 180.0f),0},{0,0,1} };
	float x = TM.M[0][2];
	float y = TM.M[1][2];
	//Translation(-x, -y);
	TM = Rotate.multi(TM);
	//Translation(x, y);
}
matrix Rotating(float theta, matrix Matrix) {
	matrix Rotate;
	Rotate.M = { {cos(theta*PI / 180.0f),-sin(theta*PI / 180.0f),0},{sin(theta*PI / 180.0f),cos(theta*PI / 180.0f),0},{0,0,1} };
	float x = Matrix.M[0][2];
	float y = Matrix.M[1][2];
	//Matrix = Translation(-x, -y, Matrix);
	Matrix = Rotate.multi(Matrix);
	//Matrix = Translation(x, y, Matrix);
	return Matrix;
}
void Square() {
	matrix square;
	square.M = { {1,-1,-1,1},{1,1,-1,-1},{1,1,1,1} };
	square = TM.multi(square);
	squares.push_back(square);
	HaveSquare=true;
}
void Triangle() {
	matrix triangle;
	triangle.M = { {0,-1,1},{1,-1,-1},{1,1,1} };
	HaveTriangle=true;
	triangle = TM.multi(triangle);
	triangles.push_back(triangle);
}
void ViewPort(float wxl, float wxr, float wyb, float wyt, float vxl, float vxr, float vyb, float vyt) {
	writeLine(round(vxl), round(vyb), round(vxl), round(vyt));
	writeLine(round(vxr), round(vyb), round(vxr), round(vyt));
	writeLine(round(vxl), round(vyb), round(vxr), round(vyb));
	writeLine(round(vxl), round(vyt), round(vxr), round(vyt));
	float Sx = (vxr - vxl) / (wxr - wxl);
	float Sy = (vyt - vyb) / (wyt - wyb);
	matrix viewRect;
	viewRect.M = { {wxl, wxr},{wyb, wyt},{1, 1} };
	viewRect = Translation(-wxl, -wyb, viewRect);
	viewRect = Scaling(Sx, Sy, viewRect);
	viewRect = Translation(vxl, vyb, viewRect);
	matrix showM;
	if (HaveTriangle) {
		for (size_t i = 0; i < triangles.size(); i++) {
			showM = Translation(-wxl, -wyb, triangles[i]);
			showM = Scaling(Sx, Sy, showM);
			showM = Translation(vxl, vyb, showM);
			std::cout << "point:" << round(triangles[i].M[0][0]) << " " << round(triangles[i].M[1][0]) << std::endl;
			std::cout << "point2:" << round(triangles[i].M[0][1]) << " " << round(triangles[i].M[1][1]) << std::endl;
			std::cout << "point3:" << round(triangles[i].M[0][2]) << " " << round(triangles[i].M[1][2]) << std::endl;
			writeLine(round(showM.M[0][0]), round(showM.M[1][0]), round(showM.M[0][1]), round(showM.M[1][1]));
			writeLine(round(showM.M[0][1]), round(showM.M[1][1]), round(showM.M[0][2]), round(showM.M[1][2]));
			writeLine(round(showM.M[0][2]), round(showM.M[1][2]), round(showM.M[0][0]), round(showM.M[1][0]));
		}
	}
	if (HaveSquare) {
		for (size_t i = 0; i < squares.size(); i++) {
			showM = Translation(-wxl, -wyb, squares[i]);
			showM = Scaling(Sx, Sy, showM);
			showM = Translation(vxl, vyb, showM);
			std::cout << "point:" << round(squares[i].M[0][0]) << " " << round(squares[i].M[1][0]) << std::endl;
			std::cout << "point2:" << round(squares[i].M[0][1]) << " " << round(squares[i].M[1][1]) << std::endl;
			std::cout << "point3:" << round(squares[i].M[0][2]) << " " << round(squares[i].M[1][2]) << std::endl;
			std::cout << "point4:" << round(squares[i].M[0][3]) << " " << round(squares[i].M[1][3]) << std::endl;
			writeLine(round(showM.M[0][0]), round(showM.M[1][0]), round(showM.M[0][1]), round(showM.M[1][1]));
			writeLine(round(showM.M[0][1]), round(showM.M[1][1]), round(showM.M[0][2]), round(showM.M[1][2]));
			writeLine(round(showM.M[0][0]), round(showM.M[1][0]), round(showM.M[0][3]), round(showM.M[1][3]));
			writeLine(round(showM.M[0][2]), round(showM.M[1][2]), round(showM.M[0][3]), round(showM.M[1][3]));
		}
	}
	for (size_t i=0; i < AllPoints.size(); i++) {
		if (AllPoints[i].x >= round(viewRect.M[0][0]) && AllPoints[i].x <= round(viewRect.M[0][1]) 
			&& AllPoints[i].y >= round(viewRect.M[1][0]) && AllPoints[i].y <= round(viewRect.M[1][1])) {
			glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2d(AllPoints[i].x, AllPoints[i].y);
			glEnd();
			glFlush();
			//std::cout << "point:" << AllPoints[i].x << " " << AllPoints[i].y << std::endl;
			Point temp = Point(AllPoints[i].x, AllPoints[i].y);
			ShowPoint.push_back(temp);
		}
	}
	AllPoints.clear();
	system("PAUSE");
}
void Initial(void)//初始化函数 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//白色背景，前3个是RGB，最后是Alpha值，用来控制透明，1.0表示完全不透明
	glMatrixMode(GL_PROJECTION);//OpenGL按照三维方式来处理图像，所以需要一个投影变换将三维图形投影到显示器的二维空间中
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);//指定使用正投影将一个x坐标在0~200，y坐标0~150范围内的矩形坐标区域投影到显示器窗口
	glClear(GL_COLOR_BUFFER_BIT);
}
void ShwoDis() {
	std::string input;
	file >> input;
	std::cout << input;
	if (input == "end") {
		exit(0);
	}
	else if (input == "#") {
		std::getline(file, input);
		std::cout << input;
	}
	else if (input == "scale") {
		float x, y;
		file >> x >> y;
		Scaling(x, y);
	}
	else if (input == "rotate") {
		float degree;
		file >> degree;
		Rotating(degree);
	}
	else if (input == "translate") {
		float x, y;
		file >> x >> y;
		Translation(x, y);
	}
	else if (input == "reset") {
		reset();
	}
	else if (input == "square") {
		Square();
	}
	else if (input == "triangle") {
		Triangle();
	}
	else if (input == "clearScreen") {
		//glFlush();
		/*for (size_t i = 0; i < ShowPoint.size(); i++) {
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_POINTS);
			glVertex2d(ShowPoint[i].x, ShowPoint[i].y);
			glEnd();
			glFlush();
		}*/
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		ShowPoint.clear();
		AllPoints.clear();
		
	}
	else if (input == "clearData") {
		clearData();
	}
	else if (input == "view") {
		float wxl, wxr, wyb, wyt, vxl, vxr, vyb, vyt;
		file >> wxl >> wxr >> wyb >> wyt >> vxl >> vxr >> vyb >> vyt;
		ViewPort(wxl, wxr, wyb, wyt, vxl, vxr, vyb, vyt);
	}
	std::cout<< std::endl;
}
void myDisplay(void)//显示回调函数
{
	glFlush();//清空OpenGL命令缓冲区，强制执行命令缓冲区中所有OpenGL函数
	//ShwoDis();
	//writeLine(100, 100, 200, 200);
}
int main(int argc, char * argv[])//这是使用glut库函数进行窗口管理
{
	system("pause");
	reset();
	clearData();
	if (argv[1])
		file.open(argv[1], std::ios::in);
	glutInit(&argc, argv);//使用glut库需要进行初始化
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设定窗口显示模式，颜色模型和缓存，这里是RGB颜色模型和单缓存
	glutInitWindowPosition(200, 200);//设定窗口的初始位置，屏幕左上角为原点，单位为像素
	glutInitWindowSize(800, 600);//设定窗口的大小
	glutCreateWindow("My Progect 2");//创建一个窗口，参数是窗口标题名
	glutDisplayFunc(&myDisplay);//将myDisplay指定为当前窗口的显示内容函数
	Initial();
	glutIdleFunc(&ShwoDis);
	//glutMouseFunc(&mouseButton);
	//glutKeyboardFunc(&keyboard);
	//glutMotionFunc(&mouseDrag);
	glutMainLoop();//使窗口框架运行起来，使显示回调函数开始工作
	
	
	return 0;
}
#include<iostream>
#include<string>
#include"Application.h"
int main(){
	const char* name = "RGS";
	int width = 800;
	int height = 600;
	
	//״̬λ��false��ʾչʾ��Ȳ���&��ϣ�true��ʾչʾBlinn-Phong��ɫģ�ͣ�����չʾ
	bool state = false;

	std::string s;
	std::cin >> s;
	while (s != "-1") {
		if (s == "DepthTest") {
			state = false;
			RGS::Application app(name, width, height, state);
			//app.SetDisplayDepthTestAndBlend();
			app.Run();
			std::cout << "The display is over." << "\n";
		}
		else if (s == "Blinn-Phong") {
			state = true;
			RGS::Application app(name, width, height, state);
			//app.SetDisplayBlinnPhongModel();
			app.Run();
			std::cout << "The display is over." << "\n";
		}
		else {
			std::cout << "Input is invalid!" << "\n";
		}
		std:: cin >> s;
	}
	std::cout << "All display is over." << "\n";
}
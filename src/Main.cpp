#include<iostream>
#include<string>
#include"Application.h"
int main(){
	const char* name = "RGS";
	int width = 800;
	int height = 600;
	
	//状态位，false表示展示深度测试&混合，true表示展示Blinn-Phong着色模型，用于展示
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
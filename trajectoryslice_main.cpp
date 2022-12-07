#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Eigen/Dense"
#include"gpscovert.h"
#include"Polynomial_insert.h"
#include <cmath>
#include <iomanip>
#include <iterator>
// #include <ceres/ceres.h>
#include"WGS84GCJ02.h"
#include"trajectory_guidelist.h"
#include"RSU.h"  
#include"Spline.h"


#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h> //11
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "./aip-cpp-sdk-4.15.8/ocr.h"
#include <fcntl.h>	  /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <string>

#define API_SAVE

using namespace std;

using Eigen::MatrixXd;
using Eigen::VectorXf;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::string;
using std::vector;
using namespace SplineSpace;

namespace my_api
{

	aip::Ocr client("23603269", "oi9p6xAUZiKCctGTET0ZXU7n", "0tfYXzYGABWfruRrOG2dp66pNQ79s0SG");
	static int write_string_to_file_append(const std::string &file_string, const std::string str)
	{
		std::ofstream OsWrite(file_string, std::ofstream::app);
		OsWrite << str;
		OsWrite << std::endl;
		OsWrite.close();
		return 0;
	}
	static vector<string> split_string(const string &str, const string &pattern)
	{
		char *strc = new char[strlen(str.c_str()) + 1];
		strcpy(strc, str.c_str()); // string转换成C-string
		vector<string> res;
		char *temp = strtok(strc, pattern.c_str());
		while (temp != NULL)
		{
			res.push_back(string(temp));
			temp = strtok(NULL, pattern.c_str());
		}
		delete[] strc;
		return res;
	}
	//参数说明：分别传入接收数据的指针,起点经纬度和，终点的经纬度)
	void zhixing(char *td, double x1, double y1, double x2, double y2)
	{

		/*
	api：91e0952f96abe5b25a60fd875ebf11ea


	https://restapi.amap.com/v3/direction/driving?key=91e0952f96abe5b25a60fd875ebf11ea&extensions=base&strategy=2&origin=116.481028,39.989643&destination=116.465302,40.004717

	*/

		//读取文件
		//std::cout << "读取文件" << std::endl;
		std::string image;

		//获取api
		//std::cout << "获取api" << std::endl;
		
		Json::Value result = client.license_plate(image,"https://restapi.amap.com/v3/direction/driving?key=91e0952f96abe5b25a60fd875ebf11ea&extensions=base&strategy=2", x1,y1,x2,y2);

		// json解析
		//std::cout << "json解析" << std::endl;
		// std::cout <<"获取到"<< result << std::endl;

		Json::Value result1 = result["route"]["paths"][0]["steps"];
		Json::Value result2 = result1;
		Json::Value result3;

		std::string data;
		for (int i = 0; i < result2.size(); ++i)
		{
			//std::cout << result2[i]["instruction"].asString() << std::endl;
			data.append(result2[i]["instruction"].asString());
			data.append("\n");
			vector<string> res;
			res = split_string(result2[i]["polyline"].asString(), ";");

			for (int i = 0; i < res.size(); i++)
			{
				//std::cout << res[i] << endl;
				data.append(res[i]);
				data.append("\n");
			}
		}
		//结果
		
		strcpy(td,data.c_str());
	}

};





int main() {

    char date[10000] = {0};
	memset(date, 0, sizeof(date));
	my_api::zhixing(date, 116.481080, 39.989643, 116.465302, 40.004717);
	std::cout << "解析成：" << std::endl;
	std::cout << date << std::endl; 









	GpsTo gps2;
 	PolyNomial polynomi;

	string out_file_name_ = "4-output.txt";
	


	/* ========================================== 前处理  =====================================================*/



	/* 1,读取txt文件里的高德坐标点 GCJ-02坐标系 */

	string in_file_name_ = "1.txt";
	ifstream in_file(in_file_name_.c_str(), ifstream::in);
	ofstream out_file(out_file_name_.c_str(), ofstream::out);

	if (!in_file.is_open()) {
		cout << "Cannot open input file: " << in_file_name_ << endl;
	}
    string line;
    int i = 0;

	VectorXd gd_pos (2);
	vector <VectorXd> gd_pos_list;

	while (getline(in_file, line) && (i <= 600)) {


		istringstream iss(line);
		double pos_x;
		iss >> pos_x; // reads first element from the current line

		char c;
		iss >> c;

		double pos_y;
		iss >> pos_y;

		gd_pos << pos_x, pos_y;
	 	gd_pos_list.push_back(gd_pos);

	}

     
	for (i = 0; i < gd_pos_list.size(); i++) {
		cout << fixed << setprecision(6)<< gd_pos_list[i][0]<<"\t"<< gd_pos_list[i][1] << endl;
	}




}

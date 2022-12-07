/*

g++ -std=c++11 ./*.cpp -o ./main  -L /usr/lib/x86_64-linux-gnu     -lcurl -lcrypto       -ljsoncpp  -I /usr/include/jsoncpp/  -L /home/china/libopenssl/ -I /home/china/libcurl/include/

*/
#include "test.h"
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

JNIEXPORT void JNICALL Java_test_fromcpp (JNIEnv *, jobject){


		char date[10000] = {0};
    	memset(date, 0, sizeof(date));
    	my_api::zhixing(date, 116.481080, 39.989643, 116.465302, 40.004717);
    	std::cout << "解析成：" << std::endl;
    	std::cout << date << std::endl;

}






#if 0
int main()
{

	char date[10000] = {0};
	memset(date, 0, sizeof(date));
	my_api::zhixing(date, 116.481080, 39.989643, 116.465302, 40.004717);
	std::cout << "解析成：" << std::endl;
	std::cout << date << std::endl; 
}

#endif

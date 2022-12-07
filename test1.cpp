/*

g++ -std=c++11 ./*.cpp -o ./main  -L /usr/lib/x86_64-linux-gnu     -lcurl -lcrypto       -ljsoncpp  -I /usr/include/jsoncpp/  -L /home/china/libopenssl/ -I /home/china/libcurl/include/

*/

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

#include<jni.h>
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


//	std::cout << "GCJ-02 Point [Wgs2Gcj]: (" << lonGcj << ", " << latGcj << ")\n";
    	//读取文件
		//std::cout << "读取文件" << std::endl;
		std::string image;

		//获取api
		//std::cout << "获取api" << std::endl;
		
		Json::Value result = client.license_plate(image,"https://restapi.amap.com/v3/direction/driving?key=91e0952f96abe5b25a60fd875ebf11ea&extensions=base&strategy=2", x1,y1,x2,y2);

		// json解析
		//std::cout << "json解析" << std::endl;
		// std::cout <<"获取到"<< result << std::endl;


		bool isString() ;

		Json::Value result1 = result["route"]["paths"][0]["steps"];
		Json::Value result2 = result1;
		Json::Value result3;

		std::string data;
		for (int i = 0; i < result2.size(); ++i)
		{
			//std::cout << result2[i]["instruction"].asString() << std::endl;
		//	data.append(result2[i]["instruction"].asString());  //读取文字提示
		//	data.append("1111");
			data.append("\n");
			vector<string> res;
			res = split_string(result2[i]["polyline"].asString(), ";");

			for (int i = 0; i < res.size(); i++)
			{



				//std::cout << res[i] << endl;
				data.append(res[i]);

				data.append("\n");
				
			}

		if(result2[i]["road"].isString())
		{
            string temp = result2[i]["road"].asCString();
    	   // cout<<"路段名字"<<temp<<endl;
		   	data.append("1111");
			data.append(",");
			data.append("1111");
			data.append("\t");
			data.append(result2[i]["road"].asString());
		   

		} else{
		
			data.append("1111");
			data.append(",");
			data.append("1111");
			data.append("\t");
			data.append("未知路段");
		
		
		}

	


		}
		//结果
		
		strcpy(td,data.c_str());
	}

};

void Stringsplit(string str, const char split)
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		cout << token << endl; // 输出
	}
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str()); 
 
	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
 
	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, d);
	}
}



JNIEXPORT void JNICALL Java_test_fromcpp(JNIEnv *env, jobject obj, jdouble ori_lon, jdouble ori_lat, jdouble des_lon, jdouble des_lat,jstring rsuin){


/*======================WGS转换为高德坐标===========================*/

/*	
	        double lonWgs = ori_lon;
        	double latWgs = ori_lat;
        	double lonWgs_ = des_lon;
        	double latWgs_ = des_lat;

         	std::cout.precision(16);
  
    	std::pair<double, double> lonlatGcj = Wgs2Gcj(lonWgs, latWgs);
    	ori_lon = lonlatGcj.first;
    	ori_lat = lonlatGcj.second;
//	std::cout << "GCJ Point: (" << ori_lon << ", " << ori_lat << ")\n";
    	std::pair<double, double> lonlatGcj_ = Wgs2Gcj(lonWgs_, latWgs_);
    	des_lon = lonlatGcj_.first;
    	des_lat = lonlatGcj_.second;
//	std::cout << "GCJ Point: (" << des_lon << ", " << des_lat << ")\n";

*/


	int error_code=0;
 /*========================调用高的地图Api==================================*/
    char date[10000] = {0};
	memset(date, 0, sizeof(date));
	my_api::zhixing(date, ori_lon, ori_lat, des_lon, des_lat);
//	std::cout << "解析成：" << std::endl;
//	std::cout << date << std::endl; 
    
    VectorXd gd_pos (2);
//	vector <VectorXd> gd_pos_list;

     Gd_pos_list gd_pos_list11;
	 vector <Gd_pos_list> gd_pos_list;

   	string str = date;
    vector<string> res_ = split(str, "\n ");
	for (int i = 0; i < res_.size(); ++i)
	{
//		cout << res_[i] <<endl;
			string str = res_[i];


			double lon;

			char dot;

			double lat;
			char ct;
			string road_name;

			stringstream ss;

			ss<< str;

			    ss>>lon;
				gd_pos_list11.pos_x = lon;
				ss>>dot;
				gd_pos_list11.str = dot;
				ss>>lat;
				gd_pos_list11.pos_y = lat;

				ss>>road_name;
				gd_pos_list11.str = road_name;

              // gd_pos_list11 << lon,dot,lat,ct,road_name;
               gd_pos_list.push_back( gd_pos_list11);

	}

	if(gd_pos_list.size()==0){
	
		cout<<"地图调用失败"<<endl;

		error_code = 0x01;
		cout.setf(ios::showbase);
		cout.setf(ios::hex);
		cout<<"error_code=="<< error_code <<endl;
		cout.unsetf(ios::hex);
	}


  for (size_t i = 0; i < gd_pos_list.size(); i++)
	{
		cout << gd_pos_list[i].pos_x<<"\t" << gd_pos_list[i].pos_y << "\t" <<gd_pos_list[i].str << endl;

	}




//	gd_pos_list.erase(unique(gd_pos_list.begin(), gd_pos_list.end()), gd_pos_list.end());

   /*===================================前处理===================================*/



	GpsTo gps2;
 	PolyNomial polynomi;

	string out_file_name = "111-output.txt";
    string out_file_name_ = "1111-output.txt";
	


	/* 1,读取txt文件里的高德坐标点 GCJ-02坐标系 */

//	ifstream in_file(in_file_name_.c_str(), ifstream::in);
  
	ofstream out_file (out_file_name.c_str(), ofstream::out);
	ofstream out_file_ (out_file_name_.c_str(), ofstream::out);

//	cout<<"取出结果"<<"list_count=="<< gd_pos_list.size() <<endl;
/*	
	for (int i = 0; i < gd_pos_list.size(); i++) {
		cout << fixed << setprecision(6)<< gd_pos_list[i][0]<<"\t"<< gd_pos_list[i][1] << endl;
	}  */


/* 实例化RSU，并添加相应属性 */

//外部传参

 const char *str1 = env->GetStringUTFChars(rsuin, 0);//c++中jstring转string 
  VectorXd rsu_input (4);
	vector <VectorXd> rsu_input_list;
   
    vector<string> rsu1 = split(str1, "\n ");

	for (int i = 0; i < rsu1.size(); ++i)
	{
	
			string str = rsu1[i];

			double rsu_lon;

			char dot;
			double rsu_lat;
			int rsu_id;
			int rsu_r;

            stringstream ss;

			ss << str;

			ss>>rsu_id;

			ss>>dot;

			ss>>rsu_r;

			ss>>dot;
			
                ss>>rsu_lon;
				ss>>dot;
				ss>>rsu_lat;
		        rsu_input << rsu_id,rsu_r,rsu_lon,rsu_lat;
               rsu_input_list.push_back(rsu_input);

	}
/*
	for(int r=0;r<rsu_input_list.size();r++){

		cout<<"rsu_list=="<<rsu_input_list[r][3]<<endl;

 
	}  */

    RSU rsu;
	vector<RSU> rsu_;

    for(int r=0;r<rsu_input_list.size();r++){

	rsu.id = rsu_input_list[r][0];
    rsu.r = rsu_input_list[r][1];
	rsu.lon = rsu_input_list[r][2];   rsu.lat = rsu_input_list[r][3];

	rsu_.push_back(rsu);
	
	}
  
	


//	Trajectory_list trajec_list_; //轨迹引导列表
	vector<Trajectory_list> trajec_list;
	Path_list path_list;  
	vector <Path_list> path_step;  //路段轨迹列表



/* ==========================分段插值添加， 逐个读取每一段的经纬轨迹，并分别插值==============================  */
	int N = gd_pos_list.size();
	
	int count = 0;
	int n=0;
	vector<VectorXd> Output;

    vector<string> road;

	for (size_t K = 0; K < N; K++)         
	{
		Trajectory_list trajec_list_;
		int i = K;
		int e = 0;

		int road_c=i;

		vector<VectorXd> Output_temp;
		Output_temp.clear();

		vector<VectorXd> Output_temp__;
		Output_temp__.clear();

		VectorXd gd_pos_(2);
		vector <VectorXd> gd_pos_list_;
		gd_pos_list_.clear();

		for ( i ; (e < 200) && (i < N); i++)
		{

			
			if (i != K)
			{



			  e = gd_pos_list[i].pos_x - gd_pos_list[i - 1].pos_x;
		gd_pos_ << gd_pos_list[i - 1].pos_x, gd_pos_list[i - 1].pos_y;
				gd_pos_list_.push_back(gd_pos_);   // 用于后续轨迹插值

			}

		   road_c = road_c + 1;

		}

		/* 标识出每一段路的名字 */
		road.push_back(gd_pos_list[road_c-1].str);
		cout << "当前路段名字----->" << road[n] << endl;

gd_pos_list_.erase(unique(gd_pos_list_.begin(), gd_pos_list_.end()), gd_pos_list_.end());




/*
		cout << "========================第" << n << "段路的高德经纬轨迹=========================" << endl;
		for (size_t i = 0; i < gd_pos_list_.size(); i++)
		{
			cout << "第" << i << "个点的高德经纬轨迹====" << fixed << setprecision(6)<< gd_pos_list_[i] << endl;
		 
		}   */

		/*  ======================================   对该段进行轨迹插值切片  ======================================   */


 

		/* 2 ，坐标投影 */


		VectorXd proj_pos(2);
		vector <VectorXd> proj_pos_list;


		for (size_t i = 0; i < gd_pos_list_.size(); i++)
		{
			double x = gd_pos_list_[i][0];
			double y = gd_pos_list_[i][1];
			double z = 100;

	//		printf("原大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);


			gps2.Blh2Wmc(x, y, z);
	//		printf("Web墨卡托坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);

			//	cout << fixed << setprecision(6) << x << endl;
			proj_pos << x, y;
			proj_pos_list.push_back(proj_pos);

			/*	gps2.Wmc2Blh(x, y, z);



				printf("转回大地经纬度坐标：%.10lf\t%.10lf\t%.10lf\n", x, y, z);*/

		}

		for (i = 0; i < proj_pos_list.size(); i++) {
		//	cout << fixed << setprecision(6) << proj_pos_list[i][0] << "\t";

		//	cout << fixed << setprecision(6) << proj_pos_list[i][1] << "\n";
		}


		/* 3, 轨迹插值切片 */

		// vector<VectorXd> Output;
	   
		double x[2000]={};	//插值点
		double y[2000]={};
		VectorXd proj_temp(2);
		if (  proj_pos_list.size()<4  )   
		{
			VectorXd proj_pos_list_temp_x(proj_pos_list.size());
			VectorXd proj_pos_list_temp_y(proj_pos_list.size());
			
			double x0[100];		//已知的数据点
			double y0[100] ;

			for (size_t l = 0; l < proj_pos_list.size(); l++)     //已知的数据点
			{


				x0[l] = proj_pos_list[l][0];
				y0[l]=  proj_pos_list[l][1];


			}

			//for (size_t l_ = 0; l_ < size(proj_pos_list); l_++)     //已知的数据点
			//{
			//	
			//	cout<<"==========" << x0[l_] << "\t" << y0[l_] << endl;

			//}
	//		cout << "===================================" << endl;

			//double x[10];	//插值点
			//double y[10];
			// VectorXd proj_temp(2);

			try
			{



				SplineInterface* sp = new Spline(x0, y0, proj_pos_list.size());	//使用接口，且使用默认边界条件
				sp->AutoInterp(10, x, y);			//求x的插值结果y

				for (int i = 0; i < 10; i++) {
					cout << x[i] << "\t" << y[i] << endl;

					proj_temp << x[i], y[i];
					Output_temp__.push_back(proj_temp);

				}


			}
			catch (SplineFailure sf)
			{
			//	cout << sf.GetMessage() << endl;
			}

		}
		else {
		
			Output_temp__ = polynomi.catMullRomSpline(proj_pos_list);

			 for (size_t i = 0; i < Output_temp__.size(); i++)

			 {



				    x[i] = Output_temp__[i][0];
					y[i] = Output_temp__[i][1];


		        }
		
		}

	
		int xx = Output_temp__.size();


		/*数组倒序,高德序列与插值序列相反的时候需要倒序*/
		if (proj_pos_list[0][0] != x[0])     // 数组倒序,高德序列与插值序列相反的时候需要倒序
		{


			int i = 0;  //循环变量1, i的值为数组第一个元素的下标

			int j = xx - 1;  //循环变量2, j的值为数组最后一个元素的下标

			double buf;  //互换时的中间存储变量

			for (; i < j; ++i, --j)  /*因为i和j已经初始化过了, 所以表达式1可以省略, 但表达式1后面的分号不能省。*/

			{



				buf = x[i];

				x[i] = x[j];

				x[j] = buf;

			}
			int i_ = 0;  

			int j_ = xx - 1;
			double buf_;
			for (; i_ < j_; ++i_, --j_)

			{
				


				buf_ = y[i_];

				y[i_] = y[j_];

				y[j_] = buf_;

			}



		}

	

	for (int i = 0; i < xx ; i++) {
			cout << x[i] << "//t" << y[i] << endl;
			/*	proj_temp << x[i], y[i];
				Output.push_back(proj_temp);*/

			Output_temp__.clear();
			proj_temp << x[i], y[i];
			Output_temp__.push_back(proj_temp);

		}




		/* 4， 转换回经纬坐标  */


		for (size_t i = 0; i < xx ; i++)
		{
			double z_ = 100;

			gps2.Wmc2Blh(x[i], y[i], z_);

			gd_pos_ << x[i], y[i];
			Output_temp.push_back(gd_pos_);   //当前路段轨迹序列

			proj_temp << x[i], y[i];
			Output.push_back(proj_temp);
			
		}




		/* 5，  GCJ-02坐标系 转换gps坐标  */



		/* ========================================== 路径分发  =====================================================*/

		/* 6，路径分发      */

		 /* 6.1，创建轨迹引导列表， 通过高德坐标点选取RSU并排序   */

		//Trajectory_list trajec_list_;
		//vector<Trajectory_list> trajec_list;

		///* 实例化RSU，并添加相应属性 */
		//RSU rsu;

		///*;
		// rsu0.id = 0;
		// rsu0.lon = 106.4840548;   rsu0.lat = 29.6635501;  //34
		// rsu1.id = 1;
		// rsu1.lon = 106.4841978;   rsu1.lat = 29.6619316; //1109
		// rsu2.id = 2;
		// rsu2.lon = 106.4850102;   rsu2.lat = 29.6618484; //1639
		// */

		//vector<RSU> rsu_;
		//rsu.id = 0;
		//rsu.lon = 106.4840548;   rsu.lat = 29.6635501;
		//rsu.r = 500;
		//rsu_.push_back(rsu);
		//rsu.id = 1;
		//rsu.lon = 106.4841978;   rsu.lat = 29.6619316;
		//rsu.r = 500;
		//rsu_.push_back(rsu);
		//rsu.id = 2;
		//rsu.lon = 106.4850102;   rsu.lat = 29.6618484;
		//rsu.r = 500;
		//rsu_.push_back(rsu);
 


		/* 6.2,   轨迹按路段切片分配给对应RSU */

	   //  int traject_secqcount = trajec_list.size();

	//	Path_list path_list;

	//	vector <Path_list> path_step;  //路段轨迹列表

			path_list.path_step_id = n;
			path_list.path_step_trajectory = Output_temp;
			
			path_step.push_back(path_list);   //带有当前路段编号的轨迹序列


		
		//cout << "recent_path_pos_count=====" << size(path_step) << endl;


		//for (size_t p = 0; p < size(path_step); p++)
		//{


		//	for (size_t p_ = 0; p_ < size(path_step[p].path_step_trajectory); p_++)
		//	{

		//		cout << "recent_path_id===" << path_step[p].path_step_id << '\n' << "recent_path_pos_list======" << path_step[p].path_step_trajectory[p_] << endl;
		//	}

		//}

		//	cout << "recent_path_id===" << path_step[n].path_step_id << endl;
			for (size_t p_ = 0; p_ < path_step[n].path_step_trajectory.size(); p_++)
			{

		//		cout  << "recent_path_pos_list======" << path_step[n].path_step_trajectory[p_] << endl;

			}


		/* 判断当前段轨迹所属RSU  (目前暂时用每个路段的起始坐标来确立RSU)       */
		int path_count = path_step.size();

		double z_=100;
		gps2.Blh2Wmc(rsu_[0].lon, rsu_[0].lat, z_);
		gps2.Blh2Wmc(rsu_[1].lon, rsu_[1].lat, z_);
		gps2.Blh2Wmc(rsu_[2].lon, rsu_[2].lat, z_);

//		cout << "b2w====" << rsu_[0].lon << '\t' << rsu_[0].lat << endl;
//		cout << "b2w====" << rsu_[1].lon << '\t' << rsu_[1].lat << endl;
//		cout << "b2w====" << rsu_[2].lon << '\t' << rsu_[2].lat << endl;

		
		int traject_secqcount = trajec_list.size();

		double tra_lon = path_step[n].path_step_trajectory[0][0];
		double tra_lat = path_step[n].path_step_trajectory[0][1];
		gps2.Blh2Wmc(tra_lon, tra_lat, z_);

		double r_e;

		// trajec_list_.rsu_secq = k;

		bool recentpath_step_in_rsu = true;
		for (size_t i = 0; i < rsu_.size(); i++)
		{

			if (recentpath_step_in_rsu)
			{



				
				r_e = sqrt((rsu_[i].lon - tra_lon) * (rsu_[i].lon - tra_lon) + (rsu_[i].lat - tra_lat) * (rsu_[i].lat - tra_lat));
		//		cout << "r_e====" << r_e << endl;




				


				if (r_e < rsu_[i].r)   // 判断在哪个RSU的覆盖半径内 ,如果在，就把rsu id 添加进引导列表
				{

				
					trajec_list_.rsu_id = rsu_[i].id;



					recentpath_step_in_rsu = false;

				}
				else
				{
	//				cout << "不在RSU范围内" << endl;
				}

			}



		}



		if (recentpath_step_in_rsu)
		{
			cout <<road[n] << "没有可覆盖的RSU,泊车引导失败" << endl;

			error_code=2;
		//	cout<< "error_code"<<error_code<<endl;

	//		K = K + gd_pos_list_.size();
		//	continue;
		    break;

		}

		gps2.Wmc2Blh(rsu_[0].lon, rsu_[0].lat, z_);
		gps2.Wmc2Blh(rsu_[1].lon, rsu_[1].lat, z_);
		gps2.Wmc2Blh(rsu_[2].lon, rsu_[2].lat, z_);


		trajec_list_.rsu_secq = traject_secqcount;
//		cout << "trajeclist_count======" << traject_secqcount << endl;

		if ((trajec_list.size() == 0) || (trajec_list_.rsu_id != trajec_list[traject_secqcount - 1].rsu_id))
		{


		
			Trajectory_withlane b;
			b.traject_poslonlat_withid = path_step[n];
			b.path_step_id = path_step[n].path_step_id;
			trajec_list_.tarject_withlane_list.push_back(b);
			trajec_list.push_back(trajec_list_);


		}
		else
		{
		
			Trajectory_withlane c;
			c.path_step_id = n;
			
			c.traject_poslonlat_withid = path_step[n];

		trajec_list[traject_secqcount - 1].tarject_withlane_list.push_back(c);

		}

	
		n = n + 1;
		K = K + gd_pos_list_.size();

	}


cout << "==============================列表结果=====================================" << endl;

    if  (trajec_list.size()==0)
	  {
		cout << "所有路段没有可覆盖的RSU,泊车引导失败" << endl;

		cout.setf(ios::showbase);
		cout.setf(ios::hex);

		cout << "error_code==" << hex << error_code << endl;
        cout.unsetf(ios::hex);
		
	  }
	else if (error_code==0x02)
	  {
		  cout.setf(ios::showbase);
		cout.setf(ios::hex);
		cout << "error_code=="  << error_code << endl;
		cout.unsetf(ios::hex);
		return;

	  }


	for (size_t i = 0; i < trajec_list.size(); i++)
	{

		cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "traclist_id=====" << trajec_list[i].rsu_secq << "\t" << "rsu_id=======" << trajec_list[i].rsu_id << endl;

		
	//	cout << "tarject_withlane_list.size===" << trajec_list[i].tarject_withlane_list.size() << endl;

		for (size_t i_ = 0; i_ < trajec_list[i].tarject_withlane_list.size(); i_++)
		{
			cout << "path_step_id===" << trajec_list[i].tarject_withlane_list[i_].path_step_id << endl;

		for (size_t p = 0; p < trajec_list[i].tarject_withlane_list[i_].traject_poslonlat_withid.path_step_trajectory.size(); p++)
			{
	

			cout << "tarject_poslonlat====" << trajec_list[i].tarject_withlane_list[i_].traject_poslonlat_withid.path_step_trajectory[p][0] <<'\t'<< trajec_list[i].tarject_withlane_list[i_].traject_poslonlat_withid.path_step_trajectory[p][1] << endl;
				

			/* 用于最后结果回放 */

			gd_pos << trajec_list[i].tarject_withlane_list[i_].traject_poslonlat_withid.path_step_trajectory[p][0], trajec_list[i].tarject_withlane_list[i_].traject_poslonlat_withid.path_step_trajectory[p][1];
				Output.push_back(gd_pos);

			}


		}

		
	}
	



    out_file <<"+++++++++java端列表输出+++++++++"<<endl;
	for(int o=0;o<trajec_list.size();o++){

		  out_file << "+++++++++++++java列表输出++++++++++++++++++++++++++" << endl;
		  out_file << fixed << setprecision(6)<<"traclist_id=====" << trajec_list[o].rsu_secq << "\t" << "rsu_id=======" << trajec_list[o].rsu_id << endl;


       for(int u=0;u< trajec_list[o].tarject_withlane_list.size();u++){
	   
		    out_file << fixed << setprecision(6)<<"path_step_id==" << trajec_list[o].tarject_withlane_list[u].path_step_id << endl;
		 

		   for(int t=0;t< trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory.size();t++ ){
		   
		 out_file << fixed << setprecision(6)<< "tarject_poslonlat====" << trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory[t][0] <<'\t'<< trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory[t][1] << endl;

		   
		   
		   }

	   
	   }  	




	}



 
	for(int o=0;o<trajec_list.size();o++){

	
       for(int u=0;u< trajec_list[o].tarject_withlane_list.size();u++){
	   
		  

		   for(int t=0;t< trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory.size();t++ ){
		   
		 out_file_ << fixed << setprecision(6)<< "[" << trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory[t][0] <<","<< trajec_list[o].tarject_withlane_list[u].traject_poslonlat_withid.path_step_trajectory[t][1] <<"]"<< endl;

		   
		   
		   }

	   
	   }  	




	}




}
    



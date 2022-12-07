import java.io.File;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.util.Scanner;

public class testco1 {
   

    public static void main(String[] args) {


    //    test demo  = new test();
     //   demo.fromcpp(106.4840516,29.6635932,106.4850181,29.6618504);
       test demo  = new test();
//	   String site = "www.runoob.com";
    /*   String rsu_in="3,500,106.4840548,29.6635501 2,500,106.4841978,29.6619316 1,500,106.4850102,29.6618484 ";  
	 */
	    String rsu_in="3,200,106.487867,29.66168 2,200,106.488167,29.658799 1,200,106.49358,29.659979 ";

		Scanner input = new Scanner(System.in);

	    System.out.print("Enter orilon: ");
        double orilon = input.nextDouble();
		 System.out.print("Enter orilat: ");
        double orilat = input.nextDouble();
		 System.out.print("Enter deslon: ");
        double deslon = input.nextDouble();
		 System.out.print("Enter deslat: ");
        double deslat = input.nextDouble();
		 
       

	//	double orilon;
//		double orilat;
//		double deslon;
//		double deslat;


		
		demo.fromcpp(orilon,orilat,deslon,deslat ,rsu_in);

    //demo.fromcpp(106.4840516,29.6635932,106.4850181,29.6618504,rsu_in);
//  demo.fromcpp(106.487926,29.661456,106.493596,29.658864,rsu_in);

 try { // 防止文件建立或读取失败，用catch捕捉错误并打印，也可以throw
 
			/* 读入TXT文件 */
//		String pathname = "/home/lighthouse/111-output.txt"; // 绝对路径或相对路径都可以，这里是绝对路径，写入文件时演示相对路径
	    String pathname = "/home/lwj/trajectory/javatest/111-output.txt";

			File filename = new File(pathname); // 要读取以上路径的input。txt文
			InputStreamReader reader = new InputStreamReader(

			new FileInputStream(filename)); // 建立一个输入流对象reader
			BufferedReader br = new BufferedReader(reader); // 建立一个对象，它把文件内容转成计算机能读懂的语言
			String line = "";
			line = br.readLine();
			while (line != null) {
			line = br.readLine(); // 一次读入一行数据
    		System.out.println(line);
			}
 
		
 
		} catch (Exception e) {
			e.printStackTrace();
		}
	}




    


}


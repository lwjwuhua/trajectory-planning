import java.io.File;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileWriter;

public class testco2 {
   

    public static void main(String[] args) {


   

 try { // 防止文件建立或读取失败，用catch捕捉错误并打印，也可以throw
 
			/* 读入TXT文件 */
//		String pathname = "/home/lighthouse/111-output.txt"; // 绝对路径或相对路径都可以，这里是绝对路径，写入文件时演示相对路径
	    String pathname = "/home/lwj/trajectory/javatest/1-output.txt";

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


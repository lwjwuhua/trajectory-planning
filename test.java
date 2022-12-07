public class test{

	static {
  
        System.load("/home/lwj/trajectory/javatest/libTestn.so");
	//	 System.load("/home/lighthouse/libTestn.so");
    }

 
	public native void fromcpp(double ori_lon, double ori_lat,double des_lon,double des_lat,String rsuin);



}

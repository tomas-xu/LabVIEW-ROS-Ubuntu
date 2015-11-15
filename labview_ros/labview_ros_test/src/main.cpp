#include "ros/ros.h"
#include "std_msgs/String.h"
#include "labview_ros_test/my_msg.h"
void myMSGCallback(const labview_ros_test::my_msg::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s], [%f]", msg->str.c_str(), msg->num);
  if(msg->arr.size()>0)
  	std::cout<<"Get data: "<<msg->arr[0]<<std::endl;
}

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
	ros::Subscriber sub2 = n.subscribe("chatter_mymsg", 1000, myMSGCallback);
	ros::spin();

	return 0;
}

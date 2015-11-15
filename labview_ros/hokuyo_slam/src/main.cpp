#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/Range.h"
#include "nav_msgs/Odometry.h"
#include <tf/transform_broadcaster.h>

ros::Publisher laser_repub;
ros::Publisher odom_repub;
void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  
  std::cout<<"lasercallback"<<std::endl;

  if(!msg) return;
  //std::cout<<"frame id: "<<msg->header.frame_id<<std::endl;

  sensor_msgs::LaserScan repub_scan = *msg;;

  repub_scan = *msg;
  repub_scan.header.stamp = ros::Time::now();
  laser_repub.publish(repub_scan);
  //std::cout<<"repub laser: "<<repub_scan.header.stamp<<std::endl;
  //std::cout<<"Getting sensor data and broadcast tf..."<<std::endl;
}
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	if(!msg) return;
	std::cout<<"Odometry frame id: "<<msg->header.frame_id<<", time: "<<msg->header.stamp<<std::endl;
	//std::cout<<"Odometry pose: "<<msg->pose.pose.position<<std::endl;
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "base_laser"));
  geometry_msgs::Point p = msg->pose.pose.position;
  geometry_msgs::Quaternion q2 = msg->pose.pose.orientation;
  q.setValue(q2.x, q2.y, q2.z, q2.w);
  transform.setOrigin( tf::Vector3(p.x, p.y, p.z) );
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));
  //republish odometry data
  nav_msgs::Odometry msg_out;
  msg_out = *msg;
  msg_out.header.stamp = ros::Time::now();
  odom_repub.publish(msg_out);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;

  laser_repub = n.advertise< sensor_msgs::LaserScan >("scan_repub", 10);
  odom_repub = n.advertise< nav_msgs::Odometry >("odom_repub", 10);
	ros::Subscriber sub = n.subscribe("/scan", 1, laserCallback);
	ros::Subscriber subOdom = n.subscribe("/odometry", 1, odomCallback);
	ros::spin();

	return 0;
}

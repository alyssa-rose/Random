#include <ros/ros.h>
#include <tf/tf.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <actionlib/server/simple_action_server.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PointStamped.h" 
#include <vector>


void move_turtlebot(double x, double y, double yaw);
void callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);
void update_end(const geometry_msgs::PointStamped::ConstPtr& msg);

int count = 0;
int pts = 0;
bool set_pt = FALSE;

int main(int argc, char **argv)
{ 
	ros::init(argc, argv, "k_point_turtle"); 
	ros::NodeHandle nh;
	
	std::cout << "Enter number of moves: ";
	std::cin >> pts;

	
	vector<double> x_cord(pts);
	vector<double> y_cord(pts);

	
	// Subscribes to two topics: the points given by the user and the position of the robot
	ros::Subscriber sub = n.subscribe("/clicked_point",pts, update_end); 

	// not sure if really needed in this case	
        ros::Subscriber sub2 = n.subscribe("/amcl_pose", pts, callback); 
	
	ros::spin();
}


void update_end(const geometry_msgs::PointStamped::ConstPtr& msg){
	// stores the ith coordinate
	x_cord[count] = msg->point.x;
	y_cord[count] = msg->point.y;
	count++;
	move_turtlebot(x_cord[count-1], y_cord[count-1], 0.0);
}
void move_turtlebot(double x, double y, double yaw){
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
		
		ac.waitForServer();
		move_base_msgs::MoveBaseGoal goal;
		
		goal.target_pose.header.stamp = ros::Time::now();
		goal.target_pose.header.frame_id = "/map";
		
		goal.target_pose.pose.position.x = x;
		goal.target_pose.pose.position.y = y;
		goal.target_pose.pose.position.z = 0.0;
		goal.target_pose.pose.orientation = tf.createQuaternionMsgFromYaw(yaw);
		ac.sendGoal(goal);
		ac.waitForResult();

}

// Func: Callback
// Param: pointer to geometry_msgs (current location)
// Does: 
void callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
	if (!set_pt) {
		ROS_INFO("Please use R-Viz to 'Publish' k # of points");
		return;
	}
	
	if (count == pts)
		count = 0;
	move_turtlebot(x_cord[count], y_cord[count], 0.0);
	count++;
}

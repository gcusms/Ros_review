/*
    需求: 订阅人的信息

*/

#include "ros/ros.h"
#include "topic_msg_test/Li.h"

void doPerson(const topic_msg_test::Li::ConstPtr& persion){
    ROS_INFO("订阅的人Li的信息:%s, %d, %.2f", persion->name.c_str(), persion->age, persion->weight);
}

int main(int argc, char *argv[])
{   
    setlocale(LC_ALL,"");

    //1.初始化 ROS 节点
    ros::init(argc,argv,"listener_person");
    //2.创建 ROS 句柄
    ros::NodeHandle nh;
    //3.创建订阅对象
    ros::Subscriber sub = nh.subscribe<topic_msg_test::Li>("chatter_person",10,doPerson);
    
    ros::spin();    
    return 0;
}

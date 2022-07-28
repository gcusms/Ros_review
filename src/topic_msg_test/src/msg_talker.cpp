#include "ros/ros.h"
#include "topic_msg_test/Li.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    //1.初始化 ROS 节点
    ros::init(argc,argv,"talker_person");

    //2.创建 ROS 句柄
    ros::NodeHandle nh;

    // //3.创建发布者对象
    ros::Publisher pub = nh.advertise<topic_msg_test::Li>("chatter_person",1000);

    //4.组织被发布的消息，编写发布逻辑并发布消息
    topic_msg_test::Li p;
    p.name = "YF LI";
    p.age = 22;
    p.weight = 45;
    static int count_ = 0;
    ros::Rate r(1);
    while (ros::ok())
    {
        count_ += 1;
        pub.publish(p);
        ROS_INFO("第%d次发送,我叫:%s,今年%d岁,体重%.2fkg",count_, p.name.c_str(), p.age, p.weight);
        r.sleep();
        ros::spinOnce();
    }



    return 0;
}
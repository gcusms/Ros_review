# :butterfly:View:butterfly:

---
**准备知识** [CmakeLists](https://cmake.org/cmake/help/latest/guide/tutorial/index.html),c++ [shell](https://www.runoob.com/linux/linux-shell.html)

### 话题通信测试

- [基本理论模型可以参考](http://www.autolabor.com.cn/book/ROSTutorials/di-2-zhang-ros-jia-gou-she-ji/22hua-ti-tong-xin/211-li-lun-mo-xing.html)
- 创建 package 功能包，名字自己定义
打开终端

~~~shell
catkin_create_pkg test_pkg roscpp rospy message_generation std_msgs rospy roscpp
~~~

之后就会生成一个名字为 **test_pkg** 的功能包文件夹里面含有 **include**, **src** 两个子文件夹，一个是用来包含头文件，一个是用来编写源 cpp 文件，主要对 src 文件夹进行操作即可

#### 流程

编写一个简单的发布方和订阅方
1 编写发布方实现；
2 编写订阅方实现；
3 编辑配置文件；
4 编译并执行；

- topic_pub.cpp 文件编写
在 src文件夹下创建.cpp 文件，然后键入

~~~cpp
 #include "ros/ros.h"
 #include "std_msgs/String.h" 
 #include <sstream>

int main(int argc, char  *argv[])
{   
    //设置编码
    setlocale(LC_ALL,"");

    //2.初始化 ROS 节点:命名(唯一)
    // 参数1和参数2 后期为节点传值会使用
    // 参数3 是节点名称，是一个标识符，需要保证运行后，在 ROS 网络拓扑中唯一
    ros::init(argc,argv,"talker");
    //3.实例化 ROS 句柄
    ros::NodeHandle nh;//该类封装了 ROS 中的一些常用功能

    //4.实例化 发布者 对象
    //泛型: 发布的消息类型
    //参数1: 要发布到的话题
    //参数2: 队列中最大保存的消息数，超出此阀值时，先进的先销毁(时间早的先销毁)
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter",10);

    //5.组织被发布的数据，并编写逻辑发布数据
    //数据(动态组织)
    std_msgs::String msg;
    // msg.data = "你好啊！！！";
    std::string msg_front = "Hello 你好！"; //消息前缀
    int count = 0; //消息计数器

    //逻辑(一秒10次)
    ros::Rate r(1);

    while (ros::ok())
    {
        //使用 stringstream 拼接字符串与编号
        std::stringstream ss;
        ss << msg_front << count;
        msg.data = ss.str();
        //发布消息
        pub.publish(msg);
        //加入调试，打印发送的消息
        ROS_INFO("发送的消息:%s",msg.data.c_str());

        //根据前面制定的发送贫频率自动休眠 休眠时间 = 1/频率；
        r.sleep();
        count++;//循环结束前，让 count 自增
        //暂无应用
        ros::spinOnce();
    }


    return 0;
}

~~~

#### 订阅方的实现

同样也是在终端打开并且进入到 src 文件夹里面,新建一个.cpp 文件，注意不要和订阅方重名

~~~cpp

// 1.包含头文件 
 #include "ros/ros.h"
 #include "std_msgs/String.h"

void doMsg(const std_msgs::String::ConstPtr& msg_p){
    ROS_INFO("好吧，我接受到的消息是:%s",msg_p->data.c_str());
    // ROS_INFO("我听见:%s",(*msg_p).data.c_str());
}
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    //2.初始化 ROS 节点:命名(唯一)
    ros::init(argc,argv,"listener");
    //3.实例化 ROS 句柄
    ros::NodeHandle nh;

    //4.实例化 订阅者 对象
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("chatter",10,doMsg);
    //5.处理订阅的消息(回调函数)
    //     6.设置循环调用回调函数
    ros::spin();//循环读取接收的数据，并调用回调函数处理

    return 0;
}

~~~

#### 运行

如果还没有将setup.bash 添加到环境变量里面， 请打开终端添加
> gedit ~/.bashrc

末尾写入 **source ~/ROS/Ros_review/devel/setup.bash**
然后每次打开就会让终端更新一次环境变量



---

## Msg 的使用

在学习的过程中，发现所有其他的文件编辑操作最终都会生成 msg 文件，同样也会生成头文件调用

- 在上次创建的 topic_msgs_test 功能包里面新建一个 msg 文件夹，然后在里面新建一个名字为 Li.msg 的文件，并且写入 

~~~cpp
string name
uint16 age
float64 weight
~~~

- 为了方便 vscode 头文件的搜寻，需要更改 .vscode 文件夹里面的c_cpp_properties.json 文件，并且添加路径即可

例如：
> "/home/sms/Ros/Ros_review/devel/include/**"
> catkin_make 

- 然后可以开始编写cpp 文件来调用 msg 文件夹里面的属性并且使用，上面的属性可以类比于小车的方向坐标位置等消息
在 src 文件夹下创建 msg_lisener.cpp 和 msg_talker.cpp 文件，前者表示倾听者，后者表示谈话者（发布者）

#### msg_lisener.cpp编写

~~~cpp
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
~~~

#### msg_lisener.cpp编写

~~~cpp
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

~~~

上述代码中包含的头文件 **"topic_msg_test/Li.h"** 就是 msg 文件夹下生成的 Li.msg 文件，不需要理解里面是什么，知道怎么调用即可

- 启动roscore
- rosrun topic_msg_test msg_lisener
- rosrun topic_msg_test msg_talker


ros 信息通信方式都是大同小异，学会了一种，其他的只是有些逻辑和操作上的区别，原理了解之后即可以。

---

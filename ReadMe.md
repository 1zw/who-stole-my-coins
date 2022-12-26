# 谁偷走了我的钱币
@Wily  
***这是感知组新人培训专用仿真包***
## 简介
- 故事背景  
阿威是一个穷屌丝，但是他一直很喜欢公司老板的女儿小花，有一天，阿威正在打大富翁，一不小心进入了游戏世界。在这里，他可以偷取背后Boss(Wizard Hence)的金币，为了挣到足够多的钱，让心爱的女人高看自己一眼，他开始了疯狂的追逐金币之路。  
- 游戏玩法  
1. 聪明的你需要帮助阿威挣到足够多的金币，但是这个游戏环境为gazebo,没有自己的消息接口，你无法控制阿威的车辆进行驾驶，吃到金币，因此请自定义相关的消息接口并完成任务。
2. 你也许还会发现只是订阅/odom的里程计数据并不能满足你数据精度的要求，那么你该如何解决这个问题呢
## 准备工作
***说明：以下所有操作中的your_work_space均为你的工作空间***
1. 这个指令是为了安装运行仿真安装包所需要的依赖包
```bash
$ sudo apt install ros-$ROS_DISTRO-joint-state-publisher ros-$ROS_DISTRO-robot-state-publisher ros-$ROS_DISTRO-gazebo-* ros-$ROS_DISTRO-ros2-control
```
2. 这个指令是为了帮你安装所需要的model
```
$ cd ~/your_work_space/src/who-stole-my-coins
$ mv  ./crazy_coins ~/.gazebo/models/ & mv  ./ground_plane ~/.gazebo/models/
```
3. 这个指令是为了帮你安装仿真环境所需要的变量
``` bash
echo "export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/your_work_space/build/coins_manager_plugin" >> ~/.bashrc
```
## 运行测试
```bash
$ cd ~/your_work_space
$ colcon build --packages-select chasing_coins coins_manager_plugin
$ ros2 launch chasing_coins chasing_coins.launch.py 
```

![运行结果](./image/%E8%BF%90%E8%A1%8C%E7%A4%BA%E4%BE%8B.png)

## 相关接口

|接口名称|作用|  
|---|---|
| /elevator_trans | 接受车辆位置，发布给gazabo,使得邻近的金币被吃掉 |
| /hrt_cmd| 类似于小乌龟控制节点，可以操控车辆运动 但仅限于平面运动   |
| /hrt_imu| imu提供当前车辆在各个方向上的速度和角速度               |
| /odom   | odom 里面搭载了里程计数据，使嘚你能够知道目前车辆的位置   |


## 你可能会遇到的小问题
1. 关闭gazebo之后launch打不开
```bash
$ killall gzclient & killall gzserver
```
2. 缺少相关的依赖包
```bash
$ sudo apt-get install ros-$ROS_DISTRO-你缺少的包名
```
3. 提示你找不到相关的插件 
```note
- 看准备工作第三条是否有误
```
## 新人需要做的工作
写一个控制功能包，吃掉所有金币，在这个功能包中，你需要向/elevator_trans话题发布车辆的位置坐标，告诉仿真环境，这样金币才会消失，单纯自己手动开是不会消失的，下面这条语句可以用来测试接口是否正常  
```
ros2 topic pub /elevator_trans std_msgs/msg/Float64MultiArray {"data : [4.8,2.0]"} -r 10
```


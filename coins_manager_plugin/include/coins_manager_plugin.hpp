#ifndef COINS_MANAGER_PLUGIN_HPP
#define COINS_MANAGER_PLUGIN_HPP

// Gazebo Includes
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/common/Plugin.hh>
#include <ignition/math/Pose3.hh>
#include <chrono>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <nav_msgs/msg/odometry.hpp>
namespace gazebo
{
class GazeboRosTemplatePrivate;

class coins_manager_plugin : public ModelPlugin  
{
    public:
        coins_manager_plugin();
    // 在插入本插件的时候，Gazebo就会调用该函数。
    // 该函数是一个虚函数，我们可以通过重载该函数来对插件进行一些初始化的操作。
    // 输入参数_model是一个指向与本插件相关联的模型的指针。
    // 输入参数_sdf则是一个指向本插件SDF元素的指针。
    public: 
    void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    virtual void OnMsg(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
    virtual void OnMsg_2(const nav_msgs::msg::Odometry::SharedPtr msg);
    double get_dis(const ignition::math::Pose3d &msg);
    virtual void OnUpdate();
    virtual void timer_cb();
    // Pointer to the model
    private: 
    physics::ModelPtr model;
    sdf::ElementPtr sdf;
    physics::Link_V linkptr;
    // Pointer to the update event connection
    event::ConnectionPtr updateConnection;
    std::time_t cur_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t las_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    double x,y,twist = 0.0;
    std::string goal_coin = "coin_39";
    double update_rate = 10;
    std::unique_ptr<GazeboRosTemplatePrivate> impl_; 
    double got = 0;
    bool send = false;
};
// 向Gazebo注册本插件
GZ_REGISTER_MODEL_PLUGIN(coins_manager_plugin)
}

#endif
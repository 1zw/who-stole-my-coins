#ifndef GAZEBO_PLUGINS__GAZEBO_ROS_TEMPLATE_HPP_
#define GAZEBO_PLUGINS__GAZEBO_ROS_TEMPLATE_HPP_
 
#include <gazebo/common/Plugin.hh>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
 
#include <memory>
 
namespace gazebo_plugins
{
 
class GazeboRosTemplatePrivate;
 
class GazeboRosTemplate : public gazebo::ModelPlugin
{
public:
  /// Constructor
  GazeboRosTemplate();
 
  /// Destructor
  virtual ~GazeboRosTemplate();
 
  void Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf) override;
 
protected:
 
  virtual void OnMsg(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
 
  virtual void OnUpdate();
  double get_dis(const ignition::math::Pose3d &msg);
private:
  std::unique_ptr<GazeboRosTemplatePrivate> impl_; 
  double x,y,twist;
};
}
 
#endif  // GAZEBO_PLUGINS__GAZEBO_ROS_TEMPLATE_HPP_
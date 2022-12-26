#include <gazebo/physics/Model.hh>
#include "demo2.hpp"
#include <gazebo_plugins/gazebo_ros_elevator.hpp>
#include <gazebo_ros/node.hpp>
#include <rclcpp/rclcpp.hpp>
 
#include <memory>
 
namespace gazebo_plugins
{
class GazeboRosTemplatePrivate
{
public:
  /// Connection to world update event. Callback is called while this is alive.
  gazebo::event::ConnectionPtr update_connection_;
  /// Node for ROS communication.
  gazebo_ros::Node::SharedPtr ros_node_;
  /// Subscriber to elevator commands
  rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_;
};
 
GazeboRosTemplate::GazeboRosTemplate()
: impl_(std::make_unique<GazeboRosTemplatePrivate>())
{
}
 
GazeboRosTemplate::~GazeboRosTemplate()
{
}
 
void GazeboRosTemplate::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf)
{
  // Create a GazeboRos node instead of a common ROS node.
  // Pass it SDF parameters so common options like namespace and remapping
  // can be handled.
  impl_->ros_node_ = gazebo_ros::Node::Get(sdf);
  // Get QoS profiles
  const gazebo_ros::QoS & qos = impl_->ros_node_->get_qos();
  // The model pointer gives you direct access to the physics object,
  // for example:
  RCLCPP_INFO(impl_->ros_node_->get_logger(), model->GetName().c_str());
  impl_->sub_ = impl_->ros_node_->create_subscription<std_msgs::msg::Float64MultiArray>(
    "elevator_trans", qos.get_subscription_qos("elevator_trans", rclcpp::QoS(1)),
    std::bind(&GazeboRosTemplate::OnMsg, this, std::placeholders::_1));
  RCLCPP_INFO(impl_->ros_node_->get_logger(), "Subscribed to [%s]", impl_->sub_->get_topic_name());
  // Create a connection so the OnUpdate function is called at every simulation
  // iteration. Remove this call, the connection and the callback if not needed.
  impl_->update_connection_ = gazebo::event::Events::ConnectWorldUpdateBegin(
    std::bind(&GazeboRosTemplate::OnUpdate, this));
}
void GazeboRosTemplate::OnMsg(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
    RCLCPP_INFO(impl_->ros_node_->get_logger(), "I heard!");
    this->x = msg->data.at(0);
    this->y = msg->data.at(1);
}
void GazeboRosTemplate::OnUpdate()
{
  // Do something every simulation iteration
  // if (this->sdf->HasElement("spin_velocity"))
  //       this->twist = this->sdf->Get<double>("spin_velocity");
  // this->model->SetAngularVel(ignition::math::Vector3d(0, 0, twist));

}
double GazeboRosTemplate::get_dis(const ignition::math::Pose3d &msg)
{
  double coin_pose_x = msg.X();
  double coin_pose_y = msg.Y();
  return sqrt(pow(coin_pose_x- this->x,2)+pow(coin_pose_y- this->y,2));
}
// Register this plugin with the simulator
// GZ_REGISTER_MODEL_PLUGIN(GazeboRosTemplate)
}  // namespace gazebo_plugins
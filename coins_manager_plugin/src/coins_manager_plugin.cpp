#include "coins_manager_plugin.hpp"
#include <gazebo_plugins/gazebo_ros_elevator.hpp>
#include <gazebo_ros/node.hpp>
#include <memory>
#include <gazebo/physics/Model.hh>
#include <rclcpp/rclcpp.hpp>
namespace gazebo
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
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub2_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    };
    coins_manager_plugin::coins_manager_plugin()
    :impl_(std::make_unique<GazeboRosTemplatePrivate>()){}
    void coins_manager_plugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {   
        this->model = _model;
        this->sdf = _sdf;
        this->linkptr = this->model->GetLinks();
        std::cerr << "\nthis plugin is connected with[" << _model->GetName() << "]" << std::endl;
        if (this->linkptr.size() == 0)
        {
            std::cerr << "coin doesn't exist, plugin will not work\n";
            return;
        }
        else  //创建ros节点
        {   
            // rclcpp::Node *node = new rclcpp::Node(this->model->GetName());
            // publisher = std::make_shared<coins_publisher>(node);
            impl_->ros_node_ = gazebo_ros::Node::Get(sdf);
            const gazebo_ros::QoS & qos = impl_->ros_node_->get_qos();
            RCLCPP_INFO(impl_->ros_node_->get_logger(), model->GetName().c_str());
            impl_->sub_ = impl_->ros_node_->create_subscription<std_msgs::msg::Float64MultiArray>(
                "elevator_trans", qos.get_subscription_qos("elevator_trans", rclcpp::QoS(1)),
                std::bind(&coins_manager_plugin::OnMsg, this, std::placeholders::_1));
            impl_->sub2_ = impl_->ros_node_->create_subscription<nav_msgs::msg::Odometry>(
                "/odom", qos.get_subscription_qos("/odom", rclcpp::QoS(1)),
                std::bind(&coins_manager_plugin::OnMsg_2, this, std::placeholders::_1));
            impl_->pub_ = impl_->ros_node_->create_publisher<std_msgs::msg::Float64MultiArray>(
            "/Score",1);
            impl_->timer_ = impl_->ros_node_->create_wall_timer(std::chrono::milliseconds(100),
            std::bind(&coins_manager_plugin::timer_cb,this));
            RCLCPP_INFO(impl_->ros_node_->get_logger(), "Subscribed to [%s]", impl_->sub_->get_topic_name());
            // Create a connection so the OnUpdate function is called at every simulation
            // iteration. Remove this call, the connection and the callback if not needed.
            impl_->update_connection_ = gazebo::event::Events::ConnectWorldUpdateBegin(
                std::bind(&coins_manager_plugin::OnUpdate, this));
        }

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&coins_manager_plugin::OnUpdate, this));
        // Safety check
        
        
    }
    void coins_manager_plugin::OnMsg(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
    {
        msg->data.resize(2);
        // this->x = msg->data.at(0);
        // this->y = msg->data.at(1);
    }void coins_manager_plugin::OnMsg_2(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        this->x = msg->pose.pose.position.x;
        this->y = msg->pose.pose.position.y;
    }
    void coins_manager_plugin::timer_cb()
    {
        std_msgs::msg::Float64MultiArray msg;
        msg.data.resize(1);
        msg.data.at(0) = (0+got);
        if (got == 1) send = true;
        impl_->pub_->publish(msg);

    }
    void coins_manager_plugin::OnUpdate()
    {
        
        if (this->sdf->HasElement("spin_velocity"))
            this->twist = this->sdf->Get<double>("spin_velocity");
        if (this->sdf->HasElement("goal_coin"))
            this->twist = this->sdf->Get<char>("goal_coin");
        if (this->sdf->HasElement("update_rate"))
            this->twist = this->sdf->Get<double>("update_rate");

        this->model->SetAngularVel(ignition::math::Vector3d(0, 0, twist));
        double distance = this->get_dis(this->model->RelativePose());
        if (distance < 1 || send == true)
        {   
            got = 1;
            if (send == true)
                this->model->Fini();

        }
    }
    double coins_manager_plugin::get_dis(const ignition::math::Pose3d &msg)
    {
        double coin_pose_x = msg.X();
        double coin_pose_y = msg.Y();
        return sqrt(pow(coin_pose_x- this->x,2)+pow(coin_pose_y- this->y,2));
    }


}   
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
class caculate :public rclcpp::Node
{
private:
    int score;
    rclcpp::Node::SharedPtr ros_node_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_;
    rclcpp::Time this_time,last_time;
public:
    caculate();
    ~caculate();
    void get_score_cb(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
    int get_score();
    void run(int argc, char **argv);
};

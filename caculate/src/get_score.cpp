#include "get_score.hpp"
class caculate_node
{
public:
    rclcpp::Node::SharedPtr ros_node_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_;
    rclcpp::Time this_time,last_time;
};
void caculate::get_score_cb(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{   
    if (msg->data.size()!=0 && msg->data.at(0))
        score++;
    this_time = this->get_clock()->now();
    if (this_time.seconds()-last_time.seconds() >= 5.0)
    {
        // printf("now score is %d\n",score);
        RCLCPP_INFO(this->get_logger(),"now score is %d\n",score);
        last_time = this_time;
    }
    
}

caculate::caculate():Node("get_score")
{   
    score = 0;
    sub_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
        "/Score",1,std::bind(&caculate::get_score_cb,this,std::placeholders::_1)
    );
    this_time = this->get_clock()->now();
    last_time = this->get_clock()->now();
}
caculate::~caculate(){}
int caculate::get_score()
{
    return score;
}
int main(int argc, char **argv)
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<caculate>());
    rclcpp::shutdown();
}
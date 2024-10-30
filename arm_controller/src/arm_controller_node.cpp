// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for specific language governing permissions and
// limitations under the License.
 
#include <chrono>
#include <functional>
#include <memory>
#include <string> 
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
 
using namespace std::chrono_literals;
 
class ControllerNode : public rclcpp::Node

{

public:
  ControllerNode(): Node("arm_controller_node")
  {
    // Subscriber al topic "joint_states"
    joint_state_subscriber = this->create_subscription<sensor_msgs::msg::JointState>(
      "joint_states", 10, std::bind(&ControllerNode::JointStateCallBack, this, std::placeholders::_1));
    // Publisher al topic "/position_controller/commands"
    joint_state_publisher = this->create_publisher<std_msgs::msg::Float64MultiArray>("/position_controller/commands", 10);
    // Timer per inviare i comandi regolarmente
    timer_ = this->create_wall_timer(
      500ms, // ogni 500 ms
      [this]() {
        auto command_msg = std_msgs::msg::Float64MultiArray();
        command_msg.data = {1.0, 0.0, 1.0, 0.0};  // Comandi di posizione dei giunti
        joint_state_publisher->publish(command_msg);
      });
  }
 
private:

  void JointStateCallBack(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(), "Current Joint Position: ");
    for(size_t i = 0; i < msg->name.size(); ++i)
    {
      RCLCPP_INFO(this->get_logger(), "Joint %s: %f", msg->name[i].c_str(), msg->position[i]);
    }
  }
 
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_subscriber;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr joint_state_publisher;
  rclcpp::TimerBase::SharedPtr timer_;

};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ControllerNode>());
  rclcpp::shutdown();

  return 0;
}


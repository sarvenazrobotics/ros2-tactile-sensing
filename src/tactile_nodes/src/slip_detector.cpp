
#include <rclcpp/rclcpp.hpp>
#include "tactile_interfaces/msg/tactile_array.hpp"
#include "tactile_interfaces/msg/slip_event.hpp"
#include <vector>
#include <cmath>

class SlipDetector : public rclcpp::Node {
public:
    SlipDetector() : Node("slip_detector"), first_message_received_(false) {
        // Match the publisher's QoS (best_effort)
        rclcpp::QoS sensor_qos(10);
        sensor_qos.best_effort();
        
        subscription_ = this->create_subscription<tactile_interfaces::msg::TactileArray>(
            "tactile_data", sensor_qos,
            std::bind(&SlipDetector::topic_callback, this, std::placeholders::_1));
        
        publisher_ = this->create_publisher<tactile_interfaces::msg::SlipEvent>("slip_events", 10);
        
        prev_forces_.resize(100, 0.0);
        RCLCPP_INFO(this->get_logger(), "Slip Detector initialized. Waiting for tactile data...");
    }

private:
    void topic_callback(const tactile_interfaces::msg::TactileArray::SharedPtr msg) {
        if (!first_message_received_) {
            prev_forces_ = msg->forces;
            first_message_received_ = true;
            return;
        }

        float max_derivative = 0.0;
        float dt = 0.001;
        
        for (size_t i = 0; i < msg->forces.size(); ++i) {
            float derivative = std::abs(msg->forces[i] - prev_forces_[i]) / dt;
            if (derivative > max_derivative) {
                max_derivative = derivative;
            }
        }

        if (max_derivative > 1000.0) {
            auto slip_msg = tactile_interfaces::msg::SlipEvent();
            slip_msg.header.stamp = this->now();
            slip_msg.slip_detected = true;
            slip_msg.max_derivative = max_derivative;
            publisher_->publish(slip_msg);
            
            RCLCPP_WARN(this->get_logger(), "SLIP DETECTED! Max derivative: %.2f N/s", max_derivative);
        }

        prev_forces_ = msg->forces;
    }

    rclcpp::Subscription<tactile_interfaces::msg::TactileArray>::SharedPtr subscription_;
    rclcpp::Publisher<tactile_interfaces::msg::SlipEvent>::SharedPtr publisher_;
    std::vector<float> prev_forces_;
    bool first_message_received_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SlipDetector>());
    rclcpp::shutdown();
    return 0;
}

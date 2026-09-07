
#include <rclcpp/rclcpp.hpp>
#include "tactile_interfaces/msg/tactile_array.hpp"
#include <vector>
#include <cmath>
#include <random>

class TactileSensorDriver : public rclcpp::Node {
public:
    TactileSensorDriver() : Node("tactile_sensor_driver"), tick_count_(0) {
        rclcpp::QoS sensor_qos(10);
        sensor_qos.best_effort();
        
        publisher_ = this->create_publisher<tactile_interfaces::msg::TactileArray>("tactile_data", sensor_qos);
        
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1),
            std::bind(&TactileSensorDriver::timer_callback, this));
        
        gen_.seed(std::random_device{}());
        dist_ = std::uniform_real_distribution<float>(-0.1, 0.1);
        
        RCLCPP_INFO(this->get_logger(), "Tactile Sensor Driver started at 1000 Hz.");
    }

private:
    void timer_callback() {
        auto msg = tactile_interfaces::msg::TactileArray();
        msg.header.stamp = this->now();
        msg.header.frame_id = "prosthetic_finger_tip";
        
        msg.forces.resize(100);
        float base_force = 5.0;
        
        tick_count_++;
        // Make the spike last for 100 ticks (100ms) so the detector reliably catches it
        if (tick_count_ % 3000 < 100) {
            base_force += 50.0;
        }

        for (int i = 0; i < 100; ++i) {
            msg.forces[i] = base_force + dist_(gen_);
        }
        
        publisher_->publish(msg);
    }

    rclcpp::Publisher<tactile_interfaces::msg::TactileArray>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int tick_count_;
    std::mt19937 gen_;
    std::uniform_real_distribution<float> dist_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TactileSensorDriver>());
    rclcpp::shutdown();
    return 0;
}


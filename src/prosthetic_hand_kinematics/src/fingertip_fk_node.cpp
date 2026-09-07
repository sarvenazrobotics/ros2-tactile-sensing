#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <memory>
#include <chrono>

class FingertipFKNode : public rclcpp::Node {
public:
    FingertipFKNode() : Node("fingertip_fk_node") {
        // Create TF2 buffer and listener
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        
        // Publisher for fingertip pose
        pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
            "fingertip_pose", 10);
        
        // Timer to query TF at 100 Hz
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&FingertipFKNode::timer_callback, this));
        
        RCLCPP_INFO(this->get_logger(), "Fingertip Forward Kinematics node started.");
        RCLCPP_INFO(this->get_logger(), "Computing FK from base_link to fingertip at 100 Hz.");
    }

private:
    void timer_callback() {
        try {
            // Look up the transform from base_link to fingertip
            geometry_msgs::msg::TransformStamped transform_stamped = 
                tf_buffer_->lookupTransform(
                    "base_link",    // Target frame
                    "fingertip",    // Source frame
                    tf2::TimePointZero  // Latest available transform
                );
            
            // Extract position
            double x = transform_stamped.transform.translation.x;
            double y = transform_stamped.transform.translation.y;
            double z = transform_stamped.transform.translation.z;
            
            // Extract orientation (quaternion)
            double qx = transform_stamped.transform.rotation.x;
            double qy = transform_stamped.transform.rotation.y;
            double qz = transform_stamped.transform.rotation.z;
            double qw = transform_stamped.transform.rotation.w;
            
            // Publish as PoseStamped
            geometry_msgs::msg::PoseStamped pose_msg;
            pose_msg.header.stamp = this->now();
            pose_msg.header.frame_id = "base_link";
            pose_msg.pose.position.x = x;
            pose_msg.pose.position.y = y;
            pose_msg.pose.position.z = z;
            pose_msg.pose.orientation.x = qx;
            pose_msg.pose.orientation.y = qy;
            pose_msg.pose.orientation.z = qz;
            pose_msg.pose.orientation.w = qw;
            
            pose_publisher_->publish(pose_msg);
            
            // Log the position (optional, can be commented out for performance)
            RCLCPP_DEBUG(this->get_logger(), 
                "Fingertip pose: [%.3f, %.3f, %.3f]", x, y, z);
                
        } catch (const tf2::TransformException &ex) {
            // This is normal at startup before TF tree is fully populated
            RCLCPP_DEBUG(this->get_logger(), "Could not transform: %s", ex.what());
        }
    }
    
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FingertipFKNode>());
    rclcpp::shutdown();
    return 0;
}

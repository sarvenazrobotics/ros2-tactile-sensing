# This message is published when a slip is detected
std_msgs/Header header
bool slip_detected
# The maximum rate of change of force (derivative) that triggered the event
float32 max_derivative 

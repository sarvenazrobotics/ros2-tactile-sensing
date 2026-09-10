import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    
    pkg_leap_gazebo = FindPackageShare('leap_hand_gazebo').find('leap_hand_gazebo')
    world_path = os.path.join(pkg_leap_gazebo, 'worlds', 'leap_hand_grasp.world')
    urdf_path = os.path.join(pkg_leap_gazebo, 'urdf', 'leap_hand_gazebo.xacro')

    # 1. Launch Gazebo Harmonic
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([FindPackageShare('ros_gz_sim'), 'launch', 'gz_sim.launch.py'])
        ]),
        launch_arguments=[
            ('gz_args', f'-r -v 4 {world_path}')
        ]
    )

    # 2. Robot State Publisher (loads URDF to /robot_description)
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': open(urdf_path).read()}],
        output='screen'
    )

    # 3. Bridge ROS2 and Gazebo (for clock and joint states)
    ros_gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',
            '/joint_states@sensor_msgs/msg/JointState[gz.msgs.Model'
        ],
        output='screen'
    )

    # 4. Spawn the LEAP Hand in Gazebo
    spawn_hand = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'leap_hand',
            '-topic', '/robot_description',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.15',  # Start slightly above the box
            '-Y', '0.0'
        ],
        output='screen'
    )

    # 5. Spawn the Joint State Broadcaster
    spawn_jsp = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster'],
        output='screen'
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        ros_gz_bridge,
        spawn_hand,
        spawn_jsp,
    ])

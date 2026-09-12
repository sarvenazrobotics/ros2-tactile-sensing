import os
import xacro
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    pkg_leap_gazebo = FindPackageShare('leap_hand_gazebo').find('leap_hand_gazebo')
    world_path = os.path.join(pkg_leap_gazebo, 'worlds', 'leap_hand_grasp.world')
    xacro_path = os.path.join(pkg_leap_gazebo, 'urdf', 'leap_hand_gazebo.xacro')

    # Process Xacro
    doc = xacro.process_file(xacro_path)
    robot_desc = doc.toprettyxml(indent='  ')

    # 1. Launch Gazebo Harmonic
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([FindPackageShare('ros_gz_sim'), 'launch', 'gz_sim.launch.py'])
        ]),
        launch_arguments=[
            ('gz_args', f'-r -v 4 {world_path}'),
        ]
    )

    # 2. Robot State Publisher
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_desc, 'use_sim_time': True}],
        output='screen'
    )

    # 3. Delayed Spawn (Waits 5 seconds for Gazebo to fully initialize)
    delayed_spawn_hand = TimerAction(
        period=5.0,
        actions=[
            Node(
                package='ros_gz_sim',
                executable='create',
                arguments=[
                    '-name', 'leap_hand',
                    '-topic', '/robot_description',
                    '-x', '0.0',
                    '-y', '0.0',
                    '-z', '0.25',
                ],
                output='screen'
            )
        ]
    )

    # 4. Delayed Controller Spawner
    delayed_spawn_jsp = TimerAction(
        period=7.0,
        actions=[
            Node(
                package='controller_manager',
                executable='spawner',
                arguments=['joint_state_broadcaster'],
                output='screen'
            )
        ]
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        delayed_spawn_hand,
        delayed_spawn_jsp,
    ])

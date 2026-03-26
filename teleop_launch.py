from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    config = PathJoinSubstitution([
        FindPackageShare('robot_teleop'), 'config', 'joy_params.yaml'
    ])

    return LaunchDescription([

        Node(
            package='joy',
            executable='joy_node',
            name='joy_node',
            parameters=[config],
        ),

        Node(
            package='teleop_twist_joy',
            executable='teleop_node',
            name='teleop_twist_joy_node',
            parameters=[config],
        ),

        Node(
            package='robot_teleop',
            executable='cmd_vel_to_serial',
            name='cmd_vel_to_serial',
            parameters=[{
                'serial_port': '/dev/ttyUSB0',
                'baud_rate': 115200,
            }],
        ),

    ])
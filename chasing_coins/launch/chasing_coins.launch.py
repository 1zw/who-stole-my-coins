import os

import xacro

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction,ExecuteProcess
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from launch.actions import IncludeLaunchDescription

'''global params'''
package_name = 'chasing_coins'
pkg_share = get_package_share_directory(package_name)

def get_argument(context, arg):
    return LaunchConfiguration(arg).perform(context)
def spawn_car(context, *args, **kwargs):
    '''vehicle params'''
    namespace = get_argument(context, 'namespace')
    robot_name = get_argument(context, 'robot_name')
    vehicle_model_config = get_argument(context, "vehicleModelConfig")
    x = get_argument(context, 'x')
    y = get_argument(context, 'y') 
    z = get_argument(context, 'z')
    roll = get_argument(context, 'roll')
    pitch = get_argument(context, 'pitch')
    yaw = get_argument(context, 'yaw')
    xacro_path = os.path.join(pkg_share,
                              'urdf', 'chasing_car.urdf.xacro')
    config_file = str(os.path.join(pkg_share, 'config', vehicle_model_config))
    urdf_path = os.path.join(pkg_share,
                             'urdf','chasing_car.urdf')
    
    if not os.path.isfile(urdf_path):
        os.mknod(urdf_path)
    doc = xacro.process_file(xacro_path,
                             mappings={
                                 'robot_name': robot_name,
                                 'config_file': config_file,
                             })
    out = xacro.open_output(urdf_path)
    out.write(doc.toprettyxml(indent='  '))

    with open(urdf_path, 'r') as urdf_file:
        robot_description = urdf_file.read()

    robot_description = doc.toxml()
    # print(robot_description)
    return [
        Node(
            name='spawn_car',
            package='gazebo_ros',
            executable='spawn_entity.py',
            output='screen',
            arguments=[
                '-entity', namespace,
                '-file', urdf_path,
                '-x', x,
                '-y', y,
                '-z', z,
                '-R', roll,
                '-P', pitch,
                '-Y', yaw,
                '-spawn_service_timeout', '60.0',
                '--ros-args', '--log-level', 'warn',
            ]
        ),
        Node(
            name='joint_state_publisher',
            package='joint_state_publisher',
            executable='joint_state_publisher',
            output='screen',
            parameters=[{
                'robot_description': robot_description,
                'rate': 200,
            }],
            arguments=[urdf_path],
            remappings=[('/joint_states', '/hrt_sim/joint_states')]
        ),

        Node(
            name='robot_state_publisher',
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{
                'robot_description': robot_description,
                'rate': 200,
            }],
            remappings=[('/joint_states', '/hrt_sim/joint_states')],
            arguments=['--ros-args', '--log-level', 'warn']
        ),
    ]


def generate_launch_description():
    
    # Launch Arguments
    ld = LaunchDescription()
    ld.add_action(DeclareLaunchArgument('namespace', default_value='hrt_sim',
                            description='Namespace of the gazebo robot'))
    ld.add_action(DeclareLaunchArgument('robot_name', default_value='chasing_car',
                            description='this car is for tutorial '),)
    ld.add_action(DeclareLaunchArgument('vehicleModelConfig',
                            default_value='vehicle_config.yaml',
                            description="Determines the file from which "
                                        "the vehicle model parameters are "
                                        "read"),)
    ld.add_action(DeclareLaunchArgument('x', default_value='-2',
                            description='Vehicle initial x position'),)
    ld.add_action(DeclareLaunchArgument('y', default_value='0',
                            description='Vehicle initial y position'),)
    ld.add_action(DeclareLaunchArgument('z', default_value='0',
                            description='Vehicle initial z position'),)
    ld.add_action(DeclareLaunchArgument('roll', default_value='0',
                            description='Vehicle initial roll'),)
    ld.add_action(DeclareLaunchArgument('pitch', default_value='0',
                            description='Vehicle initial pitch'),)
    ld.add_action(DeclareLaunchArgument('yaw', default_value='0',
                            description='Vehicle initial yaw'))
    # spawn the car
    spawn_node = OpaqueFunction(function=spawn_car)
    ld.add_action(spawn_node)

    
    # 启动算分节点
    # caculate_node = Node(
    #         package='caculate',
    #         executable='caculate',
    #         name='caculate',
    #         output='log')
    # ld.add_action(caculate_node)

    # Start Gazebo server
    gazebo_world_path = pkg_share + '/worlds/coins.world'
    # gazebo_world_path = '/opt/ros/foxy/share/gazebo_plugins/worlds/gazebo_ros_skid_steer_drive_demo.world'
    start_gazebo_cmd =  ExecuteProcess(
        cmd=['gazebo', '--verbose','-s', 'libgazebo_ros_init.so', '-s', 'libgazebo_ros_factory.so', gazebo_world_path],
        output='screen')
    # start_caculate_cmd = ExecuteProcess(
    #     cmd=['ros2', 'run', 'caculate', 'caculate'],
    #     output="screen"
    # )
    ld.add_action(start_gazebo_cmd)
    return ld
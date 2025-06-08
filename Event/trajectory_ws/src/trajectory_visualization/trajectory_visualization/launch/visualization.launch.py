from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('trajectory_visualization')
    
    # 配置rviz
    rviz_config = os.path.join(pkg_share, 'rviz', 'trajectory.rviz')
    
    return LaunchDescription([
        Node(
            package='trajectory_visualization',
            executable='trajectory_node',
            name='trajectory_node',
            output='screen'
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config],
            output='screen',
            parameters=[{
                'Grid': {
                    'Plane': 'XY',
                    'Cell Size': 1.0,
                    'Color': [0.5, 0.5, 0.5],
                    'Style': 'Lines',
                    'Alpha': 0.5,
                    'Enabled': True
                },
                'Axes': {
                    'Length': 1.0,
                    'Radius': 0.1,
                    'Alpha': 0.8,
                    'Enabled': True
                }
            }]
        )
    ])
import matplotlib.pyplot as plt
import os
from datetime import datetime
import numpy as np

def plot_gps_coordinates(data_file):
    # 检查文件是否存在
    if not os.path.exists(data_file):
        print(f"Error: Data file '{data_file}' not found")
        return
    
    # 读取数据文件
    with open(data_file, 'r') as f:
        lines = f.readlines()
    
    lons = []
    lats = []
    times = []
    
    # 跳过前两行（标题行）
    for line in lines[2:]:
        parts = line.strip().split('\t')
        if len(parts) >= 19:
            try:
                # 解析时间
                time_str = parts[1].strip()  # 获取时间字符串，如" 16:04:33.381"
                if time_str:
                    # 添加日期前缀，因为数据只有时间
                    full_time_str = "2022-01-08 " + time_str
                    time_val = datetime.strptime(full_time_str, "%Y-%m-%d %H:%M:%S.%f")
                    
                    # 获取经纬度数据
                    lon = float(parts[17])  # 经度
                    lat = float(parts[18])  # 纬度
                    
                    times.append(time_val)
                    lons.append(lon)
                    lats.append(lat)
            except (ValueError, IndexError) as e:
                print(f"Warning: Skipping line due to error - {e} | Line: {line.strip()}")
                continue
    
    # 检查数据是否有效
    if not lons or not lats:
        print("Error: No valid GPS data found in the file")
        return
    
    # 创建包含两个子图的图形，并设置样式
    plt.style.use('seaborn')  # 使用更现代的绘图样式
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    
    # 隐藏工具栏
    plt.rcParams['toolbar'] = 'None'
    
    # 子图1: 经纬度路径图 - 使用相对坐标
    lon_center = np.mean(lons)
    lat_center = np.mean(lats)
    rel_lons = [(lon - lon_center) * 100000 for lon in lons]  # 转换为米级别的相对位移
    rel_lats = [(lat - lat_center) * 100000 for lat in lats]  # 转换为米级别的相对位移
    
    ax1.plot(rel_lons, rel_lats, 'r-', linewidth=1.5, alpha=0.8)
    ax1.set_xlabel('Relative Longitude (m)')
    ax1.set_ylabel('Relative Latitude (m)')
    ax1.set_title('GPS Trajectory (relative to center)', pad=10)
    ax1.grid(True, linestyle='--', alpha=0.7)
    
    # 添加起点和终点标记
    ax1.plot(rel_lons[0], rel_lats[0], 'go', label='start', markersize=10)
    ax1.plot(rel_lons[-1], rel_lats[-1], 'ro', label='end', markersize=10)
    ax1.legend()
    
    # 子图2: 经纬度随时间变化图
    # 转换时间为相对秒数
    start_time = min(times)
    rel_times = [(t - start_time).total_seconds() for t in times]
    
    # 使用相对坐标
    ax2.plot(rel_times, rel_lons, 'b-', label='longitude', linewidth=1.5)
    ax2.plot(rel_times, rel_lats, 'g-', label='latitude', linewidth=1.5)
    ax2.set_xlabel('Time (seconds)')
    ax2.set_ylabel('Relative Position (m)')
    ax2.set_title('Position Change Over Time', pad=10)
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.legend()
    
    # 调整布局
    plt.tight_layout()
    
    # 显示图形
    plt.show()
    
    # 打印统计信息
    print(f"\n数据统计信息:")
    print(f"数据点数量: {len(times)}")
    print(f"经度范围: {min(lons):.6f}°E - {max(lons):.6f}°E (变化量: {(max(lons)-min(lons))*100000:.2f}m)")
    print(f"纬度范围: {min(lats):.6f}°N - {max(lats):.6f}°N (变化量: {(max(lats)-min(lats))*100000:.2f}m)")
    print(f"时间范围: {min(times).strftime('%H:%M:%S')} - {max(times).strftime('%H:%M:%S')} (持续时间: {max(rel_times):.1f}秒)")

if __name__ == "__main__":
    # 直接指定的数据文件路径
    default_data_file = "/home/ros2/ROS2Folder/Event/data.txt"
    import sys
    
    # 优先使用命令行参数，如果没有参数则使用默认路径
    if len(sys.argv) > 1:
        full_path = ' '.join(sys.argv[1:])
        plot_gps_coordinates(full_path)
    else:
        print(f"使用默认路径: {default_data_file}")
        plot_gps_coordinates(default_data_file)

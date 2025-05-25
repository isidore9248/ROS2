#include "../include/IMU_pkg/IMUSerialReader.h"

#include <unistd.h>

IMUSerialReader::IMUSerialReader(std::string serial_port, int serial_baud,
                                 std::string node_name)
    : Node(node_name)
{
    // 声明参数
    this->declare_parameter<std::string>("port", serial_port);
    this->declare_parameter<int>("baudrate", serial_baud);
    this->declare_parameter<std::string>("frame_id", "imu_link");

    // 获取参数
    port_ = this->get_parameter("port").as_string();
    baudrate_ = this->get_parameter("baudrate").as_int();
    frame_id_ = this->get_parameter("frame_id").as_string();

    // 初始化串口
    try
    {
        io_context_ = std::make_unique<IoContext>();
        port_config_ = std::make_unique<drivers::serial_driver::SerialPortConfig>(
            baudrate_,
            drivers::serial_driver::FlowControl::NONE,
            drivers::serial_driver::Parity::NONE,
            drivers::serial_driver::StopBits::ONE);

        serial_driver_ = std::make_unique<drivers::serial_driver::SerialDriver>(*io_context_);
        serial_driver_->init_port(port_, *port_config_);
        serial_driver_->port(); // 修改: 使用port()替代open()

        RCLCPP_INFO(this->get_logger(), "Serial port opened: %s at %d baud",
                    port_.c_str(), baudrate_);
    }
    catch (const std::exception &e)
    {
        RCLCPP_FATAL(this->get_logger(), "Failed to open serial port: %s", e.what());
        rclcpp::shutdown();
        return;
    }

    // 创建IMU数据发布者
    imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", 10);

    // 创建TF广播器
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // 创建定时器读取串口数据
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(&IMUSerialReader::readSerialData, this));
}

IMUSerialReader::~IMUSerialReader()
{
    if (serial_driver_ && serial_driver_->port()->is_open())
    {
        serial_driver_->port()->close();
    }
}

// ...existing code...

void IMUSerialReader::readSerialData()
{
    std::vector<uint8_t> buffer(1024); // 创建一个1024字节的缓冲区
    size_t bytes_read = 0;

    try
    {
        bytes_read = serial_driver_->port()->receive(buffer);

        if (bytes_read > 0)
        {
            // 将接收到的数据转换为字符串
            std::string received_data(buffer.begin(), buffer.begin() + bytes_read);
            serial_buffer_ += received_data;

            // 打印接收到的原始数据
            RCLCPP_INFO(this->get_logger(), "Received raw data: %s", received_data.c_str());

            // 处理接收到的数据
            //processSerialData();
        }
    }
    catch (const std::exception &e)
    {
        RCLCPP_ERROR(this->get_logger(), "Error reading from serial port: %s", e.what());
    }
}

void IMUSerialReader::processSerialData()
{
    // 查找数据包的结束符（这里假设使用换行符）
    size_t pos = serial_buffer_.find('\n');

    while (pos != std::string::npos)
    {
        // 提取一行数据
        std::string line = serial_buffer_.substr(0, pos);

        // 从缓冲区中删除已处理的数据
        serial_buffer_ = serial_buffer_.substr(pos + 1);

        // 打印处理后的数据行
        RCLCPP_INFO(this->get_logger(), "Processed data line: %s", line.c_str());

        // 继续查找下一个数据包
        pos = serial_buffer_.find('\n');
    }
}

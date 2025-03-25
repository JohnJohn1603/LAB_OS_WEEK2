<!-- Ý TƯỞNG -->
<!-- Sử dụng một hàng đợi thông điệp duy nhất để trao đổi dữ liệu giữa hai tiến trình.
Dùng luồng gửi để gửi thông điệp.
Dùng luồng nhận để đọc thông điệp.
Đảm bảo hai tiến trình có thể gửi và nhận qua lại. -->

<!-- TRIỂN KHAI
Tạo Message Queue
    Cả hai tiến trình dùng msgget(MSG_KEY, 0666 | IPC_CREAT) để lấy message queue.
Gửi và Nhận bằng hai luồng
    Luồng gửi (send_msg) cho phép người dùng nhập tin nhắn và gửi.
    Luồng nhận (receive_msg) chờ và in tin nhắn nhận được.
Giao tiếp hai chiều bằng message type
    Tiến trình 1 gửi tin nhắn với msg_type = 1.
    Tiến trình 2 nhận tin nhắn với msg_type = 1.
    Tiến trình 2 gửi phản hồi với msg_type = 2.
    Tiến trình 1 nhận phản hồi với msg_type = 2.
Thoát khi nhập "exit"
    Nếu một tiến trình nhập "exit", chương trình sẽ dừng. -->
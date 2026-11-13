# HỆ THỐNG ĐÈN GIAO THÔNG 2 TUYẾN ĐƯỜNG (XNUCLEO-F103RB)

Đồ án Bài tập lớn môn **Vi Xử Lý – Vi Điều Khiển (CO3009)** – Trường Đại học Bách Khoa (ĐHQG-HCM).  
Hệ thống điều khiển đèn giao thông ngã tư 2 tuyến đường tích hợp cảm biến môi trường DHT20, hiển thị LCD 16x2 I2C và vận hành hoàn toàn trên bộ lập lịch đa nhiệm hợp tác độ phức tạp $O(1)$.

---

## 1. Tính năng chính

* **Bộ lập lịch đa nhiệm $O(1)$ (Delta List Scheduler):** Quản lý các tác vụ định kỳ và tác vụ chạy một lần (one-shot task) với nhịp hệ thống `10 ms` (TIM2), hàm `SCH_Update()` đạt độ phức tạp thời gian $O(1)$.
* **Chế độ Tự động (Automatic):** Điều phối luân phiên 4 pha đèn giữa 2 tuyến đường (`Đỏ 1 - Xanh 2` $\rightarrow$ `Đỏ 1 - Vàng 2` $\rightarrow$ `Xanh 1 - Đỏ 2` $\rightarrow$ `Vàng 1 - Đỏ 2`), hiển thị đồng hồ đếm ngược thời gian thực lên LCD.
* **Chế độ Bằng tay (Manual):** Cho phép người điều khiển chuyển đổi thủ công từng trạng thái đèn của ngã tư qua nút nhấn, đảm bảo nguyên tắc an toàn không bao giờ bật xanh đồng thời 2 tuyến.
* **Chế độ Chỉnh thời gian (Tuning):** Cho phép tùy chỉnh thời gian đèn Đỏ, Vàng, Xanh (hỗ trợ nhấn giữ để tăng nhanh) và tự động cân bằng ràng buộc vật lý $T_{\text{Đỏ}} = T_{\text{Xanh}} + T_{\text{Vàng}}$.
* **Giám sát môi trường:** Đọc nhiệt độ và độ ẩm từ cảm biến **DHT20** qua giao tiếp I2C (chu kỳ 2 giây, cơ chế non-blocking) và hiển thị trực tiếp lên màn hình LCD.

---

## 2. Kết nối phần cứng (XNUCLEO-F103RB + Grove Base Shield)

| Thiết bị | Khe cắm Grove | Chân STM32F103RB | Chế độ cấu hình | User Label |
| :--- | :--- | :--- | :--- | :--- |
| Đèn giao thông tuyến 1 (Tín hiệu A, B) | `D3 - D4` | `PB3`, `PB5` | `GPIO_Output` | `TL1_A`, `TL1_B` |
| Đèn giao thông tuyến 2 (Tín hiệu A, B) | `D5 - D6` | `PB4`, `PB10` | `GPIO_Output` | `TL2_A`, `TL2_B` |
| Cụm nút nhấn 1 (Nút 0, Nút 1) | `D7 - D8` | `PA8`, `PA9` | `GPIO_Input` | `BTN_1`, `BTN_2` |
| Cụm nút nhấn 2 (Nút 2, Nút 3) | `D9 - D10` | `PC7`, `PB6` | `GPIO_Input` | `BTN_3`, `BTN_4` |
| LCD 16x2 I2C & Cảm biến DHT20 | `I2C` | `PB8` (SCL), `PB9` (SDA) | `I2C1` (100 kHz) | `I2C1_SCL`, `I2C1_SDA` |

---

## 3. Cấu trúc thư mục mã nguồn

Mã nguồn được tổ chức theo kiến trúc phân lớp rõ ràng, tách biệt hoàn toàn lớp điều khiển phần cứng (`HAL`) khỏi tầng logic máy trạng thái (`FSM`):

```text
├── Core/
│   ├── Inc/
│   │   ├── main.h              # Định nghĩa chân GPIO và cấu hình ngoại vi
│   │   ├── sch_port.h          # Giao tiếp vùng găng (Critical Section) cho Scheduler
│   │   ├── scheduler.h         # Bộ lập lịch đa nhiệm Delta List O(1)
│   │   ├── software_timer.h    # Bộ định thời mềm dùng cho các máy trạng thái
│   │   ├── button.h            # Driver đọc và chống dội 4 nút nhấn (10 ms)
│   │   ├── traffic_light.h     # Driver điều khiển module đèn giao thông 2-bit
│   │   ├── lcd.h               # Driver giao tiếp LCD 16x2 qua I2C
│   │   ├── dht20.h             # Driver đọc nhiệt độ, độ ẩm từ cảm biến DHT20
│   │   ├── global.h            # Biến trạng thái và dữ liệu dùng chung toàn cục
│   │   ├── fsm_automatic.h     # Máy trạng thái chế độ Tự động
│   │   ├── fsm_manual.h        # Máy trạng thái chế độ Bằng tay
│   │   └── fsm_tuning.h        # Máy trạng thái chế độ Chỉnh thời gian
│   └── Src/
│       └── ...                 # Các file hiện thực (.c) tương ứng
└── BTL_VXL.ioc                 # File cấu hình vi điều khiển STM32CubeMX
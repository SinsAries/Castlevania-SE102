def calculate_anchor_points():
    """
    Một tool dòng lệnh đơn giản để tính toán anchor points cho game.
    """
    print("--- Anchor Point Calculator ---")
    print("Công cụ giúp tính anchorX, anchorY một cách nhanh chóng.")
    print("Nhập 'exit' bất cứ lúc nào để thoát.\n")

    while True:
        # --- Nhập tọa độ góc trên-trái của Sprite ---
        sprite_coords_str = input("=> Nhập tọa độ GÓC TRÊN-TRÁI của sprite từ JSON (left top): ")
        if sprite_coords_str.lower() == 'exit':
            break

        # --- Nhập tọa độ tuyệt đối của bàn tay ---
        hand_coords_str = input("=> Nhập tọa độ TUYỆT ĐỐI của bàn tay từ Paint.NET (hand_x hand_y): ")
        if hand_coords_str.lower() == 'exit':
            break
            
        try:
            # Tách chuỗi nhập vào thành các con số
            sprite_left, sprite_top = map(int, sprite_coords_str.split())
            hand_x, hand_y = map(int, hand_coords_str.split())
            
            # --- Thực hiện phép tính cốt lõi ---
            anchor_x = hand_x - sprite_left
            anchor_y = hand_y - sprite_top
            
            # --- In kết quả ---
            print("\n--- KẾT QUẢ ---")
            print(f"  anchorX tính được: {anchor_x}")
            print(f"  anchorY tính được: {anchor_y}")
            print(f'  Copy và paste vào file JSON: "anchorX": {anchor_x}, "anchorY": {anchor_y}')
            print("----------------\n")

        except ValueError:
            print("\n[LỖI] Nhập không hợp lệ. Vui lòng nhập hai số nguyên cách nhau bởi dấu cách.\n")
            continue
        except Exception as e:
            print(f"\n[LỖI] Đã có lỗi xảy ra: {e}\n")
            continue

    print("Đã thoát. Chúc bạn làm việc hiệu quả!")

# Chạy tool
if __name__ == "__main__":
    calculate_anchor_points()
import json

def convert_assets_file(input_filename='assets.json', output_filename='assets_new.json'):
    """
    Đọc file assets.json với định dạng cũ và chuyển nó sang định dạng mới
    với mảng "frames" và các điểm gắn giữ chỗ cho animation tấn công.
    """
    try:
        # Mở và đọc file JSON gốc
        with open(input_filename, 'r', encoding='utf-8') as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"Lỗi: Không tìm thấy file '{input_filename}'.")
        return
    except json.JSONDecodeError as e:
        print(f"Lỗi khi đọc file JSON '{input_filename}': {e}")
        return

    if "animations" not in data:
        print("Lỗi: Không tìm thấy key 'animations' trong file JSON.")
        return
        
    original_animations = data["animations"]
    new_animations_data = {}

    # Lặp qua từng animation (ví dụ: "SimonWalkRight")
    for anim_name, anim_data in original_animations.items():
        new_anim_info = {}
        
        # Sao chép giá trị defaultFrameTime qua
        if "defaultFrameTime" in anim_data:
            new_anim_info["defaultFrameTime"] = anim_data["defaultFrameTime"]
            
        new_frames_list = []
        if "spriteIds" in anim_data:
            # Lặp qua danh sách các spriteId của animation này
            for sprite_id in anim_data["spriteIds"]:
                frame_obj = {"spriteId": sprite_id}
                
                # Nếu tên animation có chứa "Attack", thêm các điểm gắn giữ chỗ
                if "Attack" in anim_name:
                    frame_obj["anchorX"] = 0
                    frame_obj["anchorY"] = 0
                    
                new_frames_list.append(frame_obj)
        
        new_anim_info["frames"] = new_frames_list
        new_animations_data[anim_name] = new_anim_info

    # Thay thế đối tượng animations cũ bằng đối tượng mới đã được chuyển đổi
    data["animations"] = new_animations_data

    # Ghi dữ liệu mới vào file output với định dạng đẹp
    with open(output_filename, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=4)
        
    print(f"Chuyển đổi thành công '{input_filename}' -> '{output_filename}'.")
    print("Bước tiếp theo: Mở file mới và cập nhật giá trị 'anchorX', 'anchorY' cho các frame tấn công.")

# Chạy hàm chuyển đổi
if __name__ == "__main__":
    convert_assets_file()
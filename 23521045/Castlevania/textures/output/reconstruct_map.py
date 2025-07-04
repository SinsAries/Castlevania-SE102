import json
from PIL import Image
import os

# --- Cấu hình ---
TILE_WIDTH = 32
TILE_HEIGHT = 32

def reconstruct_map_from_output(input_folder: str, output_file_path: str):
    """
    Tái tạo lại ảnh bản đồ gốc từ file tileset và file map data.

    Args:
        input_folder (str): Thư mục chứa file 'tileset.png' và 'map_data.json'.
        output_file_path (str): Đường dẫn để lưu file ảnh được tái tạo.
    """
    tileset_path = os.path.join(input_folder, "tileset.png")
    map_data_path = os.path.join(input_folder, "map_data.json")

    # --- Bước 1: Đọc các file input ---
    try:
        print(f"Đang đọc map data từ '{map_data_path}'...")
        with open(map_data_path, 'r') as f:
            map_data = json.load(f)

        print(f"Đang đọc tileset từ '{tileset_path}'...")
        tileset_image = Image.open(tileset_path)

    except FileNotFoundError as e:
        print(f"Lỗi: Không tìm thấy file cần thiết. Hãy chắc chắn file '{e.filename}' tồn tại.")
        return
    
    if not map_data:
        print("Lỗi: File map data rỗng.")
        return

    # --- Bước 2: Tạo ảnh mới để vẽ lại bản đồ ---
    # Lấy kích thước bản đồ theo số lượng tile
    map_height_in_tiles = len(map_data)
    map_width_in_tiles = len(map_data[0]) # Giả sử tất cả các hàng có cùng độ dài

    # Tính toán kích thước ảnh theo pixel
    map_pixel_width = map_width_in_tiles * TILE_WIDTH
    map_pixel_height = map_height_in_tiles * TILE_HEIGHT

    print(f"Bản đồ có kích thước {map_width_in_tiles}x{map_height_in_tiles} tiles.")
    print(f"Đang tạo ảnh mới kích thước {map_pixel_width}x{map_pixel_height} pixels.")
    
    # Tạo ảnh mới với mode của tileset (thường là RGBA)
    reconstructed_image = Image.new(tileset_image.mode, (map_pixel_width, map_pixel_height))

    # --- Bước 3: Quét qua map data và vẽ lại bản đồ ---
    print("Bắt đầu tái tạo bản đồ...")
    for y_index, row in enumerate(map_data):
        for x_index, tile_index in enumerate(row):
            # Tọa độ để cắt tile từ file tileset.png
            # Vì tileset là 1 hàng ngang, tọa độ y luôn là 0.
            tile_x_pos_in_tileset = tile_index * TILE_WIDTH
            
            box = (
                tile_x_pos_in_tileset, 
                0, 
                tile_x_pos_in_tileset + TILE_WIDTH, 
                TILE_HEIGHT
            )
            
            # Cắt tile ra
            tile_to_paste = tileset_image.crop(box)

            # Tọa độ để dán tile vào ảnh đang tái tạo
            paste_x_pos = x_index * TILE_WIDTH
            paste_y_pos = y_index * TILE_HEIGHT

            # Dán tile vào
            reconstructed_image.paste(tile_to_paste, (paste_x_pos, paste_y_pos))

    # --- Bước 4: Lưu ảnh kết quả ---
    reconstructed_image.save(output_file_path)
    print(f"\nHoàn thành! Đã lưu ảnh tái tạo tại: '{output_file_path}'")


if __name__ == "__main__":
    # Thư mục chứa 'tileset.png' và 'map_data.json'
    INPUT_FOLDER = "" 
    
    # Tên file ảnh sẽ được tạo ra
    OUTPUT_IMAGE = "reconstructed_map.png"

    reconstruct_map_from_output(INPUT_FOLDER, OUTPUT_IMAGE)
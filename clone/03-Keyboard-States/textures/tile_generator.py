import json
from PIL import Image
import os
import math

# === Cấu hình mới ===
# ID của tileset, sẽ được ghi vào file JSON
TILESET_ID = "TILESET_1" 
# File tileset.png đầu ra sẽ có bao nhiêu cột?
TILESET_COLS = 16 

TILE_WIDTH = 32
TILE_HEIGHT = 32

def create_tileset_and_map_advanced(input_image_path: str, output_folder: str):
    try:
        original_image = Image.open(input_image_path).convert('RGBA')
        img_width, img_height = original_image.size
        print(f"Đã đọc ảnh '{input_image_path}' với kích thước {img_width}x{img_height}.")
        if img_width % TILE_WIDTH != 0 or img_height % TILE_HEIGHT != 0:
            print("Cảnh báo: Kích thước ảnh không chia hết cho 32x32!")
    except FileNotFoundError:
        print(f"Lỗi: Không tìm thấy file ảnh tại '{input_image_path}'")
        return

    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # TẠO TILE RỖNG MẪU (32x32 hoàn toàn trong suốt)
    # Đây là cơ sở để nhận diện các ô trống trong map
    empty_tile = Image.new('RGBA', (TILE_WIDTH, TILE_HEIGHT), (0, 0, 0, 0))
    empty_tile_bytes = empty_tile.tobytes()

    unique_tiles_map = {}
    unique_tiles_list = []
    map_layout = []
    
    print("Bắt đầu quét ảnh và tạo map layout...")
    
    for y in range(0, img_height, TILE_HEIGHT):
        map_row = []
        for x in range(0, img_width, TILE_WIDTH):
            box = (x, y, x + TILE_WIDTH, y + TILE_HEIGHT)
            tile = original_image.crop(box)
            tile_bytes = tile.tobytes()

            # So sánh với tile rỗng
            if tile_bytes == empty_tile_bytes:
                map_row.append(-1) # Nếu là tile rỗng, gán index là -1
                continue

            # Nếu không phải tile rỗng, xử lý như bình thường
            if tile_bytes not in unique_tiles_map:
                new_index = len(unique_tiles_list)
                unique_tiles_map[tile_bytes] = new_index
                unique_tiles_list.append(tile)
                map_row.append(new_index)
            else:
                index = unique_tiles_map[tile_bytes]
                map_row.append(index)
        
        map_layout.append(map_row)
    
    print("Quét ảnh hoàn tất!")
    num_unique_tiles = len(unique_tiles_list)
    print(f"Tìm thấy tổng cộng {num_unique_tiles} tiles duy nhất (không tính tile rỗng).")

    if not unique_tiles_list:
        print("Không tìm thấy tile nào để tạo tileset.")
    else:
        # Sắp xếp tileset thành lưới 2D
        tileset_cols = min(TILESET_COLS, num_unique_tiles)
        tileset_rows = math.ceil(num_unique_tiles / tileset_cols)
        
        tileset_width = tileset_cols * TILE_WIDTH
        tileset_height = tileset_rows * TILE_HEIGHT
        
        tileset_image = Image.new('RGBA', (tileset_width, tileset_height))
        print(f"Đang tạo ảnh tileset dạng lưới {tileset_cols}x{tileset_rows}...")

        for i, tile_image in enumerate(unique_tiles_list):
            # Tính toán vị trí (cột, hàng) trong lưới tileset
            col = i % tileset_cols
            row = i // tileset_cols
            position = (col * TILE_WIDTH, row * TILE_HEIGHT)
            tileset_image.paste(tile_image, position)

        tileset_path = os.path.join(output_folder, "tileset.png")
        tileset_image.save(tileset_path)
        print(f"Đã lưu ảnh tileset tại: '{tileset_path}'")

    # Tạo cấu trúc JSON mới
    output_json = {
        "tile_map": {
            "tileset_texture_id": TILESET_ID,
            "tile_width": TILE_WIDTH,
            "tile_height": TILE_HEIGHT,
            "layout": map_layout
        }
    }

    map_data_path = os.path.join(output_folder, "map_data.json")
    with open(map_data_path, 'w') as f:
        json.dump(output_json, f, indent=2) # indent=2 cho gọn hơn
    print(f"Đã lưu map data theo format mới tại: '{map_data_path}'")
    print("\nHoàn thành!")

if __name__ == "__main__":
    INPUT_IMAGE = "level1-stage1.png"
    OUTPUT_FOLDER = "output1"
    create_tileset_and_map_advanced(INPUT_IMAGE, OUTPUT_FOLDER)
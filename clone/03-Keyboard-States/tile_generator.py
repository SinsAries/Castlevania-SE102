import json
from PIL import Image
import os

# --- Cấu hình ---
TILE_WIDTH = 32
TILE_HEIGHT = 32

def create_tileset_and_map(input_image_path: str, output_folder: str):
    """
    Tạo ra một file tileset và một file map data (JSON) từ ảnh đầu vào.

    Args:
        input_image_path (str): Đường dẫn tới file ảnh bản đồ cần xử lý.
        output_folder (str): Thư mục để lưu các file output.
    """
    try:
        # Mở ảnh gốc
        original_image = Image.open(input_image_path)
        img_width, img_height = original_image.size
        print(f"Đã đọc ảnh '{input_image_path}' với kích thước {img_width}x{img_height}.")

        # Kiểm tra xem kích thước ảnh có chia hết cho kích thước tile không
        if img_width % TILE_WIDTH != 0 or img_height % TILE_HEIGHT != 0:
            print("Cảnh báo: Kích thước ảnh không chia hết cho 32x32!")

    except FileNotFoundError:
        print(f"Lỗi: Không tìm thấy file ảnh tại '{input_image_path}'")
        return

    # Tạo thư mục output nếu chưa tồn tại
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
        print(f"Đã tạo thư mục output: '{output_folder}'")

    # Dùng dictionary để lưu các tile duy nhất và chỉ số của chúng
    # key = dữ liệu bytes của tile, value = chỉ số (index)
    unique_tiles_map = {}
    
    # Dùng list để lưu các đối tượng Image của tile duy nhất theo đúng thứ tự index
    unique_tiles_list = []

    map_data = [] # Mảng 2D để lưu map data (chứa các index)
    
    print("Bắt đầu quét ảnh và tạo map data...")
    # Quét qua toàn bộ ảnh gốc theo từng ô 32x32
    for y in range(0, img_height, TILE_HEIGHT):
        map_row = [] # Một hàng trong map data
        for x in range(0, img_width, TILE_WIDTH):
            # Cắt ra một ô tile
            box = (x, y, x + TILE_WIDTH, y + TILE_HEIGHT)
            tile = original_image.crop(box)

            # Chuyển dữ liệu ảnh của tile thành bytes để làm key cho dictionary
            # Đây là cách hiệu quả để kiểm tra sự trùng lặp
            tile_bytes = tile.tobytes()

            # Nếu tile này chưa từng xuất hiện
            if tile_bytes not in unique_tiles_map:
                # Lấy chỉ số mới cho tile này
                new_index = len(unique_tiles_list)
                # Lưu chỉ số vào map
                unique_tiles_map[tile_bytes] = new_index
                # Thêm đối tượng Image vào list
                unique_tiles_list.append(tile)
                # Thêm chỉ số vào hàng của map data
                map_row.append(new_index)
            else:
                # Nếu tile đã có, lấy lại chỉ số của nó
                index = unique_tiles_map[tile_bytes]
                map_row.append(index)
        
        # Thêm hàng vào map data tổng
        map_data.append(map_row)
    
    print("Quét ảnh hoàn tất!")
    print(f"Tìm thấy tổng cộng {len(unique_tiles_list)} tiles duy nhất.")

    # --- Tạo ảnh Tileset ---
    if not unique_tiles_list:
        print("Không tìm thấy tile nào để tạo tileset.")
        return

    # Tạo một ảnh mới để chứa tất cả các tile duy nhất (xếp thành 1 hàng ngang)
    tileset_width = len(unique_tiles_list) * TILE_WIDTH
    tileset_height = TILE_HEIGHT
    
    # Sử dụng mode của ảnh gốc để giữ nguyên các kênh màu (ví dụ: 'RGBA' cho ảnh có độ trong suốt)
    tileset_image = Image.new(original_image.mode, (tileset_width, tileset_height))

    print("Đang tạo ảnh tileset...")
    # Dán từng tile duy nhất vào ảnh tileset
    for i, tile_image in enumerate(unique_tiles_list):
        position = (i * TILE_WIDTH, 0)
        tileset_image.paste(tile_image, position)

    # Lưu ảnh tileset
    tileset_path = os.path.join(output_folder, "tileset.png")
    tileset_image.save(tileset_path)
    print(f"Đã lưu ảnh tileset tại: '{tileset_path}'")

    # --- Lưu file Map Data ---
    map_data_path = os.path.join(output_folder, "map_data.json")
    with open(map_data_path, 'w') as f:
        json.dump(map_data, f, indent=4)
    print(f"Đã lưu map data tại: '{map_data_path}'")
    print("\nHoàn thành!")


if __name__ == "__main__":
    # Đặt tên file ảnh bản đồ của bạn ở đây
    INPUT_IMAGE = "./textures/level1_stage0.png" 
    
    # Đặt tên thư mục chứa kết quả
    OUTPUT_FOLDER = "output"

    create_tileset_and_map(INPUT_IMAGE, OUTPUT_FOLDER)
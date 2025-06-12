from PIL import Image
import os
import json
import numpy as np

def is_similar(tile1, tile2, threshold=0.95):
    # So sánh pixel theo tỉ lệ giống nhau
    arr1 = np.array(tile1)
    arr2 = np.array(tile2)

    if arr1.shape != arr2.shape:
        return False

    total_pixels = arr1.size
    same_pixels = np.sum(arr1 == arr2)
    similarity = same_pixels / total_pixels

    return similarity >= threshold

def process_tileset(image_path, output_image='./resource/Scene2/tilesettt.png', output_json='./resource/Scene2/mapoo.json', tile_size=32, tile_start_id=92000, similarity_threshold=0.90):
    image = Image.open(image_path).convert("RGBA")
    width, height = image.size

    tiles = []
    map_data = []
    unique_tiles = []  # Lưu tile ảnh để so sánh

    tile_id = tile_start_id

    for y in range(0, height, tile_size):
        row = []
        for x in range(0, width, tile_size):
            box = (x, y, x + tile_size, y + tile_size)
            tile = image.crop(box)

            matched_index = None
            for idx, prev_tile in enumerate(unique_tiles):
                if is_similar(tile, prev_tile, threshold=similarity_threshold):
                    matched_index = idx
                    break

            if matched_index is not None:
                row.append(matched_index)
            else:
                unique_tiles.append(tile)
                tiles.append([tile_id, x, y, x + tile_size, y + tile_size])
                row.append(len(tiles) - 1)
                tile_id += 1

        map_data.append(row)

    # Lưu lại tileset nguyên bản
    image.save(output_image)

    # Lưu JSON metadata
    output = {
        "tiles": tiles,
        "map": map_data
    }

    with open(output_json, "w") as f:
        json.dump(output, f, indent=4)

    print(f"[DONE] Saved tileset to '{output_image}' and map to '{output_json}'")
    print(f"Total unique tiles (90%+ diff threshold): {len(tiles)}")

if __name__ == "__main__":
    image_path = "./resource/Scene2/Scene2.PNG"  # <-- Thay bằng path thật
    if not os.path.isfile(image_path):
        print(f"[ERROR] Image '{image_path}' not found.")
    else:
        process_tileset(image_path)
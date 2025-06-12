from PIL import Image

# Mở ảnh gốc
original_image = Image.open("Scene3/Scene3.png")

# Lấy kích thước ảnh gốc
width, height = original_image.size

# Tính kích thước mới (một nửa)
new_size = (width // 2, height // 2)

# Tạo ảnh thu nhỏ
resized_image = original_image.resize(new_size, Image.Resampling.LANCZOS)

# Lưu ảnh mới
resized_image.save("Scene3/Scene3_small.png", "PNG")
import cv2
import numpy as np

# Đọc ảnh gốc
image = cv2.imread("./resource files/spritesheet.png")

# Lật ảnh theo chiều ngang
flipped = cv2.flip(image, 1)

# Ghép hai ảnh lại theo chiều ngang
merged = np.hstack((image, flipped))

# Lưu ảnh kết quả
cv2.imwrite("spritesheet_mirrored.png", merged)
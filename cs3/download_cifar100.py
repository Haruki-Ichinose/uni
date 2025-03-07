import os
from tensorflow.keras.datasets import cifar100
from PIL import Image

# 保存するフォルダ
output_dir = "input_images"
os.makedirs(output_dir, exist_ok=True)

# CIFAR-100データセットをロード
(x_train, y_train), (x_test, y_test) = cifar100.load_data()

# 保存する画像の枚数
num_images = 50000  # 必要に応じて変更

# データセットから画像を保存
for i in range(num_images):
    # 画像データ（RGB形式）
    img = Image.fromarray(x_train[i])
    # ファイル名を作成
    file_name = f"image_{i+1}.png"
    # 保存
    img.save(os.path.join(output_dir, file_name))
    print(f"Saved: {file_name}")

print(f"Successfully saved {num_images} images to {output_dir}")

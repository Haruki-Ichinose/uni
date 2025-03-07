import time
import csv
import numpy as np
from PIL import Image, ImageFilter
from concurrent.futures import ProcessPoolExecutor
import os

# Sobelフィルタによるエッジ検出関数
def sobel_edge_detection(image_path, output_dir):
    try:
        with Image.open(image_path) as img:
            # 画像をグレースケールに変換
            gray_img = img.convert("L")
            # NumPy配列に変換
            img_array = np.array(gray_img)
            
            # Sobelフィルタを適用
            sobel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])  # 水平方向
            sobel_y = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])  # 垂直方向
            
            # 画像の幅と高さを取得
            height, width = img_array.shape
            
            # 結果を格納するための配列
            edge_array = np.zeros_like(img_array)
            
            # Sobelフィルタを適用（エッジを計算）
            for i in range(1, height-1):
                for j in range(1, width-1):
                    # 水平方向と垂直方向の勾配を計算
                    gx = np.sum(sobel_x * img_array[i-1:i+2, j-1:j+2])
                    gy = np.sum(sobel_y * img_array[i-1:i+2, j-1:j+2])
                    # 勾配の大きさを計算（エッジの強度）
                    edge_array[i, j] = np.sqrt(gx**2 + gy**2)
            
            # 結果を画像として保存
            edge_img = Image.fromarray(np.uint8(edge_array))
            
            # プロセスIDをファイル名に追加して、一意の名前にする
            output_path = os.path.join(output_dir, f"{os.getpid()}_{os.path.basename(image_path)}")
            edge_img.save(output_path)
    except Exception as e:
        print(f"Error processing {image_path}: {e}")

# 並列処理
def process_images_in_parallel(input_dir, output_dir, num_images=None, num_workers=1):
    image_files = [os.path.join(input_dir, file) for file in os.listdir(input_dir) if file.endswith(('.png', '.jpg', '.jpeg'))]
    if num_images is not None:
        image_files = image_files[:num_images]
    os.makedirs(output_dir, exist_ok=True)

    start_time = time.time()
    with ProcessPoolExecutor(max_workers=num_workers) as executor:
        futures = [executor.submit(sobel_edge_detection, image_path, output_dir) for image_path in image_files]
        # 全てのタスクが終了するのを待つ
        for future in futures:
            future.result()  # 終了を待つ
    end_time = time.time()

    return end_time - start_time

# 逐次処理
def process_images_sequentially(input_dir, output_dir, num_images=None):
    image_files = [os.path.join(input_dir, file) for file in os.listdir(input_dir) if file.endswith(('.png', '.jpg', '.jpeg'))]
    if num_images is not None:
        image_files = image_files[:num_images]
    os.makedirs(output_dir, exist_ok=True)

    start_time = time.time()
    for image_path in image_files:
        sobel_edge_detection(image_path, output_dir)
    end_time = time.time()

    return end_time - start_time

# データ収集
def collect_data(input_dir, num_images_list, workers_list, output_dir_parallel, output_dir_sequential):
    results = []
    for num_images in num_images_list:
        for workers in workers_list:
            # 並列化の場合
            parallel_time = process_images_in_parallel(input_dir, output_dir_parallel, num_images=num_images, num_workers=workers)
            results.append(["Parallel", num_images, workers, parallel_time])

        # 逐次処理の場合
        sequential_time = process_images_sequentially(input_dir, output_dir_sequential, num_images=num_images)
        results.append(["Sequential", num_images, None, sequential_time])

    # 結果をCSVファイルに保存（ファイル名にタイムスタンプを追加）
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    with open(f'execution_times_{timestamp}.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["Method", "Num Images", "Workers", "Execution Time (s)"])
        writer.writerows(results)

# 実行
if __name__ == "__main__":
    input_directory = "input_images"
    output_directory_parallel = "output_images_parallel"
    output_directory_sequential = "output_images_sequential"
    
    # 画像数とワーカープロセス数の組み合わせ
    num_images_list = [500, 1000,5000,10000]  # 画像数
    workers_list = [1, 2, 4, 8]                          # 並列処理のワーカープロセス数

    collect_data(input_directory, num_images_list, workers_list, output_directory_parallel, output_directory_sequential)

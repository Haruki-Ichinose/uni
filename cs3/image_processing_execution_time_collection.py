import time
import csv
from PIL import Image
from concurrent.futures import ProcessPoolExecutor
import os

# グレースケール変換関数
def convert_to_grayscale(image_path, output_dir):
    try:
        with Image.open(image_path) as img:
            gray_img = img.convert("L")
            output_path = os.path.join(output_dir, os.path.basename(image_path))
            gray_img.save(output_path)
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
        for image_path in image_files:
            executor.submit(convert_to_grayscale, image_path, output_dir)
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
        convert_to_grayscale(image_path, output_dir)
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

    # 結果をCSVファイルに保存
    with open('execution_times.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["Method", "Num Images", "Workers", "Execution Time (s)"])
        writer.writerows(results)

# 実行
if __name__ == "__main__":
    input_directory = "input_images"
    output_directory_parallel = "output_images_parallel"
    output_directory_sequential = "output_images_sequential"
    
    # 画像数とワーカープロセス数の組み合わせ
    num_images_list = [500, 1000, 5000, 10000,20000,50000]  # 画像数
    workers_list = [1, 2, 4, 8]                          # 並列処理のワーカープロセス数

    collect_data(input_directory, num_images_list, workers_list, output_directory_parallel, output_directory_sequential)

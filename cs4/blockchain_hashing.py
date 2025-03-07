import hashlib
import time
import matplotlib.pyplot as plt

# 適当な文字列を定義
base_string = "blockchain_example"

# 成功するまでハッシュ値を計算する関数
def find_nonce_with_difficulty(base, difficulty):
    nonce = 0
    attempts = 0
    start_time = time.time()
    
    while True:
        # 文字列に乱数 nonce を繋げてハッシュ値を計算
        test_string = base + str(nonce)
        hash_result = hashlib.sha256(test_string.encode()).hexdigest()
        
        # ハッシュ値の先頭が "0" の連続する数 n を満たす場合に成功
        if hash_result.startswith("0" * difficulty):
            end_time = time.time()
            return nonce, hash_result, attempts, end_time - start_time
        nonce += 1
        attempts += 1

# グラフ描画関数
def plot_results(results):
    difficulties = [r['Difficulty (n)'] for r in results]
    attempts = [r['Attempts'] for r in results]
    times = [r['Elapsed Time (s)'] for r in results]

    # 試行回数のプロット
    plt.figure(figsize=(10, 6))
    plt.plot(difficulties, attempts, marker='o', label="Attempts", color="blue")
    plt.yscale('log')  # 試行回数は指数的に増加するため対数スケール
    plt.xlabel("Difficulty (n)", fontsize=14)
    plt.ylabel("Attempts (log scale)", fontsize=14)
    plt.title("Attempts vs Difficulty", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.legend(fontsize=12)
    plt.tight_layout()
    plt.savefig("attempts_vs_difficulty.png")  # 画像として保存
    plt.close()

    # 実行時間のプロット
    plt.figure(figsize=(10, 6))
    plt.plot(difficulties, times, marker='o', label="Elapsed Time (s)", color="orange")
    plt.xlabel("Difficulty (n)", fontsize=14)
    plt.ylabel("Elapsed Time (s)", fontsize=14)
    plt.title("Elapsed Time vs Difficulty", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.legend(fontsize=12)
    plt.tight_layout()
    plt.savefig("elapsed_time_vs_difficulty.png")  # 画像として保存
    plt.close()

# メイン処理
def main():
    # 難易度範囲を設定
    min_difficulty = 1
    max_difficulty = 6  # 必要に応じて 7, 8, ... に拡張

    results = []
    for difficulty in range(min_difficulty, max_difficulty + 1):
        # 計算開始メッセージ
        print(f"Starting calculation for difficulty: {difficulty}")
        
        # 計算実行
        nonce, hash_result, attempts, elapsed_time = find_nonce_with_difficulty(base_string, difficulty)
        
        # 結果を保存
        results.append({
            "Difficulty (n)": difficulty,
            "Nonce": nonce,
            "Hash": hash_result,
            "Attempts": attempts,
            "Elapsed Time (s)": elapsed_time
        })
    
    # 結果を一覧表示
    print("\nFinal Results:")
    for result in results:
        print(f"Difficulty (n): {result['Difficulty (n)']}")
        print(f"Nonce: {result['Nonce']}")
        print(f"Hash: {result['Hash']}")
        print(f"Attempts: {result['Attempts']}")
        print(f"Elapsed Time (s): {result['Elapsed Time (s)']:.6f}")
        print("-" * 40)

    # グラフを描画
    plot_results(results)

if __name__ == "__main__":
    main()


import MeCab

# MeCabのインスタンス作成
mecab = MeCab.Tagger("-d /var/lib/mecab/dic/ipadic-utf8")

# 形態素解析対象の文字列
text = "私は人工知能を受講している."
text2= "裏庭には二羽にわとりがいる."
text3= "この先生きのこるには."

# 形態素解析の結果を取得
parsed = mecab.parse(text)
parsed2 = mecab.parse(text2)  
parsed3 = mecab.parse(text3)
print(parsed, parsed2, parsed3)

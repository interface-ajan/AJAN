#!/usr/bin/python2
# -*- coding: utf-8 -*-

'''
--------------------------------------------------------------------------
動体検知 サンプル
AJAN側のUSBカメラ動作時に 本プログラムを使用して
3枚の画像から、動きがあるかどうか検出。

※本プログラムは、AJNAでカメラ取得した画像を Pythonのプログラムで
  画像認識/処理できることを示すためのサンプルプログラムです。
  本プログラムの内容につきましては 動作保証/サポートいたしかねますので
  ご了承ください。

[書式]
  >/usr/bin/python2 check_motion.py ファイル1 ファイル2 ファイル3

[使用例]
  /usr/bin/python2 check_motion.py A.jpg B.jpg C.jpg

  実行結果：
    0：動きなし
    1：動きがあり

--------------------------------------------------------------------------
'''
import sys
import os
import cv2
import numpy as np


# フレーム差分の計算
def calc_frame(img1, img2, img3, th):

    # フレームの絶対差分
    diff1 = cv2.absdiff(img1, img2)
    diff2 = cv2.absdiff(img2, img3)

    # 差分画像の論理積
    diff = cv2.bitwise_and(diff1, diff2)

    # 二値化
    diff[diff < th] = 0
    diff[diff >= th] = 255
    
    # メディアンフィルタ
    mask = cv2.medianBlur(diff, 5)

    return diff


# メイン処理 ------------------------------

motion_flg = 0

#引数取得
args = sys.argv
if (len(args) != 4):
    print "args error"
    exit()

src_file1     = args[1]
src_file2     = args[2]
src_file3     = args[3]

# 動体判定閾値
check_moment = 1300

#ファイル読み込み
src1 = cv2.imread(src_file1)
src2 = cv2.imread(src_file2)
src3 = cv2.imread(src_file3)

# フレームを3枚取得してグレースケール変換
frame1 = cv2.cvtColor(src1, cv2.COLOR_RGB2GRAY)
frame2 = cv2.cvtColor(src2, cv2.COLOR_RGB2GRAY)
frame3 = cv2.cvtColor(src3, cv2.COLOR_RGB2GRAY)

# カウント変数の初期化
cnt = 0

# フレーム間差分を計算
mask = calc_frame(frame1, frame2, frame3, th=10)

# 白色領域のピクセル数を算出
moment = cv2.countNonZero(mask)

# 白色領域のピクセル数が一定以上なら不審物有りと判定
if moment > check_moment:
    #動きを検出
    motion_flg = 1

#結果設定
print motion_flg



#!/usr/bin/python2
# -*- coding: utf-8 -*-

'''
--------------------------------------------------------------------------
顔検出 サンプル
AJAN側のUSBカメラ動作時に 本プログラムを使用して
指定した画像ファイルの顔認識を行う

※本プログラムは、AJNAでカメラ取得した画像を Pythonのプログラムで
  画像認識/処理できることを示すためのサンプルプログラムです。
  本プログラムの内容につきましては 動作保証/サポートいたしかねますので
  ご了承ください。

[書式]
  >/usr/bin/python2 check_face_file.py src_file update_file

[引数]
  src_file      顔認識させる元の画像データ
  update_file   顔認識後、顔部分に枠をつけた画像の保存先

[使用例]
  /usr/bin/python2 check_face_file.py src.jpg update.jpg

--------------------------------------------------------------------------
'''
import sys
import os
import cv2


# ************************************************
# 顔認識
#
# 引数
#    before_file  変更元のファイルパス
#    update_file  変更後に保存するファイルパス
#
# 戻り値
#    0   正常終了
#   -1   エラー
# ************************************************
def check_face(src_file, update_file):

    #学習用データ
    face_cascade_path = 'other/haarcascade_frontalface_default.xml'
    face_cascade = cv2.CascadeClassifier(face_cascade_path)

    #ファイル読み込み
    src = cv2.imread(src_file)

    #グレースケール
    src_gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)

    #顔検出
    faces = face_cascade.detectMultiScale(src_gray)
    for x, y, w, h in faces:
        #検出したら四角を描画
        cv2.rectangle(src, (x, y), (x + w, y + h), (255, 0, 0), 8)

        #検出したらモザイク化
        #ratio = 0.05
        #small = cv2.resize(src[y: y + h, x: x + w], None, fx=ratio, fy=ratio, interpolation=cv2.INTER_NEAREST)
        #src[y: y + h, x: x + w ] = cv2.resize(small, (w, h), interpolation=cv2.INTER_NEAREST)

    #ファイル出力
    cv2.imwrite(update_file, src)


# メイン処理 ------------------------------

#引数取得
args = sys.argv
if (len(args) != 3):
    print "args error"
    exit()

src_file    = args[1]
update_file = args[2]

ret = check_face(src_file, update_file)



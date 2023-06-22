#!/bin/bash
# グローバル共有の元となる redisサーバーを複数起動する為に、
# 起動用スクリプトの設定ファイルを作成支援するツール
#
# Copyright 2022 Interface Corporation

# ↓デバッグプリント有効みたいなもの
#set -x
# ↓エラー時に止まる
set -e

# エスケープ色(echo -e する)
RED="\033[91m"
CYAN="\033[96m"
NRM="\033[m"

PROGNAME=$(basename $0)

###############################################################################
# サブルーチン

usage() {
	echo "名前: グローバル共有の元となる redisサーバーを複数起動する為に"
	echo "　　  起動用スクリプトの設定ファイルを作成支援するツール"
	echo ""
	echo "書式: $PROGNAME [オプション]"
	echo "説明:"
	echo "　　　引数なし"
	echo "　　　　　質問に答えて、設定ファイルが作れます"
	echo "　　　-p <ポート番号>"
	echo "　　　　　redisサーバを立ち上げるポート番号を指定します"
	echo "　　　-f"
	echo "　　　　　最終確認なしに処理を続行します"
	echo "　　　-h or --help"
	echo "　　　　　この使い方を表示して終わります"
	exit 1
}

###############################################################################
# 前処理

# 管理者権限で動いてなかったら、停止
if [ ${EUID:-${UID}} != 0 ];then
	echo -e "${RED}このスクリプトは、管理者権限で実行してください${NRM}"
	exit
fi

# 引数チェック
echo "引数:[$0 $*]"

for OPT in "$@"
do
	case $OPT in
		-p)
			PORT="$2"
			shift
			;;
		-f)
			IS_FORCE="1"
			;;
		-h | --help)
			usage
			exit 1
			;;
	esac
done

if [ -z "$PORT" ]; then
	# 入力待ち
	echo "グローバル共有(redisサーバ)の複数起動用の設定ファイルを作ります"
	echo "新規に作りたいサーバーのポート番号を指定してください"
	echo "デフォルトはポート番号 6379です。それ以外を期待します(推奨は6380など)"
	read -p "ポート番号:" PORT
fi

if [ -z "$IS_FORCE" ]; then
	echo "ポート番号:${PORT} の設定ファイルを作ります"
	read -p "OK?(y/n):" yn
	case "$yn" in [yY]*) ;; *) echo -e "${RED}中断指示されました${NRM}"; exit ;; esac
fi

echo "ポート番号:${PORT} の設定ファイルを作ります"

###############################################################################
# 実処理

set -x	# デバッグ有効

# 設定ファイル＞/etc/redis/redis.confからの複製
REDIS_CONF=/etc/redis/redis-${PORT}.conf
cp /etc/redis/redis.conf ${REDIS_CONF}
chown -R redis:redis ${REDIS_CONF}

sed -i "s/^port .*$/port ${PORT}/" ${REDIS_CONF}
sed -i -r "s/^pidfile (.*)(\.pid)/pidfile \\1-${PORT}\\2/" ${REDIS_CONF}
sed -i -r "s/^logfile (.*)(\.log)/logfile \\1-${PORT}\\2/" ${REDIS_CONF}
sed -i -r "s/^dir (.*)/dir \\1-${PORT}/" ${REDIS_CONF}

# ↑のdir行で指定したパスを期待
mkdir -p /var/lib/redis-${PORT}
install -d -o redis -g redis -m 0750 /var/lib/redis-${PORT}

# systemdファイル＞/lib/systemd/system/redis-server.service からの複製
REDIS_SVC=/lib/systemd/system/redis-server-${PORT}.service
cp /lib/systemd/system/redis-server.service ${REDIS_SVC}

sed -i "s/redis.conf/redis-${PORT}.conf/" ${REDIS_SVC}
sed -i "s/\.pid/-${PORT}\.pid/" ${REDIS_SVC}
sed -i -r "s/(\/var\/lib\/redis)/\\1-${PORT}/" ${REDIS_SVC}
sed -i "s/redis.service/redis-${PORT}.service/" ${REDIS_SVC}

# systemdファイルを有効に
systemctl enable redis-server-${PORT}.service

set +x	# デバッグ無効

# 設定完了
echo -e "${CYAN}グローバル共有(redisサーバー)の${PORT}ポート向けサーバーの設定を作成しました${NRM}"
echo -e "サーバーを起動するには、「sudo systemctl start redis-server-${PORT}.service」と入力してください"
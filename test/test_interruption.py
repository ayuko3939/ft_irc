#!/usr/bin/env python3
import socket
import time

def main():
	# サーバーのIPアドレスとポート番号（必要に応じて変更してください）
	host = "127.0.0.1"
	port = 6667

	# 送信するコマンドの部分（例：認証やJOINなどのコマンドの途中まで送信）
	parts = [
		"PASS mypassword\r\n",
		"NICK mynick\r\n",
		"USER myuser myuser 127.0.0.1 :myrealname\r\n",
		"JOIN mychannel\r\n",
		# 以下は不完全なコマンド（改行なし）送信の例
		"PART #mychannel :Leaving the ch",
	]

	# ソケットを作成してサーバーに接続
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((host, port))
	print("サーバー {}:{} に接続しました".format(host, port))

	# 完全なコマンド部分は送信
	for part in parts[:4]:
		print("送信: {!r}".format(part))
		s.sendall(part.encode())
		time.sleep(1)

	# 不完全なコマンドの一部を送信
	for part in parts[4:5]:
		print("送信（不完全なコマンド）: {!r}".format(part))
		s.sendall(part.encode())
		time.sleep(1)

	# ここで、意図的にソケットを強制終了（半端なコマンド状態のまま切断）
	print("途中でソケットを強制終了します（半端なコマンド状態）")
	s.close()

# ※サーバーが切断後も正常に動作しているか、ログなどで確認してください。
# サーバーからの応答を受信（最大4096バイト）
if __name__ == "__main__":
	main()

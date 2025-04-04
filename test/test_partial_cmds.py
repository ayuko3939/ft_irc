#!/usr/bin/env python3
import socket
import time

# 目的：nc を使用して部分的なコマンドを送信して、正しく動くか確認する
def main():
	# サーバーのIPアドレスとポート番号
	host = "127.0.0.1"
	port = 6667

	# 送信するPASSコマンド "PASS mypassword\r\nNICK mynick\r\nUSER myuser myuser 127.0.0.1 :myrealname\r\n"
	
	# 複数回に分けるため、任意の部分に分割
	parts = ["PAS", "S my", "pass", "word\r", "\n",
			 "NI", "CK my", "nick\r\n",
			 "USER m", "yuser 0 *", " :myreal", "name\r\n",]

	# ソケットを作成しサーバーに接続
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((host, port))
	print("サーバー {}:{} に接続しました".format(host, port))

	# 各部分を順に送信し、間に1秒ずつ待機
	for part in parts:
		print("送信: {!r}".format(part))
		s.sendall(part.encode())
		time.sleep(1)

	# サーバーからの応答を受信（最大4096バイト）
	response = s.recv(4096)
	print("受信:", response.decode())

	s.close()

if __name__ == "__main__":
	main()

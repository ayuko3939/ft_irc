#!/usr/bin/env python3
import socket
import time
import sys

def main():
	# サーバーのIPアドレスとポート番号（必要に応じて変更してください）
	host = "127.0.0.1"
	port = 6667

	# 初回に送信するコマンドリスト（各行末に "\r\n" を付加）
	initial_commands = [
		"PASS mypassword\r\n",
		"NICK 444\r\n",
		"USER port4 test\r\n",
		"JOIN test\r\n"
	]

	# ソケットを作成してサーバーに接続
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((host, port))
	print(f"Connected to {host}:{port}")

	# 初回コマンドを順次送信
	for cmd in initial_commands:
		print(f"Sending: {cmd.strip()}")
		s.sendall(cmd.encode())
		time.sleep(1)  # 少し待機してサーバーの応答を確認する

	# タイムアウト設定（サーバーからの応答をブロックせずに確認するため）
	s.settimeout(0.5)

	# 初回応答を受信（可能な範囲で表示）
	try:
		while True:
			data = s.recv(4096)
			if not data:
				break
			print("Received:", data.decode())
	except socket.timeout:
		pass

	# print("Initial commands sent. Now entering interactive mode.")
	# print("各行入力後にEnterを押すと、その内容がサーバーに送信されます。Ctrl+Cで終了。")

	# インタラクティブモード：ユーザーからの入力をサーバーに送信する
	try:
		while True:
			# ユーザーからの入力を取得
			user_input = input("> ")
			if user_input == "":
				continue
			# 入力行の末尾にIRC標準の改行を付加して送信
			message = user_input + "\r\n"
			s.sendall(message.encode())
			time.sleep(0.2)
			# サーバーからの応答を非同期で受信して表示
			try:
				while True:
					data = s.recv(4096)
					if not data:
						break
					print("Received:", data.decode())
			except socket.timeout:
				# 応答がこれ以上なければ次の入力待ちへ
				pass
	except KeyboardInterrupt:
		print("\nExiting interactive mode.")
		s.close()
		sys.exit(0)

if __name__ == "__main__":
	main()

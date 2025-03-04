【Channel class】
●private
チャンネル名 (std::string)
トピック (std::string)
モード (std::string)
チャンネルパスワード (std::string)
最大接続人数 (int)
クライアントリスト (std::map<std::string, Client>)
キックされたユーザーリスト (std::vector<std::string>)
オペレーターリスト (std::vector<std::string>)


【Client class】
●private
クライアントのファイルディスクリプタ (int)
ニックネーム (std::string)
旧ニックネーム (std::string)
ユーザー名 (std::string)
本名 (std::string)
モード (std::string)
受信バッファ (std::string)
送信バッファ (std::string)
切断フラグ (bool)
登録完了フラグ (bool)
ウェルカムメッセージ送信フラグ (bool)


【Server class】
●private
サーバーソケット (int)
クライアントリスト (std::map<const int, Client>)
チャンネルリスト (std::map<std::string, Channel>)
ポート番号 (std::string)
サーバーパスワード (std::string)
日付時刻 (std::string)
MOTD (std::string)
アドレス情報 (struct addrinfo)
サーバー情報 (struct addrinfo*)
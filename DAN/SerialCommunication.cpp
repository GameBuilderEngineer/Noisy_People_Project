#include "SerialCommunication.h"

SerialCommunication* pointerSerialCommunication = NULL;

//RS232C シリアル通信
//全体的な流れ
//①CreateFile()関数でファイルをオープンする。(COMポートのハンドルを取得する)
//②SetupComm()PurgeComm()関数で送受信バッファーの設定及び初期化を行う

SerialCommunication::SerialCommunication()
{
	pointerSerialCommunication = this;
	success = false;
	//1.ポートをオープン
	//COMポートハンドルの取得
	//ポートをファイルとみなしてCreateFile()関数でオープンしてハンドルを取得する
	arduino = CreateFile(
		"\\\\.\\COM3",						//ポートの名前：どのポートを開くのか
		GENERIC_WRITE, 						//アクセスモード：通常送受信ともするので読み書き両方を指定
		0,									//共有モード：通常0に設定　再オープン禁止
		NULL, 								//セキュリティアトリビュート：通常ではNULLに設定
		OPEN_EXISTING, 						//クリエイトディストリビューション：通常COMポートは既に存在しているのでOPEN_EXISTINGとする。
		FILE_ATTRIBUTE_NORMAL, 				//属性：通信の場合属性はないのでFILE_ATTRIBUTE_NORMAL(属性なし)を指定
		NULL);								//テンプレートのハンドル：通信の場合関係ない　通常NULLを指定

	//エラー処理
	if (arduino == INVALID_HANDLE_VALUE)	//ハンドル取得に失敗した場合
	{
		MSG("PORT COULD NOT OPEN");
		success = false;
		return;
	}

	//2.送受信バッファ初期化
	//・SetupComm()関数で送受信バッファーの容量を設定する
	//・PurgeComm()関数で送受信バッファーに残っているデータを破棄する
	//(注)送受信バッファー容量はドライバー、OSが介在するのでUART(集積回路)そのもののハードウェア的サイズとは異なります。//UART（Universal Asynchronous Receiver Transmitter）シリアル通信の信号とパラレル通信用の信号の変換を行う部品（集積回路）
	
	returnValue = SetupComm(arduino, 1024, 1024);
	//エラー処理
	if (!returnValue)
	{
		MSG("SET UP FAILED");
		CloseHandle(arduino);
		success = false;
		return;
	}

	returnValue = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	//エラー処理
	if (!returnValue)
	{
		MSG("CLEAR FAILED");
		CloseHandle(arduino);
		success = false;
		return;
	}

	//3.基本通信条件の設定
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength	= sizeof(DCB);
	dcb.BaudRate	= 9600;
	dcb.fBinary		= TRUE;
	dcb.ByteSize	= 8;
	dcb.fParity		= NOPARITY;
	dcb.StopBits	= ONESTOPBIT;

	returnValue = SetCommState(arduino, &dcb);
	if (!returnValue)
	{
		MSG("SetCommState FAILED");
		CloseHandle(arduino);
		success = false;
		return;
	}

	success = true;

}


SerialCommunication::~SerialCommunication()
{
	CloseHandle(arduino);
}

//送信
void SerialCommunication::send(BYTE	data)
{
	if (!success)return;

	DWORD	sendSize;
	//DWORD	errorMask;
	returnValue = WriteFile(arduino, &data, sizeof(data), &sendSize, NULL);
	if (!returnValue)
	{
		MSG("SEND FAILED");
		CloseHandle(arduino);
		return;
	}

}

void SerialCommunicationNS::send(BYTE data)
{
	pointerSerialCommunication->send(data);
}

SerialCommunication* getSerialCommunication()
{
	return pointerSerialCommunication;
}
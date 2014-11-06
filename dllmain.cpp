#include "dllmain.hpp"
#include "vehicle/vehicle.hpp"
#include <string.h> 
void sendMessage(char *st);
/// このATSプラグインの、コンピュータ上の絶対パス
char g_module_dir[MAX_PATH];
HANDLE hRs232c;
/// DLLのメイン関数
BOOL WINAPI DllMain(
					HINSTANCE hinstDLL,  ///< DLL モジュールのハンドル
					DWORD fdwReason,     ///< 関数を呼び出す理由
					LPVOID lpvReserved   ///< 予約済み
					)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:

        {
            char fullpath[MAX_PATH];
            char drive[MAX_PATH],
                    dir[MAX_PATH];

            GetModuleFileNameA(hinstDLL, fullpath, MAX_PATH);
            _splitpath_s(fullpath, drive, MAX_PATH, dir, MAX_PATH, 0, 0, 0, 0);

            strcpy(g_module_dir, drive);
            strcat(g_module_dir, dir);
        }

        break;

	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:	
        break;
	}

	return true;
}

/// このプラグインがBVEによって読み込まれた時に呼び出される。
void WINAPI atsLoad()
{
	ats::environment::get_instance();
    ats::environment::get_instance()->set_module_dir(g_module_dir);

	ats::vehicle::get_instance();  
}

/// このプラグインがBVEから解放された時に呼び出される。
void WINAPI atsDispose()
{
	CloseHandle(hRs232c);
	ats::vehicle::terminate();

	ats::environment::terminate();

}

/// BVEがこのATSプラグインのバージョン値を取得しようとした時に呼び出される。
/// \return ATSプラグインのバージョン値
int WINAPI atsGetPluginVersion()
{
	return ATS_VERSION;
}

/// BVEに列車が読み込まれた時に呼び出される。
/// \param[in] vspec 車両の諸元
void WINAPI atsSetVehicleSpec(ATS_VEHICLESPEC vspec)
{
	char SndBuf[32];
	char RcvBuf[32];
	memset(SndBuf, 0, sizeof(SndBuf));
	memset(RcvBuf, 0, sizeof(RcvBuf));
	char port[] = "\\\\.\\COM90";

	// COMポートのオープン
	hRs232c = CreateFile(
		port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hRs232c == INVALID_HANDLE_VALUE) {
		printf("RS232C CreateFile Error\n");
	}

	// COMポートのセットアップ
	if (SetupComm(hRs232c, sizeof(RcvBuf), sizeof(SndBuf)) == FALSE){
		printf("RS232C SetupComm Error\n");
	}

	// COMポートの設定取得
	DCB dcb;
	memset(&dcb, NULL, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hRs232c, &dcb);

	// COMポートの設定変更
	dcb.BaudRate = 9600;
	dcb.Parity = 0;    // 0,1,2,3,4 = no,odd,even,mark,space 
	dcb.StopBits = 0;    // 0,1,2     = 1bits,1.5bits,2bits
	dcb.ByteSize = 8;    // 4,5,6,7,8 = 4bits,5bits,6bits,7bits,8bits
	if (SetCommState(hRs232c, &dcb) == FALSE){
		printf("RS232C SetCommState Error\n");
	}

}

/// BVEのシナリオが開始された時に呼び出される。
/// \param[in] param ブレーキハンドルの位置
void WINAPI atsInitialize(int param)
{

    ats::environment::get_instance()->initialize_status();
	ats::vehicle::get_instance()->initialize(param);

}

/// BVEがシナリオ実行中に毎フレームごとに呼び出される。
/// \param[in] vs 毎処理ごとの車両の状態
/// \param[out] p_panel 運転台へ送る値の配列 (配列の範囲: 0-255)
/// \param[out] p_sound サウンド命令の値の配列 (配列の範囲: 0-255)
/// \return 列車の操縦命令
const std::string command[8] = { "SN", "SP", "PW", "PA", "BC", "BA", "SP", "FA" };

int oldPanel[8] = {0};

ATS_HANDLES WINAPI atsElapse(ATS_VEHICLESTATE vs, int *panel, int *p_sound)
{
	char buf[10];
	if (memcmp(panel, oldPanel, sizeof(int)* 8) != 0){
		sendMessage("panel,");
		for (int i = 0; i <= 7; i++){
			sprintf(buf, "%d,", *panel);
			sendMessage(buf);
			oldPanel[i] = *panel;
			panel++;

		}

		sendMessage("\n");
		
	}
	/*
	//パネルはここ。
	panel[0] = 1; //ATS-Sn系	
	panel[1] = 1; //ATS-Sn系
	panel[2] = 1; // P電源
	panel[3] = 1; // パターン接近
	panel[4] = 1; // ブレーキ開放
	panel[5] = 1; // ブレーキ動作
	panel[6] = 1; // P受信
	panel[7] = 1; // 故障
	*/
	//sendMessage("a", 1);//sendね
	ats::environment::get_instance()->set_current_status(vs);

	ATS_HANDLES ret = ats::vehicle::get_instance()->execute(panel, p_sound);

	ats::environment::get_instance()->update();
	return ret;
}
void sendMessage(char *st){

	// 送信電文作成
//	sprintf(SndBuf, st);
	//strcpy_s(SndBuf, "abcあいう");

	// COMポートにデータ送信
	DWORD dwSize;
	OVERLAPPED ovWrite;
	memset(&ovWrite, 0, sizeof(ovWrite));
	if (WriteFile(hRs232c, st, strlen(st), &dwSize, &ovWrite) == FALSE){
		printf("RS232C WriteFile Error\n");
	}
	else{
		// 送信成功
		printf("RS232C WriteFile Success\n");
	}
}
/// プレイヤーによって力行ノッチ位置が変更された時に呼び出される。
/// \param[in] notch 変更後の力行ノッチ位置
void WINAPI atsSetPower(int notch)
{

	ats::environment::get_instance()->set_power_notch(notch);
}

/// プレイヤーによってブレーキノッチ位置が変更された時に呼び出される。
/// \param[in] notch 変更後のブレーキノッチ位置
void WINAPI atsSetBrake(int notch)
{
	ats::environment::get_instance()->set_brake_notch(notch);
}

/// プレイヤーによってレバーサーの位置が変更された時に呼び出される。
/// \param[in] pos 変更後のレバーサーの位置
void WINAPI atsSetReverser(int pos)
{
	ats::environment::get_instance()->set_reverser_position(pos);
}

/// プレイヤーによってATSプラグインで使用するキーが押された時に呼び出される。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
void WINAPI atsKeyDown(int ats_key_code)
{
	ats::environment::get_instance()->set_key_down(ats_key_code);
}

/// プレイヤーによってATSプラグインで使用するキーが押されていて、それが離された時に呼び出される。
/// \param[in] ats_key_code ATSプラグインで使用するキーのインデックス
void WINAPI atsKeyUp(int ats_key_code)
{
	ats::environment::get_instance()->set_key_up(ats_key_code);
}

/// プレイヤーによって警笛が取り扱われた時に呼び出される。
/// \param[in] ats_horn 警笛の種類
void WINAPI atsHornBlow(int ats_horn)
{
//	sendMessage("hoge");
	ats::vehicle::get_instance()->blow_horn(ats_horn);
	
}

/// BVEによって列車のドアが開かれた時に呼び出される。
void WINAPI atsDoorOpen()
{
	ats::environment::get_instance()->set_door_status(false);
}

/// BVEによって列車のドアが閉じられた時に呼び出される。
void WINAPI atsDoorClose()
{
	ats::environment::get_instance()->set_door_status(true);
}

/// BVEによって現在の信号現示が変更された時に呼び出される。
/// \param[in] signal 信号現示のインデックス
void WINAPI atsSetSignal(int signal)
{
	//sendMessage("SignalChanged\n");
	ats::environment::get_instance()->set_signal(signal);
	ats::vehicle::get_instance()->changed_signal(signal);
}

/// BVEによって地上子を通過した際に呼び出される。
/// \param[in] beacon_data 地上子の情報
void WINAPI atsSetBeaconData(ATS_BEACONDATA beacon_data)
{
	//sendMessage("BeaconPassed\n");
	ats::vehicle::get_instance()->received_beacon_data(beacon_data);
}

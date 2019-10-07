//===================================================================================================================================
//【MemoryViewer.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "MemoryViewer.h"

//===================================================================================================================================
//【トレース関数】
//===================================================================================================================================
void trace(const char* format, ...)
{
	char buff[1024];

	va_list argp;
	va_start(argp, format);
	::vsprintf_s(buff, format, argp);
	va_end(argp);

	//    ::printfDx( "%s", buff);
	::OutputDebugString(buff);
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
MemoryViewer::MemoryViewer()
{
	// CPUコア数を取得する
	SYSTEM_INFO sys_info;
	::GetSystemInfo(&sys_info);
	cpuNum = sys_info.dwNumberOfProcessors;
	trace("CPUコア数:%d\n", cpuNum);

	// 物理メモリ量
	MEMORYSTATUSEX mstatus = { sizeof(MEMORYSTATUSEX) };
	//4Gバイト以上の物理メモリ容量を取得する
	if (::GlobalMemoryStatusEx(&mstatus)) {
		physMemorys = mstatus.ullTotalPhys;
		trace("物理メモリ量:%lld\n", physMemorys);
	}
	else {
		trace("物理メモリ量取得に失敗！\n");
		isError = true;
		return;
	}

	// ハンドルの作成
	if (ERROR_SUCCESS != ::PdhOpenQuery(NULL, 0, &query)) {
		trace("PDHハンドルの作成に失敗！\n");
		query = nullptr;
		isError = true;
		return;
	}

	instanceName = getInstanceName(query);
	if (instanceName == "") {
		trace("インスタンス名取得に失敗！\n");
		isError = true;
		return;
	}

	// カウンターパスを登録
	std::string cpu_counter_path = instanceName + "\\% Processor Time";
	if (ERROR_SUCCESS != ::PdhAddCounter(query, cpu_counter_path.c_str(),
		0, &cpuCounter)) {
		trace("PDH CPUカウンターパス登録に失敗！\n");
		cpuCounter = nullptr;
		isError = true;
		return;
	}

	std::string mem_counter_path = instanceName + "\\Working Set - Private";
	if (ERROR_SUCCESS != ::PdhAddCounter(query, mem_counter_path.c_str(),
		0, &memoryCounter)) {
		trace("PDH メモリーカウンターパス登録に失敗！\n");
		memoryCounter = nullptr;
		isError = true;
		return;
	}

	update();
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
MemoryViewer::~MemoryViewer()
{
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
bool MemoryViewer::update()
{
	if (isError) return false;
	if (ERROR_SUCCESS != ::PdhCollectQueryData(query)) return false;

	PDH_FMT_COUNTERVALUE fmtvalue;
	::PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &fmtvalue);
	cpuUsage = fmtvalue.doubleValue;

	::PdhGetFormattedCounterValue(memoryCounter, PDH_FMT_LONG, NULL, &fmtvalue);
	memoryUsage = fmtvalue.longValue;

	return true;
}

//===================================================================================================================================
//【自プロセスのインスタンス名取得】
//インスタンス名
//　通常はプロセス名と同一だが、複数立ち上げると後ろに番号がついていく。
//　例：hoge hoge#1 hoge#2 hoge#3 ...等
//===================================================================================================================================
std::string MemoryViewer::getInstanceName(PDH_HQUERY query)
{
	char module_path[_MAX_PATH + 1];
	if (!::GetModuleFileName(NULL, module_path, sizeof(module_path))) {
		return "";//error
	}

	// パスからファイル名(拡張子なし)を抽出
	char process_name[_MAX_FNAME + 1];
	if (0 != _splitpath_s(module_path, NULL, 0, NULL, 0,
		process_name, sizeof(process_name), NULL, 0)) {
		return "";//error
	}

	trace("process_name:%s\n", process_name);

	const char* OBJECT_NAME = "Process";
	//インスタンスリストを受け取るバッファのサイズ取得
	DWORD counter_list_length = 0;
	DWORD instance_list_length = 0;
	if (PDH_MORE_DATA != ::PdhEnumObjectItems(NULL, NULL, OBJECT_NAME,
		NULL, &counter_list_length,
		NULL, &instance_list_length,
		PERF_DETAIL_WIZARD, 0)) {
		return "";//error
	}

	//インスタンスリストを受け取る
	std::vector<char> counter_list(counter_list_length);//バッファの確保
	std::vector<char> instance_list(instance_list_length);//バッファの確保
	PDH_STATUS sts = ::PdhEnumObjectItems(NULL, NULL, OBJECT_NAME,
		counter_list.data(), &counter_list_length,
		instance_list.data(), &instance_list_length,
		PERF_DETAIL_WIZARD, 0);
	if (sts != ERROR_SUCCESS) {
		return "";//error
	}

	// プロセスIDをキーとして、カレントインスタンス名を探す
	LONG process_id = ::GetCurrentProcessId();

	//trace("instance_list_length:%d\n", instance_list_length);
		// インスタンスリストからインスタンス名を取り出す
		//"インスタンス名\0インスタンス名\0インスタンス名\0\0"の構成になってる

	int cnt = 0;//同名プロセス数
	int i = 0;
	while (instance_list[i])
	{
		//trace("%04d: %s\n", i, &instance_list[i]);
		if (0 == std::strcmp(&instance_list[i], process_name)) { //同名？
			//インスタンス名化
			std::string instance_name = std::string("\\Process(") + process_name;
			if (cnt > 0) {
				instance_name += std::string("#") + std::to_string(cnt);
			}
			instance_name += std::string(")");
			if (process_id == getProcessID(query, instance_name)) {
				trace("pid: %d instance_name:[%s]\n", process_id, instance_name.c_str());
				return instance_name;
			}
			++cnt;
		}
		i += std::strlen(&instance_list[i]) + 1;
	}

	return "";
}

//===================================================================================================================================
//【自プロセス名の取得】
//プロセス名 = .exeを取り除いた物
//　複数プロセスある場合でも同一名称、PIDは別になる。
//　例：hoge.exe → hoge
//===================================================================================================================================
LONG MemoryViewer::getProcessID(PDH_HQUERY query, const std::string& instance_name)
{

	if (isError) return -1;

	std::string counter_path = instance_name + "\\ID Process";
	PDH_HCOUNTER counter;

	if (ERROR_SUCCESS == ::PdhAddCounter(query, counter_path.c_str(), 0, &counter)) {
		if (ERROR_SUCCESS == ::PdhCollectQueryData(query)) {
			PDH_FMT_COUNTERVALUE fmtvalue;
			::PdhGetFormattedCounterValue(counter, PDH_FMT_LONG, NULL, &fmtvalue);
			::PdhRemoveCounter(counter);
			return fmtvalue.longValue;
		}
		::PdhRemoveCounter(counter);
	}
	return -1;
}

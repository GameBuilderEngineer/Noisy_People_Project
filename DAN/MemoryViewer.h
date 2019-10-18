//===================================================================================================================================
//【MemoryViewer.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#pragma comment( lib, "pdh.lib" )

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include <string>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <vector>

//===================================================================================================================================
//【メモリ計測クラス】
//おまけでCPUも計測しています。
//精度やや難ありです。
//Visual Studioなしの場合でも多少様子が見れる程度のものと考えてください。
//将来的には、高精度にしたいです。
//===================================================================================================================================
class MemoryViewer :public Base
{
private:
	bool            isError = false;
	std::string     instanceName;
	int				cpuNum;					// CPUコア数
	PDH_HQUERY      query = nullptr;
	PDH_HCOUNTER    cpuCounter = nullptr;
	PDH_HCOUNTER    memoryCounter = nullptr;
	double			cpuUsage = 0.0;			// CPU使用率
	LONG            memoryUsage = 0;		// 物理メモリ使用量kb
	DWORDLONG       physMemorys = 0;		// 物理メモリ量kb

public:
	MemoryViewer();
	~MemoryViewer();
	bool update();
	double getCpuUsege() { return (cpuUsage / cpuNum); }		//自プロセスの使用率÷CPUコア数
	LONG   getMemoryUsege() { return memoryUsage / 1024; }		//kb
	DWORDLONG getPhysMemorys() { return physMemorys / 1024; }	//kb
	std::string getInstanceName(PDH_HQUERY query);
	LONG getProcessID(PDH_HQUERY query, const std::string& instance_name);
};

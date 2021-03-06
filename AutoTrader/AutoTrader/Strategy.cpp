#include "StdAfx.h"
#include "Strategy.h"

#include "pcap.h"
#include <pcap\usb.h>
#include <inc.h>
#include <cstring>
#include "RunningData.h"
#include "AutoTraderDlg.h"
#include "AutoTrader.h"
#include <windows.h>
#include "afxdialogex.h"
//////////////////////////////////////////////////////////////////////////
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>
// #include <cv.h>
// #include <highgui.h>
#include <stdio.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>

// #pragma comment( lib, "cv.lib" )
#pragma comment( lib, "opencv_calib3d244d.lib" )
#pragma comment( lib, "opencv_contrib244d.lib" )
#pragma comment( lib, "opencv_core244d.lib" )
#pragma comment( lib, "opencv_features2d244d.lib" )
#pragma comment( lib, "opencv_flann244d.lib" )

#pragma comment( lib, "opencv_gpu244d.lib" )
#pragma comment( lib, "opencv_haartraining_engined.lib" )
#pragma comment( lib, "opencv_highgui244d.lib" )
#pragma comment( lib, "opencv_imgproc244d.lib" )
#pragma comment( lib, "opencv_legacy244d.lib" )
#pragma comment( lib, "opencv_ml244d.lib" )

#pragma comment( lib, "opencv_nonfree244d.lib" )
#pragma comment( lib, "opencv_objdetect244d.lib" )
#pragma comment( lib, "opencv_photo244d.lib" )
#pragma comment( lib, "opencv_stitching244d.lib" )
#pragma comment( lib, "opencv_ts244d.lib" )
#pragma comment( lib, "opencv_video244d.lib" )
#pragma comment( lib, "opencv_videostab244d.lib" )
//	
#pragma comment( lib, "opencv_calib3d244.lib" )
#pragma comment( lib, "opencv_contrib244.lib" )
#pragma comment( lib, "opencv_core244.lib" )
#pragma comment( lib, "opencv_features2d244.lib" )
#pragma comment( lib, "opencv_flann244.lib" )

#pragma comment( lib, "opencv_gpu244.lib" )
#pragma comment( lib, "opencv_haartraining_engine.lib" )
#pragma comment( lib, "opencv_highgui244.lib" )
#pragma comment( lib, "opencv_imgproc244.lib" )
#pragma comment( lib, "opencv_legacy244.lib" )
#pragma comment( lib, "opencv_ml244.lib" )

#pragma comment( lib, "opencv_nonfree244.lib" )
#pragma comment( lib, "opencv_objdetect244.lib" )
#pragma comment( lib, "opencv_photo244.lib" )
#pragma comment( lib, "opencv_stitching244.lib" )
#pragma comment( lib, "opencv_ts244.lib" )
#pragma comment( lib, "opencv_video244.lib" )
#pragma comment( lib, "opencv_videostab244.lib" )
//////////////////////////////////////////////////////////////////////////

CStrategy::CStrategy(void)
{
	m_Code = _T("c1409");
	//m_Code = _T("ag1412");
	m_FastDeal = false;
}

CStrategy::CStrategy(CString code)
{
	m_Code = code;
	m_FastDeal = false;
}


CStrategy::~CStrategy(void)
{

}



vector<vector<CString>> CStrategy::GetDataFromSQL( int Num )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	m_vspdctomysql_Strategy = new VspdcToMySQL;
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,USER,PASSWD,CHARSET,Msg) == 0
	if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	{
		printf("连接成功/r/n");
		//char * code = CString2CharPt(m_Code);
		CString SQL = _T("select * from ")+m_Code+_T("  order by  ID desc limit 0,"+Int2CString(Num)+";");
		//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
		//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
		//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
		vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),34,Num,Msg);
		//CString getStr(str.c_str());
		//AUTOTRADER::Int2CString(Num)
		if( DataList.size() > 0 )
		{
			//printf(_T("查询成功/r/n"));
			//printf(str.data());
			//printf(_T("/r/n"));
			//printf(Msg);
			//HSTimer sTime(testStr);
			//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
			for (int i = 0; i<DataList.size(); i++)
			{
				vector<CString> tempVec = SplitString(DataList.at(i));
				//CString testStr = tempVec.at(30);
				ResList.push_back(tempVec);
			}
			// 第三十位 是HS时间

		}
	}
	//m_vspdctomysql->CloseMySQLConn();
	return ResList;
}

vector<vector<CString>> CStrategy::GetDataFromSQL( int StartID, int EndID )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	m_vspdctomysql_Strategy = new VspdcToMySQL;
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	{
		printf("连接成功/r/n");
		//char * code = CString2CharPt(m_Code);
		CString SQL = _T("select * from ")+m_Code+_T(" where ID between "+Int2CString(StartID)+" and " +
			Int2CString(EndID) + ";");
		//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
		//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
		//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
		vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),34,EndID - StartID,Msg);
		//CString getStr(str.c_str());
		//AUTOTRADER::Int2CString(Num)
		if( DataList.size() > 0 )
		{
			//printf(_T("查询成功/r/n"));
			//printf(str.data());
			//printf(_T("/r/n"));
			//printf(Msg);
			//HSTimer sTime(testStr);
			//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
			for (int i = 0; i<DataList.size(); i++)
			{
				vector<CString> tempVec = SplitString(DataList.at(i));
				//CString testStr = tempVec.at(30);
				ResList.push_back(tempVec);
			}
			// 第三十位 是HS时间

		}
	}
	//m_vspdctomysql->CloseMySQLConn();
	return ResList;
}


//*************************************
// 函数名:  	CString2CharPt
// 目的:	CString转char指针
// 时间：   2014/02/28
//*************************************
char* CStrategy::CString2CharPt( CString InputString )
{
	CString str = InputString;
	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n = str.GetLength();     // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	char * pFileName = new char[len+1];   //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);
	return pFileName;
}


//*************************************
// 函数名:  	SetTradeCode
// 目的:	设置交易代码
// 时间：   2014/03/27
//*************************************
void CStrategy::SetTradeCode( CString code )
{
	m_Code = code;
}


CString CStrategy::Int2CString( int InputData )
{
	 	char a[200];
	 	_itoa_s(InputData, a, 10);
	 	CString ss(a);
	 	return ss;
}

vector<CString> CStrategy::SplitString( CString DataStr )
{
	vector<CString> res;

	CString strArr = DataStr;
	LPCTSTR strSplit = _T(",");
	CStringArray* arr = new CStringArray;

	int iStart = 0 , iFind = 0;
	do
	{
		iFind = strArr.Find(strSplit , iStart);
		if(iFind == -1)
			arr->Add(strArr.Mid(iStart));
		else
			arr->Add(strArr.Mid(iStart , iFind - iStart));
		iStart = iFind + 1;
	}while(iFind != -1);

	//arr->GetCount()
	for (int i=0; i<arr->GetCount();i++)
	{
		res.push_back(arr->GetAt(i));
	}
	return res;
}


UINT CStrategy::RuntimeThread( LPVOID lpParameter )
{
	CStrategy *this2;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_dumper_t *dumpfile;
	struct pcap_pkthdr *header;
	const u_char *data;
	DWORD res;
	bool bExit;
	static long i=0,j=0;
	bool bFilter=false;//true表示过滤掉

	ListData List;

	struct ether_header *eth;
	u_char* mac_string;
	struct iphead *IPHead;
	struct arphead *ARPHead;
	in_addr ipaddr;
	// 这里有个参数
	this2=(CStrategy *)lpParameter;
	i=0;
	j=0;
	const char * adpter_char = "\\Device\\NPF_{3788FA43-59ED-4C95-83F5-061E42A46329}";
	if((adhandle= pcap_open_live(adpter_char,65536,1 ,10,errbuf)) == NULL)
	{
		::MessageBox(0,_T("不能打开网络适配器,请在网卡设置中经行设置"),_T("错误"),0);
		//EnterCriticalSection(&(this2->csThreadStop));
		this2->m_TradeSwitch=true;
		//LeaveCriticalSection(&(this2->csThreadStop));
		//SetEvent(this2->eThreadStart);
		return 0; 
	}

	// 	const char * CapFiePath = (char *)this2->CapFilePath.GetBuffer();
	// 	dumpfile=pcap_dump_open(adhandle,CapFiePath);
	// 	if(dumpfile==NULL)
	// 	{
	// 		::MessageBox(0,_T("不能打开记录文件"),_T("错误"),0);
	// 		EnterCriticalSection(&(this2->csThreadStop));
	// 		this2->bThreadStop=true;
	// 		LeaveCriticalSection(&(this2->csThreadStop));
	// 		SetEvent(this2->eThreadStart);
	// 		return 0;
	// 	}

	//EnterCriticalSection(&(this2->csThreadStop));
	this2->m_TradeSwitch=false;
	//LeaveCriticalSection(&(this2->csThreadStop));
	//SetEvent(this2->eThreadStart);

	//::MessageBox(0,_T("开始抓包"),_T("ok"),0);
	CString standard1 = _T("116"),standard2 = _T("236"),standard3 = _T("239"),standard4 = _T("129");
	CString tempIPs,xxx1,xxx2,xxx3,xxx4;
	//DWORD standardIP = inet_addr("116.236.239.129");
	DWORD standardIP = inet_addr(STANDARDIP);
	unsigned char *standIP = (unsigned char*)&standardIP;

	DWORD tempIP;
	wchar_t *WStrD;

	pcap_t *fp;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// 判断 是否是 期货的ip地址 上海电信，这里还需要添加
		// 目标ip
		char *CStr = List.dIP;
		size_t len = strlen(CStr) + 1;
		size_t converted = 0;
		wchar_t *WStr;
		WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
		mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
		CString Str2 = _T("D:")+ *WStr;
		WStrD = WStr;
		// 源ip
		CStr = List.sIP;
		tempIPs = CStr;
		tempIP = inet_addr(CStr);

		len = strlen(CStr) + 1;
		converted = 0;
		WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
		mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
		//CString Str3 = _T("S:")+ *WStr;

		if (tempIP == standardIP)
		{
			//this2->m_Show_Res.SetWindowTextW(WStrD);
			//this2->m_Show_Res2.SetWindowTextW(WStr);
			CStr = List.Text;
			this2->DecodeChar(List.Text,45);
			len = strlen(CStr) + 1;
			converted = 0;
			WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
			mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
			CString Str3 = _T("S:")+ *WStr;
			//////////////////////////////////////////////////////////////////////////
			// 解析数据包
			struct pcap_pkthdr *header2;
			const u_char *data2;
			char *p;

			p=(char *)malloc((header->caplen+1)*sizeof(u_char));
			memcpy(p,data,header->caplen);
			p[header->caplen]='\0';
			this2->DecodeChar(p,header->caplen);
			//GetDlgItem(IDC_EDIT1)->SetWindowText(p);
			CString res(p);
			vector<CString> spList = this2->SplitString(res);
			// 如果解析后容器为50 则删掉第一个
			if (spList.size() == 50)
			{
				spList.erase(spList.begin());
			}
			//
			//CString tempCode = this2->GetTradeCode(spList.at(0));
			//this2->VectorIsHas(this2->AutoList,this2->GetTradeCode(spList.at(0)))
			// 如果容器为49个
			if ( this2->GetTradeCode( spList.at(0) ) == this2->m_Code)
			{
				// 取到数据为策略数据

				//printf("这里可以添加处理程序和监控程序");
				// 声明一个策略数据 加入运行数据
				//this2->m_runData.m_Data_Vec.at(0).
				StrategyData xx_For_Add;
				xx_For_Add.ID = -1;
				xx_For_Add.CODE = this2->m_Code;
				
				xx_For_Add.HSDATE = this2->m_Date;
				xx_For_Add.OpenPrice = _ttoi(spList.at(2));
				xx_For_Add.ClosePrice = _ttoi(spList.at(3));
				xx_For_Add.HightPrice = _ttoi(spList.at(10));
				xx_For_Add.LowPrice = _ttoi(spList.at(11));
				xx_For_Add.BuyVolume = _ttoi(spList.at(5));
				xx_For_Add.SellVolume = _ttoi(spList.at(8));
				xx_For_Add.TradeVolume = _ttoi(spList.at(14));
				xx_For_Add.HoldPosition = _ttoi(spList.at(15));
				// some parament forgeted
				
				
				//151425000
				HSTimer timeTester(spList.at(47),this2->m_Date);
				xx_For_Add.HSTIME = timeTester.m_StringTimer;
				if(timeTester.isInTradeTime() == true)
				{
					// 获取数据在交易时间之内
					bool inFiveMin = this2->IsIn5Min(this2->m_runData5Min.m_Data_Vec.at(0), timeTester);
					if (inFiveMin == true)
					{
						// 如果时间没有超出 5 分钟
						if (this2->m_runData5Min.m_Data_Vec.at(0).m_Data.at( this2->m_runData5Min.m_Data_Vec.at(0).m_Data.size()-1 ).HSTIME != xx_For_Add.HSTIME )
						{
							this2->m_runData5Min.m_Data_Vec.at(0).m_Data.push_back(xx_For_Add);
							// refresh rundata index
							//this2->m_runData5Min.m_Data_Vec.at(0).m_Data.insert(this2->m_runData5Min.m_Data_Vec.at(0).m_Data.begin,xx_For_Add);
						}

					}
					else
					{
						// 如果超出了 5 分钟
						xx_For_Add.timer.SetData(spList.at(47),this2->m_Date);
						CMinuteData tempMinData(xx_For_Add);
						tempMinData.init(xx_For_Add);
						tempMinData.clearData();
						tempMinData.m_Data.push_back(xx_For_Add);
						this2->m_runData5Min.m_Data_Vec.insert(this2->m_runData5Min.m_Data_Vec.begin(), tempMinData);
						// refresh rundata index

					}
					this2->m_runData5Min.RefreshIndex();
					/////////////////////////////////////////////////////////////////////////
					// 15 分钟的 数据
					bool inFifteenMin = this2->IsIn15Min(this2->m_runData15Min.m_Data_Vec.at(0), timeTester);
					if (inFifteenMin == true)
					{
						// 如果时间没有超出 15 分钟
						if (this2->m_runData15Min.m_Data_Vec.at(0).m_Data.at( this2->m_runData15Min.m_Data_Vec.at(0).m_Data.size()-1 ).HSTIME != xx_For_Add.HSTIME )
						{
							this2->m_runData15Min.m_Data_Vec.at(0).m_Data.push_back(xx_For_Add);
							// refresh rundata index
							//this2->m_runData5Min.m_Data_Vec.at(0).m_Data.insert(this2->m_runData5Min.m_Data_Vec.at(0).m_Data.begin,xx_For_Add);
						}

					}
					else
					{
						// 如果超出了 5 分钟
						xx_For_Add.timer.SetData(spList.at(47),this2->m_Date);
						CMinuteData tempMinData(xx_For_Add);
						tempMinData.init(xx_For_Add);
						tempMinData.clearData();
						tempMinData.m_Data.push_back(xx_For_Add);
						this2->m_runData15Min.m_Data_Vec.insert(this2->m_runData15Min.m_Data_Vec.begin(), tempMinData);
						// refresh rundata index

					}
					this2->m_runData15Min.RefreshIndex();

					// Open the buy tick
					if (this2->OpenBuyTickStrategy() == true && this2->IsOrdSentNow() == true && this2->hasHoldedNow() == false)
					{
						this2->SentOpenBuyOrd(spList.at(3));
						// write log database
						//SetSentOrderStauts(inputValue,_T("Open"),_T("Buy"),Int2CString(1));
						this2->WriteLogDatabase(spList.at(3),_T("Open"),_T("Buy"),this2->Int2CString(1));
					}
					// Open the Sell tick
					if (this2->OpenSellTickStrategy() == true && this2->IsOrdSentNow() == true && this2->hasHoldedNow() == false)
					{
						this2->SentOpenSellOrd(spList.at(3));
						
						// write log database
						//SetSentOrderStauts(inputValue,_T("Open"),_T("Buy"),Int2CString(1));
						this2->WriteLogDatabase(spList.at(3),_T("Open"),_T("Sell"),this2->Int2CString(1));
					}

					// false 时 有发送订单
					if (this2->IsOrdSentNow() == false)
					{
						// if there is a order now
						// check wether it successfull
						if(this2->CheckOrderSuccessfull(spList.at(3)) == true)
						{
							// 如果成交 改为持仓 开仓情况下改为持仓， 平仓情况下清空持仓
							this2->Transform_Delete_Order(spList.at(3));
							// write log database
							// 成交
							this2->WriteLogDatabase(spList.at(3),_T("Open"),_T("Buy"),this2->Int2CString(1));
							// delete order now
							this2->CleanOrderNowTable();
							//delete from OrderNow where ID = 24;
						}

					}

					if (this2->hasHoldedNow() == true && this2->IsOrdSentNow() == true)
					{
						// check hold is buy or sell
						if (this2->HolderType() == BUYORDER)
						{
							// check wether close the position
							if (this2->CloseBuyTickStrategy() == true)
							{
								// 进行关闭开多仓操作
								// 发送闭多仓order
								this2->SentCloseBuyOrd(spList.at(3));
							}
							// check wether close the position
							if (this2->CloseSellTickStrategy() == true)
							{
								// 进行关闭开多仓操作
								// 发送闭多仓order
								this2->SentCloseSellOrd(spList.at(3));
							}
							// write log database

						}
						else if(this2->HolderType() == SELLORDER)
						{

						}

					}
				}

			}
			free(p);
			//}
			//this2->m_Get_Data.SetWindowTextW(WStr);
		}

		//////////////////////////////////////////////////////////////////////////

		res = pcap_next_ex(adhandle,&header,&data);
		if(res==0)
		{
			Sleep(100);
			continue;
		}
		else if(res<0)
		{
			break;
		}

		//pcap_dump((u_char *)dumpfile, header, data);

		eth=(ether_header *)data;
		mac_string=eth->ether_shost;
		sprintf_s(List.sMac,"%02X:%02X:%02X:%02X:%02X:%02X",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
		mac_string=eth->ether_dhost;
		sprintf_s(List.dMac,"%02X:%02X:%02X:%02X:%02X:%02X",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
		ltoa(header->caplen,List.Len,10);
		
		memcpy(List.Text,data,45);//数据不含以太网头
		//memcpy(List.Text,data,90);//数据不含以太网头
		List.Text[45]='\0';
		this2->DecodeChar(List.Text,45);

		if (ntohs(eth->ether_type) == ETHERTYPE_IP)
		{
			IPHead=(iphead *)(data+14);
			ipaddr=IPHead->ip_souce_address;
			sprintf_s(List.sIP,"%d.%d.%d.%d",ipaddr.S_un.S_un_b.s_b1,ipaddr.S_un.S_un_b.s_b2,ipaddr.S_un.S_un_b.s_b3,ipaddr.S_un.S_un_b.s_b4);
			ipaddr=IPHead->ip_destination_address;
			sprintf_s(List.dIP,"%d.%d.%d.%d",ipaddr.S_un.S_un_b.s_b1,ipaddr.S_un.S_un_b.s_b2,ipaddr.S_un.S_un_b.s_b3,ipaddr.S_un.S_un_b.s_b4);

			switch(IPHead->ip_protocol)
			{
			case 6:
				//if(!this2->FilterSet.bAllProtocol && !this2->FilterSet.bTcp){bFilter=true;break;}
				strcpy(List.Protocol,"TCP");
				sprintf_s(List.sPort,"%d",ntohs( ((tcphead *)(data+16+20))->th_sport ));
				sprintf_s(List.dPort,"%d",ntohs( ((tcphead *)(data+16+20))->th_dport ));
				break;
			case 17:
				//if(!this2->FilterSet.bAllProtocol && !this2->FilterSet.bUdp){bFilter=true;break;}
				strcpy(List.Protocol,"UDP");
				sprintf_s(List.sPort,"%d",ntohs( ((udphead *)(data+16+20))->udp_source_port ));
				sprintf_s(List.dPort,"%d",ntohs( ((udphead *)(data+16+20))->udp_destinanion_port ));
				break;
			default:
				strcpy(List.Protocol,"未知IP包");
				strcpy(List.sIP,"----------");
				strcpy(List.dIP,"----------");
				strcpy(List.sPort,"--");
				strcpy(List.dPort,"--");
				break;
			}
			ltoa(i,List.ID,10);
			ltoa(j,List.TotalPacket,10);
		}
	}
	return 0;
}




//*************************************
// 函数名:  	Start
// 目的:	开启自动化交易
// 时间：   2014/06/01
//*************************************
void CStrategy::Start()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;
	m_Date = u_Res;
	// 提取以前的数据
	vector<vector<CString>> DataList = GetDataFromSQL(30000);
	// 处理数据
	m_Data = DataFactoryFiveMinute(DataList);
	m_Data.pop_back();
	//CRunningData runData(m_Data);
	m_runData5Min._init_(m_Data);


	//////////////////////////////////////////////////////////////////////////
	m_Data.clear();
	m_Data = DataFactoryFifteenMinute(DataList);
	m_Data.pop_back();
	m_runData15Min._init_(m_Data);
	//
	BOOL temp;
	hCapThread=AfxBeginThread(RuntimeThread,(LPVOID)this);
	//hCapErrorThread=AfxBeginThread(CapErrorThread,(LPVOID)this);

	//WaitForSingleObject(eThreadStart, INFINITE);
	//EnterCriticalSection(&csThreadStop);
	//temp=bThreadStop;
	//LeaveCriticalSection(&csThreadStop);

// 	if(temp)//线程自动退出
// 	{
// 		return;
// 	}
// 	else
// 	{
// 		//Status=1;
// 		printf("开始抓包");
// 	}

}

void CStrategy::DecodeChar( char *data,DWORD len )
{
	DWORD i;
	for(i=0;i<len;i++)
		if(data[i]=='\0')
			data[i]='.';
}



//*************************************
// 函数名:  	
// 目的:	分割成为 分钟 数据
// 时间：   2014/04/10
//*************************************
vector<CMinuteData> CStrategy::DataFactoryOneMinute( vector<vector<CString>> inputData )
{
	vector<CMinuteData> res;
	// 临时读取的容器
 	vector<CString> tempCal;
 	
 	if (inputData.size() != 0)
 	{
 		vector<CString> tempVec = inputData.at(0);
 		StrategyData ss(tempVec);
 
 		CMinuteData tempMinData(ss);
		int recordpos = 0;
// 		HSTimer tempTimeOld(tempVec.at(30));
// 		HSTimer tempTimeNew(tempVec.at(30));

  		for (int i = 1; i < inputData.size(); i++)
  		{

 			StrategyData tempData(inputData.at(i));
 			//if (tempMinData.IsInner(tempData) == true)
// 			if (tempMinData.m_ID_Timer.isSameMinute(tempData.timer) == true)
//  			{
//  				// 如果该数据在 tempMinData 内 则加入
// 				bool finded = false;
// 				for (int j=0; j<res.size(); j++)
// 				{
// 					if (res.at(j).m_ID_Timer.isSameMinute(tempData.timer) )
// 					{
// 						// 如果已经有相同的了
// 						res.at(j).m_Data.push_back(tempData);
// 						//tempMinData.init(tempData);
// 						finded = true;
// 					}
// 				}
// 
//  				//tempMinData.Add(tempData);
// 				//tempMinData.init(tempData);
//  			}
// 
//  			else
//  			{
			if (res.size() != 0)
			{
				if (res.at(recordpos).m_ID_Timer.isSameMinute(tempData.timer) )
				{
					// 如果已经有相同的了
					res.at(recordpos).m_Data.push_back(tempData);
					//tempMinData.init(tempData);
					//finded = true;
				}
				else
				{
					bool finded = false;
					for (int j=0; j<res.size(); j++)
					{
						if (res.at(j).m_ID_Timer.isSameMinute(tempData.timer) )
						{
							// 如果已经有相同的了
							res.at(j).m_Data.push_back(tempData);
							//tempMinData.init(tempData);
							finded = true;
							recordpos = j;
						}
					}
					if (finded != true)
					{
						tempMinData.init(tempData);
						tempMinData.clearData();
						tempMinData.m_Data.push_back(tempData);
						res.push_back(tempMinData);
						recordpos = res.size()-1;
					}	
				}
			}
			else
			{
				bool finded = false;
				for (int j=0; j<res.size(); j++)
				{
					if (res.at(j).m_ID_Timer.isSameMinute(tempData.timer) )
					{
						// 如果已经有相同的了
						res.at(j).m_Data.push_back(tempData);
						//tempMinData.init(tempData);
						finded = true;
						recordpos = j;
					}
				}
				if (finded != true)
				{
					tempMinData.init(tempData);
					tempMinData.clearData();
					tempMinData.m_Data.push_back(tempData);
					res.push_back(tempMinData);
					recordpos = res.size()-1;
				}
			}
			//}
  		}
 	}

	return res;
}

vector<CMinuteData> CStrategy::DataFactoryFiveMinute( vector<vector<CString>> inputData )
{
	vector<CMinuteData> res;
	// 临时读取的容器
	vector<CString> tempCal;
	if (inputData.size() != 0)
	{
		vector<CString> tempVec = inputData.at(0);
		StrategyData ss(tempVec);
		CMinuteData tempMinData(ss);
		int recordpos = 0;

		for (int i = 1; i < inputData.size(); i++)
		{

			StrategyData tempData(inputData.at(i));
			if (res.size() != 0)
			{
				if (res.at(recordpos).m_ID_Timer.isFiveMinute(tempData.timer) )
				{
					// 如果已经有相同的了
					res.at(recordpos).m_Data.push_back(tempData);
					//tempMinData.init(tempData);
					//finded = true;
				}
				else
				{
					bool finded = false;
					for (int j=0; j<res.size(); j++)
					{
						if (res.at(j).m_ID_Timer.isFiveMinute(tempData.timer) )
						{
							// 如果已经有相同的了
							res.at(j).m_Data.push_back(tempData);
							//tempMinData.init(tempData);
							finded = true;
							recordpos = j;
						}
					}
					if (finded != true)
					{
						tempMinData.init(tempData);
						tempMinData.clearData();
						tempMinData.m_Data.push_back(tempData);
						res.push_back(tempMinData);
						recordpos = res.size()-1;
					}	
				}
			}
			else
			{
				bool finded = false;
				for (int j=0; j<res.size(); j++)
				{
					if (res.at(j).m_ID_Timer.isFiveMinute(tempData.timer) )
					{
						// 如果已经有相同的了
						res.at(j).m_Data.push_back(tempData);
						//tempMinData.init(tempData);
						finded = true;
						recordpos = j;
					}
				}
				if (finded != true)
				{
					tempMinData.init(tempData);
					tempMinData.clearData();
					tempMinData.m_Data.push_back(tempData);
					res.push_back(tempMinData);
					recordpos = res.size()-1;
				}
			}
			//}
		}
	}
	return res;
}

bool CStrategy::IsIn5Min( CMinuteData dataStd, HSTimer inputTime )
{
	//dataStd.m_Data
	//dataStd.m_FirstData
	//dataStd.m_FirstData.timer
	bool res = dataStd.m_FirstData.timer.isFiveMinute(inputTime);

	return res;
}



//*************************************
// 函数名:  	GetTradeCode
// 目的:	获取交易代码
// 时间：   2014/03/25
//*************************************
CString CStrategy::GetTradeCode( CString data )
{
	//data = _T(" \x16赜鳊.\x14x櫅?.E(.釪H..u\x11?t祜伬?{\vU\vr.伟?  192rb1410");
	CString res = _T("");
	CString temp;

	int roo1 = 0;
	int recordIndex = 0;
	for (int i=data.GetLength(); i>0; i--)
	{
		temp = data.GetAt(i);
		//char* tempChar = temp.GetBuffer(0);
		//if((temp.GetBuffer()>=_T('0') && temp<='9') || temp == '.')
		if(((int)temp.GetAt(0)>=48&&(int)temp.GetAt(0)<=57))
		{
			printf("");
			if (roo1 == 0)
			{
				recordIndex = i;
			}
			roo1 = roo1+1;

			// 数字计数不为1时
			if (roo1 != 1)
			{
				if (abs(i - recordIndex ) != 1)
				{
					return res;
				}
			}

			// 
			recordIndex = i;
			res = temp + res;
		}
		else
		{
			res = temp + res;
		}
	}

	return res;
}

bool CStrategy::OpenBuyTickStrategy( void )
{
	//m_runData.m_Data_Vec.at(1).GetClosePrice() < m_runData.m_Boll_Down.at(0);
	// bottom cross m3
	if (m_runData5Min.m_Boll_Down.at(0) >= m_runData5Min.m_M3.at(0)
		&& m_runData5Min.m_Boll_Down.at(1) <= m_runData5Min.m_M3.at(1)
		&& m_runData15Min.m_Boll_mid.at(0) > m_runData15Min.m_Boll_mid.at(2) )
	{
		return true;
	}
	else
	{
		return false;
	}

}


bool CStrategy::OpenSellTickStrategy( void )
{
	bool closeBtop = false;
	for (int i = 0; i < 10; i++)
	{
		if (m_runData5Min.m_Close_Price_Idx.at(i) > m_runData5Min.m_Boll_up.at(i))
		{
			closeBtop = true;
			break;
		}
	}



	if (m_runData5Min.m_Boll_mid.at(0) >= m_runData5Min.m_Close_Price_Idx.at(0)
		&& m_runData5Min.m_Boll_mid.at(1) <= m_runData5Min.m_Close_Price_Idx.at(1)
		&& m_runData15Min.m_Boll_mid.at(0) < m_runData15Min.m_Boll_mid.at(2) 
		&& closeBtop == true )
	{
		return true;
	}
	else
	{
		return false;
	}

// 	if( m_runData5Min.m_Close_Price_Idx.at(0) > m_runData5Min.m_Boll_up.at(0) && 
// 		m_runData5Min.m_Close_Price_Idx.at(1) > m_runData5Min.m_Boll_up.at(1) &&
// 		m_runData5Min.m_Close_Price_Idx.at(2) > m_runData5Min.m_Boll_up.at(2))
// 	{ 
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
}


void CStrategy::SentOpenBuyOrd(CString inputValue)
{
	m_Trader->SetCode(m_Code);
	m_Trader->SetOpenClose(0);
	m_Trader->SetVolume(_T("1"));
	m_Trader->SetValue(inputValue);
	m_Trader->SetBuy_Sell(0);
	//
	SetSentOrderStauts(inputValue,_T("Open"),_T("Buy"),Int2CString(1));
	
	//m_Trader->SendOrder();
}

bool CStrategy::IsOrdSentNow( void )
{
	int OrderNum = 0;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	//VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			//printf(Msg);
		}
		else
		{
			return true;
		}
	}

// 	if(vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
		//this->WriteError(_T("写入数据是数据库重连成功！"));
		//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
		//printf("连接成功/r/n");
		//printf(Msg);
	OrderNum = m_vspdctomysql_Strategy->GetTableNum(_T("OrderNow"));
		//char* SQL_Count = _T("select count(*) from OrderNow");
		//char* SQL_char = this->CString2CharPt(SQL);
//	}
	//vspdctomysql->CloseMySQLConn();


	if (OrderNum == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CStrategy::SetSentOrderStauts(CString price, CString openclose,CString buysell, CString volume)
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);

	//select count(*) from table
 	CString SQL_Head = _T("insert into OrderNow (CODE,Price,OpenClose,BuySell,Volume,Time,Date) values(");//HSDate,
 	CString SQL_Value = _T("'") +
 		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
 		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + timeNow + _T("',") +
		_T("'") + u_Res + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;

	char* SQL_char = this->CString2CharPt(SQL);

	VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf(Msg);
		}
		else
		{
			return;
		}
	}
// 	if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
		//this->WriteError(_T("写入数据是数据库重连成功！"));
		//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
		//printf(_T("连接成功/r/n"));
	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
	{
		printf("插入成功/r/n");
		printf(Msg);
	}
//	}


	//vspdctomysql->CloseMySQLConn();
}


int CStrategy::GetTableNum( CString tableNam )
{
	char* Msg = "";//消息变量
	int row = 0;
	//vspdctomysql = new VspdcToMySQL;
	//VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}

// 	if(vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
		//this->WriteError(_T("写入数据是数据库重连成功！"));
		//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
		printf("连接成功/r/n");
		printf(Msg);
		CString SQL_Head = _T("select count(*) from ");
		CString SQL_Value = tableNam;
		CString SQL_Tail = _T(";");
		CString SQL = SQL_Head + SQL_Value + SQL_Tail;
		//char* SQL_char = this->CString2CharPt(SQL);
		row = m_vspdctomysql_Strategy->GetTableNum(tableNam);

	//}
	//vspdctomysql->CloseMySQLConn();
	return row;
}

void CStrategy::Transform_Delete_Order()
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("select * from OrderNow;");
	//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
	//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
	//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//CString getStr(str.c_str());
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() == 1 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		vector<CString> tempVec;
		for (int i = 0; i<DataList.size(); i++)
		{
			tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间
		// 
		//ResList.at(0).at(0);	// ID 1,code; 2,Price; 3,OpenClose; 4,BuySell; 5,Volume
		// 6,Time,	7,Date, 8 Mark
		CString Open_Close = tempVec.at(3);
		//
		if (Open_Close == _T("Open"))
		{
			CString SQL_Head = _T("insert into Hold (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
			CString SQL_Value = _T("'") +
				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
				_T("'") + ResList.at(0).at(6) + _T("',") +
				_T("'") + ResList.at(0).at(7) + _T("'");
			CString SQL_Tail = _T(")");
			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
			char* SQL_char = this->CString2CharPt(SQL);
			if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
				printf("插入成功/r/n");

			//WriteLogDatabase(spList.at(3),_T("Open"),_T("Buy"),Int2CString(1));
		}
		else if(Open_Close == _T("Close"))
		{
			//CString SQL_Head = _T("insert into Hold (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
			//CString SQL_Value = _T("'") +
// 				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
// 				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
// 				_T("'") + ResList.at(0).at(6) + _T("',") +
// 				_T("'") + ResList.at(0).at(7) + _T("'");
// 			CString SQL_Tail = _T(")");
// 			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
// 			char* SQL_char = this->CString2CharPt(SQL);
			m_vspdctomysql_Strategy->CleanTable(_T("Hold"));
			//if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
			printf("清空/r/n");
		}
		

	}
	//}
	//m_vspdctomysql_Strategy->CloseMySQLConn();
}

void CStrategy::Transform_Delete_Order( CString nowValue )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("select * from OrderNow;");
	//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
	//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
	//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//CString getStr(str.c_str());
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() == 1 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		vector<CString> tempVec;
		for (int i = 0; i<DataList.size(); i++)
		{
			tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间
		// 
		//ResList.at(0).at(0);	// ID 1,code; 2,Price; 3,OpenClose; 4,BuySell; 5,Volume
		// 6,Time,	7,Date, 8 Mark
		CString Open_Close = tempVec.at(3);
		//
		if (Open_Close == _T("Open"))
		{
			CString SQL_Head = _T("insert into Hold (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
			CString SQL_Value = _T("'") +
				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
				_T("'") + ResList.at(0).at(6) + _T("',") +
				_T("'") + ResList.at(0).at(7) + _T("'");
			CString SQL_Tail = _T(")");
			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
			char* SQL_char = this->CString2CharPt(SQL);
			if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
				printf("插入成功/r/n");

			WriteLogDatabase(nowValue,_T("Open"),_T("Buy"),Int2CString(1));
		}
		else if(Open_Close == _T("Close"))
		{
			//CString SQL_Head = _T("insert into Hold (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
			//CString SQL_Value = _T("'") +
			// 				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
			// 				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
			// 				_T("'") + ResList.at(0).at(6) + _T("',") +
			// 				_T("'") + ResList.at(0).at(7) + _T("'");
			// 			CString SQL_Tail = _T(")");
			// 			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
			// 			char* SQL_char = this->CString2CharPt(SQL);
			m_vspdctomysql_Strategy->CleanTable(_T("Hold"));
			WriteLogDatabase(nowValue,_T("Close"),_T("Sell"),Int2CString(1));
			//if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
			printf("清空/r/n");
		}


	}
	//}
	//m_vspdctomysql_Strategy->CloseMySQLConn();
}

bool CStrategy::CheckOrderSuccessfull( CString nowValue )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return false;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
// 	if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
// 		printf("连接成功/r/n");
// 		//char * code = CString2CharPt(m_Code);
	CString SQL = _T("select * from OrderNow;");
	//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
	//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
	//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//CString getStr(str.c_str());
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() > 0 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		for (int i = 0; i<DataList.size(); i++)
		{
			vector<CString> tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间

	}
	CString sendValue = ResList.at(0).at(2);
	float nowV = _ttoi(nowValue);
	float sendV = _ttoi(sendValue);
	CString BuyOrSell = ResList.at(0).at(4);
	if (BuyOrSell == _T("Buy"))
	{
		if(nowV <= sendV-2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else//检查sell单成交与否
	{
		if (nowV >= sendV+2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//
// 	if (BuyOrSell == _T("Sell"))
// 	{
// 
// 	}
	//}
	//m_vspdctomysql_Strategy->CloseMySQLConn();
	return false;
}

bool CStrategy::hasHoldedNow( void )
{
	int HoldNum = GetTableNum(_T("Hold"));
	if (HoldNum == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

void CStrategy::WriteLogDatabase( CString price,CString openclose, CString buysell, CString volume )
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);

	//select count(*) from table
	CString SQL_Head = _T("insert into TradeRecord (CODE,PRICE,OpenClose,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + timeNow + _T("',") +
		_T("'") + u_Res + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;

	char* SQL_char = this->CString2CharPt(SQL);

	//VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
	}

	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
		printf("插入成功/r/n");
	//vspdctomysql->CloseMySQLConn();
}

void CStrategy::CleanOrderNowTable()
{
	//delete from TableName 清空列表
	char* Msg = "";//消息变量
	int row = 0;
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
	}

// 	if(vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
// 		//this->WriteError(_T("写入数据是数据库重连成功！"));
// 		//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
// 		printf("连接成功/r/n");
// 		printf(Msg);
// 		CString SQL_Head = _T("select count(*) from ");
// 		//CString SQL_Value = tableNam;
// 		CString SQL_Tail = _T(";");
// 		//CString SQL = SQL_Head + SQL_Value + SQL_Tail;
// 		//char* SQL_char = this->CString2CharPt(SQL);
// 		//row = vspdctomysql->GetTableNum(_T("OrderNow"));
// 		vspdctomysql->CleanTable(_T("OrderNow"));
// 	}
	m_vspdctomysql_Strategy->CleanTable(_T("OrderNow"));
	//m_vspdctomysql_Strategy->CloseMySQLConn();
	//return row;
}

bool CStrategy::CloseBuyTickStrategy( void )
{
	// 获取开仓时的数据
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("SELECT * FROM Hold LIMIT 1;");
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);

	if (DataList.size() == 1)
	{
		vector<CString> tempVec = SplitString(DataList.at(0));
		//CString OpenValue = tempVec.at(2);
		float OpenValue = _tstof(tempVec.at(2));
		float NowCloseValue = m_runData5Min.m_Close_Price_Idx.at(0);
		float NowTopValue = m_runData5Min.m_Boll_up.at(0);
		// 关买仓
		if (NowCloseValue - NowTopValue > 0)
		{
			return true;
		}
// 		if (NowCloseValue >= m_runData5Min.m_Boll_mid.at(0))
// 		{
// 			return true;
// 		}
		if ( NowCloseValue - OpenValue <= -14)
		{
			return true;
		}
		//CString NowValue = m_runData.m_Close_Price_Idx.at(0);

// 		if (tempVec.at(3) == _T("Buy"))
// 		{
// 			return BUYORDER;
// 		}
// 		else if (tempVec.at(3) == _T("Sell"))
// 		{
// 			return SELLORDER;
// 		}
	}
// 	if( m_runData.m_Close_Price_Idx.at(0)<m_runData.m_Boll_Down.at(0) && 
// 		m_runData.m_Close_Price_Idx.at(1)<m_runData.m_Boll_Down.at(1) &&
// 		m_runData.m_Close_Price_Idx.at(2)<m_runData.m_Boll_Down.at(2))
// 	if(m_runData.m_Close_Price_Idx.at(0) >= m_runData.m_Boll_mid.at(0))
// 	{
// 		return true;
// 	}
	return false;
}

void CStrategy::SetMySQLpt( VspdcToMySQL * sqlPt )
{
	m_vspdctomysql_Strategy = sqlPt;
}

void CStrategy::Stop()
{

}

int CStrategy::HolderType( void )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("SELECT * FROM Hold LIMIT 1;");
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);
	

	if( DataList.size() == 1 )
	{
		//for (int i = 0; i<DataList.size(); i++)
		//{
		vector<CString> tempVec = SplitString(DataList.at(0));
			//CString testStr = tempVec.at(30);
			//ResList.push_back(tempVec);
		//}
		// 第三十位 是HS时间
		// 
		//ResList.at(0).at(0);	
		// ID 1,code; 2,Price; 3,OpenClose; 4,BuySell; 5,Volume
		// 6,Time,	7,Date, 8 Mark
		//
		//CString testStr = tempVec.at(4);
		if (tempVec.at(3) == _T("Buy"))
		{
			return BUYORDER;
		}
		else if (tempVec.at(3) == _T("Sell"))
		{
			return SELLORDER;
		}
		else
		{
			return -1;
		}
	}
	return  -1;
}

void CStrategy::SentCloseBuyOrd( CString inputValue )
{
	m_Trader->SetCode(m_Code);
	m_Trader->SetOpenClose(1);
	m_Trader->SetVolume(_T("1"));
	m_Trader->SetValue(inputValue);
	m_Trader->SetBuy_Sell(1);
	//
	SetSentOrderStauts(inputValue,_T("Close"),_T("Sell"),Int2CString(1));

	//m_Trader->SendOrder();
}

vector<CMinuteData> CStrategy::DataFactoryFifteenMinute( vector<vector<CString>> inputData )
{
	vector<CMinuteData> res;
	// 临时读取的容器
	vector<CString> tempCal;
	if (inputData.size() != 0)
	{
		vector<CString> tempVec = inputData.at(0);
		StrategyData ss(tempVec);
		CMinuteData tempMinData(ss);
		int recordpos = 0;
		for (int i = 1; i < inputData.size(); i++)
		{
			StrategyData tempData(inputData.at(i));
			if (res.size() != 0)
			{
				if (res.at(recordpos).m_ID_Timer.isFifteenMinute(tempData.timer) )
				{
					// 如果已经有相同的了
					res.at(recordpos).m_Data.push_back(tempData);
					//tempMinData.init(tempData);
					//finded = true;
				}
				else
				{
					bool finded = false;
					for (int j=0; j<res.size(); j++)
					{
						if (res.at(j).m_ID_Timer.isFifteenMinute(tempData.timer) )
						{
							// 如果已经有相同的了
							res.at(j).m_Data.push_back(tempData);
							//tempMinData.init(tempData);
							finded = true;
							recordpos = j;
						}
					}
					if (finded != true)
					{
						tempMinData.init(tempData);
						tempMinData.clearData();
						tempMinData.m_Data.push_back(tempData);
						res.push_back(tempMinData);
						recordpos = res.size()-1;
					}	
				}
			}
			else
			{
				bool finded = false;
				for (int j=0; j<res.size(); j++)
				{
					if (res.at(j).m_ID_Timer.isFifteenMinute(tempData.timer) )
					{
						// 如果已经有相同的了
						res.at(j).m_Data.push_back(tempData);
						//tempMinData.init(tempData);
						finded = true;
						recordpos = j;
					}
				}
				if (finded != true)
				{
					tempMinData.init(tempData);
					tempMinData.clearData();
					tempMinData.m_Data.push_back(tempData);
					res.push_back(tempMinData);
					recordpos = res.size()-1;
				}
			}
		}
	}
	return res;
}

bool CStrategy::IsIn15Min( CMinuteData dataStd, HSTimer inputTime )
{
	//bool res = dataStd.m_FirstData.timer.isFiveMinute(inputTime);
	bool res = dataStd.m_FirstData.timer.isFifteenMinute(inputTime);
	return res;
}

void CStrategy::SentOpenSellOrd( CString inputValue )
{
	m_Trader->SetCode(m_Code);
	m_Trader->SetOpenClose(0);
	m_Trader->SetVolume(_T("1"));
	m_Trader->SetValue(inputValue);
	m_Trader->SetBuy_Sell(1);
	//
	SetSentOrderStauts(inputValue,_T("Open"),_T("Sell"),Int2CString(1));

	//m_Trader->SendOrder();
}

void CStrategy::SentCloseSellOrd( CString inputValue )
{
	m_Trader->SetCode(m_Code);
	m_Trader->SetOpenClose(1);
	m_Trader->SetVolume(_T("1"));
	m_Trader->SetValue(inputValue);
	m_Trader->SetBuy_Sell(0);
	//
	SetSentOrderStauts(inputValue,_T("Close"),_T("Sell"),Int2CString(1));
}

bool CStrategy::CloseSellTickStrategy( void )
{
	// 获取开仓时的数据
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("SELECT * FROM Hold LIMIT 1;");
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);

	if (DataList.size() == 1)
	{
		vector<CString> tempVec = SplitString(DataList.at(0));
		//CString OpenValue = tempVec.at(2);
		float OpenValue = _tstof(tempVec.at(2));
		float NowCloseValue = m_runData5Min.m_Close_Price_Idx.at(0);
		float NowBottomValue = m_runData5Min.m_Boll_Down.at(0);
		// 关闭卖仓
		if (NowCloseValue - NowBottomValue < 0)
		{
			return true;
		}

		if ( NowCloseValue - OpenValue >= 14)
		{
			return true;
		}

	}

	return false;
}


void CStrategy::Sim()
{
	theApp.MessageShow(_T("开始"));
	m_FastDeal = false;

	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;
	m_Date = u_Res;
	//theApp.updataApp();
	// 提取以前的数据 从固定位置之前的数据
	//
	int startID = 289908;
	int endID = startID + 40000;
	m_Sim_ID = endID + 1;
	//vector<vector<CString>> DataList = GetDataFromSQL(100000);
	//vector<CString> sssData = GetDataByID(586677);
	vector<vector<CString>> DataList = GetDataFromSQL(startID, endID);
	//vector<vector<CString>> SimDataList;
	// 截获模拟数据 pos
// 	int StartPos = 0, EndPosLength = 20000;
// 	for (int i = DataList.size() - EndPosLength; i < DataList.size(); i++)
// 	{
// 		//DataList.at(i);
// 		SimDataList.push_back(DataList.at(i));
// 	}
// 	
// 	for (int j = 0; j<DataList.size() - EndPosLength; j++)
// 	{
// 		TestDatalist.push_back(DataList.at(j));
// 	}
	// 处理数据
	m_Sim_Data = DataFactoryFiveMinute(DataList);
	m_Sim_Data.pop_back();
	//CRunningData runData(m_Data);
	m_SimData5Min._init_(m_Sim_Data);


	//////////////////////////////////////////////////////////////////////////
	m_Sim_Data.clear();
	m_Sim_Data = DataFactoryFifteenMinute(DataList);
	m_Sim_Data.pop_back();
	m_SimData15Min._init_(m_Sim_Data);
	//
	BOOL temp;
	//hCapThread=AfxBeginThread(RuntimeThread,(LPVOID)this);
	//theApp.updataApp();
	SimProgress();
	theApp.MessageShow(_T("完成"));
}

void CStrategy::SimProgress()
{
	//int i = TestDatalist.size();
	int maxID = m_Sim_ID + 10000;
	for (int i=m_Sim_ID; i < 588863; i++)
	{
		if (i == 329909)
		{
			printf("ss");
		}
		//////////////////////////////////////////////////////////////////////////
		vector<CString> testData = GetDataByID(i);
		if (testData.size() != 52)
		{
			continue;
		}
		//vector<CString> testData = TestDatalist.at(i-1);
		

		theApp.updataApp(Int2CString(i));
		if ( testData.at(1) == m_Code)
		{

 			// 取到数据为策略数据
			StrategyData xx_For_Add;
			xx_For_Add.ID = -1;
			xx_For_Add.CODE = m_Code;
// 
 			xx_For_Add.HSDATE = m_Date;
 			xx_For_Add.OpenPrice = _ttoi(testData.at(3));
			xx_For_Add.ClosePrice = _ttoi(testData.at(7));

			xx_For_Add.HightPrice = _ttoi(testData.at(11));
			xx_For_Add.LowPrice = _ttoi(testData.at(12));
			xx_For_Add.BuyVolume = _ttoi(testData.at(9));
			xx_For_Add.SellVolume = _ttoi(testData.at(8));

			xx_For_Add.TradeVolume = _ttoi(testData.at(15));
			xx_For_Add.HoldPosition = _ttoi(testData.at(16));

			HSTimer timeTester(testData.at(30),m_Date);
			xx_For_Add.HSTIME = timeTester.m_StringTimer;

			//////////////////////////////////////////////////////////////////////////
			if(timeTester.isInTradeTime() == true)
			{
			 	// 获取数据在交易时间之内
			 	bool inFiveMin = IsIn5Min(m_SimData5Min.m_Data_Vec.at(0), timeTester);
			 	if (inFiveMin == true)
			 	{
			 		// 如果时间没有超出 5 分钟
			 		if (m_SimData5Min.m_Data_Vec.at(0).m_Data.at( m_SimData5Min.m_Data_Vec.at(0).m_Data.size()-1 ).HSTIME != xx_For_Add.HSTIME )
			 		{
			 			m_SimData5Min.m_Data_Vec.at(0).m_Data.push_back(xx_For_Add);
			 			// refresh rundata index
			 			//this2->m_runData5Min.m_Data_Vec.at(0).m_Data.insert(this2->m_runData5Min.m_Data_Vec.at(0).m_Data.begin,xx_For_Add);
			 		}
			 
			 	}
			 	else
			 	{
			 		// 如果超出了 5 分钟
			 		xx_For_Add.timer.SetData(testData.at(30),m_Date);
			 		CMinuteData tempMinData(xx_For_Add);
			 		tempMinData.init(xx_For_Add);
			 		tempMinData.clearData();
			 		tempMinData.m_Data.push_back(xx_For_Add);
			 		m_SimData5Min.m_Data_Vec.insert(m_SimData5Min.m_Data_Vec.begin(), tempMinData);
					//
					//m_SimData5Min.m_Data_Vec.pop_back();
					//m_SimData5Min.PopData();
			 		// refresh rundata index
			 
			 	}
			 	m_SimData5Min.RefreshIndex();
			 	/////////////////////////////////////////////////////////////////////////
			 	// 15 分钟的 数据
			 	bool inFifteenMin = IsIn15Min(m_SimData15Min.m_Data_Vec.at(0), timeTester);
			 	if (inFifteenMin == true)
			 	{
			 		// 如果时间没有超出 15 分钟
			 		if (m_SimData15Min.m_Data_Vec.at(0).m_Data.at( m_SimData15Min.m_Data_Vec.at(0).m_Data.size()-1 ).HSTIME != xx_For_Add.HSTIME )
			 		{
			 			m_SimData15Min.m_Data_Vec.at(0).m_Data.push_back(xx_For_Add);
			 			// refresh rundata index
			 			//this2->m_runData5Min.m_Data_Vec.at(0).m_Data.insert(this2->m_runData5Min.m_Data_Vec.at(0).m_Data.begin,xx_For_Add);
			 		}
			 
			 	}
			 	else
			 	{
			 		// 如果超出了 15 分钟
			 		xx_For_Add.timer.SetData(testData.at(30),m_Date);
			 		CMinuteData tempMinData(xx_For_Add);
			 		tempMinData.init(xx_For_Add);
			 		tempMinData.clearData();
			 		tempMinData.m_Data.push_back(xx_For_Add);
			 		m_SimData15Min.m_Data_Vec.insert(m_SimData15Min.m_Data_Vec.begin(), tempMinData);
			 		// refresh rundata index
					//m_SimData15Min.PopData();
					//m_SimData15Min.m_Data_Vec.pop_back();
			 	}
			 	m_SimData15Min.RefreshIndex();
			 
				///	跳出 调用了以前的策略
			 	// Open the buy tick
			 	if (OpenBuyTickStrategy_Sim() == true && IsOrdSentNow_Sim() == true && hasHoldedNow_Sim() == false)
			 	{
			 		SentOpenBuyOrd_Sim(testData);
			 		// write log database
			 		//SetSentOrderStauts(inputValue,_T("Open"),_T("Buy"),Int2CString(1));
			 		//WriteLogDatabase_Sim(testData,_T("Open"),_T("Buy"),Int2CString(1), _T("开买仓"));
			 	}
			 	// Open the Sell tick
			 	if (OpenSellTickStrategy_Sim() == true && IsOrdSentNow_Sim() == true && hasHoldedNow_Sim() == false)
			 	{
					//testData.at(30);
			 		SentOpenSellOrd_Sim(testData);

			 		// write log database
			 		//SetSentOrderStauts(inputValue,_T("Open"),_T("Buy"),Int2CString(1));
			 		//WriteLogDatabase_Sim(testData,_T("Open"),_T("Sell"),Int2CString(1),_T("开卖仓"));
			 	}
			 
			 	// false 时 有发送订单
			 	if (IsOrdSentNow_Sim() == false)
			 	{
			 		// if there is a order now
			 		// check wether it successfull
			 		if(CheckOrderSuccessfull_Sim(testData.at(7)) == true)
			 		{
			 			// 如果成交 改为持仓 开仓情况下改为持仓， 平仓情况下清空持仓
			 			//Transform_Delete_Order_Sim(testData.at(7));
			 			// write log database
			 			// 成交
						if (GetCurrentOrder_Sim() == BUY && GetCurrentOrderType_Sim() == OPEN)
						{
							if (m_FastDeal == true)
							{
								int tempInt = _ttoi(testData.at(7));
								tempInt = tempInt + FASTDEAL;
								CString valueStr = Int2CString(tempInt);

								WriteLogDatabase_Sim(valueStr,_T("Open"),_T("Buy"),Int2CString(1), _T("成交"),testData.at(29));

								Transform_Delete_Order_Sim(valueStr);
							}
							else
							{
								WriteLogDatabase_Sim(testData,_T("Open"),_T("Buy"),Int2CString(1), _T("成交"));
								Transform_Delete_Order_Sim(testData.at(7));
							}
							//Transform_Delete_Order_Sim(testData.at(7));
						}
						else if (GetCurrentOrder_Sim() == BUY && GetCurrentOrderType_Sim() == CLOSE)
						{
							if (m_FastDeal == true)
							{
								int tempInt = _ttoi(testData.at(7));
								tempInt = tempInt + FASTDEAL;
								CString valueStr = Int2CString(tempInt);

								WriteLogDatabase_Sim(valueStr,_T("Close"),_T("Buy"),Int2CString(1), _T("成交"),testData.at(29));
							}
							else
							{
								WriteLogDatabase_Sim(testData,_T("Close"),_T("Buy"),Int2CString(1), _T("成交"));
							}

							//WriteLogDatabase_Sim(testData,_T("Close"),_T("Buy"),Int2CString(1), _T("成交"));
							CleanOrderHold_Sim();
						}
						else if (GetCurrentOrder_Sim() == SELL && GetCurrentOrderType_Sim() == OPEN)
						{
							if (m_FastDeal == true)
							{
								int tempInt = _ttoi(testData.at(7));
								tempInt = tempInt - FASTDEAL;
								CString valueStr = Int2CString(tempInt);

								WriteLogDatabase_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1), _T("成交"),testData.at(29));
								Transform_Delete_Order_Sim(valueStr);
							}
							else
							{
								WriteLogDatabase_Sim(testData,_T("Open"),_T("Sell"),Int2CString(1), _T("成交"));
								Transform_Delete_Order_Sim(testData.at(7));
							}
							//WriteLogDatabase_Sim(testData,_T("Open"),_T("Sell"),Int2CString(1), _T("成交"));
						}
						else if (GetCurrentOrder_Sim() == SELL && GetCurrentOrderType_Sim() == CLOSE)
						{
							if (m_FastDeal == true)
							{
								int tempInt = _ttoi(testData.at(7));
								tempInt = tempInt - FASTDEAL;
								CString valueStr = Int2CString(tempInt);

								WriteLogDatabase_Sim(valueStr,_T("Close"),_T("Sell"),Int2CString(1), _T("成交"),testData.at(29));
							}
							else
							{
								WriteLogDatabase_Sim(testData,_T("Close"),_T("Sell"),Int2CString(1), _T("成交"));
							}

							//WriteLogDatabase_Sim(testData,_T("Close"),_T("Sell"),Int2CString(1), _T("成交"));
							CleanOrderHold_Sim();
						}
			 			
			 			// delete order now
			 			CleanOrderNowTable_Sim();
			 			//delete from OrderNow where ID = 24;
			 		}
			 
			 	}
			 
			 	if (hasHoldedNow_Sim() == true && IsOrdSentNow_Sim() == true)
			 	{
			 		// check hold is buy or sell
			 		if (HolderType_Sim() == BUYORDER)
			 		{
			 			// check wether close the position
			 			if (CloseBuyTickStrategy_Sim() == true)
			 			{
			 				// 进行关闭开多仓操作
			 				// 发送闭多仓order
			 				//SentCloseBuyOrd_Sim(testData.at(7));
							SentCloseBuyOrd_Sim(testData);
							WriteLogDatabase_Sim(testData,_T("Close"),_T("Sell"),Int2CString(1),_T("平买仓"));

			 			}
			 			// write log database
						
			 		}
			 		else if(HolderType_Sim() == SELLORDER)
			 		{
						// check wether close the position
						if (CloseSellTickStrategy_Sim() == true)
						{
							// 进行关闭开多仓操作
							// 发送闭多仓order
							//SentCloseSellOrd_Sim(testData.at(7));
							SentCloseSellOrd_Sim(testData);
							WriteLogDatabase_Sim(testData,_T("Close"),_T("Buy"),Int2CString(1),_T("平卖仓"));
						}

			 		}
			 
			 	}

			}
		}
	}
}

bool CStrategy::OpenBuyTickStrategy_Sim( void )
{
	//m_runData5Min.m_Boll_Down.at(0) >= m_runData5Min.m_M3.at(0)
	//&& m_runData5Min.m_Boll_Down.at(1) <= m_runData5Min.m_M3.at(1)
//		&& m_runData15Min.m_Boll_mid.at(0) > m_runData15Min.m_Boll_mid.at(2)

	if (m_SimData5Min.m_Boll_Down.at(0) >= m_SimData5Min.m_Close_Price_Idx.at(0)
		&& m_SimData5Min.m_Boll_Down.at(1) <= m_SimData5Min.m_Close_Price_Idx.at(1)
		&& m_SimData5Min.m_Boll_mid.at(0) > m_SimData5Min.m_Boll_mid.at(3)
		)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CStrategy::OpenSellTickStrategy_Sim( void )
{
	bool closeBtop = false;
 	for (int i = 0; i < 10; i++)
 	{
 		if (m_SimData5Min.m_Close_Price_Idx.at(i) > m_SimData5Min.m_Boll_up.at(i))
 		{
 			closeBtop = true;
 			break;
 		}
 	}

	//return false;
// 	if (m_runData5Min.m_Boll_mid.at(0) >= m_runData5Min.m_Close_Price_Idx.at(0)
// 		&& m_runData5Min.m_Boll_mid.at(1) <= m_runData5Min.m_Close_Price_Idx.at(1)
// 		&& m_runData15Min.m_Boll_mid.at(0) < m_runData15Min.m_Boll_mid.at(2) 
// 		&& closeBtop == true )

	if (m_SimData5Min.m_Boll_up.at(0) >= m_SimData5Min.m_Close_Price_Idx.at(0)
		&& m_SimData5Min.m_Boll_up.at(1) <= m_SimData5Min.m_Close_Price_Idx.at(1)
		//&& m_runData15Min.m_Boll_mid.at(0) < m_runData15Min.m_Boll_mid.at(2) 
		&& closeBtop == true 
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CStrategy::CloseBuyTickStrategy_Sim( void )
{
	// 获取开仓时的数据
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("SELECT * FROM Hold_sim LIMIT 1;");
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);

	if (DataList.size() == 1)
	{
		vector<CString> tempVec = SplitString(DataList.at(0));
		//CString OpenValue = tempVec.at(2);
		float OpenValue = _tstof(tempVec.at(2));
		float NowCloseValue = m_SimData5Min.m_Close_Price_Idx.at(0);
		float NowTopValue = m_SimData5Min.m_Boll_up.at(0);
		// 关买仓
// 		if (NowCloseValue - NowTopValue > 0)
// 		{
// 			return true;
// 		}
		if (NowCloseValue - OpenValue > 3)
		{
			return true;
		}
		if ( NowCloseValue - OpenValue <= -10)
		{
			return true;
		}
	}
	return false;
}

bool CStrategy::CloseSellTickStrategy_Sim( void )
{
	// 获取开仓时的数据
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量

	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}

	printf("连接成功/r/n");

	CString SQL = _T("SELECT * FROM Hold_sim LIMIT 1;");
	int row = GetTableNum(_T("OrderNow"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);
// 	if (m_FastDeal == true)
// 	{
// 		return true;
// 	}

	if (DataList.size() == 1)
	{
		vector<CString> tempVec = SplitString(DataList.at(0));

		float OpenValue = _tstof(tempVec.at(2));
		float NowCloseValue = m_SimData5Min.m_Close_Price_Idx.at(0);
		float NowBottomValue = m_SimData5Min.m_Boll_Down.at(0);
		// 关闭卖仓
		if (NowCloseValue - NowBottomValue < 0)
		{
			return true;
		}
		if ( NowCloseValue - OpenValue <= -3)
		{
			return true;
		}
		if ( NowCloseValue - OpenValue >= 10)
		{
			return true;
		}

	}

	return false;
}

void CStrategy::SentOpenBuyOrd_Sim( CString inputValue )
{
// 	m_Trader->SetCode(m_Code);
// 	m_Trader->SetOpenClose(0);
// 	m_Trader->SetVolume(_T("1"));
// 	m_Trader->SetValue(inputValue);
// 	m_Trader->SetBuy_Sell(0);
	//
	CString valueStr = inputValue;
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt + FASTDEAL;
		valueStr = Int2CString(tempInt);
	}
	SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Buy"),Int2CString(1));
}

void CStrategy::SentOpenBuyOrd_Sim( vector<CString> inputData )
{
	CString valueStr = inputData.at(3);
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt + FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Buy"),Int2CString(1), inputData.at(29),_T("追加"));
		
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Buy"),Int2CString(1), inputData.at(29));
	}
	WriteLogDatabase_Sim(valueStr,_T("Open"),_T("Buy"),Int2CString(1), _T("开买仓"),inputData.at(29));
}

void CStrategy::SentCloseBuyOrd_Sim( CString inputValue )
{
// 	m_Trader->SetCode(m_Code);
// 	m_Trader->SetOpenClose(1);
// 	m_Trader->SetVolume(_T("1"));
// 	m_Trader->SetValue(inputValue);
// 	m_Trader->SetBuy_Sell(1);
	CString valueStr = inputValue;
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt - FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Sell"),Int2CString(1),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Sell"),Int2CString(1));
	}
	//
	
}

void CStrategy::SentCloseBuyOrd_Sim( vector<CString> inputData )
{
	CString valueStr = inputData.at(3);
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt - FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Sell"),Int2CString(1), inputData.at(29),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Sell"),Int2CString(1), inputData.at(29));
	}
	
}

void CStrategy::SetSentOrderStauts_Sim( CString price,CString openclose, CString buysell, CString volume )
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);

	//select count(*) from table
	CString SQL_Head = _T("insert into OrderNow_sim (CODE,Price,OpenClose,BuySell,Volume,Time,Date) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + timeNow + _T("',") +
		_T("'") + u_Res + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;

	char* SQL_char = this->CString2CharPt(SQL);

	VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf(Msg);
		}
		else
		{
			return;
		}
	}

	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
	{
		printf("插入成功/r/n");
		printf(Msg);
	}

}

void CStrategy::SetSentOrderStauts_Sim( CString price,CString openclose, CString buysell, CString volume,CString time )
{
// 	SYSTEMTIME st;
// 	GetSystemTime(&st);
 	CString u_Year,u_Month,u_Day,u_Res;
// 	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
// 	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;
// 
// 	time_t t = time(0); 
// 	char tmp[64]; 
// 	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
// 	CString timeNow(tmp);
	int pos = time.Find(' ');
	u_Res = time.Left(pos);
	//select count(*) from table
	CString SQL_Head = _T("insert into OrderNow_sim (CODE,Price,OpenClose,BuySell,Volume,Time,Date) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + time + _T("',") +
		_T("'") + u_Res + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;

	char* SQL_char = this->CString2CharPt(SQL);

	VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf(Msg);
		}
		else
		{
			return;
		}
	}

	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
	{
		printf("插入成功/r/n");
		printf(Msg);
	}
}

void CStrategy::SetSentOrderStauts_Sim( CString price,CString openclose, CString buysell, CString volume,CString time,CString mark )
{
	// 	SYSTEMTIME st;
	// 	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	// 	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	// 	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;
	// 
	// 	time_t t = time(0); 
	// 	char tmp[64]; 
	// 	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	// 	CString timeNow(tmp);
	int pos = time.Find(' ');
	u_Res = time.Left(pos);
	//select count(*) from table
	CString SQL_Head = _T("insert into OrderNow_sim (CODE,Price,OpenClose,BuySell,Volume,Time,Date,Mark) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + time + _T("',") +
		_T("'") + u_Res + _T("',") +
		_T("'") + mark + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;

	char* SQL_char = this->CString2CharPt(SQL);

	VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf(Msg);
		}
		else
		{
			return;
		}
	}

	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
	{
		printf("插入成功/r/n");
		printf(Msg);
	}
}

void CStrategy::SentOpenSellOrd_Sim( CString inputValue )
{
// 	m_Trader->SetCode(m_Code);
// 	m_Trader->SetOpenClose(0);
// 	m_Trader->SetVolume(_T("1"));
// 	m_Trader->SetValue(inputValue);
// 	m_Trader->SetBuy_Sell(1);

	//
	CString valueStr = inputValue;
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt - FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1));
	}
	

}

void CStrategy::SentOpenSellOrd_Sim( vector<CString> inputData )
{
	CString valueStr = inputData.at(7);
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt - FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1),inputData.at(29),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1),inputData.at(29));
	}
	WriteLogDatabase_Sim(valueStr,_T("Open"),_T("Sell"),Int2CString(1),_T("开卖仓"),inputData.at(29));
}

void CStrategy::SentCloseSellOrd_Sim( CString inputValue )
{
// 	m_Trader->SetCode(m_Code);
// 	m_Trader->SetOpenClose(1);
// 	m_Trader->SetVolume(_T("1"));
// 	m_Trader->SetValue(inputValue);
// 	m_Trader->SetBuy_Sell(0);
	//
	CString valueStr = inputValue;
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt + FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Buy"),Int2CString(1),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Buy"),Int2CString(1));
	}
	
}

void CStrategy::SentCloseSellOrd_Sim( vector<CString> inputData )
{
	CString valueStr = inputData.at(7);
	if (m_FastDeal == true)
	{
		int tempInt = _ttoi(valueStr);
		tempInt = tempInt + FASTDEAL;
		valueStr = Int2CString(tempInt);
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Buy"),Int2CString(1),inputData.at(29),_T("追价"));
	}
	else
	{
		SetSentOrderStauts_Sim(valueStr,_T("Close"),_T("Buy"),Int2CString(1),inputData.at(29));
	}
	
}

bool CStrategy::IsOrdSentNow_Sim( void )
{
	int OrderNum = 0;
	char* Msg = "";//消息变量
	//vspdctomysql = new VspdcToMySQL;
	//VspdcToMySQL *vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			//printf(Msg);
		}
		else
		{
			return true;// 这里返回True 比较安全
		}
	}

	OrderNum = m_vspdctomysql_Strategy->GetTableNum(_T("OrderNow_sim"));

	if (OrderNum == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CStrategy::hasHoldedNow_Sim( void )
{
	int HoldNum = GetTableNum(_T("Hold_sim"));
	if (HoldNum == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

int CStrategy::HolderType_Sim( void )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return -1;
		}
	}
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
	// 	if(m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
	// 	{
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("SELECT * FROM Hold_sim LIMIT 1;");
	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,1,Msg);


	if( DataList.size() == 1 )
	{
		//for (int i = 0; i<DataList.size(); i++)
		//{
		vector<CString> tempVec = SplitString(DataList.at(0));
		if (tempVec.at(3) == _T("Buy"))
		{
			return BUYORDER;
		}
		else if (tempVec.at(3) == _T("Sell"))
		{
			return SELLORDER;
		}
		else
		{
			return -1;
		}
	}
	return  -1;
}

void CStrategy::Transform_Delete_Order_Sim()
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量

	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return;
		}
	}
	printf("连接成功/r/n");
	
	CString SQL = _T("select * from OrderNow_sim;");
	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);

	if( DataList.size() == 1 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		vector<CString> tempVec;
		for (int i = 0; i<DataList.size(); i++)
		{
			tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间
		// 
		//ResList.at(0).at(0);	// ID 1,code; 2,Price; 3,OpenClose; 4,BuySell; 5,Volume
		// 6,Time,	7,Date, 8 Mark
		CString Open_Close = tempVec.at(3);
		//
		if (Open_Close == _T("Open"))
		{
			CString SQL_Head = _T("insert into Hold_sim (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
			CString SQL_Value = _T("'") +
				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
				_T("'") + ResList.at(0).at(6) + _T("',") +
				_T("'") + ResList.at(0).at(7) + _T("'");
			CString SQL_Tail = _T(")");
			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
			char* SQL_char = this->CString2CharPt(SQL);
			if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
				printf("插入成功/r/n");
		}
		else if(Open_Close == _T("Close"))
		{
			m_vspdctomysql_Strategy->CleanTable(_T("Hold"));
			printf("清空/r/n");
		}
	}
}

void CStrategy::Transform_Delete_Order_Sim( CString nowValue )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	//m_vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return;
		}
	}
	printf("连接成功/r/n");
	//char * code = CString2CharPt(m_Code);
	CString SQL = _T("select * from OrderNow_sim;");
	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//CString getStr(str.c_str());
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() == 1 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		
		vector<CString> tempVec;
		for (int i = 0; i<DataList.size(); i++)
		{
			tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		CString Open_Close = tempVec.at(3);
		//
		if (Open_Close == _T("Open"))
		{
			CString SQL_Head = _T("insert into Hold_sim (CODE,PRICE,BuySell,VOLUME,TIME,DATE) values(");
			CString SQL_Value = _T("'") +
				m_Code + _T("',") + ResList.at(0).at(2) + _T(",") +  
				_T("'") + ResList.at(0).at(4) + _T("',") + ResList.at(0).at(5) + _T(",") +
				_T("'") + ResList.at(0).at(6) + _T("',") +
				_T("'") + ResList.at(0).at(7) + _T("'");
			CString SQL_Tail = _T(")");
			CString SQL = SQL_Head + SQL_Value + SQL_Tail;
			char* SQL_char = this->CString2CharPt(SQL);
			if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
				printf("插入成功/r/n");

			//WriteLogDatabase(nowValue,_T("Open"),_T("Buy"),Int2CString(1));
		}
		else if(Open_Close == _T("Close"))
		{
			m_vspdctomysql_Strategy->CleanTable(_T("Hold_sim"));
			//WriteLogDatabase(nowValue,_T("Close"),_T("Sell"),Int2CString(1));
			printf("清空/r/n");
		}
	}
}

void CStrategy::CleanOrderNowTable_Sim()
{
	//delete from TableName 清空列表
	char* Msg = "";//消息变量
	int row = 0;
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
	}
	m_vspdctomysql_Strategy->CleanTable(_T("OrderNow_sim"));
}

void CStrategy::WriteLogDatabase_Sim( CString price,CString openclose, CString buysell, CString volume, CString mark )
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);
	u_Res = mark;

	CString SQL_Head = _T("insert into TradeRecord_sim (CODE,PRICE,OpenClose,BuySell,VOLUME,TIME,DATE) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + timeNow + _T("',") +
		_T("'") + u_Res + _T("'");
	CString SQL_Tail = _T(")");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;
	char* SQL_char = this->CString2CharPt(SQL);
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
	}
	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
		printf("插入成功/r/n");
}

void CStrategy::WriteLogDatabase_Sim( vector<CString> inputData,CString openclose, CString buysell, CString volume, CString mark )
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);
	u_Res = inputData.at(29);

	CString SQL_Head = _T("insert into TradeRecord_sim (CODE,PRICE,OpenClose,BuySell,VOLUME,TIME,DATE,MARK) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + inputData.at(7) + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + u_Res + _T("',") +
		_T("'") + u_Res + _T("',") + 
		_T("'") + mark + _T("'");
	CString SQL_Tail = _T(");");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;
	char* SQL_char = this->CString2CharPt(SQL);
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
	}
	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
		printf("插入成功/r/n");
}

void CStrategy::WriteLogDatabase_Sim( CString price,CString openclose, CString buysell, CString volume, CString mark, CString timer )
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	CString u_Year,u_Month,u_Day,u_Res;
	u_Year.Format(_T("%d"),st.wYear);u_Month.Format(_T("%d"),st.wMonth);u_Day.Format(_T("%d"),st.wDay);
	u_Res = u_Year+_T("-")+u_Month+_T("-")+u_Day;

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) ); 
	CString timeNow(tmp);
	u_Res = timer;

	CString SQL_Head = _T("insert into TradeRecord_sim (CODE,PRICE,OpenClose,BuySell,VOLUME,TIME,DATE,MARK) values(");//HSDate,
	CString SQL_Value = _T("'") +
		m_Code + _T("',") + price + _T(",'") + openclose +_T("',") + 
		_T("'") + buysell + _T("',") + volume + _T(",") +
		_T("'") + u_Res + _T("',") +
		_T("'") + u_Res + _T("',") + 
		_T("'") + mark + _T("'");
	CString SQL_Tail = _T(");");
	CString SQL = SQL_Head + SQL_Value + SQL_Tail;
	char* SQL_char = this->CString2CharPt(SQL);
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			printf("连接成功/r/n");
			printf(Msg);
		}
	}
	if(m_vspdctomysql_Strategy->InsertData(SQL_char,Msg) == 0)
		printf("插入成功/r/n");
}

bool CStrategy::CheckOrderSuccessfull_Sim( CString nowValue )
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return false;
		}
	}
	CString SQL = _T("select * from OrderNow_sim;");
	
	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() > 0 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		for (int i = 0; i<DataList.size(); i++)
		{
			vector<CString> tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间

	}
	CString sendValue = ResList.at(0).at(2);
	float nowV = _ttoi(nowValue);
	float sendV = _ttoi(sendValue);
	CString BuyOrSell = ResList.at(0).at(4);
	if (m_FastDeal == true)
	{
		return true;
	}
	if (BuyOrSell == _T("Buy"))
	{
		if(nowV <= sendV-2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else//检查sell单成交与否
	{
		if (nowV >= sendV+2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

OPENCLOSE CStrategy::GetCurrentOrderType_Sim()
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return UNKNOWORDER;
		}
	}
	CString SQL = _T("select * from OrderNow_sim;");

	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() > 0 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		for (int i = 0; i<DataList.size(); i++)
		{
			vector<CString> tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间

	}
//	CString sendValue = ResList.at(0).at(2);
// 	float nowV = _ttoi(nowValue);
// 	float sendV = _ttoi(sendValue);
	CString OpenOrClose = ResList.at(0).at(3);
	if (OpenOrClose == _T("Open"))
	{
		return OPEN;
	}
	else//检查sell单成交与否
	{
		return CLOSE;
	}
	return UNKNOWORDER;
}

BUYSELL CStrategy::GetCurrentOrder_Sim()
{
	CString res("");
	vector<vector<CString>> ResList;
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
		else
		{
			return UNKNOW;
		}
	}
	CString SQL = _T("select * from OrderNow_sim;");

	int row = GetTableNum(_T("OrderNow_sim"));
	vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),9,row,Msg);
	//AUTOTRADER::Int2CString(Num)
	if( DataList.size() > 0 )
	{
		printf("查询成功/r/n");
		//printf(str.data());
		printf("/r/n");
		printf(Msg);
		//HSTimer sTime(testStr);
		//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
		for (int i = 0; i<DataList.size(); i++)
		{
			vector<CString> tempVec = SplitString(DataList.at(i));
			//CString testStr = tempVec.at(30);
			ResList.push_back(tempVec);
		}
		// 第三十位 是HS时间

	}
	//	CString sendValue = ResList.at(0).at(2);
	// 	float nowV = _ttoi(nowValue);
	// 	float sendV = _ttoi(sendValue);
	CString OpenOrClose = ResList.at(0).at(3);
	if (OpenOrClose == _T("Buy"))
	{
		return BUY;
	}
	else//检查sell单成交与否
	{
		return SELL;
	}
	return UNKNOW;
}

void CStrategy::CleanOrderHold_Sim()
{
	//delete from TableName 清空列表
	char* Msg = "";//消息变量
	int row = 0;
	//vspdctomysql = new VspdcToMySQL;
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			printf("连接成功/r/n");
			printf(Msg);
		}
	}
	m_vspdctomysql_Strategy->CleanTable(_T("Hold_sim"));
}

vector<CString> CStrategy::GetDataByID( int ID )
{
	CString res("");
	vector<CString> resVec;
	vector< vector<CString> > ResList;
	char* Msg = "";//消息变量
	if (m_vspdctomysql_Strategy->mysql.net.buff == NULL)
	{
		if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
		{
			//this->WriteError(_T("写入数据是数据库重连成功！"));
			//if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
			//printf(Msg);
		}

	}
	
	//m_vspdctomysql->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0
// 	if(m_vspdctomysql_Strategy->ConnMySQL(HOST,PORT,DBNAME,DatabaseUSER,PASSWD,CHARSET,Msg) == 0)
// 	{
		//printf("连接成功/r/n");
		//char * code = CString2CharPt(m_Code);
		CString SQL = _T("select * from ")+m_Code+_T(" where ID = "+Int2CString(ID) + ";");
		//CString SQL = _T("select * from c1409_View order by  MySqlTimeStamp desc limit 0,"+Int2CString(Num)+";");
		//CString SQL = _T("SELECT HSTimeStamp from c1409 ORDER BY ID desc LIMIT 0,1");
		//string str = m_vspdctomysql->SelectData(CString2CharPt(SQL),1,1,Msg);
		vector<CString> DataList = m_vspdctomysql_Strategy->GetDataVec(CString2CharPt(SQL),34,1,Msg);
		//CString getStr(str.c_str());
		//AUTOTRADER::Int2CString(Num)
		if( DataList.size() > 0 )
		{
			//printf(_T("查询成功/r/n"));
			//printf(str.data());
			//printf(_T("/r/n"));
			//printf(Msg);
			//HSTimer sTime(testStr);
			//HSTimer sTime(SplitString(DataList.at(DataList.size()-1)).at(30));
			for (int i = 0; i<DataList.size(); i++)
			{
				vector<CString> tempVec = SplitString(DataList.at(i));
				//CString testStr = tempVec.at(30);
				ResList.push_back(tempVec);
			}
			// 第三十位 是HS时间

		}
	//}
	//m_vspdctomysql->CloseMySQLConn();
	if (ResList.size() == 1)
	{
		return ResList.at(0);
	}
	return resVec;
}

void CStrategy::SetTrader( CTrader *trader )
{
	m_Trader = trader;
}

bool CStrategy::CheckOrderSuccessfullbySnap( CString nowValue )
{
	m_Trader->ListSnapshot(m_Trader->m_bmp2);
	Sleep(1000);
	bool res = CompareBmp(m_Trader->m_bmp1, m_Trader->m_bmp2);
	if (res == false)
	{
		CopyFile(m_Trader->m_bmp2, m_Trader->m_bmp1, false);
	}
	return res;
}

bool CStrategy::CompareBmp( CString bmp1, CString bmp2 )
{
 	char* imagefile1 = CString2CharPt(bmp1);
 	char* imagefile2 = CString2CharPt(bmp2);
 
 	int HistogramBins = 256;  
 	float HistogramRange1[2]={0,255};  
 	float *HistogramRange[1]={&HistogramRange1[0]};
 
 
 	// 
 	IplImage *image1=cvLoadImage(imagefile1, 0);  
 	IplImage *image2=cvLoadImage(imagefile2, 0);  
 
 	CvHistogram *Histogram1 = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY,HistogramRange);  
 	CvHistogram *Histogram2 = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY,HistogramRange);  
 
 	cvCalcHist(&image1, Histogram1);  
 	cvCalcHist(&image2, Histogram2);  
 
 	cvNormalizeHist(Histogram1, 1);  
 	cvNormalizeHist(Histogram2, 1);  
 
 	// CV_COMP_CHISQR,CV_COMP_BHATTACHARYYA这两种都可以用来做直方图的比较，值越小，说明图形越相似  
 	printf("CV_COMP_CHISQR : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_CHISQR)); // 卡方 
 	float test1 = cvCompareHist(Histogram1,Histogram2,CV_COMP_CHISQR);
 	printf("CV_COMP_BHATTACHARYYA : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_BHATTACHARYYA));//常态分布  
 	float test2 = cvCompareHist(Histogram1, Histogram2, CV_COMP_BHATTACHARYYA);
 
 	// CV_COMP_CORREL, CV_COMP_INTERSECT这两种直方图的比较，值越大，说明图形越相似  
 	printf("CV_COMP_CORREL : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL));//相关系数
 	float test3 = cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL);
 	printf("CV_COMP_INTERSECT : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_INTERSECT));//交集法  
 	float test4 = cvCompareHist(Histogram1, Histogram2, CV_COMP_INTERSECT);
 
 	cvReleaseImage(&image1);  
 	cvReleaseImage(&image2);  
 	cvReleaseHist(&Histogram1);  
 	cvReleaseHist(&Histogram2);  

	if (abs(test1 - 0) <= 0.0001 && abs(test2-0) <= 0.001 && abs(test3 - 1) <= 0.01 && abs(test4 - 1) <= 0.01)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}


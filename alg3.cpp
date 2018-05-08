#include <iostream>
#include <ctime>
#include <math.h>
#include <windows.h> 
#include "alg.h"
#include "read_chains.cpp" 
#include "print.cpp"
//#include "print.cpp" 

bool checkTraffic(int path[], int update_path[], int demand) {
	bool flag = true;
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] += demand;
		BW[path[step-1]-1][path[step]-1] += demand;
	}
	for(int step = 1; update_path[step] > 0; ++step) {
		if(BW[update_path[step]-1][update_path[step-1]-1] < demand) {
			cout<<update_path[step]<<" "<<update_path[step-1]<<" "<< BW[update_path[step]-1][update_path[step-1]-1];
			flag = false;
		}
	}
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] -= demand;
		BW[path[step-1]-1][path[step]-1] -= demand;
	}
	return flag;
}

void chooseIns(int i, bool *nf_done, struct CFC Chains[]) {
	int type = Chains[i].service_type;    // 五种服务链中的哪一种	
//	int rd = (Chains[i].ins + rand()%(num_of_ins[type]-1) + 1) % num_of_ins[type];

	Chains[i].update_ins = rand()%num_of_ins[type];    // 选择一种实现方式(考虑服务链分配失败) 
//	cout<<"第"<<i+1<<"条服务链："<<type<<" "<<Chains[i].ins<<" "<<Chains[i].update_ins<<endl; 

	int phy = 0;    // 第 n 个物理特征
	for(int f : phy_feature_set) {

		if(chain_types[type][Chains[i].update_ins][f] == 1) {
			*nf_done = false;
//			cout<<"nf_done："<<*nf_done<<endl; 
//			cout<<"物理特征："<<chain_types[type][Chains[i].ins][f]<<endl;
			
			Chains[i].update_node = service_nodes[phy][rand()%4];    // NF 节点序号 
			Chains[i].update_phy = phy;
//			cout<<"新节点："<<Chains[i].update_node<<endl;
			if(RS[Chains[i].update_node-37] <= 0) {
//				cout<<"新节点用不了"<<endl;
				Chains[i].update_node = Chains[i].node;
				Chains[i].update_ins = Chains[i].ins;
				Chains[i].update_phy = Chains[i].phy;
				return;
			}
			
			if(Chains[i].ins != Chains[i].update_ins) {
//				cout<<"新旧两种实现方式不同"<<endl;
				if(Chains[i].node > 0) {
//					cout<<"采用旧节点："<<Chains[i].node<<endl;
					Chains[i].update_node = Chains[i].node;
				}
//				cout<<"return"<<endl;
			}

//			cout<<Chains[i].update_node<<endl;
			
			
			return;
		}
	}
 	phy++;
}

void choosePath(int i, bool *nf_done, struct CFC Chains[]) {
//	cout<<*nf_done<<" "<<Chains[i].update_node<<endl; 
//	cout<<Chains[i].update_node<<endl;
	if(Chains[i].ins != Chains[i].update_ins && Chains[i].phy >= 0 && Chains[i].update_phy >= 0) {
		Chains[i].update_phy = Chains[i].phy;
		memcpy(Chains[i].update_path, Chains[i].path, 4*MAX_PATH_LENGTH);
		return;
	} 
//	cout<<*nf_done<<endl; 
	int step = 0; 
	int src_sw = (Chains[i].src-1)/3 + 28;
	int sink_sw = (Chains[i].sink-1)/3 + 28;
//	cout<<src_sw<<" "<<sink_sw<<endl;
	Chains[i].update_path[step++] = Chains[i].src;
//	cout<<Chains[i].update_path[step-1]<<" ";
	Chains[i].update_path[step++] = src_sw;
//	cout<<Chains[i].update_path[step-1]<<" ";

	if(sw_edge[src_sw - 28] == Chains[i].update_node) {    // 若选择了本网的边缘云 
		Chains[i].update_path[step++] = Chains[i].update_node;
//		cout<<Chains[i].update_path[step-1]<<" ";
		Chains[i].update_path[step++] = src_sw;
//		cout<<Chains[i].update_path[step-1]<<" ";
		*nf_done = true; 
	}
	
	int target_sw;    // 确定将要通向的交换机
	int t_RT = 43;
	if(*nf_done == false) {    // 若仍需经过云端, 确定对面的交换机和路由器 	
		target_sw = cloud_sw[Chains[i].update_node - 37];
		if(target_sw != -1) {    // node 是边缘云 
			t_RT = sw_RT[target_sw - 28];
		} 
	}
	else {    // 无 NF、已经过 NF 
		target_sw = sink_sw;
		t_RT = sw_RT[target_sw - 28];
	}

	// 无 NF、已经过 NF、未经过 NF 都需要这一步
	int s_RT = sw_RT[src_sw - 28]; 
	Chains[i].update_path[step++] = s_RT;
//	cout<<Chains[i].update_path[step-1]<<" ";
//	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%3], 4*4);
//	cout<<"Router"<<" ";
	for(; Chains[i].update_path[step] > 0; ++step) {}
	
	if(target_sw == sink_sw && *nf_done == true) {  // 无 NF、已经过 NF 的路径添加完毕 
		Chains[i].update_path[step++] = sink_sw;
//		cout<<Chains[i].update_path[step-1]<<" ";
		Chains[i].update_path[step] = Chains[i].sink;
//		cout<<Chains[i].update_path[step]<<" ";
	}
	
	else {
		// 添加 NF 这一段的路径 
		if(target_sw > 0) {
			Chains[i].update_path[step++] = target_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
		}
		Chains[i].update_path[step++] = Chains[i].update_node;
//		cout<<Chains[i].update_path[step-1]<<" ";
		*nf_done = true;
		if(target_sw > 0) {
			Chains[i].update_path[step++] = target_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
		}
		
		if(target_sw == sink_sw) {    // NF 在汇聚节点侧的路径添加完毕 

//			cout<<Chains[i].update_path[step-1]<<" ";
			Chains[i].update_path[step] = Chains[i].sink;
//			cout<<Chains[i].update_path[step]<<" ";
		}
		
		else{
			s_RT = t_RT;
			t_RT = sw_RT[sink_sw - 28];
			Chains[i].update_path[step++] = s_RT;
//			cout<<Chains[i].update_path[step-1]<<" ";
//	        memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
//			memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
			memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%3], 4*4);
//			cout<<"Router"<<" ";
			for(; Chains[i].update_path[step] > 0; ++step) {}
			Chains[i].update_path[step++] = sink_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
			Chains[i].update_path[step] = Chains[i].sink;
//			cout<<Chains[i].update_path[step]<<" ";
		}
		
	} 
	
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
	if(!checkTraffic(Chains[i].path, Chains[i].update_path, Chains[i].demand)) {
		memcpy(Chains[i].update_path, Chains[i].path, MAX_PATH_LENGTH*4);
		Chains[i].update_node = Chains[i].node;
		Chains[i].update_ins = Chains[i].ins;
		Chains[i].update_phy = Chains[i].phy;
//		cout<<"不够分配"<<endl;
	}
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
}

void init() {

	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {	
		
		bool nf_done = true;    // NF 是否已经过(或根本没有 NF) 
		
		chooseIns(i, &nf_done, Input_Chains);   // 失败后恢复 
		
		// 挑选路径
		choosePath(i, &nf_done, Input_Chains);   // 失败后恢复 

		Input_Chains[i].cost = cost(i, Input_Chains);
//		cout<<"COST: "<<Input_Chains[i].cost<<endl;

		// 扣除资源和带宽 
		updateTraffic(Input_Chains[i].path, Input_Chains[i].update_path, Input_Chains[i].demand);
		if(Input_Chains[i].node > 0) {
			RS[Input_Chains[i].node - 37] += Input_Chains[i].demand;
		}
		if(Input_Chains[i].update_node > 0) {
			RS[Input_Chains[i].update_node - 37] -= Input_Chains[i].demand;
		}
		memcpy(Input_Chains[i].path, Input_Chains[i].update_path, 4*MAX_PATH_LENGTH);
		Input_Chains[i].ins = Input_Chains[i].update_ins;
		Input_Chains[i].phy = Input_Chains[i].update_phy;
		Input_Chains[i].node = Input_Chains[i].update_node;
	}
} 

void classify() {
	srand((unsigned)time(NULL));
	double sumC = 0.0;
	int lowC = 1000, highC = 0;
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
		lowC = lowC >= Allocated_Chains[i].cost? Allocated_Chains[i].cost: lowC;
		highC = highC <= Allocated_Chains[i].cost? Allocated_Chains[i].cost: highC;
		sumC += Allocated_Chains[i].cost;
	}
	int count = 0;
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
//		double Pc = pow(Allocated_Chains[i].cost/sumC, 1.0/(double)NUM_OF_ALLOCATED_CHAINS);
//		double Pc = Allocated_Chains[i].cost/sumC;

		double Pc = Allocated_Chains[i].cost;

		int pr = rand()%(highC - lowC) + lowC;

		if(pr - Pc < 0) {
			realc[count++] = i;
		}
//		realc[count++] = i;  // 全加进来试试 

	}
	num_of_realc = count;
//	cout<<"选择已分配的服务链："<<endl;
//	for(int i = 0; realc[i] >= 0; ++i) {
//		cout<<realc[i]<<" ";
//	}
}

void update(int i, struct CFC Chains[]) {
	updateTraffic(Chains[i].path, Chains[i].update_path, Chains[i].demand);
	if(Chains[i].node > 0) {
		RS[Chains[i].node - 37] += Chains[i].demand;
	}
	if(Chains[i].node > 0) {
		RS[Chains[i].node - 37] -= Chains[i].demand;
	}
//	cout<<"更新："<<to_be_updated<<endl;
	Chains[i].node = Chains[i].update_node;
	memcpy(Chains[i].path, Chains[i].update_path, 4*MAX_PATH_LENGTH);
//	cout<<"cost变化："<<Chains[i].cost<<" "<<Chains[i].update_cost<<endl;
	Chains[i].cost = Chains[i].update_cost;
}

void action() { 
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		bool nf_done = true;
//		cout<<"选取节点..."<<endl;
		chooseIns(i, &nf_done, Input_Chains);
//		cout<<"done！"<<endl;
//		cout<<"清空路径..."<<endl;
		memset(Input_Chains[i].update_path, 0, MAX_PATH_LENGTH*4);
//		cout<<"done！"<<endl; 
//		cout<<"选取路径..."<<endl;
		choosePath(i, &nf_done, Input_Chains);
//		cout<<"done！"<<endl;
		
		Input_Chains[i].update_cost = cost(i, Input_Chains);
	}
	
	for(int c: realc) {
		if(c <= 0) {
			break;
		}
		bool nf_done = true;
//		cout<<"选取节点..."<<endl;
		chooseIns(c, &nf_done, Allocated_Chains);
//		cout<<"done！"<<endl;
//		cout<<"清空路径..."<<endl;
		memset(Allocated_Chains[c].update_path, 0, MAX_PATH_LENGTH*4);
//		cout<<"done！"<<endl; 
//		cout<<"选取路径..."<<endl;
		choosePath(c, &nf_done, Allocated_Chains);
//		cout<<"done！"<<endl;
		
		Allocated_Chains[c].update_cost = cost(c, Allocated_Chains);
	}
	
	double max_perform = 0.0;
	int to_be_updated = -1, i = 0;
	for(; i < NUM_OF_INPUT_CHAINS; ++i) {
		if(max_perform < Input_Chains[i].cost - Input_Chains[i].update_cost) {
//			cout<<"I - "<<i<<"的cost变少了："<<" "<<Input_Chains[i].cost<<" "<<Input_Chains[i].update_cost<<endl;
			max_perform = Input_Chains[i].cost - Input_Chains[i].update_cost;
			to_be_updated = i;
		}
	}
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		if(max_perform < Allocated_Chains[c].cost - Allocated_Chains[c].update_cost) {
//			cout<<"A - "<<c<<"的cost变少了："<<" "<<Allocated_Chains[c].cost<<" "<<Allocated_Chains[c].update_cost<<endl;
			max_perform = Allocated_Chains[c].cost - Allocated_Chains[c].update_cost;
			to_be_updated = c + NUM_OF_INPUT_CHAINS;
		}
	}
	if(to_be_updated >= NUM_OF_INPUT_CHAINS) {
		cout<<"更新 A  "<<to_be_updated - NUM_OF_INPUT_CHAINS<<endl;
//		cout<<Allocated_Chains[to_be_updated - NUM_OF_INPUT_CHAINS].node<<" "<<Allocated_Chains[to_be_updated - NUM_OF_INPUT_CHAINS].update_node<<endl;
		update(to_be_updated - NUM_OF_INPUT_CHAINS, Allocated_Chains);
//		cout<<"done！"<<endl;
	}
	else if(to_be_updated >= 0){
		cout<<"更新 I  "<<to_be_updated<<endl;
//		cout<<Input_Chains[to_be_updated].node<<" "<<Input_Chains[to_be_updated].update_node<<endl;
		update(to_be_updated, Input_Chains);
//		cout<<"done！"<<endl;
	}
	else {
		cout<<U<<endl;
		return;
	}
	U = 0.0;
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		U += Input_Chains[i].cost;
	}
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
		U += Allocated_Chains[i].cost;
	}
	cout<<U<<endl;
}

int main() {
	srand((unsigned)time(NULL));    // 随机种子调用一次即可
	
	read();

	DWORD start, stop;  
    unsigned int t = 0;  
    start = GetTickCount(); 
    
	// 将所有新服务链的先选一种初始配置 
	init();
	
//	printChoice();

	// 选择参与此次调整的已分配服务链 
//	classify();
	
	// 策略更新 
	for(int times = 0; times < 500; ++times) {
//		cout<<"第"<<times<<"次："<<endl;
		action();
//		cout<<endl;
	}
	
	stop = GetTickCount();  
    printf("\n运行时间: %lld ms\n", stop - start);
    
	printChoice();
//	printBW();
//	printRS();

}

#include <iostream>
#include <ctime>
#include <math.h>
#include <windows.h> 
#include "NewAlg.h"
#include "input.cpp" 


void choosePath(int i, bool *nf_done, struct CFC Chains[]) {
	int step = 0; 
	int src_sw = (Chains[i].src-1)/3 + 28;
	int sink_sw = (Chains[i].sink-1)/3 + 28;
//	cout<<src_sw<<" "<<sink_sw<<endl;
	Chains[i].update_path[step++] = src_sw;

	if(sw_edge[src_sw - 28] == Chains[i].update_node) {    // 若选择了本网的边缘云 
		Chains[i].update_path[step++] = Chains[i].update_node;
		Chains[i].update_path[step++] = src_sw;
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
//	memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
	for(; Chains[i].update_path[step] > 0; ++step) {}
	
	if(target_sw == sink_sw && *nf_done == true) {  // 无 NF、已经过 NF 的路径添加完毕 
		Chains[i].update_path[step] = sink_sw;
		return;
	}
	
	// 添加 NF 这一段的路径 
	if(target_sw > 0) {
		Chains[i].update_path[step++] = target_sw;
	}
	Chains[i].update_path[step++] = Chains[i].update_node;
	*nf_done = true;
	if(target_sw > 0) {
		Chains[i].update_path[step++] = target_sw;
	}
	
	if(target_sw == sink_sw) {    // NF 在汇聚节点侧的路径添加完毕 
		Chains[i].update_path[step] = sink_sw;
		return;
	}
	
	s_RT = t_RT;
	t_RT = sw_RT[sink_sw - 28];
	Chains[i].update_path[step++] = s_RT;
//	memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
	for(; Chains[i].update_path[step] > 0; ++step) {}
	Chains[i].update_path[step] = sink_sw;
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < 15; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
}

void init(int n) {
	srand((unsigned)time(NULL));    // 随机种子调用一次即可 
	
	for(int i = 0; i < n; ++i) {	
		
		bool nf_done = true;    // NF 是否已经过(或根本没有 NF) 
		
		int type = Input_Chains[i].service_type;    // 五种服务链中的哪一种		
		Input_Chains[i].ins = rand()%(num_of_ins[type]);    // 选择一种实现方式(考虑服务链分配失败) 
//		cout<<"第"<<i+1<<"条服务链："<<type<<" "<<Input_Chains[i].ins<<endl; 

		int phy = 0;    // 第 n 个物理特征
		for(int f : phy_feature_set) {
//			cout<<chain_types[type][Input_Chains[i].ins][f]<<endl;
			if(chain_types[type][Input_Chains[i].ins][f] == 1) {
				Input_Chains[i].node = service_nodes[phy][rand()%4];    // NF 节点序号 
//				cout<<Input_Chains[i].node<<endl;
				Input_Chains[i].phy = phy;
				nf_done = false;
				break;
			}
			++phy;
		}
		
		Input_Chains[i].update_node = Input_Chains[i].node;
		
		// 挑选路径
		choosePath(i, &nf_done, Input_Chains);
		memcpy(Input_Chains[i].path, Input_Chains[i].update_path, 16*4);
		
		Input_Chains[i].cost = cost(i, Input_Chains);
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
	}
	num_of_realc = count;
//	for(int i = 0; realc[i] >= 0; ++i) {
//		cout<<realc[i]<<" ";
//	}
}

void update(int i, struct CFC Chains[]) {
//	cout<<"更新："<<to_be_updated<<endl;
	Chains[i].node = Chains[i].update_node;
	memcpy(Chains[i].path, Chains[i].update_path, 4*16);
	Chains[i].cost = Chains[i].update_cost;
}

void action() { 
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		bool nf_done = true;
		if(Input_Chains[i].node != 0) {    // 具有 NF 
			Input_Chains[i].update_node = service_nodes[Input_Chains[i].phy][rand()%4];
			nf_done = false;
		}

		memset(Input_Chains[i].update_path, 0, 16*4);
		choosePath(i, &nf_done, Input_Chains);
		
		Input_Chains[i].update_cost = cost(i, Input_Chains);
	}
	
	for(int c: realc) {
		if(c <= 0) {
			break;
		}
		bool nf_done = true;
		if(Allocated_Chains[c].node != 0) {    // 具有 NF 
			Allocated_Chains[c].update_node = service_nodes[Allocated_Chains[c].phy][rand()%4];
			nf_done = false;
		}

		memset(Allocated_Chains[c].update_path, 0, 16*4);
		choosePath(c, &nf_done, Allocated_Chains);
		
		Allocated_Chains[c].update_cost = cost(c, Allocated_Chains);
	}
	
	double max_cost = 0.0;
	int to_be_updated = -1, i = 0;
	for(; i < NUM_OF_INPUT_CHAINS; ++i) {
		if(max_cost <= Input_Chains[i].cost - Input_Chains[i].update_cost) {
			max_cost = Input_Chains[i].cost - Input_Chains[i].update_cost;
			to_be_updated = i;
		}
	}
	for(int c: realc) {
		if(max_cost <= Allocated_Chains[c].cost - Allocated_Chains[c].update_cost) {
			max_cost = Allocated_Chains[c].cost - Allocated_Chains[c].update_cost;
			to_be_updated = i;
		}
	}
	if(to_be_updated >= NUM_OF_INPUT_CHAINS) {
		update(to_be_updated - NUM_OF_INPUT_CHAINS, Allocated_Chains);
	}
	else if(to_be_updated >= 0){
		update(to_be_updated, Input_Chains);
	}
	double U = 0.0;
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		U += Input_Chains[i].cost;
	}
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
		U += Allocated_Chains[i].cost;
	}
	cout<<U<<endl;
}

void printChoice() {
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		cout<<"第"<<i+1<<"条服务链："<<Input_Chains[i].service_type<<" "<<Input_Chains[i].ins<<endl;
		cout<<"NF："<<Input_Chains[i].node<<endl;
		for(int step = 0; step < 15; ++step) {
			cout<<Input_Chains[i].path[step]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	for(int c: realc) {
		if(c <= 0) {
			break;
		}
		cout<<"第"<<c+1<<"条服务链："<<Allocated_Chains[c].service_type<<" "<<Allocated_Chains[c].ins<<endl;
		cout<<"NF："<<Allocated_Chains[c].node<<endl;
		for(int step = 0; step < 15; ++step) {
			cout<<Allocated_Chains[c].path[step]<<" ";
		}
		cout<<endl;
	}
}

int main() {
	
	input();

	DWORD start, stop;  
    unsigned int t = 0;  
    start = GetTickCount(); 
    
	// 将所有新服务链的先选一种初始配置 
	init(NUM_OF_INPUT_CHAINS);

	// 选择参与此次调整的服务链 
	classify();
	
	// 策略更新 
	for(int times = 0; times < 55; ++times) {
		action();
	}
	
//	printChoice();
	
	stop = GetTickCount();  
    printf("\n运行时间: %lld ms\n", stop - start);
    
}

#include <iostream>
#include <ctime>
#include <math.h>
#include <windows.h> 
#include "alg.h"
#include "read_chains.cpp" 
//#include "print.cpp" 

bool checkTraffic(int path[], int demand) {
	for(int step = 1; path[step] > 0; ++step) {
		if(BW[path[step]-1][path[step-1]-1] < demand) {
//			cout<<path[step]<<" "<<path[step-1]<<" "<< BW[path[step]-1][path[step-1]-1];
			return false;
		}
	}
	return true;
}

void addTraffic(int path[], int demand) {
	for(int step = 1; step < MAX_PATH_LENGTH; ++step) {
		BW[path[step]-1][path[step-1]-1] -= demand;
	}
}

void choosePath(int i, bool *nf_done, struct CFC Chains[]) {
	int step = 0; 
	int src_sw = (Chains[i].src-1)/3 + 28;
	int sink_sw = (Chains[i].sink-1)/3 + 28;
//	cout<<src_sw<<" "<<sink_sw<<endl;
	Chains[i].update_path[step++] = Chains[i].src;
	Chains[i].update_path[step++] = src_sw;

	if(sw_edge[src_sw - 28] == Chains[i].update_node) {    // ��ѡ���˱����ı�Ե�� 
		Chains[i].update_path[step++] = Chains[i].update_node;
		Chains[i].update_path[step++] = src_sw;
		*nf_done = true; 
	}
	
	int target_sw;    // ȷ����Ҫͨ��Ľ�����
	int t_RT = 43;
	if(*nf_done == false) {    // �����辭���ƶ�, ȷ������Ľ�������·���� 	
		target_sw = cloud_sw[Chains[i].update_node - 37];
		if(target_sw != -1) {    // node �Ǳ�Ե�� 
			t_RT = sw_RT[target_sw - 28];
		} 
	}
	else {    // �� NF���Ѿ��� NF 
		target_sw = sink_sw;
		t_RT = sw_RT[target_sw - 28];
	}

	// �� NF���Ѿ��� NF��δ���� NF ����Ҫ��һ��
	int s_RT = sw_RT[src_sw - 28]; 
	Chains[i].update_path[step++] = s_RT;
//	memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
	for(; Chains[i].update_path[step] > 0; ++step) {}
	
	if(target_sw == sink_sw && *nf_done == true) {  // �� NF���Ѿ��� NF ��·�������� 
		Chains[i].update_path[step++] = sink_sw;
		Chains[i].update_path[step] = Chains[i].sink;
		return;
	}
	
	// ��� NF ��һ�ε�·�� 
	if(target_sw > 0) {
		Chains[i].update_path[step++] = target_sw;
	}
	Chains[i].update_path[step++] = Chains[i].update_node;
	*nf_done = true;
	if(target_sw > 0) {
		Chains[i].update_path[step++] = target_sw;
	}
	
	if(target_sw == sink_sw) {    // NF �ڻ�۽ڵ���·�������� 
		Chains[i].update_path[step++] = sink_sw;
		Chains[i].update_path[step] = Chains[i].sink;
		return;
	}
	
	s_RT = t_RT;
	t_RT = sw_RT[sink_sw - 28];
	Chains[i].update_path[step++] = s_RT;
//	memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
	memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
	for(; Chains[i].update_path[step] > 0; ++step) {}
	Chains[i].update_path[step++] = sink_sw;
	Chains[i].update_path[step] = Chains[i].sink;
	
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
	if(!checkTraffic(Chains[i].update_path, Chains[i].demand)) {
		memcpy(Chains[i].update_path, Chains[i].path, MAX_PATH_LENGTH*4);
		Chains[i].update_node = Chains[i].node;
//		cout<<"��������"<<endl;
	}
	else {
		addTraffic(Chains[i].update_path, Chains[i].demand);
	}
}

void init(int n) {
	srand((unsigned)time(NULL));    // ������ӵ���һ�μ��� 
	
	for(int i = 0; i < n; ++i) {	
		
		bool nf_done = true;    // NF �Ƿ��Ѿ���(�����û�� NF) 
		
		int type = Input_Chains[i].service_type;    // ���ַ������е���һ��		
		Input_Chains[i].ins = rand()%(num_of_ins[type]-1)+1;    // ѡ��һ��ʵ�ַ�ʽ(���Ƿ���������ʧ��) 
//		cout<<"��"<<i+1<<"����������"<<type<<" "<<Input_Chains[i].ins<<endl; 

		int phy = 0;    // �� n ����������
		for(int f : phy_feature_set) {
//			cout<<chain_types[type][Input_Chains[i].ins][f]<<endl;
			if(chain_types[type][Input_Chains[i].ins][f] == 1) {
				Input_Chains[i].node = service_nodes[phy][rand()%4];    // NF �ڵ���� 
//				cout<<Input_Chains[i].node<<endl;
				Input_Chains[i].phy = phy;
				nf_done = false;
				break;
			}
			++phy;
		}
		
		Input_Chains[i].update_node = Input_Chains[i].node;
		
		// ��ѡ·��
		choosePath(i, &nf_done, Input_Chains);
		memcpy(Input_Chains[i].path, Input_Chains[i].update_path, MAX_PATH_LENGTH*4);
		
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
//	cout<<"ѡ���ѷ���ķ�������"<<endl;
//	for(int i = 0; realc[i] >= 0; ++i) {
//		cout<<realc[i]<<" ";
//	}
}

void update(int i, struct CFC Chains[]) {
//	cout<<"���£�"<<to_be_updated<<endl;
	Chains[i].node = Chains[i].update_node;
	memcpy(Chains[i].path, Chains[i].update_path, 4*MAX_PATH_LENGTH);
//	cout<<"cost�仯��"<<Chains[i].cost<<" "<<Chains[i].update_cost<<endl;
	Chains[i].cost = Chains[i].update_cost;
	
}

void action() { 
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		bool nf_done = true;
		if(Input_Chains[i].node != 0) {    // ���� NF 
			Input_Chains[i].update_node = service_nodes[Input_Chains[i].phy][rand()%4];
			nf_done = false;
		}

		memset(Input_Chains[i].update_path, 0, MAX_PATH_LENGTH*4);
		choosePath(i, &nf_done, Input_Chains);
		
		Input_Chains[i].update_cost = cost(i, Input_Chains);
	}
	
	for(int c: realc) {
		if(c <= 0) {
			break;
		}
		bool nf_done = true;
		if(Allocated_Chains[c].node != 0) {    // ���� NF 
			Allocated_Chains[c].update_node = service_nodes[Allocated_Chains[c].phy][rand()%4];
			nf_done = false;
		}

		memset(Allocated_Chains[c].update_path, 0, MAX_PATH_LENGTH*4);
		choosePath(c, &nf_done, Allocated_Chains);
		
		Allocated_Chains[c].update_cost = cost(c, Allocated_Chains);
	}
	
	double max_perform = 0.0;
	int to_be_updated = -1, i = 0;
	for(; i < NUM_OF_INPUT_CHAINS; ++i) {
		if(max_perform < Input_Chains[i].cost - Input_Chains[i].update_cost) {
//			cout<<"I - "<<i<<"��cost�����ˣ�"<<" "<<Input_Chains[i].cost<<" "<<Input_Chains[i].update_cost<<endl;
			max_perform = Input_Chains[i].cost - Input_Chains[i].update_cost;
			to_be_updated = i;
		}
	}
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		if(max_perform < Allocated_Chains[c].cost - Allocated_Chains[c].update_cost) {
//			cout<<"A - "<<c<<"��cost�����ˣ�"<<" "<<Allocated_Chains[c].cost<<" "<<Allocated_Chains[c].update_cost<<endl;
			max_perform = Allocated_Chains[c].cost - Allocated_Chains[c].update_cost;
			to_be_updated = c + NUM_OF_INPUT_CHAINS;
		}
	}
	if(to_be_updated >= NUM_OF_INPUT_CHAINS) {
		cout<<"���� A  "<<to_be_updated - NUM_OF_INPUT_CHAINS<<endl;
		update(to_be_updated - NUM_OF_INPUT_CHAINS, Allocated_Chains);
	}
	else if(to_be_updated >= 0){
		cout<<"���� I  "<<to_be_updated<<endl;
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

int main() {
	
	read();

	DWORD start, stop;  
    unsigned int t = 0;  
    start = GetTickCount(); 
    
	// �������·���������ѡһ�ֳ�ʼ���� 
	init(NUM_OF_INPUT_CHAINS);

	// ѡ�����˴ε������ѷ�������� 
	classify();
	
	// ���Ը��� 
	for(int times = 0; times < 60; ++times) {
		action();
	}
	
	stop = GetTickCount();  
    printf("\n����ʱ��: %lld ms\n", stop - start);
    
	printChoice();

}

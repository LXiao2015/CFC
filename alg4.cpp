#include <iostream>
#include <ctime>
#include <cmath>
#include <windows.h> 
#include "alg.h"
#include "read_chains.cpp" 
#include "print.cpp"

bool checkTraffic(int path[], int update_path[], int demand) {
	bool flag = true;
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] += demand;
		BW[path[step-1]-1][path[step]-1] += demand;
	}
	for(int step = 1; update_path[step] > 0; ++step) {
		if(BW[update_path[step]-1][update_path[step-1]-1] < demand) {
//			cout<<update_path[step]<<" "<<update_path[step-1]<<" "<< BW[update_path[step]-1][update_path[step-1]-1];
			flag = false;
		}
	}
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] -= demand;
		BW[path[step-1]-1][path[step]-1] -= demand;
	}
	return flag;
}

void chooseNode(int i, bool *nf_done, struct CFC Chains[], int type, int ins) {

//	cout<<"��"<<i+1<<"����������"<<type<<" "<<Chains[i].ins<<" "<<Chains[i].update_ins<<endl; 

	int phy = 0;    // �� n ����������
	for(int f : phy_feature_set) {

		if(chain_types[type][ins][f] == 1) {
			*nf_done = false;
//			cout<<"nf_done��"<<*nf_done<<endl; 
//			cout<<"����������"<<chain_types[type][Chains[i].ins][f]<<endl;
			
			Chains[i].update[ins].unode = service_nodes[phy][rand()%4];    // NF �ڵ���� 
			Chains[i].update[ins].uphy = phy;
//			cout<<"�½ڵ㣺"<<Chains[i].update_node<<endl;
			if(RS[Chains[i].update[ins].unode - 37] <= 0) {
//				cout<<"�½ڵ��ò���"<<endl;
				Chains[i].update[ins].unode = Chains[i].node;
				Chains[i].update[ins].uphy = Chains[i].phy;
				return;
			}
			
//			cout<<Chains[i].update_node<<endl;
			
			return;
		}
	}
 	phy++;

 		
}

void choosePath(int i, bool *nf_done, struct CFC Chains[], int ins) {
//	cout<<*nf_done<<" "<<Chains[i].update[ins].unode<<endl; 

	int step = 0; 
	int src_sw = (Chains[i].src-1)/3 + 28;
	int sink_sw = (Chains[i].sink-1)/3 + 28;
//	cout<<src_sw<<" "<<sink_sw<<endl;
	Chains[i].update[ins].upath[step++] = Chains[i].src;
//	cout<<Chains[i].update_path[step-1]<<" ";
	Chains[i].update[ins].upath[step++] = src_sw;
//	cout<<Chains[i].update_path[step-1]<<" ";

	if(sw_edge[src_sw - 28] == Chains[i].update[ins].unode) {    // ��ѡ���˱����ı�Ե�� 
		Chains[i].update[ins].upath[step++] = Chains[i].update[ins].unode;
//		cout<<Chains[i].update[ins].upath[step-1]<<" ";
		Chains[i].update[ins].upath[step++] = src_sw;
//		cout<<Chains[i].update[ins].upath[step-1]<<" ";
		*nf_done = true; 
	}
	
	int target_sw;    // ȷ����Ҫͨ��Ľ�����
	int t_RT = 43;
	if(*nf_done == false) {    // �����辭���ƶ�, ȷ������Ľ�������·���� 	
		target_sw = cloud_sw[Chains[i].update[ins].unode - 37];
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
	Chains[i].update[ins].upath[step++] = s_RT;
//	cout<<Chains[i].update_path[step-1]<<" ";
	memcpy(Chains[i].update[ins].upath + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
//	memcpy(Chains[i].update[ins].upath + step, RT_Paths[s_RT-42][t_RT-42][rand()%3], 4*4);
//	cout<<"Router"<<" ";
	for(; Chains[i].update[ins].upath[step] > 0; ++step) {}
	
	if(target_sw == sink_sw && *nf_done == true) {  // �� NF���Ѿ��� NF ��·�������� 
		Chains[i].update[ins].upath[step++] = sink_sw;
//		cout<<Chains[i].update_path[step-1]<<" ";
		Chains[i].update[ins].upath[step] = Chains[i].sink;
//		cout<<Chains[i].update_path[step]<<" ";
	}
	
	else {
		// ��� NF ��һ�ε�·�� 
		if(target_sw > 0) {
			Chains[i].update[ins].upath[step++] = target_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
		}
		Chains[i].update[ins].upath[step++] = Chains[i].update[ins].unode;
//		cout<<Chains[i].update_path[step-1]<<" ";
		*nf_done = true;
		if(target_sw > 0) {
			Chains[i].update[ins].upath[step++] = target_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
		}
		
		if(target_sw == sink_sw) {    // NF �ڻ�۽ڵ���·�������� 

//			cout<<Chains[i].update_path[step-1]<<" ";
			Chains[i].update[ins].upath[step] = Chains[i].sink;
//			cout<<Chains[i].update_path[step]<<" ";
		}
		
		else{
			s_RT = t_RT;
			t_RT = sw_RT[sink_sw - 28];
			Chains[i].update[ins].upath[step++] = s_RT;
//			cout<<Chains[i].update_path[step-1]<<" ";
//	        memcpy(path + step, RT_Paths[s_RT-42][t_RT-42][0], 4*4);
			memcpy(Chains[i].update[ins].upath + step, RT_Paths[s_RT-42][t_RT-42][rand()%5], 4*4);
//			memcpy(Chains[i].update_path + step, RT_Paths[s_RT-42][t_RT-42][rand()%3], 4*4);
//			cout<<"Router"<<" ";
			for(; Chains[i].update[ins].upath[step] > 0; ++step) {}
			Chains[i].update[ins].upath[step++] = sink_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
			Chains[i].update[ins].upath[step] = Chains[i].sink;
//			cout<<Chains[i].update_path[step]<<" ";
		}
		
	} 
	
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
	if(!checkTraffic(Chains[i].path, Chains[i].update[ins].upath, Chains[i].demand)) {
		memcpy(Chains[i].update[ins].upath, Chains[i].path, MAX_PATH_LENGTH*4);
		Chains[i].update[ins].unode = Chains[i].node;

		Chains[i].update[ins].uphy = Chains[i].phy;
//		cout<<"��������"<<endl;
	}
	
//	cout<<Chains[i].update_node<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update_path[step]<<" ";
//	}
//	cout<<endl;
}

void init() {

	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {	
		
		bool nf_done = true;    // NF �Ƿ��Ѿ���(�����û�� NF) 
		
		int type = Input_Chains[i].service_type;    // ���ַ������е���һ��
		int ins = rand()%num_of_ins[type];    // ѡ��һ��ʵ�ַ�ʽ(���Ƿ���������ʧ��)
		Input_Chains[i].ins = ins;
		if(ins != 0) {
			chooseNode(i, &nf_done, Input_Chains, type, ins);   // ʧ�ܺ�ָ� 
		
			// ��ѡ·��
			choosePath(i, &nf_done, Input_Chains, ins);   // ʧ�ܺ�ָ�
			
			// �۳���Դ�ʹ��� 
			updateTraffic(Input_Chains[i].path, Input_Chains[i].update[ins].upath, Input_Chains[i].demand);
			if(Input_Chains[i].node > 0) {
				RS[Input_Chains[i].node - 37] += Input_Chains[i].demand;
			}
			if(Input_Chains[i].update[ins].unode > 0) {
				RS[Input_Chains[i].update[ins].unode - 37] -= Input_Chains[i].demand;
			}
			memcpy(Input_Chains[i].path, Input_Chains[i].update[ins].upath, 4*MAX_PATH_LENGTH);
	
			Input_Chains[i].phy = Input_Chains[i].update[ins].uphy;
			Input_Chains[i].node = Input_Chains[i].update[ins].unode;
		}

		Input_Chains[i].cost = cost(i, Input_Chains, ins);
//		cout<<"COST: "<<Input_Chains[i].cost<<endl;

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
//		realc[count++] = i;  // ȫ�ӽ������� 

	}
	num_of_realc = count;
//	cout<<"ѡ���ѷ���ķ�������"<<endl;
//	for(int i = 0; realc[i] >= 0; ++i) {
//		cout<<realc[i]<<" ";
//	}
}

void update(int i, struct CFC Chains[], int ins) {
	updateTraffic(Chains[i].path, Chains[i].update[ins].upath, Chains[i].demand);
	if(Chains[i].node > 0) {
		RS[Chains[i].node - 37] += Chains[i].demand;
	}
	if(Chains[i].node > 0) {
		RS[Chains[i].node - 37] -= Chains[i].demand;
	}
//	cout<<"���£�"<<to_be_updated<<endl;
	Chains[i].node = Chains[i].update[ins].unode;
	memcpy(Chains[i].path, Chains[i].update[ins].upath, 4*MAX_PATH_LENGTH);
//	cout<<"cost�仯��"<<Chains[i].cost<<" "<<Chains[i].update_cost<<endl;
	Chains[i].cost = Chains[i].update[ins].ucost;
	Chains[i].ins = ins;
}

void action() { 
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		int type = Input_Chains[i].service_type;    // ���ַ������е���һ��

		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			bool nf_done = true;
	//		cout<<"ѡȡ�ڵ�..."<<endl;
			chooseNode(i, &nf_done, Input_Chains, type, ins);
	//		cout<<"done��"<<endl;
	//		cout<<"���·��..."<<endl;
			memset(Input_Chains[i].update[ins].upath, 0, MAX_PATH_LENGTH*4);
	//		cout<<"done��"<<endl; 
	//		cout<<"ѡȡ·��..."<<endl;
			choosePath(i, &nf_done, Input_Chains, ins);
	//		cout<<"done��"<<endl;
			
			Input_Chains[i].update[ins].ucost = cost(i, Input_Chains, ins);
		}
		
	}
	
	for(int c: realc) {
		if(c <= 0) {
			break;
		}
		int type = Allocated_Chains[c].service_type;    // ���ַ������е���һ��

		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			bool nf_done = true;
	//		cout<<"ѡȡ�ڵ�..."<<endl;
			chooseNode(c, &nf_done, Allocated_Chains, type, ins);
	//		cout<<"done��"<<endl;
	//		cout<<"���·��..."<<endl;
			memset(Allocated_Chains[c].update[ins].upath, 0, MAX_PATH_LENGTH*4);
	//		cout<<"done��"<<endl; 
	//		cout<<"ѡȡ·��..."<<endl;
			choosePath(c, &nf_done, Allocated_Chains, ins);
	//		cout<<"done��"<<endl;
			
			Allocated_Chains[c].update[ins].ucost = cost(c, Allocated_Chains, ins);
		}
		
	}
	
//	double max_perform = -100.0;
//	double max_perform = 0.0;
	double min_perform = 1000.0, Qf; 
	int update_chain = -1, update_ins = -1;
	double b = 70000.0, t = 300.0;
	
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		int type = Input_Chains[i].service_type;
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			Qf = exp(t - b*(Input_Chains[i].cost - Input_Chains[i].update[ins].ucost))/num_of_ins[type];
			if(min_perform > Qf) {
	//			cout<<"I - "<<i<<"��cost�����ˣ�"<<" "<<Input_Chains[i].cost<<" "<<Input_Chains[i].update_cost<<endl;
				min_perform = exp(Input_Chains[i].cost - Input_Chains[i].update[ins].ucost)/num_of_ins[type];
				update_chain = i;
				update_ins = ins; 
			}
		} 
		
	}
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		int type = Allocated_Chains[c].service_type;    // ���ַ������е���һ��
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			Qf = exp(t - b*(Allocated_Chains[c].cost - Allocated_Chains[c].update[ins].ucost))/num_of_ins[type];
			if(min_perform > Qf) {
	//			cout<<"A - "<<c<<"��cost�����ˣ�"<<" "<<Allocated_Chains[c].cost<<" "<<Allocated_Chains[c].update_cost<<endl;
				min_perform = exp(Allocated_Chains[c].cost - Allocated_Chains[c].update[ins].ucost)/num_of_ins[type];
				update_chain = c + NUM_OF_INPUT_CHAINS;
				update_ins = ins;
			}
		}
		
	}
	if(update_chain >= NUM_OF_INPUT_CHAINS) {
		cout<<"���� A  "<<update_chain - NUM_OF_INPUT_CHAINS<<" "<<update_ins<<endl;
//		cout<<Allocated_Chains[to_be_updated - NUM_OF_INPUT_CHAINS].node<<" "<<Allocated_Chains[to_be_updated - NUM_OF_INPUT_CHAINS].update_node<<endl;
		update(update_chain - NUM_OF_INPUT_CHAINS, Allocated_Chains, update_ins);
//		cout<<"done��"<<endl;
	}
	else if(update_chain >= 0){
		cout<<"���� I  "<<update_chain<<" "<<update_ins<<endl;
//		cout<<Input_Chains[to_be_updated].node<<" "<<Input_Chains[to_be_updated].update_node<<endl;
		update(update_chain, Input_Chains, update_ins);
//		cout<<"done��"<<endl;
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
	srand((unsigned)time(NULL));    // ������ӵ���һ�μ���
	
	read();

	DWORD start, stop;  
    unsigned int t = 0;  
    start = GetTickCount(); 
    
	// �������·���������ѡһ�ֳ�ʼ���� 
	init();
	
//	printChoice();

	// ѡ�����˴ε������ѷ�������� 
	classify();
//	
//	// ���Ը��� 
	for(int times = 0; times < 777; ++times) {
//		cout<<"��"<<times<<"�Σ�"<<endl;
		action();
//		cout<<endl;
	}
	
	stop = GetTickCount();  
    printf("\n����ʱ��: %lld ms\n", stop - start);
    
	printChoice();
//	printBW();
//	printRS();

}

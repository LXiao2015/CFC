#include <iostream>
#include <ctime>
#include <cmath>
#include <windows.h> 
#include "alg.h"
#include "read_chains.cpp" 

using namespace std;

void chooseNode(int i, bool *nf_done, struct CFC Chains[], int type, int ins) {
	Chains[i].update[ins].succ = true;
//	cout<<"��"<<i+1<<"����������"<<type<<" "<<Chains[i].ins<<" "<<ins<<endl; 

	int phy = 0;    // �� n ����������
	for(int f : phy_feature_set) {

		if(chain_types[type][ins][f] == 1) {
			*nf_done = false;
//			cout<<"nf_done��"<<*nf_done<<endl; 
//			cout<<"����������"<<chain_types[type][Chains[i].ins][f]<<endl;
			
			Chains[i].update[ins].unode = service_nodes[phy][rand()%count_of_nfnode[phy]];    // NF �ڵ���� 
			Chains[i].update[ins].uphy = phy;
//			cout<<"�½ڵ㣺"<<Chains[i].update[ins].unode<<endl;
			if(!checkCapacity(Chains, i, ins)) {
//				cout<<"�½ڵ��ò���"<<endl;
				Chains[i].update[ins].unode = Chains[i].node;
//				Chains[i].update[ins].uphy = Chains[i].phy;
				if(Chains[i].node <= 0 || !checkCapacity(Chains, i, ins)) {    // ԭ��Ҳû�� NF �ڵ�, ����ԭ�ȵĽڵ�Ҳ������ɣ�����ʵ�ַ�ʽʧ�� 
//					cout << "�ɽڵ�Ҳ�ò���" << endl;
					*nf_done = true;
					Chains[i].update[ins].succ = false;
					Chains[i].update[ins].uphy = Chains[i].phy;
				}
				return;
			}
			
//			cout<<"���½ڵ㣺"<<Chains[i].update[ins].unode<<endl;
			
			return;
		}
		phy++;    // ����λ�� 
	}
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
	int t_RT = 40;    // ֱ�������Ƶ�·���� 
	if(*nf_done == false) {    // �����辭���ƶ�, ȷ������Ľ�������·���� 	
		target_sw = cloud_sw[Chains[i].update[ins].unode - 41];
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
	if(s_RT != t_RT) {
		int rd = rand()%5;
		memcpy(Chains[i].update[ins].upath + step, RT_Paths[s_RT-37][t_RT-37][rd], 4*4);

//		cout<<"Router"<<" ";
		
		for(; Chains[i].update[ins].upath[step] > 0; ++step) {
//			cout<<Chains[i].update[ins].upath[step]<<" ";
		}
	}
	
	
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

			if(s_RT != t_RT) {
				int rd = rand()%5;
				memcpy(Chains[i].update[ins].upath + step, RT_Paths[s_RT-37][t_RT-37][rd], 4*4);

//				cout<<"Router"<<" ";
				for(; Chains[i].update[ins].upath[step] > 0; ++step) {
//					cout<<Chains[i].update[ins].upath[step]<<" ";
				}
			}
			
			Chains[i].update[ins].upath[step++] = sink_sw;
//			cout<<Chains[i].update_path[step-1]<<" ";
			Chains[i].update[ins].upath[step] = Chains[i].sink;
//			cout<<Chains[i].update_path[step]<<" ";
		}	
	} 
	
//	cout<<Chains[i].update[ins].unode<<endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update[ins].upath[step]<<" ";
//	}
//	cout<<endl;
	if(!checkTraffic(Chains[i].path, Chains[i].update[ins].upath, Chains[i].demand, Chains[i].phy, Chains[i].update[ins].uphy)) {
		memcpy(Chains[i].update[ins].upath, Chains[i].path, MAX_PATH_LENGTH*4);
		Chains[i].update[ins].unode = Chains[i].node;

		Chains[i].update[ins].uphy = Chains[i].phy;
		Chains[i].update[ins].succ = false;
//		cout<<"��������"<<endl;
	}
	else {
		Chains[i].update[ins].succ &= true;
//		cout<<"·������ɹ���"<<endl;
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
//		Input_Chains[i].ins = ins;
//		cout<<"I ��ʼ - "<<i<<endl;
		if(ins != 0) {
			chooseNode(i, &nf_done, Input_Chains, type, ins);   // ʧ�ܺ�ָ� 
			if(Input_Chains[i].update[ins].succ != false) {
				// ��ѡ·��
				choosePath(i, &nf_done, Input_Chains, ins);   // ʧ�ܺ�ָ�
			}	
			if(Input_Chains[i].update[ins].succ != false) {
				// �۳���Դ�ʹ��� 
				updateTraffic(Input_Chains[i].path, Input_Chains[i].update[ins].upath, Input_Chains[i].demand, Input_Chains[i].phy, Input_Chains[i].update[ins].uphy);
				updateCapacity(Input_Chains, i, ins);
				memcpy(Input_Chains[i].path, Input_Chains[i].update[ins].upath, 4*MAX_PATH_LENGTH);
				
				Input_Chains[i].ins = ins;
				Input_Chains[i].phy = Input_Chains[i].update[ins].uphy;
				Input_Chains[i].node = Input_Chains[i].update[ins].unode;
				
			}
		}
 		if(Input_Chains[i].update[ins].succ != false) {
			Input_Chains[i].fT = singleCost(i, Input_Chains, ins);	
		}
		else {
			Input_Chains[i].fT = singleCost(i, Input_Chains, 0);
		}
	}
} 

void classify() {
//	cout<<"classify"<<endl;

	int lowC = 1000, highC = 0;
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
		lowC = lowC >= Allocated_Chains[i].fT? Allocated_Chains[i].fT: lowC;
		highC = highC <= Allocated_Chains[i].fT? Allocated_Chains[i].fT: highC;
	}
	int count = 0;
	for(int i = 0; i < NUM_OF_ALLOCATED_CHAINS; ++i) {
//		double Pc = pow(Allocated_Chains[i].cost/sumC, 1.0/(double)NUM_OF_ALLOCATED_CHAINS);
//		double Pc = Allocated_Chains[i].cost/sumC;
		double Pc = Allocated_Chains[i].fT;
		int pr = rand()%(highC - lowC) + lowC;

		if(pr - Pc < 0) {
			realc[count++] = i;
		}
//		realc[count++] = i;  // ȫ�ӽ������� 
	}
	num_of_realc = count;
	cout<<"ѡ���ѷ���ķ�������"<<endl;
	for(int i = 0; realc[i] >= 0; ++i) {
		cout<<realc[i]<<" ";
	}
}

void update(int i, struct CFC Chains[], int ins, float update_cost) {
//	printUsage();
//	cout << "·���仯��" << endl;
//	
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].path[step]<<" ";
//	}
//	cout << endl;
//	for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
//		cout<<Chains[i].update[ins].upath[step]<<" ";
//	}
//	cout << endl;

//	cout << "�ڵ�ѡ��仯��" << Chains[i].node << " " << Chains[i].update[ins].unode << endl;
//	cout << "���ͣ�" << Chains[i].service_type << "  " << "ins�仯��" << Chains[i].ins << ins << endl; 
//	cout << "phy�仯��" << Chains[i].phy << Chains[i].update[ins].uphy << endl; 
	
//	cout << "������С��" << Chains[i].demand << endl;
	updateTraffic(Chains[i].path, Chains[i].update[ins].upath, Chains[i].demand, Chains[i].phy, Chains[i].update[ins].uphy);
	updateCapacity(Chains, i, ins);
	
//	printUsage();
//	cout<<"���£�"<<i<<endl;
	Chains[i].node = Chains[i].update[ins].unode;
	memcpy(Chains[i].path, Chains[i].update[ins].upath, 4*MAX_PATH_LENGTH);
//	cout<<"singlecost�仯��"<<Chains[i].fT<<" "<<Chains[i].update[ins].uT<<endl;
	Chains[i].fT = Chains[i].update[ins].uT;
	Chains[i].ins = ins;
	Chains[i].phy = Chains[i].update[ins].uphy;
	
	// update T
	T = update_cost;
//	cout<<T<<endl;
//	printCost();
}

void action() { 
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		int type = Input_Chains[i].service_type;    // ���ַ������е���һ��
//		cout<<"i = "<<i<<endl;
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			bool nf_done = true;
//			cout<<"ѡȡ�ڵ�..."<<endl;
			chooseNode(i, &nf_done, Input_Chains, type, ins);
//			cout<<"done��"<<endl;
//			cout<<"���·��..."<<endl;
			memset(Input_Chains[i].update[ins].upath, 0, MAX_PATH_LENGTH*4);
//			cout<<"done��"<<endl; 

			if(Input_Chains[i].update[ins].succ != false) {
//				cout<<"ѡȡ·��..."<<endl;
				choosePath(i, &nf_done, Input_Chains, ins);
//			    cout<<"done��"<<endl;		
			}
			if(Input_Chains[i].update[ins].succ != false) {
//				cout<<"�����������"<<i<<"��"<<ins<<"��cost��"<<endl;
				Input_Chains[i].update[ins].uT = singleCost(i, Input_Chains, ins);
			}
		}
	}
	
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		int type = Allocated_Chains[c].service_type;    // ���ַ������е���һ��
//		cout<<"c = "<<c<<endl;
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			bool nf_done = true;
//			cout<<"ѡȡ�ڵ�..."<<endl;
			chooseNode(c, &nf_done, Allocated_Chains, type, ins);
//			cout<<"done��"<<endl;
//			cout<<"���·��..."<<endl;
			memset(Allocated_Chains[c].update[ins].upath, 0, MAX_PATH_LENGTH*4);
//			cout<<"done��"<<endl; 

			if(Allocated_Chains[c].update[ins].succ != false) {
//				cout<<"ѡȡ·��..."<<endl;
				choosePath(c, &nf_done, Allocated_Chains, ins);
//			    cout<<"done��"<<endl;
//				
			}
			if(Allocated_Chains[c].update[ins].succ != false) {
//				cout<<"�����������"<<c<<"��"<<ins<<"��cost��"<<endl;
				Allocated_Chains[c].update[ins].uT = singleCost(c, Allocated_Chains, ins);
			}

		}	
	}

//	double max_perform = -100.0;
//	double max_perform = 0.0;
	double min_perform = 1000.0, Qf; 
	int update_chain = -1, update_ins = -1;
	float update_cost;
	double b = 7.0, t = 3.0;

	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		int type = Input_Chains[i].service_type;
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			if(Input_Chains[i].update[ins].succ == false) {
//				cout << i << "���������" << endl;
				continue;
			}
//			cout<<newCost(Input_Chains, i, ins)<<" + "<<Input_Chains[i].update[ins].uT<<" - "<<Input_Chains[i].fT<<endl;
			float new_cost = newCost(Input_Chains, i, ins) + Input_Chains[i].update[ins].uT - Input_Chains[i].fT;
//			cout<<i<<"!!!!!"<<new_cost<<" "<<T<<endl;
//			Qf = exp(t - b*(Input_Chains[i].cost - Input_Chains[i].update[ins].ucost))/num_of_ins[type];
			Qf = exp(t - b*(1/new_cost - 1/T))/4;    // ����ķ�ĸӦ���ǿ�ѡ�� node 
//			cout<<Qf<<endl;	
//			if(Input_Chains[i].update[ins].ucost != Input_Chains[i].cost && min_perform > Qf) {
			if(min_perform > Qf) {
	//			cout<<"I - "<<i<<"��cost�����ˣ�"<<" "<<Input_Chains[i].cost<<" "<<Input_Chains[i].update_cost<<endl;
				min_perform = Qf;
				update_chain = i;
				update_ins = ins; 
				update_cost = new_cost;
			}
		} 
	}
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		int type = Allocated_Chains[c].service_type;    // ���ַ������е���һ��
		
		for(int ins = 0; ins < num_of_ins[type]; ++ins) {
			if(Allocated_Chains[c].update[ins].succ == false) {
//				cout << c << " ���������" << endl;
				continue;
			}
//			cout<<newCost(Input_Chains, c, ins)<<" + "<<Allocated_Chains[c].update[ins].uT<<" - "<<Allocated_Chains[c].fT<<endl;
			float new_cost = newCost(Allocated_Chains, c, ins) + Allocated_Chains[c].update[ins].uT - Allocated_Chains[c].fT;
//			cout<<c<<"!!!!!"<<new_cost<<" "<<T<<endl;
//			Qf = exp(t - b*(Allocated_Chains[c].cost - Allocated_Chains[c].update[ins].ucost))/num_of_ins[type];
			Qf = exp(t - b*(1/new_cost - 1/T))/4;    // ����ķ�ĸӦ���ǿ�ѡ�� node
//			cout<<Qf<<endl;		
//			if(Allocated_Chains[c].update[ins].ucost != Allocated_Chains[c].cost && min_perform > Qf) {
			if(min_perform > Qf) {
	//			cout<<"A - "<<c<<"��cost�����ˣ�"<<" "<<Allocated_Chains[c].cost<<" "<<Allocated_Chains[c].update_cost<<endl;
				min_perform = Qf;
				update_chain = c + NUM_OF_INPUT_CHAINS;
				update_ins = ins;
				update_cost = new_cost;
			}
		}
	}
	
//	cout << "δ����ǰ�� T ����" << T << endl;
//	printUsage();
//	cout << endl;
	if(update_chain >= NUM_OF_INPUT_CHAINS) {
//		cout<<"���� A  "<<update_chain - NUM_OF_INPUT_CHAINS<<" "<<update_ins<<endl;
//		cout<<Allocated_Chains[update_chain - NUM_OF_INPUT_CHAINS].node<<" "<<Allocated_Chains[update_chain - NUM_OF_INPUT_CHAINS].update[update_ins].unode<<endl;
		update(update_chain - NUM_OF_INPUT_CHAINS, Allocated_Chains, update_ins, update_cost);
//		cout<<"done��"<<endl;
	}
	else if(update_chain >= 0){
//		cout<<"���� I  "<<update_chain<<" "<<update_ins<<endl;
//		cout<<Input_Chains[update_chain].node<<" "<<Input_Chains[update_chain].update[update_ins].unode<<endl;
		update(update_chain, Input_Chains, update_ins, update_cost);
//		cout<<"done��"<<endl;
	}
	else {
//		cout<<"û�и���"<<T<<endl;
		return;
	}
}

int main() {
	srand((unsigned)time(NULL));    // ������ӵ���һ�μ���
	
//	cout << "before read" << endl;
	read();
//	cout << "read over" << endl;
	printRS();
//	printBW();
//	printUsage();
	DWORD start, stop1 = 0, stop2;  
    start = GetTickCount(); 
    stop1 = start;
//    printRS();
	// �������·���������ѡһ�ֳ�ʼ���� 
	init();
//	cout << "init over" << endl;

//	printRS();
//	printChoice();
//	printUsage();

	totalCost();
	cout << T << endl;
//	cout << "cost is caculated" << endl;
//	printRS();

//	printBW();
	// ѡ�����˴ε������ѷ�������� 
	classify();
//	
	// ���Ը��� 
	for(int times = 0; times < 300; ++times) {
//		cout<<"��"<<times<<"�Σ�"<<endl;
//		printRS();
		stop2 = GetTickCount();  
		if((stop2 - stop1) >= 10) {
			printf("\n����ʱ��: %lld ms\n", stop2 - start);
			cout << T << endl;
			stop1 = stop2;
		}
		action();
//		printUsage();
//		cout<<endl;
	}
	
	stop2 = GetTickCount();  
    printf("\n����ʱ��: %lld ms\n", stop2 - start);

	printChoice();
//	printFeature();
	printUsage();
	printCost(); 
//	printBW();
	printRS();
}

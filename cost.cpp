#include "lcs.cpp"

int getLength(int seq[], int maxLen) {
	int l = 0;
	while(l < maxLen && seq[l] > 0) l++;
	return l;
}

float singleCost(int i, struct CFC Chains[], int ins) { 
	double cff = 0.0, cu = 0.0;
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
//		cout<<chain_types[Chains[i].service_type][ins][j]<<" ";
		cff += (chain_types[Chains[i].service_type][ins][j] == true)? 0: feature_failure_cost[Chains[i].service_type][j];
//		cout<<cff<<endl;
	}
//	cout<<endl;
	int samepath[MAX_PATH_LENGTH] = {0};

	int l1 = getLength(Chains[i].update[ins].upath, MAX_PATH_LENGTH);
	int l2 = getLength(Chains[i].ini_path, MAX_PATH_LENGTH);

	int samelength = lcs(Chains[i].update[ins].upath, l1, Chains[i].ini_path, l2, samepath);

	bool firstnotsame = 1;
	int s = samelength - 1, count = 0, node = 0;
	for(int step = 0; (node = Chains[i].update[ins].upath[step]) > 0; ++step) {
		if(samepath[s] != node) {
			if(firstnotsame) {
				firstnotsame = 0;
				count++;
			}
			else count++;	
		}
		s--;
	} 
	count--;    // 汇聚节点不需要更新 
//	cout<<"需要发送更新消息的点："<<count<<endl;
	cu = count * update_msg_cost;
	
//	cout<<Chains[i].service_type<<" "<<ins<<endl;
//	cout<<"NF："<<Chains[i].node<<endl;	
//	cout<<"cff: "<<cff<<"  cu: "<<cu<<endl;
	return cff + cu;
}

void totalCost() {
	double CR = 0.0, CI = 0.0;
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		T += Input_Chains[i].fT;
//		cout<<T<<endl;
	}

	for(int c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
		T += Allocated_Chains[c].fT;
//		cout<<T<<endl;
	}

	for(int i = 0; i < NUM_OF_NFNODES; ++i) {
		CR += (node_used[i] > 0? 1: 0) * node_using_cost[i];
	}
	T += CR;
	
	for(int i = 0; i < NUM_OF_CLOUDS; ++i) {
		for(int j = 0; j < 3; ++j) {
			CI += node_vnf_count[i][j] * node_init_cost;
		}
	}
	T += CI;
	
//	printUsage();
}

float newCost(struct CFC Chains[], int i, int ins) {
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	if(node == unode && uphy == phy) {
		return T;
	}
	
	double newT = T;
	double demand = Chains[i].demand;
//	cout << "here " << newT << endl;
	// node using cost
	if(unode != node) {
		if(node_used[node - 41] == 1) {
			newT -= node_using_cost[node - 41];
		}
		if(node_used[unode - 41] == 0) {
			newT += node_using_cost[unode - 41];
		}
	}
//	cout << newT << endl;
	// vnf initiating cost
	
	if(node > 41) {
		newT -= node_vnf_count[node - 42][phy] * node_init_cost;
//		cout << newT << endl;
		newT += (int)((node_vnf_demand[node - 42][phy] - demand + unit_rps[phy] - 1)/unit_rps[phy]) * node_init_cost;
//		cout << newT << endl;
	}
	if(unode > 41) {
		newT -= node_vnf_count[unode - 42][uphy] * node_init_cost;	
//		cout << newT << endl;
		newT += (int)((node_vnf_demand[unode - 42][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]) * node_init_cost;
//		cout << newT << " here" << endl;
	}
//	cout<<newT<<endl;
	return newT;
}


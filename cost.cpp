#include "lcs.cpp"

int getLength(int seq[], int maxLen) {
	int l = 0;
	while(l < maxLen && seq[l] > 0) l++;
	return l;
}

float singleCost(int i, struct CFC Chains[], int ins) { 
	float cff = 0.0, cu = 0.0;
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
//		cout<<chain_types[Chains[i].service_type][Chains[i].update_ins][j]<<" ";
		cff += (1 - chain_types[Chains[i].service_type][ins][j]) * feature_failure_cost[Chains[i].service_type][j];
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
//	cout<<"需要发送更新消息的点："<<count<<endl;
	cu = count * update_msg_cost;
	
//	cout<<Chains[i].service_type<<" "<<Chains[i].ins<<endl;
//	cout<<"NF："<<Chains[i].node<<endl;
//	
//	cout<<"cff: "<<cff<<"  cu: "<<cu<<endl;
	return cff + cu;
}

void totalCost() {
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
	
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		if(Input_Chains[i].node != 40) {
			node_vnf_demand[Input_Chains[i].node - 37][Input_Chains[i].phy] += Input_Chains[i].demand;
		}
	}
	for(int c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
		if(Allocated_Chains[c].node != 40) {
			node_vnf_demand[Allocated_Chains[c].node - 37][Allocated_Chains[c].phy] += Allocated_Chains[c].demand;
		}
	}
	for(int i = 0; i < NUM_OF_CLOUDS; ++i) {
		for(int j = 0; j < 3; ++j) {
			CI += (int)(node_vnf_count[i][j] * node_init_cost);
		}
	}
	T += CI;
	
//	cout<<CR<<" "<<CI<<" "<<T<<endl;
}

float newCost(struct CFC Chains[], int i, int ins) {
	// node using cost
	float newT = T;
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	float demand = Chains[i].demand;
//	cout<<newT<<endl;
	if(unode != node) {
		if(node_used[node - 37] == 1) {
			newT -= node_using_cost[node - 37];
		}
		if(node_used[unode - 37] == 0) {
			newT += node_using_cost[unode - 37];
		}
	}
//	cout<<newT<<endl;
	// vnf initiating cost
	if(node != 40) {
		newT -= (int)((node_vnf_demand[node - 37][phy] + unit_rps[phy] - 1)/unit_rps[phy]) * node_init_cost;
//		cout<<newT<<endl;
		newT += (int)((node_vnf_demand[node - 37][phy] - demand + unit_rps[phy] - 1)/unit_rps[phy]) * node_init_cost;
//		cout<<newT<<endl;
	}
	if(unode != 40) {
		newT -= (int)((node_vnf_demand[unode - 37][uphy] + unit_rps[uphy] - 1)/unit_rps[uphy]) * node_init_cost;
//		cout<<newT<<endl;
		newT += (int)((node_vnf_demand[unode - 37][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]) * node_init_cost;
//		cout<<newT<<endl;
	}
//	cout<<newT<<endl;
	return newT;
}

//minimize 
//    sum( c in cfc, j in Feature_Model ) (1 - f_choice[c][j]) * feature_failure_cost[c.type][j] +    // CF 有了 
//    sum( n in nfnode ) used[n] * node_using_cost[n] +   
//    sum( i in cnode, v in vnf_feature ) instance_count[i][v] * init_cost +    // CR
//    sum( l in path, r in req ) flow[r][l] * update_msg_cost;    // CU 有了 

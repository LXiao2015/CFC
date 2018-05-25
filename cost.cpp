#include "lcs.cpp"

int getLength(int seq[], int maxLen) {
	int l = 0;
	while(l < maxLen && seq[l] > 0) l++;
	return l;
}

float single_cost(int i, struct CFC Chains[], int ins) { 
	float cff = 0.0, cu = 0.0;
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
//		cout<<chain_types[Chains[i].service_type][Chains[i].update_ins][j]<<" ";
		cff += (1 - chain_types[Chains[i].service_type][ins][j]) * feature_failure_cost[Chains[i].service_type][j];
	}
	CFF += cff;
//	cout<<endl;
	int samepath[MAX_PATH_LENGTH] = {0};

	int l1 = getLength(Chains[i].update[ins].upath, MAX_PATH_LENGTH);
	int l2 = getLength(Chains[i].path, MAX_PATH_LENGTH);

	int samelength = lcs(Chains[i].update[ins].upath, l1, Chains[i].path, l2, samepath);

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
	CU += cu;
	
//	cout<<Chains[i].service_type<<" "<<Chains[i].update_ins<<endl;
//	cout<<"NF："<<Chains[i].update_node<<endl;
//	
	cout<<CFF<<" "<<CU<<endl;
}

float totalcost() {
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		single_cost(i, Input_Chains, Input_Chains[i].ins);
	}
//	// 已分配的在 read() 里加过了 
//	for(int c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
//		single_cost(c, Allocated_Chains, Allocated_Chains[c].ins);
//	}
	
	for(int i = 0; i < NUM_OF_NFNODES; ++i) {
		CR += (node_used[i] > 0? 1: 0) * node_using_cost[i];
	}
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		node_vnf_demand[Input_Chains[i].node - 37][Input_Chains[i].phy] += Input_Chains[i].demand;
	}
	for(int c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
		node_vnf_demand[Allocated_Chains[c].node - 37][Allocated_Chains[c].phy] += Allocated_Chains[c].demand;
	}
}

//minimize 
//    sum( c in cfc, j in Feature_Model ) (1 - f_choice[c][j]) * feature_failure_cost[c.type][j] +    // CF 有了 
//    sum( n in nfnode ) used[n] * node_using_cost[n] +   
//    sum( i in cnode, v in vnf_feature ) instance_count[i][v] * init_cost +    // CR
//    sum( l in path, r in req ) flow[r][l] * update_msg_cost;    // CU 有了 

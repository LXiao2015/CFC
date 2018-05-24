#include "lsc.cpp"

int getLength(int seq[], int maxLen) {
	int l = 0;
	while(l < maxLen && seq[l] > 0) l++;
	return l;
}

double cost(int i, struct CFC Chains[], int ins) {    // 求整体？ 
	double CR = 0.0, CF = 0.0, CFF = 0.0, CU = 0.0;
	int node;
	for(int step = 0; (node = Chains[i].update[ins].upath[step]) > 0; ++step) {
//		if(node != )
		CR += node_using_cost[node - 1] * Chains[i].demand;
	}
	if(Chains[i].update[ins].unode > 0) {
		CR += resource_cost_of_node[Chains[i].update[ins].unode - 37] * Chains[i].demand;
	}
	
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
//		cout<<chain_types[Chains[i].service_type][Chains[i].update_ins][j]<<" ";
		CFF += (1 - chain_types[Chains[i].service_type][ins][j]) * feature_failure_cost[Chains[i].service_type][j];
	}
//	cout<<endl;
	int samepath[MAX_PATH_LENGTH] = {0};

	int l1 = getLength(Chains[i].update[ins].upath, MAX_PATH_LENGTH);
	int l2 = getLength(Chains[i].path, MAX_PATH_LENGTH);

	int samelength = lcs(Chains[i].update[ins].upath, l1, Chains[i].path, l2, samepath);

	bool firstnotsame = 1;
	int s = samelength - 1, count = 0;
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
	CU += count * update_msg_cost;    // 这个是瞎写的, 应该与最开始分配的结果比较 
	
//	cout<<Chains[i].service_type<<" "<<Chains[i].update_ins<<endl;
//	cout<<"NF："<<Chains[i].update_node<<endl;
//	
//	cout<<CR<<" "<<CFF<<" "<<CU<<endl;
	
//	return CR + CF + CFF + CU; 
	return CR + CFF + CU; 
}


//minimize 
//    sum( c in cfc, j in Feature_Model ) (1 - f_choice[c][j]) * feature_failure_cost[c.type][j] +    // CF 有了 
//    sum( n in nfnode ) used[n] * node_using_cost[n] +  有了 
//    sum( i in cnode, v in vnf_feature ) instance_count[i][v] * init_cost +    // CR
//    sum( l in path, r in req ) flow[r][l] * update_msg_cost;    // CU

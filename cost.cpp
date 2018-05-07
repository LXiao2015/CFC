double cost(int i, struct CFC Chains[]) {
	double CR = 0.0, CF = 0.0, CFF = 0.0, CU = 0.0;
	if(Chains[i].update_node > 0) {
		for(int step = 0; Chains[i].update_path[step] > 0; ++step) {
			CR += use_cost_of_node[Chains[i].update_path[step] - 1];
		}
		CR += resource_cost_of_node[Chains[i].update_node - 37] * Chains[i].demand;
	}
	
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
		CFF += (1 - chain_types[Input_Chains[i].service_type][Chains[i].ins][j]) * feature_failure_cost[Chains[i].service_type][j];
	}
	CU += (Chains[i].update_node ^ Chains[i].node) * update_msg_cost;    // 这个是瞎写的, 应该与最开始分配的结果比较 
	return CR + CF + CFF + CU; 
}

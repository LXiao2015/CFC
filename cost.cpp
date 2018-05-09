double cost(int i, struct CFC Chains[]) {
	double CR = 0.0, CF = 0.0, CFF = 0.0, CU = 0.0;
	
	for(int step = 0; Chains[i].update_path[step] > 0; ++step) {
		CR += use_cost_of_node[Chains[i].update_path[step] - 1] * Chains[i].demand;
	}
	if(Chains[i].update_node > 0) {
		CR += resource_cost_of_node[Chains[i].update_node - 37] * Chains[i].demand;
	}
	
//	CF = chain_failure_cost[Input_Chains[i].service_type];
	for(int j = 0; j < NUM_OF_FEATURES; ++j) {
//		cout<<chain_types[Chains[i].service_type][Chains[i].update_ins][j]<<" ";
		CFF += (1 - chain_types[Chains[i].service_type][Chains[i].update_ins][j]) * feature_failure_cost[Chains[i].service_type][j];
	}
//	cout<<endl;
	CU += (1 - (int)(Chains[i].update_node == Chains[i].ini_node)) * update_msg_cost;    // 这个是瞎写的, 应该与最开始分配的结果比较 
	
//	cout<<Chains[i].service_type<<" "<<Chains[i].update_ins<<endl;
//	cout<<"NF："<<Chains[i].update_node<<endl;
//	
//	cout<<CR<<" "<<CFF<<" "<<CU<<endl;
	
//	return CR + CF + CFF + CU; 
	return CR + CFF + CU; 
}

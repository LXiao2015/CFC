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

void updateTraffic(int path[], int update_path[], int demand) {
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] += demand;
		BW[path[step-1]-1][path[step]-1] += demand;
	}
	for(int step = 1; update_path[step] > 0; ++step) {
		BW[update_path[step]-1][update_path[step-1]-1] -= demand;
		BW[update_path[step-1]-1][update_path[step]-1] -= demand;
	}
}

bool checkCapacity(struct CFC Chains[], int i, int ins) {
	bool flag = false;
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	float demand = Chains[i].demand;
	
	if(node > 0) {
		if(node != 40) {
			node_vnf_demand[node - 37][phy] -= demand;
		}
		else CAP += demand;
	}
	
	if(unode != 40) {
		if((RS[unode - 37][0] >= node_resource[unode - 37][0] && RS[unode - 37][1] >= node_resource[unode - 37][1]) ||
(int)((node_vnf_demand[unode - 37][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]) == node_vnf_count[unode - 37][uphy]) {
		  	flag = true;
		}	
	}
	else {
		if(demand <= CAP) flag = true;
	}
	
	if(node > 0) {
		if(node != 40) {
			node_vnf_demand[node - 37][phy] -= demand;
		}
	}
	else CAP -= demand;
	
	return flag;
}

void updateCapacity(struct CFC Chains[], int i, int ins) {
	int tmp;
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	float demand = Chains[i].demand;
	
	if(node > 0) {
		if(node != 40) {
			node_vnf_demand[node - 37][phy] -= demand;
			tmp = node_vnf_count[node - 37][phy];
			node_vnf_count[node - 37][phy] = (int)((node_vnf_demand[node - 37][phy] - demand + unit_rps[phy] - 1)/unit_rps[phy]);
			tmp -= node_vnf_count[node - 37][phy];
			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[node - 37][j] += node_resource[phy][j] * tmp;
				}
			}
		}
		else CAP += demand;
	}
	
	if(unode > 0) {
		if(unode != 40) {
			node_vnf_demand[unode - 37][uphy] += demand;
			tmp = node_vnf_count[unode - 37][uphy];
			node_vnf_count[unode - 37][uphy] = (int)((node_vnf_demand[unode - 37][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]);
			tmp = node_vnf_count[unode - 37][uphy] - tmp;
			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[unode - 37][j] -= node_resource[uphy][j] * tmp;
				}
			}
		}
		else CAP -= demand;
	}
}
